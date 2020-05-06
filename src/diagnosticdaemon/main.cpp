/*
 * Copyright (C) 2019 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
*/

// - brief
//   Main entry point
//

#include <iostream>
#include <boost/asio.hpp>

#include "ConfigurationDepot.h"

using boost::asio::ip::udp;

int main()
{
  try
  {
    std::cout << "Start" << std::endl;
    boost::asio::io_service ios;

    ConfigurationDepot depot(ios);
    depot.createConfiguration();

    ios.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
  std::cout << "Stop" << std::endl;
  return 0;
}