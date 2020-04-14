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

#include "pugixml.hpp"


ComponentConfig::ComponentConfig(const std::map<std::string,std::string>& attributes,ConfigurationDepot& depot):Component(attributes,depot)
{
}

void ComponentConfig::acceptMsg(std::array<uint8_t,maxMsgLenght_>& msg,unsigned int size,udp::endpoint senderEndPoint,Severity)
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
    std::ifstream stream(configmsg);
    pugi::xml_document messageDoc;
    result = messageDoc.load(stream);
    if(result.status == pugi::status_file_not_found)
    if(result.status != pugi::status_ok)
    {
        Log(Severity::error)<<"message not in xml format"<<std::endl;
        return;
    }
    //TODO

//<filtermessageset type="address" destinationaddress="192.168.0.101:9000" rules="x:10.0.1.4 x:10.0.1.5 i:10.0.1.6" propagatetoboard="false" persistence="false"/>

    
}