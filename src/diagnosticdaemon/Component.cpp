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
    name_=asString(confsyntax::name,attributes);
    type_=asString(confsyntax::type,attributes);
    mode_=asString(confsyntax::mode,attributes);
    enable_=asBool(confsyntax::enable,attributes);
    destination_=asString(confsyntax::destination,attributes);
}

const std::map<std::string,std::string> Component::getParameterMap() const
{
    return parametersMap_;
}

