# CMake script for configuring Newton Game Dynamics 2.30 on Windows
# Author: Toni Machado

if( NOT NEWTONGD_FOUND )

	#----------
	
	if( NOT NEWTONGD_DIR )
		set( NEWTONGD_DIR $ENV{NEWTONGD_ROOT} )
	endif( NOT NEWTONGD_DIR )

	#----------
		
	set( NEWTONGD_VERSION "2.30" CACHE STRING "Select the desired version" )
	set_property( CACHE NEWTONGD_VERSION PROPERTY STRINGS "2.30" "2.28" )

	#----------
	
	set( NEWTONGD_OS_TYPE NOT_DEFINED )
	
	#----------
	
	if( WIN32 )
		
		#----------
		
		set( NEWTONGD_OS_TYPE Windows )
		
		#----------
		
		set( NEWTONGD_INCLUDE_DIRS
			${NEWTONGD_DIR}/sdk
			${NEWTONGD_DIR}/sdk/dContainers
			${NEWTONGD_DIR}/sdk/dCustomJoints
			${NEWTONGD_DIR}/sdk/dMath
			${NEWTONGD_DIR}/sdk/dScene		
		)
		
		#----------
		
		Configure_System_Architecture()
				
		if( ${SYSTEM_ARCH} STREQUAL x86 )
			set( SYS_ARCH_DIR x32 )
		else( ${SYSTEM_ARCH} STREQUAL x86 )
			set( SYS_ARCH_DIR x64 )
		endif( ${SYSTEM_ARCH} STREQUAL x86 )
		
		#----------
		
		set( NEWTONGD_LINK_DIRS
			${NEWTONGD_DIR}/sdk/${SYS_ARCH_DIR}/dll_vs9
			${NEWTONGD_DIR}/sdk/${SYS_ARCH_DIR}/lib_vs9_md
		)

		if( ${NEWTONGD_VERSION} STREQUAL 2.30 )
			set( NEWTONGD_LIBRARIES
				debug		newton_d
				optimized 	newton
				debug		dContainers_d
				optimized	dContainers
				debug		dJointLibrary_d
				optimized 	dJointLibrary
				debug 		dMath_d
				optimized 	dMath
				debug		dScene_d
				optimized	dScene
				debug		tinyxml_d.lib
				optimized	tinyxml.lib
			)
		elseif( ${NEWTONGD_VERSION} STREQUAL 2.28 )
			set( NEWTONGD_LIBRARIES
				newton
				debug dContainers_d
				optimized dContainers
				debug dJointLibrary_d
				optimized dJointLibrary
				debug dMath_d
				optimized dMath
				debug dScene_d
				optimized dScene
			)
		endif( ${NEWTONGD_VERSION} STREQUAL 2.30 )
		
	else( WIN32 )
	
		message( STATUS "The NewtonGD configuration package, currently, just supports Windows operating systems." )
		
	endif( WIN32 )
			
endif( NOT NEWTONGD_FOUND )

#----------

if( NEWTONGD_INCLUDE_DIRS AND NEWTONGD_LINK_DIRS AND NEWTONGD_LIBRARIES )
	
		include_directories( ${NEWTONGD_INCLUDE_DIRS} )
		link_directories( ${NEWTONGD_LINK_DIRS} )
		set_property( GLOBAL APPEND PROPERTY LINK_LIBRARIES ${NEWTONGD_LIBRARIES} )
		
		set( NEWTONGD_FOUND TRUE )
		message( STATUS "Configuring Newton Game Dynamics " ${NEWTONGD_VERSION} " to be used in " ${NEWTONGD_OS_TYPE} " OS " ${SYSTEM_ARCH} "."  )
		
endif( NEWTONGD_INCLUDE_DIRS AND NEWTONGD_LINK_DIRS AND NEWTONGD_LIBRARIES )
