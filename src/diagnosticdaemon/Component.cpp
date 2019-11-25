/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

#include "Component.h"

#include <iostream>
/*
Component::Component(const std::map<std::string,std::string>& attributes,ConfigurationDepot& depot):depot_(depot),parameters_(pugi::xml_node())
{}
*/
Component::Component(const pugi::xml_node& node,ConfigurationDepot& depot):depot_(depot),parameters_(node)
{
    name_=node.attribute(confsintax::name).value();
    protocol_=node.attribute(confsintax::protocol).value();
    mode_=node.attribute(confsintax::mode).value();
    enable_=node.attribute(confsintax::enable).as_bool();
    destination_=node.attribute(confsintax::destination).value();
}