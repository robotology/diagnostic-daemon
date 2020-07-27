################################################################################
#                                                                              #
# Copyright (C) 2020 Fondazione Istitito Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# @author Luca Tricerri <luca.tricerri@iit.it>

include(FetchContent)

project(diagnosticdaemonparser)

set(ParserSrc     Parser.cpp
                  BitStream.cpp )
set(ParserHdr     Parser.h 
                  BitStream.h)

add_library(${PROJECT_NAME} ${ParserSrc} ${ParserHdr} )
target_link_libraries(${PROJECT_NAME} pugixml::pugixml diagnosticdaemonlog)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                                  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)


set_target_properties(${PROJECT_NAME} PROPERTIES
                                      VERSION ${CMAKE_PROJECT_VERSION}
                                      PUBLIC_HEADER "${ParserHdr}"
                                      OUTPUT_NAME "diagnosticdaemonparser")

# install
install(
TARGETS ${PROJECT_NAME}
EXPORT ${CMAKE_PROJECT_NAME}
LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# install
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/msgdepot DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
