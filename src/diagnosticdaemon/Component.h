/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

// - brief
//   base class for components
//

#pragma once

#include "Sintax.h"
#include "Log.h"

#include <boost/asio.hpp>
#include <memory>
#include <set>

using boost::asio::ip::udp;

class ConfigurationDepot;

class Component
{
    protected:
        static constexpr unsigned int maxMsgLenght_{1500};        

    public:
        Component(const std::map<std::string,std::string>& attributes,ConfigurationDepot&);
        virtual ~Component(){}

        virtual void acceptMsg(std::array<uint8_t,maxMsgLenght_>& msg,unsigned int size,udp::endpoint senderEndPoint,Severity severity)=0;
        virtual void acceptMsg(std::string&,unsigned int ,udp::endpoint,Severity){};
        std::string getName() const {return name_;}
        const std::map<std::string,std::string> getParameterMap() const;

    protected:

        ConfigurationDepot& depot_;
        const std::map<std::string,std::string> parametersMap_;

        std::string name_;
        std::string type_;
        bool enable_;
        std::string mode_;
        std::string destination_;

};

using Component_sptr=std::shared_ptr<Component>;
