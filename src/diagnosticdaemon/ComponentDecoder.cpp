/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

#include "ComponentDecoder.h"
#include "ConfigurationDepot.h"
#include "Log.h"

#include <iomanip>
#include <fstream>


#include "Decoder.h"

ComponentDecoder::ComponentDecoder(const std::map<std::string,std::string>& attributes,ConfigurationDepot& depot):Component(attributes,depot)
{
    decoder_.init({});
}

void ComponentDecoder::acceptMsg(std::array<uint8_t,maxMsgLenght_>& msg,unsigned int size,udp::endpoint senderEndPoint,Severity severity)
{
    Log(Severity::debug)<<"ComponentDecoder"<<std::endl;

    Log(Severity::none)<<"******RAW-MSG******"<<std::endl;
    for(size_t index=0;index<size;++index)
    {
        Log(Severity::none)<< std::hex<< std::setfill('0') << std::setw(2)<<(int)msg[index]<<" ";
    }
    Log(Severity::none)<<std::endl;
    Log(Severity::none)<<"******END RAW-MSG******"<<std::endl;    
    
    std::stringstream ss;
    ss<<senderEndPoint;
    auto msgs=decoder_.decode(msg.data(), static_cast<uint16_t>(size), embot::prot::eth::IPv4(ss.str().c_str()));
    
    for(auto& currentMsg:msgs)
    {
        depot_.route(currentMsg.first,currentMsg.first.size(),destination_,senderEndPoint,currentMsg.second);
    }
    decoder_.clear();
}