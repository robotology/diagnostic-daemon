#pragma once

#include "EOMDiagnosticUdpMsg.h"
#include "ConfigurationConst.h"

#include <boost/asio.hpp>

#include "pugixml.hpp"

#include <memory>

class ConfigurationDepot;;

class Component
{
    public:
        Component(boost::asio::io_service &ios,const pugi::xml_node&,ConfigurationDepot&);
        virtual ~Component(){};
        virtual void acceptMsg(EOMDiagnosticUdpMsg&)=0;
        std::string getName() const {return name_;};

    protected:
        boost::asio::io_service &ios_;
        ConfigurationDepot& depot_;
        const pugi::xml_node& parameters_;

        std::string name_;
        std::string protocol_;
        bool enable_;
        std::string mode_;
        std::string destination_;

};

using InOut_sptr=std::shared_ptr<Component>;