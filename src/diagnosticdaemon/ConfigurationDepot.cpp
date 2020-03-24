/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

#include "ConfigurationDepot.h"
#include "ComponentUdp.h"
#include "ComponentFile.h"
#include "ComponentConsole.h"

ConfigurationDepot::ConfigurationDepot(boost::asio::io_service &io_service): ios_(io_service)
{}

bool ConfigurationDepot::createConfiguration()
{
    pugi::xml_parse_result result = doc_.load_file(confsintax::configurationfile);
    if(result.status == pugi::status_file_not_found)
    {
        std::cout<<"ERROR: config.xml not found"<<std::endl;
        return false;
    }
    if(result.status != pugi::status_ok)
    {
        std::cout<<"ERROR: config.xml reading"<<std::endl;
        return false;
    }

    pugi::xpath_node_set components = doc_.select_nodes(confsintax::component);   
    for(auto currentComponent:components)
    {
        pugi::xml_node node=currentComponent.node();    
      
        auto components=createInOut(xmlAttributeToMap(node));
        if(components)
        {
            depot_.push_back(components);
        }
    }
    
    return true;
}

InOut_sptr ConfigurationDepot::createInOut(const pugi::xml_node& node)
{
    InOut_sptr components;
    std::string protocol=node.attribute(confsintax::protocol).value();
    bool enable=node.attribute(confsintax::enable).as_bool();
    if(!enable)
        return InOut_sptr();

    switch(componentTypeLookup[protocol])
    {
        case (uint8_t)ComponentType::udpbroadcast:
        case (uint8_t)ComponentType::udp:
        {
            components=std::make_shared<ComponentUdp>(ios_,node,*this);
            return components;
        }
        case (uint8_t)ComponentType::file:                
        {
            components=std::make_shared<ComponentFile>(node,*this);
            return components;
        }
        case (uint8_t)ComponentType::console:                
        {
            components=std::make_shared<ComponentConsole>(node,*this);
            return components;
        }            
        default:
        {}
            //TODO error
    }
    return InOut_sptr();
}

InOut_sptr ConfigurationDepot::createInOut(const std::map<std::string,std::string>& attributes)
{
    InOut_sptr components;
    std::string protocol=attributes.at(confsintax::protocol);
    bool enable =asBool(confsintax::enable,attributes);
    
    if(!enable)
        return InOut_sptr();

    switch(componentTypeLookup[protocol])
    {
        case (uint8_t)ComponentType::udpbroadcast:
        case (uint8_t)ComponentType::udp:
        {
            components=std::make_shared<ComponentUdp>(ios_,attributes,*this);
            return components;
        }
        case (uint8_t)ComponentType::file:                
        {
            components=std::make_shared<ComponentFile>(attributes,*this);
            return components;
        }
        case (uint8_t)ComponentType::console:                
        {
            components=std::make_shared<ComponentConsole>(attributes,*this);
            return components;
        }            
        default:
        {}
            //TODO error
    }
    return InOut_sptr();
}

std::map<std::string,std::string> xmlAttributeToMap(const pugi::xml_node& node)
{
    std::map<std::string,std::string> out;
    for (pugi::xml_attribute attr: node.attributes())
    {
        out[attr.name()]=attr.value();
    }
    return out;
}

bool asBool(const std::string& name,const std::map<std::string,std::string>& attributes)
{
    //TODO add check    
    bool tmp;
    std::istringstream(attributes.at(name)) >> std::boolalpha >> tmp;
    return tmp;
}

std::string asString(const std::string& name,const std::map<std::string,std::string>& attributes)
{
    //TODO add check
    return attributes.at(name);
}

int asInt(const std::string& name,const std::map<std::string,std::string>& attributes)
{
    //TODO add check
    auto str=attributes.at(name);
    return std::stoi(str);
}