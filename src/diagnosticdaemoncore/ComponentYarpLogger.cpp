/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

#include "ComponentYarpLogger.h"
#include "ConfigurationDepot.h"
#include "Log.h"

#include <iomanip>
#include <fstream>

ComponentYarpLogger::ComponentYarpLogger(const std::map<std::string,std::string>& attributes,ConfigurationDepot& depot):Component(attributes,depot)
{
#ifdef COMPILE_WITHYARP_DEF
    yarp::os::Network::init();
#endif       
}

void ComponentYarpLogger::acceptMsg(std::string& msg,unsigned int ,udp::endpoint senderEndPoint,Severity severity)
{
    Log(Severity::debug)<<"ComponentYarpLogger"<<std::endl;
    std::stringstream ss;
    ss<<senderEndPoint;
    forewardtoYarpLogger(msg,severity);    
}

void ComponentYarpLogger::acceptMsg(std::array<uint8_t,maxMsgLenght_>&,unsigned int,udp::endpoint,Severity)
{
    Log(Severity::debug)<<"ComponentYarpLogger"<<std::endl;
    Log(Severity::error)<<"Todo"<<std::endl;
}


#ifdef COMPILE_WITHYARP_DEF
void ComponentYarpLogger::forewardtoYarpLogger(const std::string& data,Severity severity)
{
    switch (severity)
    {
        case Severity::info:
            yInfo()<<data;
            break;
        case Severity::debug:
            yDebug()<<data;;
            break;
        case Severity::warning:
            yWarning()<<data;;
            break;
        case Severity::error:
            yError()<<data;;
            break;
        default:
        case Severity::fatal:
            yFatal()<<data;;
            break;
    }

}
#else
void ComponentYarpLogger::forewardtoYarpLogger(const std::string& ,Severity ){};
#endif    