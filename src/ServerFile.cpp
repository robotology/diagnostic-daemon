#include "ServerFile.h"
#include "MsgDescriptionExt.h"

ServerFile::ServerFile(boost::asio::io_service &io_service,const pugi::xml_node& node,ConfigurationDepot& depot):Server(io_service,node,depot)
{
    filename_=node.attribute(confsintax::value).value();
    fstream_.open(filename_);
}

void ServerFile::acceptMsg(EOMDiagnosticUdpMsg& msg) 
{
    msg.dump(&ropSeverity,&ropCode,&ropString,fstream_);
};