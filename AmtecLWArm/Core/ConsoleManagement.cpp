#include "ConsoleManagement.h"
#include <iostream>

void Print_Usage( void )
{
	std::cout << std::endl << std::endl <<
		"--------------------------------------------------------------------------------" << std::endl << 
		"Mode of usage for AmtecLWA_2Arms:" << std::endl << std::endl <<
		" --help               -> This screen" << std::endl <<
		" --name <device_name> -> Specify the name the arm will use on the network" << std::endl <<
		" --conf <config.ini>  -> Arm configuration file" << std::endl <<
		"--------------------------------------------------------------------------------" << std::endl << std::endl;
}

void Print_Options( void )
{
	std::cout << std::endl <<
		"--------------------------------------------------------------------------------" << std::endl <<
		"Options for AmtecLWA:" << std::endl <<
		"F1:\tDisplay this help;" << std::endl <<
		"H:\tGo home from rest position;" << std::endl <<
		"R:\tGo to rest position from home;" << std::endl <<
		"S:\tArm status;" << std::endl <<
		"ESC:\tExit." << std::endl <<
		"--------------------------------------------------------------------------------" << std::endl << std::endl;
}
