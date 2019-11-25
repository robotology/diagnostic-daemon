/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

#include <boost/array.hpp>
#include <boost/bind.hpp>

#include <iostream>
#include <string>

#include "ComponentUdp.h"
#include "MsgDescriptionExt.h"
#include "ConfigurationDepot.h"

using namespace boost::asio;

ComponentUdp::ComponentUdp(boost::asio::io_service &ios,const pugi::xml_node& node,ConfigurationDepot& depot)
    : Component(node,depot),
      port_(node.attribute(confsintax::rxport).as_int()),
      txport_(node.attribute(confsintax::txport).as_int()),
      rxSocket_(ios, udp::endpoint(udp::v4(), port_)),
      txSocket_(ios),
      receiverEndpoint_(udp::endpoint(ip::address::from_string(node.attribute(confsintax::address).value()), txport_)),
      ios_(ios)
{
  emsAddress_=node.attribute(confsintax::address).value();
  std::string addressfilter=node.attribute(confsintax::addressfilter).value();
  analyzeAddress(addressfilter);

  txSocket_.open(boost::asio::ip::udp::v4());

  rxSocket_.async_receive_from(
      boost::asio::buffer(rxData_, maxMsgLenght_), senderEndpoint_,
      boost::bind(&ComponentUdp::handleReceiveFrom, this,
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
}

void ComponentUdp::handleReceiveFrom(const boost::system::error_code &error, size_t size)
{
  if (!error && size > 0)
  {
    std::cout << "Rx from:"<<senderEndpoint_.address()  << " received bytes:"<<std::hex<<size<<" Max byte:"<<(int)maxMsgLenght_<< std::endl; //test

    if(discardMsgByFilter(senderEndpoint_.address()))
    {
      std::cout << "Discard msg for filter"<<std::endl;
    }
    else
    {
      depot_.route(rxData_,size,destination_,senderEndpoint_);
    }
    
  }
  else
  {
    std::cout << "Error rx:"<<error<< std::endl;
  }

  rxSocket_.async_receive_from(
      boost::asio::buffer(rxData_, maxMsgLenght_), senderEndpoint_,
      boost::bind(&ComponentUdp::handleReceiveFrom, this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
}

void ComponentUdp::handleSendTo(const boost::system::error_code &err, size_t size)
{
  std::cout << "Sent size:"<<size<<" error:"<<err<< std::endl; //test
}

void ComponentUdp::send(const std::array<uint8_t,maxMsgLenght_>& message,unsigned int size)
{
    txSocket_.async_send_to(
      boost::asio::buffer(message,size), receiverEndpoint_,
      boost::bind(&ComponentUdp::handleSendTo, this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
}

void ComponentUdp::acceptMsg(std::array<uint8_t,maxMsgLenght_>& msg,unsigned int size,udp::endpoint senderEndPoint)
{
    send(msg,size);
}

void ComponentUdp::analyzeAddress(const std::string& addresses)
{
    std::set<std::string> tmp;
    auto out=tokenize(addresses);
    for(const std::string& current:out)
    {
        if(current.substr(0,2)=="x:")
            excludedAddresses_.insert(boost::asio::ip::make_address(current.substr(2,current.size()-2)));
        else if(current.substr(0,2)=="i:")
            includededAddresses_.insert(boost::asio::ip::make_address(current.substr(2,current.size()-2)));
        else
        {
            std::cout<<"Error filter"<<std::endl;
        }
    }
}

bool ComponentUdp::discardMsgByFilter(const boost::asio::ip::address& address)
{
  if(!excludedAddresses_.empty())
  {
    if(excludedAddresses_.find(address)==excludedAddresses_.end())
    {
      return false;
    }
    return true;
  }

  if(!includededAddresses_.empty())
  {}
  return false;
}