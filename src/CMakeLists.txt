################################################################################
#                                                                              #
# Copyright (C) 2019 Fondazione Istitito Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# @author Luca Tricerri <luca.tricerri@iit.it> 

include(FetchContent)

FetchContent_Declare(
	pugixml
	GIT_REPOSITORY https://github.com/zeux/pugixml.git
	GIT_TAG        origin/master
)

if(DIAGNOSTICDAEMON_USE_EXTERNAL_PUGIXML)
  find_package(pugixml REQUIRED)
else()
  FetchContent_GetProperties(pugixml)
  if(NOT pugixml)
	  FetchContent_Populate(pugixml)
	  add_subdirectory(${pugixml_SOURCE_DIR} ${pugixml_BINARY_DIR})
  endif()
endif()



option(COMPILE_WITHUNITTEST "Compile also unittest" OFF)

# The unit tests uses io_service::run_for that is available from boost 1.66
if(COMPILE_WITHUNITTEST AND Boost_VERSION_STRING GREATER_EQUAL 1.66.0)
  FetchContent_Declare(
    unittest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        origin/master
  )
  FetchContent_GetProperties(unittest)
  if(NOT unittest)
    FetchContent_Populate(unittest)
    add_subdirectory(unittest)
    add_subdirectory(${unittest_SOURCE_DIR} ${unittest_BINARY_DIR})
  endif()
endif()

add_subdirectory(diagnosticdaemonlog)
add_subdirectory(diagnosticdaemonparser)
add_subdirectory(diagnosticdaemoncore)
add_subdirectory(diagnosticdaemon)
#add_subdirectory(diagnosticdaemonui)

