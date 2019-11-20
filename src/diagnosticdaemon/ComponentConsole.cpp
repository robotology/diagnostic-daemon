/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

#include "ComponentConsole.h"
#include "MsgDescriptionExt.h"
#include "ConfigurationDepot.h"

#include <iomanip>

#include "Decoder.h"

ComponentConsole::ComponentConsole(boost::asio::io_service &io_service,const pugi::xml_node& node,ConfigurationDepot& depot):Component(io_service,node,depot)
{
    thread_=std::make_unique<std::thread>(&ComponentConsole::inputLoop,this);
}

void ComponentConsole::acceptMsg(std::array<uint8_t,maxMsgLenght_>& msg,unsigned int size,udp::endpoint senderEndPoint)
{
    for(size_t index=0;index<size;++index)
    {
        std::cout<< std::hex<< std::setfill('0') << std::setw(2)<<(int)msg[index]<<" ";
    }

    Decoder decoder;
    decoder.init({});
    std::stringstream ss;
    ss<<senderEndPoint;
    decoder.decode(msg.data(),size,ss.str().c_str());  
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
            //std::cout<<"local dumptofile:"<<server_.dumpToFile_<<std::endl;
            //std::cout<<"local dumptoconsole:"<<server_.dumpToConsole_<<std::endl;
            continue;            
        }        
        else if(data_=="help")
        {
            std::cout<<std::endl;
            std::cout<<"******************"<<std::endl;
            std::cout<<"Available command:"<<std::endl;
            std::cout<<"******************"<<std::endl;  
            std::cout<<"*start"<<std::endl;
            std::cout<<"*stop"<<std::endl;
            std::cout<<"*forceflush"<<std::endl;
            std::cout<<"*unforceflush"<<std::endl;
            std::cout<<"*dumptofile"<<std::endl;
            std::cout<<"*dumptoconsole"<<std::endl;
            std::cout<<"*show"<<std::endl;
            std::cout<<"*help"<<std::endl;
            std::cout<<"******************"<<std::endl;
            continue;
        }            
        else 
            ;//rop=EOMDiagnosticRopMsg::Info{(uint16_t)254,(uint16_t)DiagnosticRopSeverity::trace,1,1,14,15,16,17,18};    
        //msg.addRop(rop);

        //udpMsg.fill(0);
        //msg.createUdpPacketData(udpMsg);TODO LUCA
        //depot_.route(msg,destination_);TODO LUCA
        //msg.reset();
    }
}