#pragma once

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

class ServerUdp
{
private:
    constexpr static unsigned int dataLenght_=1024;
    constexpr static const char pc104Address_[]="127.0.0.1";//"10.0.1.104";
    constexpr static unsigned int port_{11000}; 

public:
    ServerUdp(boost::asio::io_service &io_service);

    void handle_receive_from(const boost::system::error_code &error, size_t bytes_recvd);
    void handle_send_to(const boost::system::error_code &error, size_t bytes_sent);

    void send(std::array<unsigned char,dataLenght_>& message);

private:
    boost::asio::io_service &ios_;
    udp::socket rxSocket_;
    udp::socket txSocket_;
    udp::endpoint senderEndpoint_;
    udp::endpoint receiverEndpoint_;

    char rxData_[dataLenght_];
};
