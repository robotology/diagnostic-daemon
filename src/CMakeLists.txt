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
FetchContent_GetProperties(pugixml)
if(NOT pugixml)
	FetchContent_Populate(pugixml)
	add_subdirectory(${pugixml_SOURCE_DIR} ${pugixml_BINARY_DIR})
endif()

add_subdirectory(diagnosticdaemon)
add_subdirectory(parser)
#add_subdirectory(diagnosticdaemonui)
