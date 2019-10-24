#include <boost/array.hpp>
#include <boost/bind.hpp>

#include "ComponentUdp.h"
#include "MsgDescriptionExt.h"
#include "ConfigurationDepot.h"
#include "RopParser.h"

using namespace boost::asio;

ComponentUdp::ComponentUdp(boost::asio::io_service &io_service,const pugi::xml_node& node,ConfigurationDepot& depot)
    : Component(io_service,node,depot),
      port_(node.attribute(confsintax::port).as_int()),
      txport_(node.attribute(confsintax::txport).as_int()),
      rxSocket_(io_service, udp::endpoint(udp::v4(), port_)),
      txSocket_(io_service),
      receiverEndpoint_(udp::endpoint(ip::address::from_string(node.attribute(confsintax::address).value()), txport_))
{
  emsAddress_=node.attribute(confsintax::address).value();
  
  txSocket_.open(boost::asio::ip::udp::v4());

  rxSocket_.async_receive_from(
      boost::asio::buffer(rxData_, EOMDiagnosticUdpMsg::getSize()), senderEndpoint_,
      boost::bind(&ComponentUdp::handleReceiveFrom, this,
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
}

void ComponentUdp::handleReceiveFrom(const boost::system::error_code &error, size_t bytes_recvd)
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
      boost::bind(&ComponentUdp::handleReceiveFrom, this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
}

void ComponentUdp::handleSendTo(const boost::system::error_code &err, size_t size)
{
  std::cout << "Sent size:"<<size<<" error:"<<err<< std::endl; //test
}

void ComponentUdp::send(std::array<uint8_t,EOMDiagnosticUdpMsg::getSize()>& message)
{
    txSocket_.async_send_to(
      boost::asio::buffer(message), receiverEndpoint_,
      boost::bind(&ComponentUdp::handleSendTo, this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
}

void ComponentUdp::acceptMsg(EOMDiagnosticUdpMsg& msg) 
{
    std::array<uint8_t, EOMDiagnosticUdpMsg::getSize()> udpMsg;
    udpMsg.fill(0);
    msg.createUdpPacketData(udpMsg);
    send(udpMsg);
};