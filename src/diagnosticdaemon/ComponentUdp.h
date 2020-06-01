/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

// - brief
//   UDP component class
//

#pragma once

#include <array>
#include "boost/asio.hpp"

#include "Component.h"

class ComponentUdp: public Component
{
public:
    ComponentUdp(boost::asio::io_service &ios,const std::map<std::string,std::string>&attributes,ConfigurationDepot&depot);
    virtual ~ComponentUdp(){};

    void acceptMsg(std::array<uint8_t,maxMsgLenght_>& msg,unsigned int size,udp::endpoint senderEndPoint,Severity severity) override;
    void acceptMsg(std::string&,unsigned int ,udp::endpoint,Severity) override ;

private:

    std::string emsAddress_="10.0.1.1";
    unsigned int port_{11000}; 
    unsigned int txport_{11000};
    bool broadcast_{false}; 

    udp::socket rxSocket_;
    udp::socket txSocket_;
    udp::endpoint senderEndpoint_;
    udp::endpoint receiverEndpoint_;

    std::array<uint8_t,maxMsgLenght_> rxData_;

    void handleReceiveFrom(const boost::system::error_code &error, size_t bytes_recvd);
    void handleSendTo(const boost::system::error_code &error, size_t bytes_sent);

    void send(const std::array<uint8_t,maxMsgLenght_>& message,unsigned int size);

    void analyzeAddress(const std::string& addresses);
    std::set<boost::asio::ip::address> excludedAddresses_;
    std::set<boost::asio::ip::address> includededAddresses_;
    bool discardMsgByFilter(const boost::asio::ip::address& address);

    boost::asio::io_service &ios_;
};

using ComponentUdp_sptr=std::shared_ptr<ComponentUdp>;