/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

// - brief
//   Depot for components
//

#pragma once

#include "pugixml.hpp"

#include <boost/asio.hpp>

#include <list>
#include <map>

#include "Component.h"
#include "ConfigurationConst.h"

using boost::asio::ip::udp;

class ConfigurationDepot
{
    public:
        ConfigurationDepot(boost::asio::io_service &io_service);
        bool createConfiguration();

        template <typename T> bool route(T&,unsigned int size,const std::string& destination,udp::endpoint senderEndPoint);

    private:
        pugi::xml_document doc_;
        boost::asio::io_service &ios_;
        std::list<InOut_sptr> depot_;       
        InOut_sptr createInOut(const pugi::xml_node& node);
        InOut_sptr createInOut(const std::map<std::string,std::string>& attributes);
};

template <typename T> static std::vector<T> tokenize(const std::string& destinations) 
{
    std::vector<T> out;
    std::istringstream ss{ destinations };
    out = std::vector<T>{ std::istream_iterator<T>{ss},std::istream_iterator<T>() };
    return out;
}

template <typename T> bool ConfigurationDepot::route(T& msg,unsigned int size,const std::string& destinations,udp::endpoint senderEndPoint)
{
    const auto tokenizedDestination=tokenize<std::string>(destinations);

    for(auto current:depot_)
    {
        for(const std::string& currentDestination:tokenizedDestination)
        {
            if(current->getName()==currentDestination)
                current->acceptMsg(msg,size,senderEndPoint);
        }  
    }
    
    return true;
}

std::map<std::string,std::string> xmlAttributeToMap(const pugi::xml_node& node);
bool asBool(const std::string& name,const std::map<std::string,std::string>& attributes);
std::string asString(const std::string& name,const std::map<std::string,std::string>& attributes);
int asInt(const std::string& name,const std::map<std::string,std::string>& attributes);