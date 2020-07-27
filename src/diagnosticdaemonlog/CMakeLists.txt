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

# We are using <experimental/filesystem> because <filesystem> is available since gcc 8.0.
add_compile_definitions(${PROJECT_NAME} _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING)
target_link_libraries(${PROJECT_NAME}  ${Common_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                                  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)


set_target_properties(${PROJECT_NAME} PROPERTIES
                                      VERSION ${CMAKE_PROJECT_VERSION}
                                      PUBLIC_HEADER "${LogHdr}"
                                      OUTPUT_NAME "diagnosticdaemonlog")

# install
install(
TARGETS ${PROJECT_NAME}
EXPORT ${CMAKE_PROJECT_NAME}
LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
