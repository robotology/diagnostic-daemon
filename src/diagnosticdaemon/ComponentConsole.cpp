/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

#include "ComponentConsole.h"
#include "ConfigurationDepot.h"
#include "Log.h"

#include <iomanip>
#include <fstream>


#include "Decoder.h"

ComponentConsole::ComponentConsole(const std::map<std::string,std::string>& attributes,ConfigurationDepot& depot):Component(attributes,depot)
{
    enableYarpLogger_= asBool(confsintax::enableyarplogger,attributes);
    decoder_.init({});
    thread_=std::make_unique<std::thread>(&ComponentConsole::inputLoop,this);
}

void ComponentConsole::acceptMsg(std::array<uint8_t,maxMsgLenght_>& msg,unsigned int size,udp::endpoint senderEndPoint)
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
    decoder_.decode(msg.data(), static_cast<uint16_t>(size), embot::prot::eth::IPv4(ss.str().c_str()),enableYarpLogger_);
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
        else if(data_=="test")
        {
            std::vector<uint8_t> msg{0x78,0x56,0x34,0x12,0x00,0x01,0x01,0x00,0x1f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xf1,0x05,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x04,
                                    0xf8,0x00,0x05,0x00,0x02,0x00,0xd1,0x06,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x03,0x04,0x00,0x00,0x00,0x00,0x00,0x00,
                                    0x00,0x00,0x00,0x00,0x65,0x6d,0x62,0x4f,0x62,0x6a,0x49,0x4d,0x55,0x3a,0x20,0x42,0x4f,0x41,0x52,0x44,0x20,0x65,0x6d,0x73,0x34,0x20,
                                    0x40,0x20,0x31,0x30,0x2e,0x30,0x2e,0x31,0x2e,0x31,0x20,0x28,0x65,0x62,0x31,0x2d,0x69,0x73,0x2d,0x6d,0x79,0x2d,0x6e,0x61,0x6d,0x65,
                                    0x29,0x6f,0x70,0x65,0x6e,0x28,0x29,0x20,0x63,0x6f,0x72,0x72,0x65,0x63,0x74,0x6c,0x79,0x20,0x73,0x74,0x61,0x72,0x74,0x73,0x20,0x73,
                                    0x65,0x72,0x76,0x69,0x63,0x65,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x21,0x43,0x65,0x87};

            decoder_.decode(msg.data(),msg.size(),"192.100.200.100"); 
            continue;            
        }
        else if(data_=="file")
        {
            /*
            std::ifstream ifs("data.log");
            std::string line;
            while (std::getline(ifs, line))
            {
                auto out=tokenize(line);
                std::vector<uint8_t> msg;;
                std::transform(out.begin(), out.end(), std::back_inserter(msg),
                   [](const std::string& current) -> uint8_t { return (uint8_t)std::strtol((current).c_str(),nullptr,16); });

                decoder_.decode(msg.data(),msg.size(),"192.100.200.100");
            }
             */
            continue;            
        }                
        else if(data_=="help")
        {
            std::cout<<std::endl;
            std::cout<<"******************"<<std::endl;
            std::cout<<"Available command:"<<std::endl;
            std::cout<<"******************"<<std::endl;  
            //std::cout<<"*start"<<std::endl;
            ////std::cout<<"*stop"<<std::endl;
            //std::cout<<"*forceflush"<<std::endl;
            //std::cout<<"*unforceflush"<<std::endl;
            //std::cout<<"*dumptofile"<<std::endl;
            //std::cout<<"*dumptoconsole"<<std::endl;
            //std::cout<<"*show"<<std::endl;
            std::cout<<"*file - read from data.log <<e1 23 12 00 00 ...>>"<<std::endl;
            std::cout<<"*test - test msg"<<std::endl;
            std::cout<<"*help"<<std::endl;
            std::cout<<"******************"<<std::endl;
            continue;
        }            
        else 
        {
            std::cout<<"I can understand"<<std::endl;
            ;//rop=EOMDiagnosticRopMsg::Info{(uint16_t)254,(uint16_t)DiagnosticRopSeverity::trace,1,1,14,15,16,17,18}; 
        }
   
        //msg.addRop(rop);

        //udpMsg.fill(0);
        //msg.createUdpPacketData(udpMsg);TODO LUCA
        //depot_.route(msg,destination_);TODO LUCA
        //msg.reset();
    }
}
