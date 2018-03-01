# List all modules here (this is a symbolic name):
set( EXTERNAL_MODULES
	amtec_lwa_7dof
	barrett_hand_826X
	pantilt_ptud46
	serial_asio
)

# For each MODULE above add a variable named MODULE_DIR
# which specifies its location on your disk. 

set( MODULES_DIR $ENV{MAINPROJECT}/Hardware/YARP_Modules )

foreach( mod ${EXTERNAL_MODULES} )
	set( ${mod}_DIR ${MODULES_DIR}/${mod} )
endforeach()
