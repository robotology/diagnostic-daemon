#include "ConfigurationDepot.h"
#include "ComponentUdp.h"
#include "ComponentFile.h"
#include "ComponentConsole.h"

ConfigurationDepot::ConfigurationDepot(boost::asio::io_service &io_service): ios_(io_service)
{}

bool ConfigurationDepot::createConfiguration()
{
    pugi::xml_parse_result result = doc_.load_file("config.xml"/*confsintax::configurationFile*/);
    result=result;

    pugi::xpath_node_set components = doc_.select_nodes(confsintax::component);   
    for(auto currentComponent:components)
    {
        pugi::xml_node node=currentComponent.node();    
      
        auto components=createInOut(node);
        if(components)
        {
            depot_.push_back(components);
        }
    }
    
    return true;
}

InOut_sptr ConfigurationDepot::createInOut(const pugi::xml_node& node)
{
    InOut_sptr components;
    std::string protocol=node.attribute(confsintax::protocol).value();
    bool enable=node.attribute(confsintax::enable).as_bool();
    if(!enable)
        return InOut_sptr();

    switch(protocolTypeLookup[protocol])
        {
            case (uint8_t)ComponentType::udpbroadcast:
            case (uint8_t)ComponentType::udp:
            {
                components=std::make_shared<ComponentUdp>(ios_,node,*this);
                return components;
            }
            case (uint8_t)ComponentType::file:                
            {
                components=std::make_shared<ComponentFile>(ios_,node,*this);
                return components;
            }
            case (uint8_t)ComponentType::console:                
            {
                components=std::make_shared<ComponentConsole>(ios_,node,*this);
                return components;
            }            
            default:
            {}
                //TODO error
        }
    return InOut_sptr();
}

bool ConfigurationDepot::route(EOMDiagnosticUdpMsg& msg,const std::string& destinations)
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