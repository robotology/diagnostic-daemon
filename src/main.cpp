#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "ServerUdp.h"

using boost::asio::ip::udp;

int main()
{
  try
  {

    boost::asio::io_service ios;

    using namespace std; // For atoi.
    ServerUdp server(ios);

    std::array<unsigned char,1024> message{65,65};
    server.send(message);

    ios.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}