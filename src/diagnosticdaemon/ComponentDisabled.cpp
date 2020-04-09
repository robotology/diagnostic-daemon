/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

#include "ComponentDisabled.h"
#include "ConfigurationDepot.h"
#include <iomanip>


ComponentDisabled::ComponentDisabled(const std::map<std::string,std::string>& attributes,ConfigurationDepot& depot):Component(attributes,depot)
{
}

void ComponentDisabled::acceptMsg(std::array<uint8_t,maxMsgLenght_>& ,unsigned int ,udp::endpoint,Severity severity )
{  
}