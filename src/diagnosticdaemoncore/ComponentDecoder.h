/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

// - brief
//   Decoder component class
//

#pragma once

#include <iostream>
#include <memory>
#include <thread>

#include "Component.h"
#include "Decoder.h"

class ComponentDecoder: public Component
{
    public:
        ComponentDecoder(const std::map<std::string,std::string>& attributes,ConfigurationDepot& depot);

        void acceptMsg(std::array<uint8_t,maxMsgLenght_>& msg,unsigned int size,udp::endpoint senderEndPoint,Severity severity) override;

    private:
        Decoder decoder_;
};

using ComponentDecoder_sptr=std::shared_ptr<ComponentDecoder>;
