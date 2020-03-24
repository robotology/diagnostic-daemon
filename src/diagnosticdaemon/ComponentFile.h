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

class ComponentFile: public Component
{
    public:
        ComponentFile(const pugi::xml_node& node,ConfigurationDepot&);
        ComponentFile(const std::map<std::string,std::string>& attributes,ConfigurationDepot& depot);        
        virtual ~ComponentFile(){};

        void acceptMsg(std::array<uint8_t,maxMsgLenght_>& msg,unsigned int size,udp::endpoint senderEndPoint) override;

    private:        
        std::ofstream fstream_;

        std::string filename_;

};

using ComponentFile_sptr=std::shared_ptr<ComponentFile>;