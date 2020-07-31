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

#include "pugixml.hpp"

#include "Component.h"

class ComponentConfig: public Component
{
    public:
        ComponentConfig(const std::map<std::string,std::string>& attributes,ConfigurationDepot& depot);

        void acceptMsg(std::array<uint8_t,maxMsgLenght_>& msg,unsigned int size,udp::endpoint senderEndPoint,Severity severity) override;

    private:
        void manageFilterSet(pugi::xml_document& configurationDoc,const pugi::xml_node& messagenode);
};

using ComponentConfig_sptr=std::shared_ptr<ComponentConfig>;
