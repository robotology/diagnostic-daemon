#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "ServerUdp.h"
#include "ConsoleCommand.h"
#include "RopParser.h"

using boost::asio::ip::udp;

int main()
{
  try
  {
    boost::asio::io_service ios;

    EOMDiagnosticRopMsg rop=EOMDiagnosticRopMsg(EOMDiagnosticRopMsg::Info{(uint16_t)DiagnosticRopCode::stoplog,(uint16_t)DiagnosticRopSeverity::critical,3,4,5,6,7,8,9});
    RopParser parser;
    parser.parse(rop);
    parser.dump();
    return 1;
    ServerUdp server(ios);
    ConsoleCommand console(server);

    ios.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}