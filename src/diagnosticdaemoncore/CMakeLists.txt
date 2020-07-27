################################################################################
#                                                                              #
# Copyright (C) 2019 Fondazione Istitito Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# @author Luca Tricerri <luca.tricerri@iit.it>

project(diagnosticdaemoncore)

set(DiagnosticDaemonCoreSrc     
                            ComponentUdp.cpp
                            ComponentFile.cpp
                            ComponentConsole.cpp
                            Component.cpp
                            ComponentDisabled.cpp
                            ConfigurationDepot.cpp
                            Decoder.cpp
                            ComponentDecoder.cpp
                            ComponentConfig.cpp
                            ComponentYarpLogger.cpp
                            ComponentSynch.cpp
                            MsgPrepare.cpp
                        )

set(DiagnosticDaemonCoreHdr ComponentUdp.h
                            ComponentFile.h
                            ComponentConsole.h
                            Component.h
                            ComponentDisabled.h
                            ConfigurationDepot.h
                            ElapsedTime.h
                            Syntax.h
                            Decoder.h
                            ComponentDecoder.h
                            ComponentYarpLogger.h
                            ComponentConfig.h
                            ComponentSynch.h
                            MsgPrepare.h
                       )

add_library(${PROJECT_NAME} ${DiagnosticDaemonCoreSrc} ${DiagnosticDaemonCoreHdr} )
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                                  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(${PROJECT_NAME}  ${Common_LIBRARIES}
                                       diagnosticdaemonlog
                                       pugixml::pugixml
                                       diagnosticdaemonparser
                                       icub_firmware_shared::embot
                                       icub_firmware_shared::embobj
                                       )

if (COMPILE_WITHYARP_DEF)
  target_compile_definitions(${PROJECT_NAME} PRIVATE -DCOMPILE_WITHYARP_DEF)
  set(YARP_MINIMUM_REQUIRED_VERSION 3.0.102)
  find_package(YARP REQUIRED COMPONENTS os conf)
  target_link_libraries(${PROJECT_NAME} YARP::YARP_os YARP::YARP_conf YARP::YARP_init)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
                                      VERSION ${CMAKE_PROJECT_VERSION}
                                      PUBLIC_HEADER "${DiagnosticDaemonCoreHdr}"
                                      OUTPUT_NAME "diagnosticdaemoncore")

# install
install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${CMAKE_PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(FILES ${CMAKE_CURRENT_LIST_DIR}/config.xml DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/msgready DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
