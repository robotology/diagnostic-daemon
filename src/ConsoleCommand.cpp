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
    while(active_)
    {
        std::cin>>data_;
        std::cout<<data_;


        EOMDiagnosticUdpMsg msg;
        EOMDiagnosticRopMsg rop;
        if(data_=="start")
            rop=EOMDiagnosticRopMsg(EOMDiagnosticRopMsg::Info{(uint16_t)DiagnosticRopCode::startlog,(uint16_t)DiagnosticRopSeverity::trace,1,1,14,15,16,17,18});
        else if(data_=="stop")
            rop=EOMDiagnosticRopMsg(EOMDiagnosticRopMsg::Info{(uint16_t)DiagnosticRopCode::stoplog,(uint16_t)DiagnosticRopSeverity::trace,1,1,14,15,16,17,18});    
        else 
            rop=EOMDiagnosticRopMsg(EOMDiagnosticRopMsg::Info{(uint16_t)254,(uint16_t)DiagnosticRopSeverity::trace,1,1,14,15,16,17,18});    
        msg.addRop(rop);
        std::array<uint8_t, EOMDiagnosticUdpMsg::getSize()> udpMsg;
        msg.createUdpPacketData(udpMsg);

        server_.send(udpMsg);
        std::this_thread::sleep_for(10ms);
    }
}