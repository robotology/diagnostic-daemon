#include "RopParser.h"
#include "EOMDiagnosticRopMsg.h"

#include <iostream>
#include <iomanip>


RopParser::RopParser()
{
    pugi::xml_parse_result result = doc_.load_file("msgdepot/rop.xml");
    result=result;
    includePreparser();
    //doc_.save(std::cout);    
}

std::list<std::pair<std::string,std::string>> RopParser::parse(const EOMDiagnosticRopMsg& rop)
{
    std::list<std::pair<std::string,std::string>> msg;
    uint16_t index{0};
    pugi::xml_document currentDoc_;
    currentDoc_.reset(doc_);
    pugi::xpath_node_set params = currentDoc_.select_nodes(paramkey_); 

    for(auto current:params)
    {
        pugi::xml_node node=current.node();
        bool deleted=checkIfParamIsToBeDeleted(node);
        if(deleted)
        {
            //std::cout<<"Erase"<<std::endl;
            node.parent().remove_child(node);
            continue;
        }
    
        uint8_t size=node.attribute(sizekey_).as_int();
        std::string encoding=node.attribute(encoding_).value();
        
        if(size!=0)
        {
            if(size==16)
            {
                node.attribute(valuekey_)=rop.data_.param_[index];
                ++index;
            }
            else if(size==64)
            {
                if(encoding==littleendian_)
                {
                    uint64_t tmp=rop.data_.time_;
                    node.attribute(valuekey_)=swapBinary(tmp);
                }
                else
                {
                    node.attribute(valuekey_)=rop.data_.time_;
                }
             }
        }
        
        std::string name=node.attribute(namekey_).value();
        std::string value=node.attribute(valuekey_).value();
        bool show=node.attribute(showkey_).as_bool();
        if(show)
            msg.push_back({name,value});
    }

    return msg;
    //currentDoc_.save(std::cout);
}

bool RopParser::checkIfParamIsToBeDeleted(const pugi::xml_node& node)
{
    pugi::xml_attribute attr;
    std::string conditionalname;
    std::string conditionalvalue;
    if ((attr = node.attribute(conditionnamekey_)))
    {
        //std::cout<<"Check conditional"<<std::endl;
        conditionalname=attr.value();
        if(conditionalname==std::string(nonekey_))
            return false;

        if((attr = node.attribute(conditionvaluekey_)))
        {
            conditionalvalue=attr.value();
        }
        else
        {
            //TODO error
            //std::cout<<"Error:conditionvalue"<<std::endl;
            return false;
        }

        //looking for conditional node
        pugi::xml_node conditionalNode=node.parent().parent().find_child_by_attribute(namekey_,conditionalname.c_str());
        if(conditionalNode.empty())
        {
            return true;
        }

        if((attr = conditionalNode.attribute(valuekey_)))
        {
            //std::cout<<"Check cond node"<<std::endl;
            if(attr.value()==conditionalvalue)
                return false;
            return true;
        }
        else
        {
            //TODO error
            //std::cout<<"Error:value"<<std::endl;
            return false;
        }        
    }

    return false;
}

void RopParser::includePreparser()
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

void RopParser::dump(const std::list<std::pair<std::string,std::string>>& msg)
{
    for(auto current:msg)
    {
        std::cout<<std::setfill('-')<<std::setw(20)<<current.first<<current.second<<std::endl;
    }
}

uint64_t RopParser::swapBinary(uint64_t value)  const
{
    value = ((value & 0x00000000FFFFFFFFull) << 32) | ((value & 0xFFFFFFFF00000000ull) >> 32);
    value = ((value & 0x0000FFFF0000FFFFull) << 16) | ((value & 0xFFFF0000FFFF0000ull) >> 16);
    value = ((value & 0x00FF00FF00FF00FFull) << 8)  | ((value & 0xFF00FF00FF00FF00ull) >> 8);
    return value;
}
