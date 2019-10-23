#include <iostream>
#include <fstream>
#include <array>

#include <boost/array.hpp>
#include <boost/asio.hpp>

#include "ServerUdp.h"
#include "MsgDescriptionExt.h"
#include "ConfigurationDepot.h"
#include "RopParser.h"

using namespace boost::asio;

ServerUdp::ServerUdp(boost::asio::io_service &io_service,const pugi::xml_node& node,ConfigurationDepot& depot)
    : Server(io_service,node,depot),
      rxSocket_(io_service, udp::endpoint(udp::v4(), node.attribute(confsintax::port).as_int())),
      txSocket_(io_service),
      receiverEndpoint_(udp::endpoint(ip::address::from_string(node.attribute(confsintax::address).value()), port_))
{
  port_=node.attribute(confsintax::port).as_int();
  emsAddress_=node.attribute(confsintax::address).value();
  
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

    EOMDiagnosticUdpMsg msg;
    msg.parse(rxData_);
    depot_.route(msg,destination_);
    /*
    if(dumpToConsole_)
    {
      std::ostream &stream=std::cout;
      msg.dump(&ropSeverity,&ropCode,&ropString,stream);
    }

    if(dumpToFile_)
    {
      msg.dump(&ropSeverity,&ropCode,&ropString,fstream_);
    }

    std::cout<<"***************************************"<<std::endl;
    std::cout<<"***************XML PARSER**************"<<std::endl;
    for(uint8_t index=0;index<msg.getCurrentRopNumber();++index)
    {
      RopParser parser;
      EOMDiagnosticRopMsg rop;
      std::cout<<"**************ROP_BEGIN**************"<<std::endl;
      msg.getRop(rop,index);
      parser.parse(rop);
      parser.dump();
      std::cout<<"**************ROP_END****************"<<std::endl;
    }
    std::cout<<"***************************************"<<std::endl;
    std::cout<<"***************************************"<<std::endl;
    */
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