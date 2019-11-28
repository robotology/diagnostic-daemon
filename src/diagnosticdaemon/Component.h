/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

// - brief
//   base class for components
//

#pragma once

#include "ConfigurationConst.h"

#include "pugixml.hpp"

#include <boost/asio.hpp>
#include <memory>
#include <set>

using boost::asio::ip::udp;

class ConfigurationDepot;;

class Component
{
    protected:
        static constexpr unsigned int maxMsgLenght_{1500};
    public:
        Component(const std::map<std::string,std::string>& attributes,ConfigurationDepot&);
        Component(const pugi::xml_node&,ConfigurationDepot&);
        virtual ~Component(){};
        virtual void acceptMsg(std::array<uint8_t,maxMsgLenght_>& msg,unsigned int size,udp::endpoint senderEndPoint)=0;
        std::string getName() const {return name_;};

    protected:
        
        ConfigurationDepot& depot_;
        const pugi::xml_node& parameters_;

        std::string name_;
        std::string protocol_;
        bool enable_;
        std::string mode_;
        std::string destination_;

        static std::vector<std::string> tokenize(const std::string& value)
        {
            std::istringstream iss(value);
            std::vector<std::string> results(std::istream_iterator<std::string>{iss},std::istream_iterator<std::string>());
            return results;
        }

};

using InOut_sptr=std::shared_ptr<Component>;