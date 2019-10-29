#include "Component.h"

Component::Component(boost::asio::io_service &ios,const pugi::xml_node& node,ConfigurationDepot& depot):ios_(ios),depot_(depot),parameters_(node)
{
    name_=node.attribute(confsintax::name).value();
    protocol_=node.attribute(confsintax::protocol).value();
    mode_=node.attribute(confsintax::mode).value();
    enable_=node.attribute(confsintax::enable).as_bool();
    destination_=node.attribute(confsintax::destination).value();
}