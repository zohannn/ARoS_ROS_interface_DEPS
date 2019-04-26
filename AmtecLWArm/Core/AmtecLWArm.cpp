// AmtecLWArm.cpp : Defines the entry point for the console application.
//

#pragma warning ( disable : 4996 )

//------------------------------------------------------------------------------
#include "stdafx.h"
#include <conio.h>
#include <fstream>
#include <iostream>

//------------------------------------------------------------------------------
// Standard Template Library
#include <algorithm>
#include <vector>

//------------------------------------------------------------------------------
// ACE
#include <ace/OS_NS_stdio.h>
#include <ace/OS_NS_unistd.h>
#include <ace/OS_NS_signal.h>
#include <ace/OS_NS_stdlib.h>

//------------------------------------------------------------------------------
// YARP
#include <yarp/os/Network.h>
#include <yarp/os/Property.h>
#include <yarp/os/Time.h>

//------------------------------------------------------------------------------
#include "ArmMovements.h"
#include "commands.h"
#include "ConsoleManagement.h"
#include "Files_Directories.h"
#include "YarpCommunicationServerAmtecLWArm.h"

//------------------------------------------------------------------------------

#define DEFAULT_ARM_NAME	"/aros/lwa7dof_right"
#define DEFAULT_CONF_FILE	"../config/amtec_aros_no_gripper_right.ini"

//------------------------------------------------------------------------------
static bool end_execution = false;

/*Handler to catch INT and TERM signal*/
static int handler( int )
{
    static int ct = 0;
    ct++;
    if (ct>3) 
	{
        std::cout << "Aborting..." << std::endl;
		ACE_OS::exit(1);
    }
    
	std::cout << "[try " << ct << " of 3] Trying to shut down" << std::endl;

    end_execution = true;

	return 1;
}

//------------------------------------------------------------------------------
int main( int argc, char* argv[] )
{
	std::string 
		sDevice = "",
		sArm_Config = "",
		str_aux = "";

	std::vector< std::string > vs_Command_Line;
	
	bool 
		bHelp = false, 
		isOption = false,
		bMaintenance = false,
		bGoHome = false,
		bRestPosition = false;
	
	int 
		i = 0, 
		nArgs = 0;

	//Install Handlers for not abruptly shut down
#ifndef WIN32
	ACE_OS::signal(SIGINT, (ACE_SignalHandler) handler);
	ACE_OS::signal(SIGTERM, (ACE_SignalHandler) handler);
#else
	SetConsoleCtrlHandler( (PHANDLER_ROUTINE)handler, TRUE );
#endif

	for( i=1 ; i<argc ; i++ )
	{
		str_aux = argv[i];
		if( str_aux.compare( "--help" ) == 0 )
		{
			bHelp = true;
		}

		else if( str_aux.compare( "--maintenance" ) == 0 )
		{
			bMaintenance = true;
		}

		else if( str_aux.compare( "--gohome" ) == 0 )
		{
			bGoHome = true;
		}
		else if( str_aux.compare( "--restposition" ) == 0 )
		{
			bRestPosition = true;
		}

		vs_Command_Line.push_back( argv[i] );
	}

	std::cout << std::endl << std::endl <<
		"------------------------------------------------------------------" << std::endl << std::endl <<
		"Mode of usage for AmtecLWArm:" << std::endl << std::endl <<
		" --help               -> This screen" << std::endl <<
		" --name <device_name> -> Specify the name the arm will use on the network" << std::endl <<
		" --conf <config.ini>  -> Arm configuration file" << std::endl <<
		" --maintenance        -> Perform arm maintenance (config file is required)" << std::endl << std::endl <<
		"------------------------------------------------------------------" << std::endl << std::endl;
	
	if( bHelp ) return -1;

	else
	{
		sDevice = "";
		sArm_Config = "";
		nArgs = vs_Command_Line.size();
		i=0;
		while( i<nArgs )
		{
			isOption = ( vs_Command_Line[i].compare( 0, 2, "--" ) == 0 );
			if( isOption && ( (i+1)<nArgs ) )
			{
				if( ( vs_Command_Line[i+1][0] != '-' ) )
				{
					if( vs_Command_Line[i].compare( "--name" ) == 0 )
					{
						sDevice = vs_Command_Line[i+1];
						i += 2;
						continue;
					}
					else if( vs_Command_Line[i].compare( "--conf" ) == 0 )
					{
						sArm_Config = vs_Command_Line[i+1];
						i += 2;
						continue;
					}
				}
			}

			i++;
		}

		if( sDevice.compare( "" ) == 0 )
		{
			std::cout << std::endl << "Assuming the default device name for the arm: " << DEFAULT_ARM_NAME << std::endl;
			sDevice = DEFAULT_ARM_NAME;
		}
		else
		{
			if( sDevice[0] != '/' )
				sDevice.insert( 0, "/" );
			
			std::cout << std::endl <<
				"Using device name for the arm: " << sDevice << std::endl;
		}

		if( sArm_Config.compare( "" ) == 0 )
		{
			std::cout << std::endl <<
				"Assuming arm configuration file: " << DEFAULT_CONF_FILE << std::endl;

			sArm_Config = DEFAULT_CONF_FILE;
		}
		else
		{
			int index = sArm_Config.size() - 4;
			if( sArm_Config.compare( index, 4, ".ini" ) != 0 ) sArm_Config.append( ".ini" );

			if( !FileExists( sArm_Config ) )
			{
				sArm_Config =  "";
				char * var_mainproj = getenv( "MAINPROJECT" );
				if( var_mainproj != NULL ) sArm_Config =  var_mainproj;
				sArm_Config += DEFAULT_CONF_FILE;
				
				std::cerr << std::endl <<
					"Specified configuration file doesn't exists, attempting to use preconfigured..." << std::endl;
			}
		}
	}

	std::cout << std::endl <<
		"Using arm configuration file: " << sArm_Config << std::endl;

	int ifind=sArm_Config.find("_left");

	if(ifind>=0)
		sDevice = "/aros/lwa7dof_left";

	BOOL setHighprio = SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	if (!setHighprio)
	{
		//std::string str = GetLastErrorAsString();
		std::cout<< "Error: Failed to set priority"<<std::endl;
	}

	//Create instance of yarp network for handling initialization/final procedures.
	yarp::os::Network yarp_net;

	CYarpCommunicationServerAmtecLWArm comm_manager( sDevice, sArm_Config );
	bool condition = comm_manager.open();

	if( !condition )
	{
		std::cerr << std::endl << "Error Initializing server" << std::endl;

		comm_manager.close();
		yarp_net.fini();
		return -1;
	}

	//------------------------------------------------------------------------------------------------------
	// Perform arm maintenance if it's the case
	if( bMaintenance )
	{
		ArmMaintenance( &comm_manager, sArm_Config );

	//	return 0; // Maintenance done! Exit program.
		end_execution = true;
	}

	if( bGoHome )
	{
		ArmGoHome( &comm_manager, sArm_Config );
	}

	if( bRestPosition )
	{
		RestPosition ( &comm_manager, sArm_Config );

	//	return 0; // Maintenance done! Exit program.
		end_execution = true;
	}
	
	//--------------------------------------------------------------------------
	int 
		key = 0,
		nJoints = 0;

	std::cout << std::endl << std::endl;
	Print_Options();

	while( !end_execution && ( key != 27 ) )
	{
		yarp::os::Time::delay( 1.0 );	//delay in seconds

		// Test if there was a key pressed.
		if( !_kbhit() ) continue;

		// Get the pressed key.
		key = _getch();
		
		//----------------------------------------------------------------------
		// Handle key presses

		switch( key )
		{
		case 'H':
		case 'h':
			std::cout << "Take arm to home position from rest." << std::endl;
			ArmGoHome( &comm_manager, sArm_Config );
			break;

		case 'R':
		case 'r':
			std::cout << "Take arm to rest position from home." << std::endl;
			RestPosition( &comm_manager, sArm_Config );
			break;

		case 'S':
		case 's':
			std::cout << "Arm status:" << std::endl;
			nJoints = comm_manager.GetNumberJoints();
			for( int iJoint = 0; iJoint < nJoints; iJoint++ )
			{
				std::cout << "Joint " << iJoint << ": " << 
					( comm_manager.Amtec_Arm.Test_Module( iJoint + 1 ) ? "OK" : "ERR!" ) << std::endl;
			}
			break;

		case 59: // F1
			Print_Options();
			break;

		default:
			break;
		}
	} 

	comm_manager.close();

	yarp_net.fini();

	return 0;
}
