#pragma once

#include <iostream>
#include <fstream>
#include <memory>

#include "Server.h"

class ServerConsole: public Server
{
    public:
        ServerConsole(boost::asio::io_service &io_service,const pugi::xml_node& node,ConfigurationDepot&);
        virtual ~ServerConsole(){};

        void acceptMsg(const EOMDiagnosticUdpMsg&) override;

    private:        

};

using ServerConsole_sptr=std::shared_ptr<ServerConsole>;