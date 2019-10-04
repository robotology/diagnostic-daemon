#pragma once

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "EOMDiagnosticUdpMsg.h"

using boost::asio::ip::udp;

class DiagnosticParser;

class ServerUdp
{
public:
    //constexpr static unsigned int dataLenght_=520;
    constexpr static const char emsAddress_[]="10.0.1.1";
    constexpr static unsigned int port_{11000}; 

    ServerUdp(boost::asio::io_service &io_service,DiagnosticParser& parser);

    void handleReceiveFrom(const boost::system::error_code &error, size_t bytes_recvd);
    void handleSendTo(const boost::system::error_code &error, size_t bytes_sent);

    void send(std::array<uint8_t,EOMDiagnosticUdpMsg::getSize()>& message);

private:
    boost::asio::io_service &ios_;
    DiagnosticParser& parser_; 
    udp::socket rxSocket_;
    udp::socket txSocket_;
    udp::endpoint senderEndpoint_;
    udp::endpoint receiverEndpoint_;

    std::array<uint8_t,EOMDiagnosticUdpMsg::getSize()> rxData_;
};
