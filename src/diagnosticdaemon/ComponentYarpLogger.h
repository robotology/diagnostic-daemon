/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

// - brief
//   Console component class
//

#pragma once

#include <iostream>
#include <memory>
#include <thread>

#ifdef COMPILE_WITHYARP_DEF 
#include <yarp/os/api.h>
#include <yarp/os/Log.h>
#include <yarp/os/Network.h>
#include <yarp/os/LogStream.h>
#endif

#include "Component.h"
#include "Log.h"

class ComponentYarpLogger: public Component
{
    public:
        ComponentYarpLogger(const std::map<std::string,std::string>& attributes,ConfigurationDepot& depot);
        virtual ~ComponentYarpLogger() {}

        void acceptMsg(std::array<uint8_t,maxMsgLenght_>& msg,unsigned int size,udp::endpoint senderEndPoint,Severity severity) override;
        void acceptMsg(std::string& msg,unsigned int size,udp::endpoint senderEndPoint,Severity severity) override;

    private:
        bool active_{true};
        void forewardtoYarpLogger(const std::string& data,Severity severity/*,embot::prot::eth::diagnostic::TYP severity*/); 
};

using ComponentYarpLogger_sptr=std::shared_ptr<ComponentYarpLogger>;
