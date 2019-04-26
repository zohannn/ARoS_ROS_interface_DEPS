#-------------------------------------------------------------------------------
# CMake minimum required version and used policy
cmake_minimum_required( VERSION 2.8 )
cmake_policy( VERSION ${CMAKE_VERSION} )
#-------------------------------------------------------------------------------

project( AmtecLWArm )

#-------------------------------------------------------------------------------
# Test environment variable MAINPROJECT
if( DEFINED ENV{MAINPROJECT} )

	set( MAIN_PROJECT $ENV{MAINPROJECT} )
	set( LIBRARIES_DIR ${MAIN_PROJECT}/Libraries )

	include( ${LIBRARIES_DIR}/CMakeScripts/CommonScripts.cmake )
	
	set( MODULES_DIR $ENV{MAINPROJECT}/Hardware/YARP_Modules )
	set( WRAPPERS_DIR $ENV{MAINPROJECT}/Hardware/YARP_Wrappers )

else()
	message( SEND_ERROR "Environment variable 'MAINPROJECT' not defined!" )
endif()

#-------------------------------------------------------------------------------
# Define global MSVC heap allocation parameters
Setup_Global_MSVC_Heap_Allocation( ON 0 )

#--------------------------------------------------------------------------------------------
# Boost
if( DEFINED ENV{Boost_ROOT} )
	set( USE_Boost ON CACHE BOOL "Link Boost in ${PROJECT_NAME}? Only required if YARP uses Boost, to fix dependencies." )
	if( USE_Boost )
		#set(Boost_USE_STATIC_LIBS ON)
		find_package( Boost REQUIRED thread)
		include_directories( ${Boost_INCLUDE_DIRS} )
		link_directories( ${Boost_LIBRARY_DIRS} )
	endif()
endif()

#-------------------------------------------------------------------------------

SubDirList( SUB_DIRECTORIES_LIST ${CMAKE_CURRENT_SOURCE_DIR} )

# include_directories( 
	# ${SUB_DIRECTORIES_LIST}
	# ${MODULES_DIR}/amtec_lwa_7dof
# ) 

# Configure_Package( Amtec_PStepServer ${MAIN_PROJECT}/Hardware/YARP_Wrappers/Amtec_PStepServer )

#-------------------------------------------------------------------------------
# Directories with source code to be compiled
set( SRC_DIRS 
	${SUB_DIRECTORIES_LIST}
	Core
	Arm
	Communications
	${MODULES_DIR}/amtec_lwa_7dof
	${MODULES_DIR}/barrett_hand_826X
	${MODULES_DIR}/serial_asio
	${WRAPPERS_DIR}/Amtec_PStepServer/include
	${WRAPPERS_DIR}/Amtec_PStepServer/src
)

#Project dependencies include directories
include_directories(
	${SRC_DIRS}
	$ENV{ACE_ROOT}
)

link_directories(
	${MODULES_DIR}/amtec_lwa_7dof/lib
	$ENV{ACE_ROOT}/lib
)

	
Configure_Package(YARP)
Configure_Package(FilesDirs)
Configure_Package(YarpCommunication)

#-------------------------------------------------------------------------------
# Create groups of files: Header and Source files

set( HEADER_EXTENSIONS *.h *.hpp )
set( SOURCE_EXTENSIONS *.cpp )

# Look through all external source directories for source and header files
foreach( source_dir ${SRC_DIRS} )

	# Find source files in directories
	foreach( source_ext ${SOURCE_EXTENSIONS} )
		file( GLOB_RECURSE FOUND_FILES ${source_dir}/${source_ext} )
		set( SOURCE_FILES ${SOURCE_FILES} ${FOUND_FILES} )
	endforeach()
	
	# Find header files in directories
	foreach( header_ext ${HEADER_EXTENSIONS} )
		file( GLOB_RECURSE FOUND_FILES ${source_dir}/${header_ext} )
		set( HEADER_FILES ${HEADER_FILES} ${FOUND_FILES} )
	endforeach()

endforeach()

# Create a list with resource files
#file( GLOB RESOURCE_FILES res/*.rc* res/*.ico )

# Exclude all source and header files in build folder
file( GLOB_RECURSE FILES_TO_REMOVE ${CMAKE_CURRENT_BINARY_DIR}/*.* )
list( REMOVE_ITEM HEADER_FILES ${FILES_TO_REMOVE} )
list( REMOVE_ITEM SOURCE_FILES ${FILES_TO_REMOVE} )

Create_File_Groups(  "${HEADER_FILES}" "${SOURCE_FILES}")

#-------------------------------------------------------------------------------
# Define what files to be compiled for the application
set( myApp_SRCS
	${SOURCE_FILES}
	${HEADER_FILES}
	#${RESOURCE_FILES}
)

#--------------------------------------------------------------------------------------------

#create the executable
add_executable( ${PROJECT_NAME} ${myApp_SRCS} )

target_link_libraries( ${PROJECT_NAME} ${MODULES_DIR}/amtec_lwa_7dof/lib/m5apiw32.lib )
target_link_libraries( ${PROJECT_NAME} $ENV{ACE_ROOT}/lib/ACEd.lib )
Link_libraries_to_project()

#-------------------------------------------------------------------------------

# Setup_Application_Standard_Config(
	# YARP
	# FilesDirs
	# YarpCommunication
# )
