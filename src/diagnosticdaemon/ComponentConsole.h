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

#include "Component.h"
#include "Decoder.h"

class ComponentConsole: public Component
{
    public:
        ComponentConsole(const pugi::xml_node& node,ConfigurationDepot&);
        ComponentConsole(const std::map<std::string,std::string>& attributes,ConfigurationDepot& depot);
        virtual ~ComponentConsole(){};

        void acceptMsg(std::array<uint8_t,maxMsgLenght_>& msg,unsigned int size,udp::endpoint senderEndPoint) override;

    private:
        std::unique_ptr<std::thread> thread_;
        void inputLoop();
        bool active_{true};

        Decoder decoder_;
};

using ComponentConsole_sptr=std::shared_ptr<ComponentConsole>;