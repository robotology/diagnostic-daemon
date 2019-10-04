################################################################################
#                                                                              #
# Copyright (C) 2019 Fondazione Istitito Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# @author Luca Tricerri <luca.tricerri@iit.it> 
include(CMakePackageConfigHelpers)

set(DiagnosticDaemonSrc     main.cpp
                            ServerUdp.cpp
                            ConsoleCommand.cpp
                        )

set(DiagnosticDaemonHdr     ServerUdp.h
                            ConsoleCommand.h
                       )



include_directories(${Boost_INCLUDE_DIRS})
include_directories(../../icub-firmware-shared/eth/embot ../../icub-firmware-shared/eth/embobj/core/core)
add_executable(diagnosticdaemon ${DiagnosticDaemonSrc} ${DiagnosticDaemonHdr} )

find_package(Boost COMPONENTS system filesystem REQUIRED)
target_link_libraries(diagnosticdaemon 
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  pthread
)


# install
install(TARGETS diagnosticdaemon RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})