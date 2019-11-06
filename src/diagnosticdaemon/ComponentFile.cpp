#include "ComponentFile.h"
#include "MsgDescriptionExt.h"
#include <iomanip>

ComponentFile::ComponentFile(boost::asio::io_service &io_service,const pugi::xml_node& node,ConfigurationDepot& depot):Component(io_service,node,depot)
{
    filename_=node.attribute(confsintax::value).value();
    fstream_.open(filename_);
}

void ComponentFile::acceptMsg(EOMDiagnosticUdpMsg& msg) 
{
    msg.dump(&ropSeverity,&ropCode,&ropString,fstream_);
}

void ComponentFile::acceptMsg(std::array<uint8_t,maxMsgLenght_>& udpMsg)
{
    //EOMDiagnosticUdpMsg msg;
    //msg.parse(udpMsg);
    //acceptMsg(msg);

    for(uint8_t current:udpMsg)
    {
        fstream_<< std::hex<< std::setfill('0') << std::setw(2)<<(int)current<<" ";
    }
    fstream_<<std::endl;
}