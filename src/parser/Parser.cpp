#include "Parser.h"
#include "BitStream.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>

Parser::Parser()
{
    pugi::xml_parse_result result = doc_.load_file("msgdepot/udpheader.xml");
    if(result.status == pugi::status_file_not_found)
    {
        std::cout<<"config.xml not found"<<std::endl;
        return ;
    }
    if(result.status != pugi::status_ok)
    {
        std::cout<<"config.xml reading"<<std::endl;
        return ;
    }

    includePreparser();
}

bool Parser::checkIfParamIsToBeDeleted(const pugi::xml_node& node)
{
    std::string debug=node.attribute(namekey_).value();
    pugi::xml_attribute attr;
    std::string conditionalname;
    std::vector<std::string> conditionalvalues;
    attr = node.attribute(conditionnamekey_);
    if (attr)
    {
        //std::cout<<"Check conditional"<<std::endl;
        conditionalname=attr.value();
        if(conditionalname==std::string(nonekey_))
            return false;

        if((attr = node.attribute(conditionorvaluekey_)))
        {
            conditionalvalues=(tokenize(attr.value()));
        }
        else
        {
            //TODO error
            //std::cout<<"Error:conditionvalue"<<std::endl;
            return false;
        }

        //looking for conditional node
        pugi::xml_node conditionalNode=node.parent().find_child_by_attribute(namekey_,conditionalname.c_str());
        if(conditionalNode.empty())
        {
            return true;
        }

        if((attr = conditionalNode.attribute(valuekey_)))
        {
            auto out=std::find_if(conditionalvalues.begin(),conditionalvalues.end(),[&](const std::string& current){
                return attr.value()==current;
                });
            if(out!=conditionalvalues.end())
            {
                //std::cout<<"hitconditional:"<<conditionalname<<std::endl;
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
            std::cout<<"Error:value"<<std::endl;
            return true;
        }        
    }

    return false;
}

unsigned int Parser::checkRepetitionNumber(const pugi::xml_node& node)
{
    std::string debug=node.attribute(namekey_).value();
    pugi::xml_attribute attr;
    std::string repetitionname;
    std::string repetitionnamenode;
    if ((attr = node.attribute(repetitionnamekey_)))
    {
        repetitionnamenode=attr.value();
        if(repetitionnamenode==std::string(nonekey_))
            return 1;

        //looking for repetition node vale
        pugi::xml_node repetitionNode=node.parent().find_child_by_attribute(namekey_,repetitionnamenode.c_str());
        if(repetitionNode.empty())
        {
            return true;
        }
        attr = repetitionNode.attribute(valuekey_);
        std::string repetitions=repetitionnamenode=attr.value();
        //std::cout<<"hitrepetition:"<<repetitions<<std::endl;
        return std::atoi(repetitions.c_str());
    }
    return 1;
}

void Parser::updateVariabileLength(pugi::xml_node& node)
{
    std::string debug=node.attribute(namekey_).value();
    pugi::xml_attribute attr;
    pugi::xml_attribute conditionalLenght=node.attribute(conditionallengthkey_);
    std::string variabilelengthname;
    std::string variabilelengthvalue;
    if (!conditionalLenght.empty())
    {
        variabilelengthname=conditionalLenght.value();
        if(variabilelengthname==std::string(nonekey_))
            return ;

        //looking for repetition node vale
        pugi::xml_node conditionalLengthNode=node.parent().find_child_by_attribute(namekey_,variabilelengthname.c_str());
        if(conditionalLengthNode.empty())
        {
            return;
        }
        attr = conditionalLengthNode.attribute(valuekey_);
        std::string length=attr.value();
        int lenValue=std::atoi(length.c_str());
        node.attribute(sizekey_)=lenValue;
        std::string debug=node.attribute(namekey_).value();
        //int debug1=node.attribute(sizekey_).as_int();
        std::cout<<"New size for:"<<debug<<":"<<node.attribute(sizekey_).as_int()<<std::endl;
        return;
    }
    return;
}

void Parser::fillNodeWithValue(std::list<std::tuple<std::string,std::string,std::string>>& msg,const std::array<uint8_t,maxMsgLenght_>& rop,pugi::xml_node& node,uint16_t &byteindex,uint8_t& bitindex)
{
    uint16_t size=node.attribute(sizekey_).as_int();
    std::string encoding=node.attribute(encoding_).value();
    std::string uom=node.attribute(uomkey_).value();
    std::stringstream ss;

    if(uom==bitkey_)
    {
           uint8_t tmp=rop[byteindex];
                tmp=(tmp>>(8-bitindex-size));
                uint8_t mask=make_mask<uint8_t>(size);
                tmp=tmp&mask;
                node.attribute(valuekey_)=tmp;

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
                node.attribute(valuekey_)=rop[byteindex];
                ss<<(std::to_string)(rop[byteindex]);
                ++byteindex;
                break;
            }
            case 2:
            {
                uint16_t tmp=*(uint16_t*)&rop[byteindex];
                /*if(encoding==littleendian_)
                {
                    tmp=swapBinary(tmp);
                } */   

                node.attribute(valuekey_)=tmp;
                ss<<tmp;
                byteindex+=2;
                break;
            }
            case 4:
            {
                uint32_t tmp=*(uint32_t*)&rop[byteindex];
                byteindex+=4;
                node.attribute(valuekey_)=tmp;
                ss<<tmp;
                break;
            }
            case 8:
            {
                uint64_t tmp=*(uint64_t*)&rop[byteindex];
                /*if(encoding==littleendian_)
                {
                    tmp=swapBinary(tmp);
                } */   
                byteindex+=8;
                node.attribute(valuekey_)=tmp;
                ss<<tmp;                
                break;
            }
            case 0:
            break;
            default:
            std::cout<<"Error in byte assign:"<<size<<std::endl;
            /*
            {
                node.attribute(valuekey_)="..";
                for(int t=0;t<size/8;++t)
                {
                    ss<<(char)(rop[byteindex]);                
                    byteindex++;
                }
                break;
            }*/
        }
    }

    std::string name=node.attribute(namekey_).value();
    std::string value=ss.str();
    std::string longdescription=node.attribute(longdescription_).value();
    bool show=node.attribute(showkey_).as_bool();
        if(show)
            msg.push_back({name,value,longdescription});
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

void Parser::dump(const std::list<std::tuple<std::string,std::string,std::string>>& msg)
{
    for(auto current:msg)
    {
        std::cout<<std::setiosflags(std::ios::left)<<std::setfill('-')<<std::setw(20)<<std::get<0>(current)<<std::setfill('-')<<std::setw(20)<<std::get<1>(current)<<std::setfill('-')<<std::setw(20)<<std::get<2>(current)<<std::endl;
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

void Parser::visit(pugi::xml_node node,std::list<std::tuple<std::string,std::string,std::string>>& msg,const std::array<uint8_t,maxMsgLenght_>& rop,uint16_t &byteindex,uint8_t &bitindex)
{
    
    for (pugi::xml_node current = node.first_child(); current; current = current.next_sibling())
    {
        //std::cout<<node.attribute(namekey_).value()<<"::"<<current.attribute(namekey_).value()<<std::endl;

        int repetition=checkRepetitionNumber(current);

        for(int repindex=0;repindex<repetition;++repindex)
        {
            
            bool deleted=checkIfParamIsToBeDeleted(current);
            if(deleted)
            {
                //std::cout<<"Erase"<<std::endl;
                current.parent().remove_child(current);
                continue;
            }

            updateVariabileLength(current);
            
            fillNodeWithValue(msg,rop,current,byteindex,bitindex);
            
            visit(current,msg,rop,byteindex,bitindex);
       }

    }
}

std::list<std::tuple<std::string,std::string,std::string>> Parser::parse(const std::array<uint8_t,maxMsgLenght_>& rop)
{
    std::list<std::tuple<std::string,std::string,std::string>> msg;
    uint16_t byteindex{0};
    uint8_t indexbit{0};
    pugi::xml_document currentDoc_;
    currentDoc_.reset(doc_);
    std::string nodeName{"//"};
    nodeName+=param_;
    pugi::xpath_node_set params = currentDoc_.select_nodes(nodeName.c_str()); 

    visit(currentDoc_.document_element(), msg,rop,byteindex,indexbit);

    return msg;
    //currentDoc_.save(std::cout);
}


bool Parser::parse(const std::string& msg,std::vector<uint8_t>& data)
{
    BitStream out(maxMsgLen_);

    pugi::xml_document doc;
    doc.load_string(msg.c_str());
    pugi::xml_node params = doc.child(udpheader_);
    
    bool ret{true};
    for (auto currentNode:params)
    {
        uint8_t size=currentNode.attribute(sizekey_).as_int();
        uint8_t value=currentNode.attribute(valuekey_).as_int();
        std::string uom=currentNode.attribute(uomkey_).value();
        if(uom==bitkey_)
            ret=ret && out.addBits(value,size);
        else
            ret=ret && out.addBytes(value,size);
    }

    out.dump(); 
    data=out.data_;
    return ret;
}


