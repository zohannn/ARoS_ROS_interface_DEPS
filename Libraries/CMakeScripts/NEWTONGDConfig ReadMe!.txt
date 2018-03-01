The file NEWTONGDConfig.cmake is provided to allow automatic configuration of
Newton Game Dynamics for Windows using CMake's find_package instruction.

------------------------------------------------------------------------------

How to use:

	Copy 'NEWTONGDConfig.cmake' file to where Newton Game Dynamics library is installed, tipically $ENV{NEWTONGD_ROOT}
	
	then on the project's CMakeLists simply use:
		
		set( NEWTONGD_DIR	$ENV{NEWTONGD_ROOT} )
		find_package( NEWTONGD REQUIRED )

All done!