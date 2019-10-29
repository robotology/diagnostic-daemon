#include <iostream>
#include <chrono>
#include <ctime>    
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "ConfigurationDepot.h"
#include "RopParser.h"

using boost::asio::ip::udp;

class ElapsedTime
{
  private:
    std::chrono::steady_clock::time_point start_;
    std::chrono::steady_clock::time_point end_;

  public:
  ElapsedTime()
  {
      start_ = std::chrono::steady_clock::now();
  }
  ~ElapsedTime()
  {
      end_ = std::chrono::steady_clock::now();
      std::cout<<std::chrono::duration_cast<std::chrono::milliseconds>(end_ - start_).count()<<std::endl;
  }
};

int main()
{
  try
  {
    boost::asio::io_service ios;

    ConfigurationDepot depot(ios);
    depot.createConfiguration();

    ios.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}