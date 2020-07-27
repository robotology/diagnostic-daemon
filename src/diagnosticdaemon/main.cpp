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
    Log(Severity::info) << "Start" << std::endl;
    boost::asio::io_service ios;

    ConfigurationDepot depot(ios);
    depot.createConfiguration();

    ios.run();
  }
  catch (std::exception& e)
  {
    Log(Severity::error) << "Exception: " << e.what() << "\n";
  }
  Log(Severity::info) << "Stop" << std::endl;
  return 0;
}