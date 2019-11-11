################################################################################
#                                                                              #
# Copyright (C) 2019 Fondazione Istitito Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# @author Luca Tricerri <luca.tricerri@iit.it> 
include(CMakePackageConfigHelpers)

set(DiagnosticDaemonSrc     main.cpp
                            ComponentUdp.cpp
                            ComponentFile.cpp
                            ComponentConsole.cpp
                            RopParser.cpp
                            pugixml.cpp
                            Component.cpp
                            ConfigurationDepot.cpp
                            ../../../icub-firmware-shared/eth/embot/DiagnosticsHost.cpp
                            ../../../icub-firmware-shared/eth/embobj/plus/comm-v2/icub/EoError.c
                            ../../../icub-firmware-shared/eth/embot/ROPstream.cpp
                            ../../../icub-firmware-shared/eth/embot/ROPframe.cpp 
                            ../../../icub-firmware-shared/eth/embot/ROPdescriptor.cpp
                        )

set(DiagnosticDaemonHdr     ComponentUdp.h
                            ComponentFile.h
                            ComponentConsole.h
                            RopParser.h
                            Component.h
                            ConfigurationDepot.h            
                       )



include_directories(${Boost_INCLUDE_DIRS})
include_directories(../../../icub-firmware-shared/eth/embobj/plus/comm-v2/icub ../../../icub-firmware-shared/eth/embot ../../../icub-firmware-shared/eth/embobj/core/core ../../../icub-firmware ../../../icub-firmware/emBODY/eBcode/arch-arm/embot/binary)
add_executable(diagnosticdaemon ${DiagnosticDaemonSrc} ${DiagnosticDaemonHdr} )

find_package(Boost COMPONENTS system filesystem REQUIRED)
target_link_libraries(diagnosticdaemon 
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  pthread
)

file(COPY ${CMAKE_CURRENT_LIST_DIR}/config.xml DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

# install
install(TARGETS diagnosticdaemon RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})