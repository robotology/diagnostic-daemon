#include <iostream>
#include <array>

#include <boost/array.hpp>
#include <boost/asio.hpp>

#include "ServerUdp.h"
#include "MsgDescriptionExt.h"

using namespace boost::asio;

ServerUdp::ServerUdp(boost::asio::io_service &io_service)
    : ios_(io_service),
      rxSocket_(io_service, udp::endpoint(udp::v4(), port_)),
      txSocket_(io_service),
      receiverEndpoint_(udp::endpoint(ip::address::from_string(emsAddress_), port_))
{
  txSocket_.open(boost::asio::ip::udp::v4());

  rxSocket_.async_receive_from(
      boost::asio::buffer(rxData_, EOMDiagnosticUdpMsg::getSize()), senderEndpoint_,
      boost::bind(&ServerUdp::handleReceiveFrom, this,
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
}

void ServerUdp::handleReceiveFrom(const boost::system::error_code &error, size_t bytes_recvd)
{
  std::cout << "****"<< std::endl;
  if (!error && bytes_recvd > 0)
  {
    std::cout << "Rx from:"<<senderEndpoint_  << " received bytes:"<<std::hex<<bytes_recvd<< std::endl; //test
/*
    std::stringstream ss;
    std::copy(rxData_.begin()+0,rxData_.begin()+24,std::ostream_iterator<int>(ss," "));
    std::cout<<"H:"<<std::hex<<ss.str()<<std::endl;

    ss.str("");
    std::copy(rxData_.begin()+24,rxData_.begin()+48,std::ostream_iterator<int>(ss," "));
    std::cout<<"B:"<<std::hex<<ss.str()<<std::endl;

    ss.str("");
    std::copy(rxData_.begin()+48,rxData_.begin()+72,std::ostream_iterator<int>(ss," "));
    std::cout<<"B:"<<std::hex<<ss.str()<<std::endl;

    ss.str("");
    std::copy(rxData_.begin()+72,rxData_.begin()+76,std::ostream_iterator<int>(ss," "));
    std::cout<<"F:"<<std::hex<<ss.str()<<std::endl;    
*/
    EOMDiagnosticUdpMsg msg;
    msg.parse(rxData_);
    msg.dump(&ropSeverity,&ropCode,&ropString);
  }

  rxSocket_.async_receive_from(
      boost::asio::buffer(rxData_, EOMDiagnosticUdpMsg::getSize()), senderEndpoint_,
      boost::bind(&ServerUdp::handleReceiveFrom, this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
}

void ServerUdp::handleSendTo(const boost::system::error_code &, size_t size)
{
  std::cout << "Sent size:"<<size<< std::endl; //test
}

void ServerUdp::send(std::array<uint8_t,EOMDiagnosticUdpMsg::getSize()>& message)
{
    txSocket_.async_send_to(
      boost::asio::buffer(message), receiverEndpoint_,
      boost::bind(&ServerUdp::handleSendTo, this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
}