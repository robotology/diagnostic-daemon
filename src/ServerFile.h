#pragma once

#include <iostream>
#include <fstream>
#include <memory>

#include "Server.h"

class ServerFile: public Server
{
    public:
        ServerFile(boost::asio::io_service &io_service,const pugi::xml_node& node,ConfigurationDepot&);
        virtual ~ServerFile(){};

        void acceptMsg(const EOMDiagnosticUdpMsg&) override;

    private:        
        std::ofstream fstream_;

        std::string filename_;

};

using ServerFile_sptr=std::shared_ptr<ServerFile>;