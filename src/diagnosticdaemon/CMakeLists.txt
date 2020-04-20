################################################################################
#                                                                              #
# Copyright (C) 2019 Fondazione Istitito Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# @author Luca Tricerri <luca.tricerri@iit.it>

set(DiagnosticDaemonSrc     main.cpp
                            ComponentUdp.cpp
                            ComponentFile.cpp
                            ComponentConsole.cpp
                            pugixml.cpp
                            Component.cpp
                            ComponentDisabled.cpp
                            ConfigurationDepot.cpp
                            Decoder.cpp
                            ComponentDecoder.cpp
                            ComponentConfig.cpp
                            ComponentYarpLogger.cpp
                        )

set(DiagnosticDaemonHdr     ComponentUdp.h
                            ComponentFile.h
                            ComponentConsole.h
                            Component.h
                            ComponentDisabled.h
                            ConfigurationDepot.h
                            ElapsedTime.h
                            Sintax.h
                            pugixml.hpp
                            pugiconfig.hpp
                            Decoder.h
                            ComponentDecoder.h
                            ComponentYarpLogger.h
                            ComponentConfig.h
                            Log.h
                       )

add_executable(diagnosticdaemon ${DiagnosticDaemonSrc} ${DiagnosticDaemonHdr} )

target_link_libraries(diagnosticdaemon pthread
                                       Boost::system
                                       icub_firmware_shared::embot
                                       icub_firmware_shared::embobj
                                       )

if (COMPILE_WITHYARP_DEF)
  target_compile_definitions(diagnosticdaemon PRIVATE -DCOMPILE_WITHYARP_DEF)
  set(YARP_MINIMUM_REQUIRED_VERSION 3.0.102)
  find_package(YARP REQUIRED COMPONENTS os conf)
  target_link_libraries(diagnosticdaemon YARP::YARP_os YARP::YARP_conf YARP::YARP_init)
endif()

# install
install(TARGETS diagnosticdaemon RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${CMAKE_CURRENT_LIST_DIR}/config.xml DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
