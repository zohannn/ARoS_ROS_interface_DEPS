The file CMU1394Config.cmake is provided to allow automatic configuration of
Intel Carnegie Mellon API for Windows using CMake's find_package instruction.

How to use:

Copy this file to where CMU1394 is installed, tipically $ENV{CMU1394_ROOT}

then on the project's CMakeLists simply use:

set( CMU1394_DIR	$ENV{CMU1394_ROOT} )
find_package( CMU1394 REQUIRED )


All done!