/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

// - brief
//   File component class
//

#pragma once

#include <iostream>
#include <fstream>
#include <memory>

#include "Component.h"

class ComponentDisabled: public Component
{
    public:
        ComponentDisabled(const std::map<std::string,std::string>& attributes,ConfigurationDepot& depot);        
        virtual ~ComponentDisabled(){};

        void acceptMsg(std::array<uint8_t,maxMsgLenght_>& msg,unsigned int size,udp::endpoint senderEndPoint) override;
};

using ComponentDisabled_sptr=std::shared_ptr<ComponentDisabled>;