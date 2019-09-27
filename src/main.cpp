#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;


int main()
{
  try
  {

    boost::asio::io_service ios;

    using namespace std; // For atoi.
//    server s(io_service, atoi(argv[1]));

    ios.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}