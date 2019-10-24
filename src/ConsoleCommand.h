#pragma once

#include <memory>
#include <thread>

class ServerUdp;

class ConsoleCommand
{
    public:
        ConsoleCommand(ServerUdp& server);

    private:
        std::unique_ptr<std::thread> thread_;
        void inputLoop();
        bool active_{true};

        std::string data_;

        ServerUdp& server_;
};