################################################################################
#                                                                              #
# Copyright (C) 2019 Fondazione Istitito Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# @author Luca Tricerri <luca.tricerri@iit.it>

project(diagnosticdaemonlog)

set(LogSrc  Log.cpp
                        )

set(LogHdr  Log.h
                       )
add_library(${PROJECT_NAME} ${LogSrc} ${LogHdr} )

target_link_libraries(${PROJECT_NAME}  ${Common_LIBRARIES} stdc++fs)
target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>")

# install
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
