#
# Searches and hopefully finds gtk on windows -- by nat.

# Copyright: (C) 2009 RobotCub Consortium
# Authors: Lorenzo Natale
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

# Assumes that the environment variable GTK_BASEPATH is set to the place
# where GTK libs have been unpacked/installed. This is the default 
# behavior of glade for win32, users that want to install gtk manually
# must define GTK_BASEPATH accordingly.
#
# Creates:
# GTK_INCLUDE_DIR   - Directories to include to use GTK
# GTK_LIBRARIES     - Files to link against to use GTK
# GTK_FOUND         - If false, don't try to use GTK
# GTK_GL_FOUND      - If false, don't try to use GTK's GL features

# Added: 03/11/2011, Lorenzo
# Support gtkplus from gtkmm x64, search also using GTKMM64_BASEPATH (this variable is set by the installer).
#

# first check GTKMM64
set(GTK_BASEPATH $ENV{GTKMM64_BASEPATH})

# if that failed check GTKMM_BASEPATH
if (NOT GTK_BASEPATH)
    set(GTK_BASEPATH $ENV{GTKMM_BASEPATH})
endif()

# if everything failed search GTK_BASEPATH
if (NOT GTK_BASEPATH)
    set(GTK_BASEPATH $ENV{GTK_BASEPATH})
endif()

# header files
FIND_PATH(GTK_gtk_INCLUDE_PATH gtk/gtk.h ${GTK_BASEPATH}/include/gtk-2.0/)

# libraries
FIND_LIBRARY( GTK_gtk_LIBRARY
  NAMES  gtk-win32-2.0
  PATHS ${GTK_BASEPATH}/lib)

# GTK_INCLUDE_DIR   - Directories to include to use GTK
# GTK_LINK_FLAGS    - Files to link against to use GTK
# GTK_FOUND         - If false, don't try to use GTK
# GTK_GL_FOUND      - If false, don't try to use GTK's GL features

IF(GTK_gtk_INCLUDE_PATH AND GTK_gtk_LIBRARY)
    SET(GTK_FOUND TRUE)
    SET(GTKPLUS_INCLUDE_DIR ${GTK_gtk_INCLUDE_PATH} )
    SET( GTKPLUS_LINK_FLAGS  ${GTK_gtk_LIBRARY} )
ENDIF()

if (GTK_gdk_pixbuf_INCLUDE_PATH)
    set(GTKPLUS_INCLUDE_DIR ${GTKPLUS_INCLUDE_DIR} ${GTK_gdk_pixbuf_INCLUDE_PATH})
endif()
