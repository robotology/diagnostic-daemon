/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

#include "Component.h"
#include "ConfigurationDepot.h"

#include <iostream>

Component::Component(const std::map<std::string,std::string>& attributes,ConfigurationDepot& depot):depot_(depot),parameters_(pugi::xml_node())
{
    name_=asString(confsintax::name,attributes);
    protocol_=asString(confsintax::protocol,attributes);
    mode_=asString(confsintax::mode,attributes);
    enable_=asBool(confsintax::enable,attributes);
    destination_=asString(confsintax::destination,attributes);
}

Component::Component(const pugi::xml_node& node,ConfigurationDepot& depot):depot_(depot),parameters_(node)
{
    name_=node.attribute(confsintax::name).value();
    protocol_=node.attribute(confsintax::protocol).value();
    mode_=node.attribute(confsintax::mode).value();
    enable_=node.attribute(confsintax::enable).as_bool();
    destination_=node.attribute(confsintax::destination).value();
}