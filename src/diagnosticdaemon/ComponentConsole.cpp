/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

#include "ComponentConsole.h"
#include "ConfigurationDepot.h"
#include "Log.h"

#include <iomanip>

#include "Decoder.h"

ComponentConsole::ComponentConsole(const std::map<std::string,std::string>& attributes,ConfigurationDepot& depot):Component(attributes,depot)
{
    #warning TODO-acemor: we must give to embot::core::Config a time base
    embot::core::Config cfg {{nullptr, nullptr}};
    embot::core::init(cfg);

    decoder_.init({});
    thread_=std::make_unique<std::thread>(&ComponentConsole::inputLoop,this);
}

void ComponentConsole::acceptMsg(std::array<uint8_t,maxMsgLenght_>& msg, unsigned int size,udp::endpoint senderEndPoint)
{
    Log(Severity::none)<<"******RAW-MSG******"<<std::endl;
    for(size_t index=0;index<size;++index)
    {
        Log(Severity::none)<< std::hex<< std::setfill('0') << std::setw(2)<<(int)msg[index]<<" ";
    }
    Log(Severity::none)<<std::endl;
    Log(Severity::none)<<"******END RAW-MSG******"<<std::endl;    
    Log(Severity::none)<<"******DECODED-MSG******"<<std::endl;
    std::stringstream ss;
    ss<<senderEndPoint;
    decoder_.decode(msg.data(), static_cast<uint16_t>(size), embot::prot::eth::IPv4(ss.str().c_str()));
    Log(Severity::none)<<"******END DECODED-MSG******"<<std::endl;
    Log(Severity::none)<<std::endl;
}

void ComponentConsole::inputLoop()
{
    //std::array<uint8_t, maxMsgLenght_> udpMsg;
    //EOMDiagnosticUdpMsg msg;
    while(active_)
    {
        std::string data_;
        std::cin>>data_;
        
        //EOMDiagnosticRopMsg::Info rop;
        if(data_=="start")
            ;//rop=EOMDiagnosticRopMsg::Info{(uint16_t)DiagnosticRopCode::startlog,(uint16_t)DiagnosticRopSeverity::trace,1,1,14,15,16,17,18};
        else if(data_=="stop")
            ;//rop=EOMDiagnosticRopMsg::Info{(uint16_t)DiagnosticRopCode::stoplog,(uint16_t)DiagnosticRopSeverity::trace,1,1,14,15,16,17,18};    
        else if(data_=="forceflush")
            ;//rop=EOMDiagnosticRopMsg::Info{(uint16_t)DiagnosticRopCode::forceflush,(uint16_t)DiagnosticRopSeverity::trace,1,1,14,15,16,17,18};                
        else if(data_=="unforceflush")
            ;//rop=EOMDiagnosticRopMsg::Info{(uint16_t)DiagnosticRopCode::unforceflush,(uint16_t)DiagnosticRopSeverity::trace,1,1,14,15,16,17,18};   
        else if(data_=="dumptofile")
        {
            //server_.dumpToFile_=!server_.dumpToFile_;   
            continue;
        }
        else if(data_=="dumptoconsole")
        {
            //server_.dumpToFile_=!server_.dumpToConsole_;             
            continue;            
        }
        else if(data_=="show")
        {
            //Log(Severity::none)<<"local dumptofile:"<<server_.dumpToFile_<<std::endl;
            //Log(Severity::none)<<"local dumptoconsole:"<<server_.dumpToConsole_<<std::endl;
            continue;            
        }        
        else if(data_=="help")
        {
            Log(Severity::none)<<std::endl;
            Log(Severity::none)<<"******************"<<std::endl;
            Log(Severity::none)<<"Available command:"<<std::endl;
            Log(Severity::none)<<"******************"<<std::endl;  
            Log(Severity::none)<<"*start"<<std::endl;
            Log(Severity::none)<<"*stop"<<std::endl;
            Log(Severity::none)<<"*forceflush"<<std::endl;
            Log(Severity::none)<<"*unforceflush"<<std::endl;
            Log(Severity::none)<<"*dumptofile"<<std::endl;
            Log(Severity::none)<<"*dumptoconsole"<<std::endl;
            Log(Severity::none)<<"*show"<<std::endl;
            Log(Severity::none)<<"*help"<<std::endl;
            Log(Severity::none)<<"******************"<<std::endl;
            continue;
        }            
        else 
            {
                ;//rop=EOMDiagnosticRopMsg::Info{(uint16_t)254,(uint16_t)DiagnosticRopSeverity::trace,1,1,14,15,16,17,18};
            }    
        //msg.addRop(rop);

        //udpMsg.fill(0);
        //msg.createUdpPacketData(udpMsg);TODO LUCA
        //depot_.route(msg,destination_);TODO LUCA
        //msg.reset();
    }
}
