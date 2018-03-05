// BarrettHand.cpp : Defines the entry point for the console application.
//

//#pragma warning ( disable : 4996 )

//------------------------------------------------------------------------------
#include "stdafx.h"
#include <conio.h>
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
#include "YarpCommunicationServerBarrettHand.h"

//------------------------------------------------------------------------------

#define DEFAULT_HAND_NAME	"/aros/barretthand_right"
#define DEFAULT_CONF_FILE	"C:/Work/Repository/Hardware/YARP_Wrappers/BarrettHand/Config/barrett_right.ini"

//------------------------------------------------------------------------------
static bool end_execution = false;

// Handler to catch INT and TERM signal 
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
		sHand_Config = "", 
		str_aux = "";

	std::vector< std::string > vs_Command_Line;

	bool bHelp, isOption;
	int i, nArgs;

	//Install Handlers for not abruptly shut down
	ACE_OS::signal(SIGINT, (ACE_SignalHandler) handler);
	ACE_OS::signal(SIGTERM, (ACE_SignalHandler) handler);

	bHelp = false;
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
		"Mode of usage for BarrettHand:" << std::endl << std::endl <<
		" --help               -> This screen" << std::endl <<
		" --name <device_name> -> Specify the name the barrett hand will use on the network" << std::endl <<
		" --conf <config.ini>  -> Arm configuration file" << std::endl <<
		"------------------------------------------------------------------" << std::endl << std::endl;

	if( bHelp ) return -1;

	else
	{
		sDevice = "";
		sHand_Config = "";
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
						sHand_Config = vs_Command_Line[i+1];
						i += 2;
						continue;
					}
				}
			}

			i++;
		}

		if( sDevice.compare( "" ) == 0 )
		{
			std::cout << std::endl <<
				"Assuming the default device name for the hand: " << DEFAULT_HAND_NAME << std::endl;

			sDevice = DEFAULT_HAND_NAME;
		}
		else
		{
			if( sDevice[0] != '/' ) 
				sDevice.insert( 0, "/" );

			std::cout << std::endl <<
				"Using device name for the hand: "<< sDevice << std::endl;
		}

		if( sHand_Config.compare( "" ) == 0 )
		{
			std::cout << std::endl <<
				"Assuming hand configuration file: "<< DEFAULT_CONF_FILE << std::endl;

			sHand_Config = DEFAULT_CONF_FILE;
		}
		else
		{
			int index = sHand_Config.size() - 4;
			
			if( sHand_Config.compare( index, 4, ".ini" ) != 0 ) 
				sHand_Config.append( ".ini" );

			std::cout << std::endl <<
				"Using hand configuration file: " << sHand_Config << std::endl;
		}

	}

	yarp::os::Network yarp_net;

	CYarpCommunicationServerBarrettHand comm_manager( sDevice, sHand_Config );
	bool condition = comm_manager.open();

	if( !condition )
	{
		std::cerr << std::endl << 
			"Error Initializing server" << std::endl;
		yarp_net.fini();
		return -1;
	}

	double 
		HandTorque = 0.0,
		HandSpread = 0.0,
		HandConfig[4];

	int key = 0;

	std::cout << std::endl << std::endl << 
		"Press ESC to exit" << std::endl << std::endl;

	do 
	{
		
		if(comm_manager.Finger1.size()>0 && comm_manager.Finger2.size()>0 && comm_manager.Finger3.size()>0)
		{
			//comm_manager.mutex_barrett.wait();
			HandConfig[1]=(int)comm_manager.Finger1[0];
			HandConfig[2]=(int)comm_manager.Finger2[0];
			HandConfig[3]=(int)comm_manager.Finger3[0];
			
			if(HandConfig[1] == HandConfig[2] && HandConfig[1]==HandConfig[3])
			{
				comm_manager.BarrettHand.positionMoveFingers(HandConfig[1]);
			}
			else if(HandConfig[1] == HandConfig[2])
			{
				comm_manager.BarrettHand.positionMoveTwoFingers(HandConfig);
			}
			else
			{
				comm_manager.BarrettHand.positionMoveFingers(HandConfig);
			}
			//comm_manager.mutex_barrett.post();

			comm_manager.Finger1.erase(comm_manager.Finger1.begin());
			comm_manager.Finger2.erase(comm_manager.Finger2.begin());
			comm_manager.Finger3.erase(comm_manager.Finger3.begin());

			
		}
		else 
		{
			if(comm_manager.Finger1.size()>0)
			{
				//comm_manager.mutex_barrett.wait();
				HandConfig[1]=comm_manager.Finger1[0];
				
				comm_manager.BarrettHand.positionMove(1,HandConfig[1]);
				//comm_manager.mutex_barrett.post();

				comm_manager.Finger1.erase(comm_manager.Finger1.begin());
			}
			if(comm_manager.Finger2.size()>0)
			{
				//comm_manager.mutex_barrett.wait();
				HandConfig[2]=comm_manager.Finger2[0];
				
				comm_manager.BarrettHand.positionMove(2,HandConfig[2]);
				//comm_manager.mutex_barrett.post();
				
				comm_manager.Finger2.erase(comm_manager.Finger2.begin());
				
			}
			if(comm_manager.Finger3.size()>0)
			{
				//comm_manager.mutex_barrett.wait();
				HandConfig[3]=comm_manager.Finger3[0];
				
				comm_manager.BarrettHand.positionMove(3,HandConfig[3]);
				
				//comm_manager.mutex_barrett.post();
				comm_manager.Finger3.erase(comm_manager.Finger3.begin());
			}
		}

		if(comm_manager.Spread.size()>0)
		{
			//comm_manager.mutex_barrett.wait();
			HandConfig[0]=comm_manager.Spread[0];
			
			comm_manager.BarrettHand.positionMove(0,HandConfig[0]);
			//comm_manager.mutex_barrett.post();

			comm_manager.Spread.erase(comm_manager.Spread.begin());
		}


		// Test if there was a key pressed.
		if( _kbhit() )
		{
			// Get the pressed key.
			key = _getch();
		}

		//yarp::os::Time::delay( 0.1 );	//delay in seconds
	} while( !end_execution && ( key != 27 ) );
	
	comm_manager.close();

	yarp_net.fini();

	return 0;
}

