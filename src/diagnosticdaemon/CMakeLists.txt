################################################################################
#                                                                              #
# Copyright (C) 2019 Fondazione Istitito Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# @author Luca Tricerri <luca.tricerri@iit.it>

project(diagnosticdaemon)

set(DiagnosticDaemonSrc     main.cpp
                          
                        )

set(DiagnosticDaemonHdr    
                           
                       )

add_executable(${PROJECT_NAME} ${DiagnosticDaemonSrc} ${DiagnosticDaemonHdr} )

target_link_libraries(${PROJECT_NAME}  ${Common_LIBRARIES}
                                       diagnosticdaemonlog
                                       diagnosticdaemoncore
                                       pugixml::pugixml
                                       diagnosticdaemonparser
                                       icub_firmware_shared::embot
                                       icub_firmware_shared::embobj
                                       Boost::system
                                       )

# install
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
