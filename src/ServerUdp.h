#pragma once

#include <cstdlib>

#include <iostream>
#include <fstream>
#include <memory>

#include <boost/bind.hpp>

#include "Server.h"

using boost::asio::ip::udp;

class ServerUdp: public Server
{
public:
    //constexpr static unsigned int dataLenght_=520;
    std::string emsAddress_="10.0.1.1";
    unsigned int port_{11000}; 

    ServerUdp(boost::asio::io_service &io_service,const pugi::xml_node& node,ConfigurationDepot&);
    virtual ~ServerUdp(){};

    void handleReceiveFrom(const boost::system::error_code &error, size_t bytes_recvd);
    void handleSendTo(const boost::system::error_code &error, size_t bytes_sent);

    void send(std::array<uint8_t,EOMDiagnosticUdpMsg::getSize()>& message);

    bool dumpToFile_{true};
    bool dumpToConsole_{true};

    void acceptMsg(const EOMDiagnosticUdpMsg&) override {};

private:
    
    udp::socket rxSocket_;
    udp::socket txSocket_;
    udp::endpoint senderEndpoint_;
    udp::endpoint receiverEndpoint_;

    std::array<uint8_t,EOMDiagnosticUdpMsg::getSize()> rxData_;

    static constexpr const char * filename_="MsgDump.txt";
    std::ofstream fstream_{filename_};
};

using ServerUdp_sptr=std::shared_ptr<ServerUdp>;
