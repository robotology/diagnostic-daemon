#pragma once

#include <memory>
#include <thread>

class ConsoleCommand
{
    public:
        ConsoleCommand();

    private:
        std::unique_ptr<std::thread> thread_;        
        void inputLoop();
        bool active_{true};

        std::string data_;
};