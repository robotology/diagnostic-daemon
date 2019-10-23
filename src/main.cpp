#include <iostream>
#include <chrono>
#include <ctime>    
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "ConfigurationDepot.h"
#include "ConsoleCommand.h"
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
    /*
    EOMDiagnosticRopMsg rop=EOMDiagnosticRopMsg(EOMDiagnosticRopMsg::Info{(uint16_t)DiagnosticRopCode::stoplog,(uint16_t)DiagnosticRopSeverity::critical,3,4,5,6,7,8,9});
    RopParser parser;
    {
      ElapsedTime elapsed;
      for(int t=0;t<100000;++t)
        parser.parse(rop);
    }  
    parser.dump();
    return 1;
    */
    //ServerUdp server(ios,pugi::xml_node());
    //ConsoleCommand console(server);
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