#include "Server.h"

Server::Server(boost::asio::io_service &ios,const pugi::xml_node& node,ConfigurationDepot& depot):ios_(ios),parameters_(node),depot_(depot)
{
    name_=node.attribute(confsintax::name).value();
    protocol_=node.attribute(confsintax::protocol).value();
    mode_=node.attribute(confsintax::mode).value();
    enable_=node.attribute(confsintax::enable).as_bool();
    destination_=node.attribute(confsintax::destination).value();
}