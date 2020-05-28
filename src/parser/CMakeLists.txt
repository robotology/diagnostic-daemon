################################################################################
#                                                                              #
# Copyright (C) 2020 Fondazione Istitito Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# @author Luca Tricerri <luca.tricerri@iit.it>

include(FetchContent)



project(parser)

#option(COMPILE_WITHUNITTEST "Compile also unittest for parser" OFF)
#if(COMPILE_WITHUNITTEST)
#  FetchContent_Declare(
#    unittest
#    GIT_REPOSITORY https://github.com/google/googletest.git
#    GIT_TAG        origin/master
#  )
#  FetchContent_GetProperties(unittest)
#  if(NOT unittest)
#    FetchContent_Populate(unittest)
#    add_subdirectory(unittest)
#    #add_subdirectory(${unittest_SOURCE_DIR} ${unittest_BINARY_DIR})
#  endif()
#endif()

set(ParserSrc     Parser.cpp
                  BitStream.cpp )
set(ParserHdr     Parser.h 
                  BitStream.h)

add_library(${PROJECT_NAME} ${ParserSrc} ${ParserHdr} )
target_link_libraries(${PROJECT_NAME} pugixml::pugixml)
target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>")

# install
install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR})
