/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

#include "Component.h"
#include "ConfigurationDepot.h"

#include <iostream>

Component::Component(const std::map<std::string,std::string>& attributes,ConfigurationDepot& depot):depot_(depot),parametersMap_(attributes)
{
    name_=asString(confsintax::name,attributes);
    type_=asString(confsintax::type,attributes);
    mode_=asString(confsintax::mode,attributes);
    enable_=asBool(confsintax::enable,attributes);
    destination_=asString(confsintax::destination,attributes);
}

const std::map<std::string,std::string> Component::getParameterMap() const
{
    return parametersMap_;
}

