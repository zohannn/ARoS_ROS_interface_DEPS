cmake_minimum_required(VERSION 2.4)
cmake_policy( VERSION 2.8.3 )

#--------------------------------------------------------------------------------------------
# Other projects that are required
	
if( NOT TARGET CommonDef )
	set( CommonDef_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/CommonCoopDyn/CommonDef )
	add_subdirectory( ${CommonDef_DIR} ${CommonDef_BUILD_DIR} )
endif( NOT TARGET CommonDef )

#--------------------------------------------------------------------------------------------

include_directories(
	${CommonDef_DIR}
) 

#--------------------------------------------------------------------------------------------
# Linker definitions

link_directories(
	${CommonDef_BUILD_DIR}/${CMAKE_BUILD_TYPE}
	)

link_libraries( CommonDef )

#--------------------------------------------------------------------------------------------

set( CommonDef_FOUND TRUE )