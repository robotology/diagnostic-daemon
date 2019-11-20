/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

#include "Component.h"

#include <iostream>

Component::Component(boost::asio::io_service &ios,const pugi::xml_node& node,ConfigurationDepot& depot):ios_(ios),depot_(depot),parameters_(node)
{
    name_=node.attribute(confsintax::name).value();
    protocol_=node.attribute(confsintax::protocol).value();
    mode_=node.attribute(confsintax::mode).value();
    enable_=node.attribute(confsintax::enable).as_bool();
    destination_=node.attribute(confsintax::destination).value();
}