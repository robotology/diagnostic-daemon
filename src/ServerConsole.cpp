#include "ServerConsole.h"
#include "MsgDescriptionExt.h"

ServerConsole::ServerConsole(boost::asio::io_service &io_service,const pugi::xml_node& node,ConfigurationDepot& depot):Server(io_service,node,depot)
{
}

void ServerConsole::acceptMsg(const EOMDiagnosticUdpMsg& msg) 
{
    msg.dump(&ropSeverity,&ropCode,&ropString,std::cout);
};