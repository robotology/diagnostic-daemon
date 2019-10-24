#pragma once

#include "pugixml.hpp"

#include <boost/asio.hpp>

#include <list>
#include <map>

#include "Component.h"
#include "ConfigurationConst.h"

class ConfigurationDepot
{
    public:
        ConfigurationDepot(boost::asio::io_service &io_service);
        bool createConfiguration();

        bool route(EOMDiagnosticUdpMsg&,const std::string& destination);

    private:
        pugi::xml_document doc_;
        boost::asio::io_service &ios_;
        std::list<InOut_sptr> depot_;       
        InOut_sptr createInOut(const pugi::xml_node& node);
};

template <typename T> static std::vector<T> tokenize(const std::string& destinations) {
    std::vector<T> out;
    std::istringstream ss{ destinations };
    out = std::vector<T>{ std::istream_iterator<T>{ss},std::istream_iterator<T>() };
    return out;
}