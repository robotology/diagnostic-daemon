/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

#include "ComponentConfig.h"
#include "ConfigurationDepot.h"
#include "Log.h"

#include <iomanip>
#include <fstream>

ComponentConfig::ComponentConfig(const std::map<std::string,std::string>& attributes,ConfigurationDepot& depot):Component(attributes,depot)
{
}

void ComponentConfig::acceptMsg(std::array<uint8_t,maxMsgLenght_>& msg,unsigned int ,udp::endpoint,Severity)
{
    Log(Severity::debug)<<"ComponentConfig msg received"<<std::endl;

    //*Config file
    pugi::xml_document configurationDoc;
    pugi::xml_parse_result result = configurationDoc.load_file(confsintax::configurationfile);
    if(result.status == pugi::status_file_not_found)
    {
        Log(Severity::error)<<"config.xml not found"<<std::endl;
        return;
    }
    if(result.status != pugi::status_ok)
    {
        Log(Severity::error)<<"config.xml reading"<<std::endl;
        return;
    }

    //*Config message
    std::string configmsg(msg.begin(),msg.end());
    std::stringstream stream(configmsg);
    pugi::xml_document messageDoc;
    result = messageDoc.load(stream);
    if(result.status != pugi::status_ok)
    {
        Log(Severity::error)<<"message not in xml format:"<<stream.str()<<std::endl;
        return;
    }
    
    std::string tmp{"//"};
    tmp+=confmessage::message; 
    pugi::xpath_node message = messageDoc.select_node(tmp.c_str());   
    pugi::xml_node messagenode=message.node();
    std::string messagename=messagenode.attribute(confmessage::name).value();

    if(messagename==confmessage::filtermessageset)
        manageFilterSet(configurationDoc,messagenode);
    else
    {
        Log(Severity::error)<<"Unknown config message"<<std::endl;
    }    
}

void ComponentConfig::manageFilterSet(pugi::xml_document& configurationDoc,const pugi::xml_node& messagenode)
{
    Log(Severity::debug)<<"FilterSet"<<std::endl;

    std::string tmp=messagenode.attribute(confmessage::destinationaddress).value();
    std::vector<std::string> destinationaddress=tokenize<std::string>(tmp);
    std::string rulesvalue=messagenode.attribute(confmessage::rules).value();

    tmp={"//"};
    tmp+=confsintax::component; 
    auto nodes=configurationDoc.select_nodes(tmp.c_str());
    bool found{false};
    for(const pugi::xpath_node& xnode:nodes)
    {   
        pugi::xml_node node=xnode.node();
        if(node.attribute(confsintax::address).value()==destinationaddress[0])
        {
            found=true;
            node.attribute(confsintax::rules).set_value(rulesvalue.c_str());
        }
    }
    if(!found)
    {
        Log(Severity::error)<<"FilterSet rules not found dest address to find:"<<destinationaddress[0]<<std::endl;
    }

    configurationDoc.save_file("conf_new.xml");
}