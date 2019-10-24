#include "ComponentFile.h"
#include "MsgDescriptionExt.h"

ComponentFile::ComponentFile(boost::asio::io_service &io_service,const pugi::xml_node& node,ConfigurationDepot& depot):Component(io_service,node,depot)
{
    filename_=node.attribute(confsintax::value).value();
    fstream_.open(filename_);
}

void ComponentFile::acceptMsg(EOMDiagnosticUdpMsg& msg) 
{
    msg.dump(&ropSeverity,&ropCode,&ropString,fstream_);
};