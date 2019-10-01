#include <iostream>
#include <array>

#include <boost/array.hpp>
#include <boost/asio.hpp>

#include "ServerUdp.h"

using namespace boost::asio;

ServerUdp::ServerUdp(boost::asio::io_service &io_service)
    : ios_(io_service),
      rxSocket_(io_service, udp::endpoint(udp::v4(), port_)),
      txSocket_(io_service),
      receiverEndpoint_(udp::endpoint(ip::address::from_string(emsAddress_), port_))
{
  txSocket_.open(boost::asio::ip::udp::v4());

  rxSocket_.async_receive_from(
      boost::asio::buffer(rxData_, dataLenght_), senderEndpoint_,
      boost::bind(&ServerUdp::handleReceiveFrom, this,
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
}

void ServerUdp::handleReceiveFrom(const boost::system::error_code &error, size_t bytes_recvd)
{
  if (!error && bytes_recvd > 0)
  {
    std::cout << "Rx from:"<<senderEndpoint_ <<" data:"<< rxData_ << std::endl; //test
  }

  rxSocket_.async_receive_from(
      boost::asio::buffer(rxData_, dataLenght_), senderEndpoint_,
      boost::bind(&ServerUdp::handleReceiveFrom, this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
}

void ServerUdp::handleSendTo(const boost::system::error_code &, size_t)
{
  std::cout << "Sent to" << std::endl; //test
}

void ServerUdp::send(std::array<unsigned char,dataLenght_>& message)
{
    txSocket_.async_send_to(
      boost::asio::buffer(message), receiverEndpoint_,
      boost::bind(&ServerUdp::handleSendTo, this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
}