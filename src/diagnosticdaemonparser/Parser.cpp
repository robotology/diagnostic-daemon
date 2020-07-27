/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

// - brief
//   Bit stream class
//

#include "Parser.h"
#include "BitStream.h"
#include "Syntax.h"
#include "Log.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>

Parser::Parser()
{
}

bool Parser::start()
{
    pugi::xml_parse_result result = doc_.load_file(msgroot_);
    if(result.status == pugi::status_file_not_found)
    {
        Log(Severity::error)<<"Parser config.xml not found"<<std::endl;
        return false;
    }
    if(result.status != pugi::status_ok)
    {
        Log(Severity::error)<<"Parser config.xml reading"<<std::endl;
        return false;
    }

    includePreparser();
    started_=true;
    return true;
}

std::list<std::tuple<std::string,std::string,std::string>> Parser::parse(const std::vector<uint8_t>& bytetData,pugi::xml_document& xmlSupportDoc/*supporto e debug*/)
{
    if(!started_)
    {
        Log(Severity::error)<<"Parser not started"<<std::endl;
        return std::list<std::tuple<std::string,std::string,std::string>>();
    }


    std::list<std::tuple<std::string,std::string,std::string>> out;
    uint16_t byteindex{0};
    uint8_t indexbit{0};
    xmlSupportDoc.reset(doc_);
    std::string nodeName{"//"};
    nodeName+=parsersyntax::param;
    pugi::xpath_node_set params = xmlSupportDoc.select_nodes(nodeName.c_str()); 

    pugi::xml_node tmp=xmlSupportDoc.document_element();
    visit(tmp, out,bytetData,byteindex,indexbit);

    return out;
}


bool Parser::parse(const std::string& xmlStr,std::vector<uint8_t>& bytetData)
{
    if(!started_)
    {
        Log(Severity::error)<<"Parser not started"<<std::endl;
        return false;
    }

    pugi::xml_document doc;
    doc.load_string(xmlStr.c_str());
    return parse(doc,bytetData);
}

bool Parser::parse(const pugi::xml_document& xmlDoc,std::vector<uint8_t>& bytetData)
{
    if(!started_)
    {
        Log(Severity::error)<<"Parser not started"<<std::endl;
        return false;
    }

    BitStream out(maxMsgLen_);
   
    std::string toFind{"//"};
    toFind+=parsersyntax::param;
    const pugi::xpath_node_set params = xmlDoc.select_nodes(toFind.c_str());


    bool ret{true};
    for (auto currentNodePath:params)
    {
        pugi::xml_node currentNode=currentNodePath.node();
        std::string debug=currentNode.attribute(parsersyntax::namekey).value();
        uint8_t size=currentNode.attribute(parsersyntax::sizekey).as_int();
        std::string test=currentNode.attribute(parsersyntax::valuekey).value();
        unsigned long value=currentNode.attribute(parsersyntax::valuekey).as_ullong();
        std::string uom=currentNode.attribute(parsersyntax::uomkey).value();
        std::string encoding=currentNode.attribute(parsersyntax::encodingkey).value();
        if(uom==parsersyntax::bitkey)
            ret=ret && out.addBits(value,size);
        else
            ret=ret && out.addBytes(value,size,encoding);
    }

    //out.dump(); 
    bytetData=out.data_;
    return ret;
}


bool Parser::checkIfParamIsToBeDeleted(const pugi::xml_node& node)
{
    std::string debug=node.attribute(parsersyntax::namekey).value();
    pugi::xml_attribute attr;
    std::string conditionalname;
    std::vector<std::string> conditionalvalues;
    attr = node.attribute(parsersyntax::conditionnamekey);
    if (attr)
    {
        //Log(Severity::debug)<<"Check conditional"<<std::endl;
        conditionalname=attr.value();
        if(conditionalname==std::string(parsersyntax::nonekey))
            return false;

        if((attr = node.attribute(parsersyntax::conditionorvaluekey)))
        {
            conditionalvalues=(tokenize<std::string>(attr.value()));
        }
        else
        {
            //TODO error
            //Log(Severity::error)<<"Error:conditionvalue"<<std::endl;
            return false;
        }

        //looking for conditional node
        pugi::xml_node conditionalNode=node.parent().find_child_by_attribute(parsersyntax::namekey,conditionalname.c_str());
        if(conditionalNode.empty())
        {
            return true;
        }

        if((attr = conditionalNode.attribute(parsersyntax::valuekey)))
        {
            auto out=std::find_if(conditionalvalues.begin(),conditionalvalues.end(),[&](const std::string& current){
                return attr.value()==current;
                });
            if(out!=conditionalvalues.end())
            {
                //Log(Severity::debug)<<"hitconditional:"<<conditionalname<<std::endl;
                return false;
            }
            else
            {
                //unsatisfied condition
                return true;
            }
        }
        else
        {
            //TODO error
            Log(Severity::error)<<"Error:value"<<std::endl;
            return true;
        }        
    }

    return false;
}

unsigned int Parser::checkRepetitionNumber(const pugi::xml_node& node)
{
    std::string debug=node.attribute(parsersyntax::namekey).value();
    pugi::xml_attribute attr;
    std::string repetitionname;
    std::string repetitionnamenode;
    if ((attr = node.attribute(parsersyntax::repetitionnamekey)))
    {
        repetitionnamenode=attr.value();
        if(repetitionnamenode==std::string(parsersyntax::nonekey))
            return 1;

        //looking for repetition node vale
        pugi::xml_node repetitionNode=node.parent().find_child_by_attribute(parsersyntax::namekey,repetitionnamenode.c_str());
        if(repetitionNode.empty())
        {
            Log(Severity::error)<<"repetitionnamenode not found:"<<debug<<std::endl;
            return 0;
        }
        attr = repetitionNode.attribute(parsersyntax::valuekey);
        std::string repetitions=repetitionnamenode=attr.value();
        //Log(Severity::debug)<<"hitrepetition:"<<repetitions<<std::endl;
        return std::atoi(repetitions.c_str());
    }
    return 1;
}

void Parser::updateVariabileLength(pugi::xml_node& node)
{
    std::string debug=node.attribute(parsersyntax::namekey).value();
    pugi::xml_attribute attr;
    pugi::xml_attribute conditionalLenght=node.attribute(parsersyntax::conditionallengthkey);
    std::string variabilelengthname;
    std::string variabilelengthvalue;
    if (!conditionalLenght.empty())
    {
        variabilelengthname=conditionalLenght.value();
        if(variabilelengthname==std::string(parsersyntax::nonekey))
            return ;

        //looking for conditional node vale
        pugi::xml_node conditionalLengthNode=node.parent().find_child_by_attribute(parsersyntax::namekey,variabilelengthname.c_str());
        if(conditionalLengthNode.empty())
        {
            Log(Severity::error)<<"conditionalLengthNode not found:"<<debug<<std::endl;
            return;
        }
        attr = conditionalLengthNode.attribute(parsersyntax::valuekey);
        std::string length=attr.value();
        int lenValue=std::atoi(length.c_str());
        node.attribute(parsersyntax::sizekey)=lenValue;
        std::string debug=node.attribute(parsersyntax::namekey).value();
        //int debug1=node.attribute(parsersyntax::sizekey).as_int();
        Log(Severity::debug)<<"New size for:"<<debug<<":"<<node.attribute(parsersyntax::sizekey).as_int()<<std::endl;
        return;
    }
    return;
}

void Parser::fillNodeWithValue(std::list<std::tuple<std::string,std::string,std::string>>& outStruct,const std::vector<uint8_t>& bytetData,pugi::xml_node& node,uint16_t &byteindex,uint8_t& bitindex)
{
    uint16_t size=node.attribute(parsersyntax::sizekey).as_int();
    std::string encoding=node.attribute(parsersyntax::encodingkey).value();
    std::string uom=node.attribute(parsersyntax::uomkey).value();
    std::stringstream ss;

    if(uom==parsersyntax::bitkey)
    {
           uint8_t tmp=bytetData[byteindex];
                tmp=(tmp>>(8-bitindex-size));
                uint8_t mask=makeMask<uint8_t>(size);
                tmp=tmp&mask;
                node.attribute(parsersyntax::valuekey)=tmp;

                bitindex+=size;
                if(bitindex==8)
                {
                    bitindex=0;
                    byteindex++;
                }
                ss<<std::to_string(tmp);
    }
    else
    {
        //TODO da cambiare
        switch(size)
        {
            case 1:
            {
                node.attribute(parsersyntax::valuekey)=bytetData[byteindex];
                ss<<(std::to_string)(bytetData[byteindex]);
                ++byteindex;
                break;
            }
            case 2:
            {
                uint16_t tmp=*(uint16_t*)&bytetData[byteindex];
                /*if(encoding==parsersyntax::littleendian)
                {
                    tmp=swapBinary(tmp);
                } */   

                node.attribute(parsersyntax::valuekey)=tmp;
                ss<<tmp;
                byteindex+=2;
                break;
            }
            case 4:
            {
                uint32_t tmp=*(uint32_t*)&bytetData[byteindex];
                byteindex+=4;
                node.attribute(parsersyntax::valuekey)=tmp;
                ss<<tmp;
                break;
            }
            case 8:
            {
                uint64_t tmp=*(uint64_t*)&bytetData[byteindex];
                /*if(encoding==parsersyntax::littleendian)
                {
                    tmp=swapBinary(tmp);
                } */   
                byteindex+=8;
                node.attribute(parsersyntax::valuekey)=tmp;
                ss<<tmp;                
                break;
            }
            case 0:
            break;
            default:
            Log(Severity::error)<<"Error in byte assign:"<<size<<std::endl;
        }
    }

    std::string name=node.attribute(parsersyntax::namekey).value();
    std::string value=ss.str();
    std::string longdescription=node.attribute(parsersyntax::longdescription).value();
    bool show=node.attribute(parsersyntax::showkey).as_bool();
    if(show)
        outStruct.push_back({name,value,longdescription});
}

void Parser::includePreparser()
{
    pugi::xpath_node_set include;
    include= doc_.select_nodes("//xi:include");
    for(auto current:include)
    {
        pugi::xml_node node=current.node();
        std::string file=node.attribute("href").value();
        pugi::xml_document includedoc;
        pugi::xml_parse_result result=includedoc.load_file(file.c_str());
        result=result;
        pugi::xml_node includeroot=includedoc.root().first_child();
        node.parent().insert_copy_after(includeroot,node);
        node.parent().remove_child(node);
    }
}

void Parser::dump(const std::list<std::tuple<std::string,std::string,std::string>>& outStruct)
{
    for(auto current:outStruct)
    {
        Log(Severity::debug)<<"Parser:"<<std::setiosflags(std::ios::left)<<std::setfill('-')<<std::setw(20)<<std::get<0>(current)<<std::setfill('-')<<std::setw(20)<<std::get<1>(current)<<std::setfill('-')<<std::setw(20)<<std::get<2>(current)<<std::endl;
    }
}

uint16_t Parser::swapBinary(uint16_t value)  const
{
    value = ((value & 0x00FF00FF00FF00FFull) << 8)  | ((value & 0xFF00FF00FF00FF00ull) >> 8);
    return value;
}

uint64_t Parser::swapBinary(uint64_t value)  const
{
    value = ((value & 0x00000000FFFFFFFFull) << 32) | ((value & 0xFFFFFFFF00000000ull) >> 32);
    value = ((value & 0x0000FFFF0000FFFFull) << 16) | ((value & 0xFFFF0000FFFF0000ull) >> 16);
    value = ((value & 0x00FF00FF00FF00FFull) << 8)  | ((value & 0xFF00FF00FF00FF00ull) >> 8);
    return value;
}

void Parser::visit(pugi::xml_node& node,std::list<std::tuple<std::string,std::string,std::string>>& outStruct,const std::vector<uint8_t>& bytetData,uint16_t &byteindex,uint8_t &bitindex)
{
    for (pugi::xml_node current = node.first_child(); current; current = current.next_sibling())
    {
        std::string debug=current.attribute(parsersyntax::namekey).value();
        int repetition=checkRepetitionNumber(current);

        for(int repindex=0;repindex<repetition;++repindex)
        {
            
            bool deleted=checkIfParamIsToBeDeleted(current);
            if(deleted)
            {
                //Code to be checked begin
                auto toRemove=current;
                current = current.previous_sibling();
                current.parent().remove_child(toRemove);
                //Code to be checked end
                continue;
            }

            updateVariabileLength(current);
            
            fillNodeWithValue(outStruct,bytetData,current,byteindex,bitindex);
            
            visit(current,outStruct,bytetData,byteindex,bitindex);
       }

    }
}
