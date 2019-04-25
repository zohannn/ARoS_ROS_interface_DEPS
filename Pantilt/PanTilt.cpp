// PanTilt.cpp : Defines the entry point for the console application.
//

#pragma warning( disable : 4996 )

//------------------------------------------------------------------------------
#include "stdafx.h"
#include <conio.h>

//------------------------------------------------------------------------------
// Standard Template Library
#include <vector>
#include <string>
#include <iostream>

//------------------------------------------------------------------------------
// ACE
#include <ace/OS_NS_stdio.h>
#include <ace/OS_NS_unistd.h> 
#include <ace/OS_NS_signal.h>
#include <ace/OS_NS_stdlib.h>

//------------------------------------------------------------------------------
// YARP
#include <yarp/os/Network.h>
#include <yarp/os/Time.h>
#include "YarpCommunicationServerPanTilt.h"

//------------------------------------------------------------------------------
// Utilities
#include "Files_Directories.h"
#include "EnvironmentVars.h"

//----------------------------------------------------------------

#define DEFAULT_PANTILT_NAME	"/aros/pantilt"
#define DEFAULT_CONF_FILE		"../config/pantilt.ini"

//------------------------------------------------------------------------------
// Handle signal Ctrl+C to terminate execution

static bool end_execution = false;

/*Handler to catch INT and TERM signal*/
static int handler( int ) 
{
    static int ct = 0;
    ct++;
    if( ct>3 )
	{
        std::cout << "Aborting..." << std::endl;
		ACE_OS::exit( 1 );
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
		sPanTilt_Config = "",
		str_aux = "";

	std::vector< std::string > vs_Command_Line;
	
	bool 
		bHelp = false, 
		isOption = false;

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
			break;
		}

		vs_Command_Line.push_back( argv[i] );
	}

	std::cout << std::endl << std::endl << 
		"------------------------------------------------------------------" << std::endl << std::endl <<
		"Mode of usage for PanTilt:" << std::endl << std::endl << 
		" --help               -> This screen" << std::endl <<
		" --name [device_name] -> Specify the name the pantilt will use on the network" << std::endl <<
		" --conf [config.ini]  -> PanTilt configuration file" << std::endl << std::endl << 
		"------------------------------------------------------------------" << std::endl << std::endl;
	
	if( bHelp ) return 0;

	else
	{
		sDevice = "";
		
		//Default value for Pan Tilt configuration file
		sPanTilt_Config = get_Env_Var_ANSI( "MAINPROJECT" );
		sPanTilt_Config += "/Hardware/YARP_Wrappers/PanTilt/";
		sPanTilt_Config += DEFAULT_CONF_FILE;
		
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
						sPanTilt_Config = vs_Command_Line[i+1];
						i += 2;
						continue;
					}
				}
			}

			i++;
		}

		if( sDevice.compare( "" ) == 0 )
		{
			std::cout << "Assuming the default device name for the pantilt: " << DEFAULT_PANTILT_NAME << std::endl;
			sDevice = DEFAULT_PANTILT_NAME;
		}
		else
		{
			if( sDevice[0] != '/' ) sDevice.insert( 0, "/" );
			std::cout << "Using device name for the pantilt: " << sDevice.c_str() << std::endl;
		}

		if( sPanTilt_Config.compare( "" ) == 0 )
		{
			std::cout << "Assuming pantilt configuration file: " << DEFAULT_CONF_FILE << std::endl;
			sPanTilt_Config = DEFAULT_CONF_FILE;
		}
		else
		{
			int index = sPanTilt_Config.size() - 4;
			if( sPanTilt_Config.compare( index, 4, ".ini" ) != 0 ) sPanTilt_Config.append( ".ini" );
			
			if( FileExists( sPanTilt_Config ) )
			{
				std::cout << "Using pantilt configuration file: " << sPanTilt_Config.c_str() << std::endl;
			}
			else
			{
				std::cout << "Assuming pantilt configuration file: " << DEFAULT_CONF_FILE << std::endl;
				sPanTilt_Config = DEFAULT_CONF_FILE;
			}
		}
	}

	yarp::os::Network yarp_net;
	yarp_net.init();
	

	CYarpCommunicationServerPanTilt pantilt_server_thread( sDevice, sPanTilt_Config );

	if( !pantilt_server_thread.open() )
	{
		std::cerr << "Error Initializing server" << std::endl << "Press any key to exit!" << std::endl;
		yarp_net.fini();
		getchar();
		return -1;
	}
	//--------------------------------------------------------------------------
	int key = 0;

	std::cout << std::endl << "Press ESC to exit" << std::endl << std::endl;

	while( !end_execution && ( key != 27 ) )
	{
		yarp::os::Time::delay( 0.5 ); //delay in seconds

		// Test if there was a key pressed.
		if( !_kbhit() ) continue;

		// Get the pressed key.
		key = _getch();
	} 

	pantilt_server_thread.close();

	yarp_net.fini();

	return 0;
}
