/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

// - brief
//   The class contains all the components.
//

#pragma once

#include "pugixml.hpp"

#include <boost/asio.hpp>

#include <list>
#include <map>

#include "Component.h"
#include "Sintax.h"
#include "const.h"

using boost::asio::ip::udp;

class ConfigurationDepot
{
    public:
        ConfigurationDepot(boost::asio::io_service &io_service);
        bool createConfiguration();

        template <typename T> bool route(T&,unsigned int size,const std::string& destination,udp::endpoint senderEndPoint);

        bool save();

    private:
        boost::asio::io_service &ios_;
        std::list<Component_sptr> depot_;       
        Component_sptr createComponent(const std::map<std::string,std::string>& attributes);

        std::map<std::string,std::string> xmlAttributeToMap(const pugi::xml_node& node) const;
        void mapAttributeToXml(pugi::xml_node& node,const std::map<std::string,std::string>& in) const;
};

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
