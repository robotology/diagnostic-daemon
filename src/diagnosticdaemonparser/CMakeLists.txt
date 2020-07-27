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
target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>")

# install
install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/msgdepot DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
