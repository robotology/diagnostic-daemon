#include <chrono>
#include <iostream>

#include "ConsoleCommand.h"
#include "ServerUdp.h"
#include "MsgDescription.h"

using namespace std::chrono_literals;

ConsoleCommand::ConsoleCommand(ServerUdp& server):server_(server)
{
    thread_=std::make_unique<std::thread>(&ConsoleCommand::inputLoop,this);
}

void ConsoleCommand::inputLoop()
{
    std::array<uint8_t, EOMDiagnosticUdpMsg::getSize()> udpMsg;
    EOMDiagnosticUdpMsg msg;
    while(active_)
    {
        std::cin>>data_;
        
        EOMDiagnosticRopMsg::Info rop;
        if(data_=="start")
            rop=EOMDiagnosticRopMsg::Info{(uint16_t)DiagnosticRopCode::startlog,(uint16_t)DiagnosticRopSeverity::trace,1,1,14,15,16,17,18};
        else if(data_=="stop")
            rop=EOMDiagnosticRopMsg::Info{(uint16_t)DiagnosticRopCode::stoplog,(uint16_t)DiagnosticRopSeverity::trace,1,1,14,15,16,17,18};    
        else if(data_=="forceflush")
            rop=EOMDiagnosticRopMsg::Info{(uint16_t)DiagnosticRopCode::forceflush,(uint16_t)DiagnosticRopSeverity::trace,1,1,14,15,16,17,18};                
        else if(data_=="unforceflush")
            rop=EOMDiagnosticRopMsg::Info{(uint16_t)DiagnosticRopCode::unforceflush,(uint16_t)DiagnosticRopSeverity::trace,1,1,14,15,16,17,18};   
        else if(data_=="dumptofile")
        {
            server_.dumpToFile_=!server_.dumpToFile_;   
            continue;
        }
        else if(data_=="dumptoconsole")
        {
            server_.dumpToFile_=!server_.dumpToConsole_;             
            continue;            
        }
        else if(data_=="show")
        {
            std::cout<<"local dumptofile:"<<server_.dumpToFile_<<std::endl;
            std::cout<<"local dumptoconsole:"<<server_.dumpToConsole_<<std::endl;
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
            rop=EOMDiagnosticRopMsg::Info{(uint16_t)254,(uint16_t)DiagnosticRopSeverity::trace,1,1,14,15,16,17,18};    
        msg.addRop(rop);

        udpMsg.fill(0);
        msg.createUdpPacketData(udpMsg);

        server_.send(udpMsg);
        msg.reset();
    }
}