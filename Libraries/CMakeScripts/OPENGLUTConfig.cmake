# CMake script for configuring OpenGL Glut v3.7.6 on Windows
# Author: Toni Machado

IF( NOT OPENGLUT_FOUND )

	if( WIN32 )
	
		if( NOT OPENGLUT_DIR )
			set( OPENGLUT_DIR $ENV{OPENGL_ROOT} )
		endif( NOT OPENGLUT_DIR )

		set( OPENGLUT_INCLUDE_DIRS
			${OPENGLUT_DIR}/include
		)
		
		set( OPENGLUT_LINK_DIRS
			${OPENGLUT_DIR}/bin
		)

		if( CMAKE_CL_64 )
			set( OPENGLUT_LIBRARIES
			glut64
		)
		else()
			set( OPENGLUT_LIBRARIES
			glut32
		)
		endif()
		

	else( WIN32 )
	
		message( "This OPENGLUT CMakePackage only has support for Windows systems." )
	
	endif( WIN32 )

endif( NOT OPENGLUT_FOUND )

#----------

if( OPENGLUT_INCLUDE_DIRS AND OPENGLUT_LINK_DIRS AND OPENGLUT_LIBRARIES )

	include_directories( ${OPENGLUT_INCLUDE_DIRS} )
	link_directories( ${OPENGLUT_LINK_DIRS} )
	set_property( GLOBAL APPEND PROPERTY LINK_LIBRARIES ${OPENGLUT_LIBRARIES} )

	set( OPENGLUT_FOUND TRUE )
	message( STATUS "Using OpenGL Utilities v3.7.6." )
	
endif( OPENGLUT_INCLUDE_DIRS AND OPENGLUT_LINK_DIRS AND OPENGLUT_LIBRARIES )