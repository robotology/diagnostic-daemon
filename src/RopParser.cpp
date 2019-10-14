#include "RopParser.h"
#include "EOMDiagnosticRopMsg.h"

#include <iostream>


RopParser::RopParser()
{
    pugi::xml_parse_result result = doc_.load_file("msgdepot/rop.xml");
    includePreparser();
    //doc_.save(std::cout);
    params_= doc_.select_nodes(paramkey_);    
}

void RopParser::parse(const EOMDiagnosticRopMsg& rop)
{
    uint16_t index{0};

    for(auto current:params_)
    {
        pugi::xml_node node=current.node();
        bool deleted=checkIfParamIsToBeDeleted(node);
        if(deleted)
        {
            //std::cout<<"Erase"<<std::endl;
            node.parent().remove_child(node);
            continue;
        }
    
        if(node.attribute(sizekey_).as_int()!=0)
        {
            node.attribute(valuekey_)=rop.data_.param_[index];
            ++index;
        }
        std::string name=node.attribute(namekey_).value();
        std::string value=node.attribute(valuekey_).value();
        bool show=node.attribute(showkey_).as_bool();
        if(show)
            msg_.push_back({name,value});
    }
    doc_.save(std::cout);
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
        pugi::xml_node includeroot=includedoc.root().first_child();
        node.parent().insert_copy_after(includeroot,node);
        node.parent().remove_child(node);
    }
}

void RopParser::dump()
{
    for(auto current:msg_)
    {
        std::cout<<"P_name:"<<current.first<<" P_value:"<<current.second<<std::endl;
    }

}