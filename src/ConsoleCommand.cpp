#include <chrono>
#include <iostream>

#include "ConsoleCommand.h"
#include "ServerUdp.h"

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
        std::array<uint8_t,EOMDiagnosticUdpMsg::getSize()> message{67,68,69};
        server_.send(message);
        std::this_thread::sleep_for(10ms);
    }
}