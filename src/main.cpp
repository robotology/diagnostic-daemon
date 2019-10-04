#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "ServerUdp.h"
#include "ConsoleCommand.h"
#include "DiagnosticParser.h"

using boost::asio::ip::udp;

int main()
{
  try
  {
    boost::asio::io_service ios;

    DiagnosticParser parser;
    ServerUdp server(ios,parser);
    ConsoleCommand console(server);

    ios.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}