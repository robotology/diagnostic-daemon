/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

#include "ConfigurationDepot.h"
#include "ComponentUdp.h"
#include "ComponentFile.h"
#include "ComponentConsole.h"
#include "ComponentDisabled.h"
#include "ComponentDecoder.h"
#include "ComponentYarpLogger.h"
#include "Log.h"

ConfigurationDepot::ConfigurationDepot(boost::asio::io_service &io_service): ios_(io_service)
{}

bool ConfigurationDepot::createConfiguration()
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(confsintax::configurationfile);
    if(result.status == pugi::status_file_not_found)
    {
        Log(Severity::error)<<"config.xml not found"<<std::endl;
        return false;
    }
    if(result.status != pugi::status_ok)
    {
        Log(Severity::error)<<"config.xml reading"<<std::endl;
        return false;
    }

    std::string tmp{"//"};
    tmp+=confsintax::component;
    pugi::xpath_node_set components = doc.select_nodes(tmp.c_str());   
    for(auto currentComponent:components)
    {
        pugi::xml_node node=currentComponent.node();    
      
        auto components=createComponent(xmlAttributeToMap(node));
        if(components)
        {
            depot_.push_back(components);
        }
    }

    save();
    
    return true;
}

Component_sptr ConfigurationDepot::createComponent(const std::map<std::string,std::string>& attributes)
{
    Component_sptr components;
    std::string protocol=attributes.at(confsintax::protocol);
    bool enable =asBool(confsintax::enable,attributes);
    
    if(!enable)
        protocol=confsintax::disabled;

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
        case (uint8_t)ComponentType::disabled:                
        {
            components=std::make_shared<ComponentDisabled>(attributes,*this);
            return components;
        } 
        case (uint8_t)ComponentType::decoder:                
        {
            components=std::make_shared<ComponentDecoder>(attributes,*this);
            return components;
        }         
        case (uint8_t)ComponentType::yarplogger:                
        {
            components=std::make_shared<ComponentYarpLogger>(attributes,*this);
            return components;
        }                            
        default:
        {
            //TODO error
        }     
    }
    return Component_sptr();
}

bool ConfigurationDepot::save()
{
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child(confsintax::configuration);
    for(auto current:depot_)
    {
        auto myparameters=current->getParameterMap();
        mapAttributeToXml(node,myparameters);
    }
    doc.save_file("test.xml");
    return true;
}

std::map<std::string,std::string> ConfigurationDepot::xmlAttributeToMap(const pugi::xml_node& node) const
{
    std::map<std::string,std::string> out;
    for (pugi::xml_attribute attr: node.attributes())
    {
        out[attr.name()]=attr.value();
    }
    return out;
}

void ConfigurationDepot::mapAttributeToXml(pugi::xml_node& node,const std::map<std::string,std::string>& in) const
{
    pugi::xml_node mynode=node.append_child(confsintax::component);

    for(auto current:in)
    {
        mynode.append_attribute(current.first.c_str()) = current.second.c_str();
    }
}
