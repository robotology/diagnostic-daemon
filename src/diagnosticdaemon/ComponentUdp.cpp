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
#include "ConfigurationDepot.h"
#include "Log.h"

using namespace boost::asio;

ComponentUdp::ComponentUdp(boost::asio::io_service &ios,const std::map<std::string,std::string>&attributes,ConfigurationDepot&depot)
    : Component(attributes,depot),
      port_(asInt(confsyntax::rxport,attributes)),
      txport_(asInt(confsyntax::txport,attributes)),
      broadcast_(asBool(confsyntax::broadcast,attributes)),
      rxSocket_(ios, udp::endpoint(udp::v4(), port_)),
      txSocket_(ios),
      ios_(ios)
{ 
  txSocket_.open(boost::asio::ip::udp::v4());
  if(broadcast_)
  {
    boost::asio::socket_base::broadcast option(true);
    txSocket_.set_option(option);
    //txSocket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
    //txSocket_.set_option(boost::asio::socket_base::broadcast(true));
    //receiverEndpoint_={boost::asio::ip::address_v4::broadcast(), txport_};            
  }
  //else
  {
    receiverEndpoint_=udp::endpoint(ip::address::from_string(asString(confsyntax::address,attributes)), txport_);
  }
  
  emsAddress_=asString(confsyntax::address,attributes);
  std::string rules=asString(confsyntax::rules,attributes);
  analyzeAddress(rules);

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
    Log(Severity::debug) <<getName() << " Rx from:"<<senderEndpoint_.address()  << " received bytes:"<<std::hex<<size<<" Max byte:"<<(int)maxMsgLenght_<< std::endl; //test

    if(discardMsgByFilter(senderEndpoint_.address()))
    {
      Log(Severity::debug)<<getName() << " Discard msg for filter"<<std::endl;
    }
    else
    {
      depot_.route(rxData_,size,destination_,senderEndpoint_,Severity::none);
    } 
  }
  else
  {
    Log(Severity::debug) <<getName() << " Error rx:"<<error.value()<< std::endl;
  }

  rxSocket_.async_receive_from(
      boost::asio::buffer(rxData_, maxMsgLenght_), senderEndpoint_,
      boost::bind(&ComponentUdp::handleReceiveFrom, this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
}

void ComponentUdp::handleSendTo(const boost::system::error_code &err, size_t size)
{
  Log(Severity::debug) <<getName() << " Sent msg, size:"<<size<<" error:"<<err.value()<< " endpoint:"<<senderEndpoint_<<std::endl; //test
}

void ComponentUdp::send(const std::array<uint8_t,maxMsgLenght_>& message,unsigned int size)
{
    txSocket_.async_send_to(
      boost::asio::buffer(message,size), receiverEndpoint_,
      boost::bind(&ComponentUdp::handleSendTo, this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
}

void ComponentUdp::acceptMsg(std::string& msg,unsigned int size,udp::endpoint,Severity)
{
    Log(Severity::debug)<<getName()<<" Accept message1"<<std::endl;
    std::array<uint8_t,maxMsgLenght_> tosend;
    std::copy(msg.begin(),msg.end(),tosend.data());
    send(tosend,size);
}

void ComponentUdp::acceptMsg(std::array<uint8_t,maxMsgLenght_>& msg,unsigned int size,udp::endpoint,Severity)
{
    Log(Severity::debug)<<getName()<<" Accept message2"<<std::endl;
    send(msg,size);
}

void ComponentUdp::analyzeAddress(const std::string& addresses)
{
    std::set<std::string> tmp;
    std::vector<std::string> out=tokenize<std::string>(addresses);
    for(const std::string& current:out)
    {
        // TODO with boost 1.67 substitute from_string with make_address
        if(current.substr(0,2)=="x:")
            excludedAddresses_.insert(boost::asio::ip::address::from_string(current.substr(2,current.size()-2)));
        else if(current.substr(0,2)=="i:")
            includededAddresses_.insert(boost::asio::ip::address::from_string(current.substr(2,current.size()-2)));
        else
        {
            Log(Severity::error)<<getName() <<" filter not well configured"<<std::endl;
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