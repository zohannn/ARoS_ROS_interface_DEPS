cmake_minimum_required( VERSION 2.8 )
cmake_policy( VERSION ${CMAKE_VERSION} )

include( $ENV{MAINPROJECT}/Libraries/CMakeScripts/CommonScripts.cmake )

project( cvBlob )

#--------------------------------------------------------------------------------------------
# Configuration of necessary packages	
Configure_Package( OpenCV )

#--------------------------------------------------------------------------------------------
# Apply standard library configuration settings
Setup_Library_Standard_Config()
