################################################################################
#                                                                              #
# Copyright (C) 2019 Fondazione Istitito Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# @author Luca Tricerri <luca.tricerri@iit.it> 
set(EM_DIR ../../../icub-firmware/emBODY/eBcode/arch-arm/)
set(FIRMWARE-SHARED_DIR ../../../icub-firmware-shared/eth/)

set(DiagnosticDaemonSrc     main.cpp
                            ComponentUdp.cpp
                            ComponentFile.cpp
                            ComponentConsole.cpp
                            pugixml.cpp
                            Component.cpp
                            ConfigurationDepot.cpp
                            ${EM_DIR}/embot/eprot/ROPstream.cpp
                            ${EM_DIR}/embot/eprot/ROPframe.cpp 
                            ${EM_DIR}/embot/eprot/ROPdescriptor.cpp
                            ${EM_DIR}/embot/eprot/DiagnosticsHost.cpp
                            ${EM_DIR}/embot/common/embot_common.cpp
                            ${EM_DIR}/embot/binary/embot_binary.cpp
                            ${FIRMWARE-SHARED_DIR}/embobj/plus/comm-v2/icub/EoError.c
                        )
                        
set(DiagnosticDaemonHdr     ComponentUdp.h
                            ComponentFile.h
                            ComponentConsole.h
                            Component.h
                            ConfigurationDepot.h
                            ElapsedTime.h
                            pugixml.hpp
                            pugiconfig.hpp            
                       )

include_directories(${Boost_INCLUDE_DIRS})
include_directories(
                    ../../../icub-firmware-shared/eth/embobj/core/core
                    ../../../icub-firmware-shared/eth/embobj/plus/comm-v2/icub
                    ../../../icub-firmware/emBODY/eBcode/arch-arm/embot/eprot
                    ../../../icub-firmware/emBODY/eBcode/arch-arm/embot/common
                    ../../../icub-firmware/emBODY/eBcode/arch-arm/embot/binary)
                    
add_executable(diagnosticdaemon ${DiagnosticDaemonSrc} ${DiagnosticDaemonHdr} )

find_package(Boost 1.67 REQUIRED COMPONENTS system REQUIRED)

target_link_libraries(diagnosticdaemon 
  ${Boost_SYSTEM_LIBRARY}
  pthread
)

file(COPY ${CMAKE_CURRENT_LIST_DIR}/config.xml DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

# install
install(TARGETS diagnosticdaemon RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})