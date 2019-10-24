#pragma once

#include <iostream>
#include <fstream>
#include <memory>
#include <thread>

#include "Server.h"

class ServerConsole: public Server
{
    public:
        ServerConsole(boost::asio::io_service &io_service,const pugi::xml_node& node,ConfigurationDepot&);
        virtual ~ServerConsole(){};

        void acceptMsg(EOMDiagnosticUdpMsg&) override;

    private:
        std::unique_ptr<std::thread> thread_;
        void inputLoop();
        bool active_{true};
};

using ServerConsole_sptr=std::shared_ptr<ServerConsole>;