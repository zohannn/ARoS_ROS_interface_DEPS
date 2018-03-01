The file OPENGLUTConfig.cmake is provided to allow automatic configuration of
OpenGL Utilities v3.7.6 for Windows using CMake's find_package instruction.

------------------------------------------------------------------------------

How to use:

	Copy 'OPENGLUTConfig.cmake' file to where OpenGL Utilities v3.7.6 library is installed, tipically $ENV{OPENGLUT_ROOT}
	
	then on the project's CMakeLists simply use:
		
		set( OPENGLUT_DIR	$ENV{OPENGLUT_ROOT} )
		find_package( OPENGLUT REQUIRED )

All done!