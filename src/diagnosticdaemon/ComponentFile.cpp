/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

#include "ComponentFile.h"
#include "ConfigurationDepot.h"
#include <iomanip>


ComponentFile::ComponentFile(const std::map<std::string,std::string>& attributes,ConfigurationDepot& depot):Component(attributes,depot)
{
    filename_=asString(confsintax::value,attributes);
    fstream_.open(filename_);
}

void ComponentFile::acceptMsg(std::string& msg,unsigned int,udp::endpoint,Severity)
{
    fstream_<< msg<<std::endl;    
}

void ComponentFile::acceptMsg(std::array<uint8_t,maxMsgLenght_>& msg,unsigned int size,udp::endpoint,Severity)
{
    for(size_t index=0;index<size;++index)
    {
        fstream_<< std::hex<< std::setfill('0') << std::setw(2)<<(int)msg[index]<<" ";
    }
    fstream_<<std::endl;    
}