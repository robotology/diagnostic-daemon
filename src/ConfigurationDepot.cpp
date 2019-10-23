#include "ConfigurationDepot.h"
#include "ServerUdp.h"
#include "ServerFile.h"
#include "ServerConsole.h"

ConfigurationDepot::ConfigurationDepot(boost::asio::io_service &io_service): ios_(io_service)
{}

bool ConfigurationDepot::createConfiguration()
{
    pugi::xml_parse_result result = doc_.load_file("config.xml"/*confsintax::configurationFile*/);

    pugi::xpath_node_set servers = doc_.select_nodes(confsintax::server);   
    for(auto currentServer:servers)
    {
        pugi::xml_node node=currentServer.node();    
      
        auto server=createInOut(node);
        if(server)
        {
            depot_.push_back(server);
        }
    }
    
    return true;
}

InOut_sptr ConfigurationDepot::createInOut(const pugi::xml_node& node)
{
    InOut_sptr server;
    std::string protocol=node.attribute(confsintax::protocol).value();
    bool enable=node.attribute(confsintax::enable).as_bool();
    if(!enable)
        return InOut_sptr();

    switch(protocolTypeLookup[protocol])
        {
            case (uint8_t)ServerType::udpbroadcast:
            case (uint8_t)ServerType::udp:
            {
                server=std::make_shared<ServerUdp>(ios_,node,*this);
                return server;
            }
            case (uint8_t)ServerType::file:                
            {
                server=std::make_shared<ServerFile>(ios_,node,*this);
                return server;
            }
            case (uint8_t)ServerType::console:                
            {
                server=std::make_shared<ServerConsole>(ios_,node,*this);
                return server;
            }            
            default:
            {}
                //TODO error
        }
    return InOut_sptr();
}

bool ConfigurationDepot::route(const EOMDiagnosticUdpMsg& msg,const std::string& destinations)
{
    auto tokenizedDestination=tokenize<std::string>(destinations);

    for(auto current:depot_)
    {
        for(const std::string& currentDestination:tokenizedDestination)
        {
            if(current->getName()==currentDestination)
                current->acceptMsg(msg);
        }  
    }
    
    return true;
}