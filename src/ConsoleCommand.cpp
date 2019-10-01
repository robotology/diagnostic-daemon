#include <chrono>
#include <iostream>

#include "ConsoleCommand.h"

using namespace std::chrono_literals;

ConsoleCommand::ConsoleCommand()
{
    thread_=std::make_unique<std::thread>(&ConsoleCommand::inputLoop,this);
}

void ConsoleCommand::inputLoop()
{
    while(active_)
    {
        std::cin>>data_;
        std::cout<<data_;
        std::this_thread::sleep_for(10ms);
    }
}