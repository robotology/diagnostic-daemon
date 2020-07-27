/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

// - brief
//   File component class
//

#pragma once

#include <thread>
#include <memory>

#include "Component.h"

class ComponentSynch: public Component
{
    public:
        ComponentSynch(const std::map<std::string,std::string>& attributes,ConfigurationDepot& depot);        
        virtual ~ComponentSynch();

        void acceptMsg(std::array<uint8_t,maxMsgLenght_>& msg,unsigned int size,udp::endpoint senderEndPoint,Severity severity) override;
        void acceptMsg(std::string& msg,unsigned int size,udp::endpoint senderEndPoint,Severity severity) override;

    private:      
        std::unique_ptr<std::thread> thread_;
        void work();
        bool active_{true};  
        unsigned int synchtime_{10000};
};

using ComponentSynch_sptr=std::shared_ptr<ComponentSynch>;