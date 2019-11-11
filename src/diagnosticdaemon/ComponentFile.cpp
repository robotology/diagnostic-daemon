/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

#include "ComponentFile.h"
#include "MsgDescriptionExt.h"
#include <iomanip>

ComponentFile::ComponentFile(boost::asio::io_service &io_service,const pugi::xml_node& node,ConfigurationDepot& depot):Component(io_service,node,depot)
{
    filename_=node.attribute(confsintax::value).value();
    fstream_.open(filename_);
}

void ComponentFile::acceptMsg(std::array<uint8_t,maxMsgLenght_>& msg,unsigned int size)
{
    for(size_t index=0;index<size;++index)
    {
        fstream_<< std::hex<< std::setfill('0') << std::setw(2)<<(int)msg[index]<<" ";
    }
    fstream_<<std::endl;    
}