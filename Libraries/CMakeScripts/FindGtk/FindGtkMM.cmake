# Copyright: (C) 2009 RobotCub Consortium
# Authors: Giorgio Metta, Lorenzo Natale
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

IF(UNIX)
    INCLUDE(FindGtkMMUnix)
ELSE(UNIX)
    IF(WIN32 AND NOT CYGWIN)
        INCLUDE(FindGtkMMWin32)
    ENDIF(WIN32 AND NOT CYGWIN)
ENDIF(UNIX)

#----------

if( GtkMM_FOUND )

	include_directories( ${GtkMM_INCLUDE_DIRS} )
	link_libraries( ${GtkMM_LIBRARIES} )

	message( STATUS "Using GtkMM v." ${GtkMM_VERSION} "." ${GtkMM_MAJOR_VERSION} "." ${GtkMM_MINOR_VERSION} )
	
endif( GtkMM_FOUND )
# GtkMM_INCLUDE_DIRS   - Directories to include to use GTKMM
# GtkMM_LIBRARIES      - Files to link against to use GTKMM
# GtkMM_C_FLAGS        - Flags to pass to the C/C++ compiler for GTKMM.
# GtkMM_FOUND          - If false, don't try to use GTKMM
# GtkMM_VERSION        - version info
# GtkMM_MAJOR_VERSION  - major version
# GtkMM_MINOR_VERSION  - minor version