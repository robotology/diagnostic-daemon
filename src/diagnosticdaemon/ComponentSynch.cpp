/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

#include "ComponentSynch.h"
#include "ConfigurationDepot.h"
#include "MsgPrepare.h"
#include <iomanip>
#include <boost/asio.hpp>



ComponentSynch::ComponentSynch(const std::map<std::string,std::string>& attributes,ConfigurationDepot& depot):Component(attributes,depot)
{
    thread_=std::make_unique<std::thread>(&ComponentSynch::work,this);
    synchtime_=asInt(confsyntax::synchtime,attributes);
}

void ComponentSynch::acceptMsg(std::string& ,unsigned int,udp::endpoint,Severity)
{
}

void ComponentSynch::acceptMsg(std::array<uint8_t,maxMsgLenght_>& ,unsigned int ,udp::endpoint,Severity)
{

}

void ComponentSynch::work()
{
    while(active_)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(synchtime_));
        Log(Severity::debug)<<getName()<<" Send msg rop time"<<std::endl;

        MsgPrepare msgprepare;
        std::vector<uint8_t> tosendVector=msgprepare.prepareFromName("roptime");
        
        unsigned int msgSize=tosendVector.size();
        std::array<uint8_t,maxMsgLenght_> toSend;
        std::copy(tosendVector.begin(), tosendVector.begin() + msgSize, toSend.begin());

        udp::endpoint senderEndpoint{};
        depot_.route(toSend,msgSize,destination_,senderEndpoint,Severity::none);
    }
}      

ComponentSynch::~ComponentSynch()
{
    active_=false;
    thread_->join();
}