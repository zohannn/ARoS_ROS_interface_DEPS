#include "YarpCommunicationServerBarrettHand.h"
#include "TimeCount.h"
#include <iostream>

#include <yarp/os/Time.h>

CYarpCommunicationServerBarrettHand::CYarpCommunicationServerBarrettHand( const std::string name, const std::string dev_config_file )
: CYarpCommunication( name, "", true )
, sConfig( dev_config_file )
, newcomand( false )
, newcomandtorque( false )
, isPicked( false )
{
	for( int i=1 ; i<4 ; i++ )
		HandConfig[i] = 50.0;

	Finger1.clear();
	Finger2.clear();
	Finger3.clear();
	Spread.clear();
}

bool CYarpCommunicationServerBarrettHand::Init( void )
{
	yarp::os::Property prop;
	
	if( !prop.fromConfigFile( sConfig.c_str() ) )
	{
		std::cerr << "Error reading configuration file." << std::endl;
		return false;
	}

	bool bHandInit = BarrettHand.open( prop );

	if( !bHandInit )
		std::cerr << std::endl << 
			"Error opening Device Driver!" << std::endl;
	
	return bHandInit;
}

void CYarpCommunicationServerBarrettHand::Fini( void )
{
	if( !BarrettHand.close() )
		std::cerr << "Error closing Device Driver!" << std::endl;
}

bool CYarpCommunicationServerBarrettHand::Initialize( void )
{
	mutex_Command.wait();
	bool flag = BarrettHand.Initialize();
	mutex_Command.post();
	return flag;
}

bool CYarpCommunicationServerBarrettHand::InitJoint( std::vector<float> fData )
{
	mutex_Command.wait();

	int i = -1;
	do
	{
		i++;
		ivec[i] = (int) fData[i];
	}
	while( ivec[i] != -1 );
		
	bool flag = BarrettHand.Initialize( ivec );
	
	mutex_Command.post();
	
	return flag;
}

bool CYarpCommunicationServerBarrettHand::isHandAvailable( void )
{
	size_t 
		size_s = Spread.size(),
		size_f1 = Finger1.size(),
		size_f2 = Finger2.size(),
		size_f3 = Finger3.size();

	return ( ( size_s < 1 ) && ( size_f1 < 1 ) && ( size_f2 < 1 ) && ( size_f3 < 1 ) );
}

bool CYarpCommunicationServerBarrettHand::OpenJoint( std::vector<float> fData )
{
	mutex_Command.wait();
	
	int i = -1;
	do
	{
		i++;
		ivec[i] = (int) fData[i];
	}while( ivec[i] != -1 );

	bool flag = BarrettHand.OpenHand( ivec );
	
	mutex_Command.post();
	
	return flag;
}

bool CYarpCommunicationServerBarrettHand::Open( void )
{
	mutex_Command.wait();
	bool flag = BarrettHand.OpenHand();
	mutex_Command.post();
	return flag;
}


bool CYarpCommunicationServerBarrettHand::CloseJoint( std::vector<float> fData )
{
	mutex_Command.wait();
	
	int i = -1;
	do
	{
		i++;
		ivec[i] = (int) fData[i];
	}while( ivec[i] != -1 );

	bool flag = BarrettHand.CloseHand( ivec );
	
	mutex_Command.post();
	
	return flag;
}

bool CYarpCommunicationServerBarrettHand::Close( void )
{
	mutex_Command.wait();
	bool flag = BarrettHand.CloseHand();
	mutex_Command.post();
	return flag;
}

bool CYarpCommunicationServerBarrettHand::OpenTorqueJoint( std::vector<float> fData )
{
	mutex_Command.wait();
	
	int i = -1;
	do
	{
		i++;
		ivec[i] = (int) fData[i];
	}while( ivec[i] != -1 );
	
	bool flag = BarrettHand.TorqueOpen( ivec );
	
	mutex_Command.post();
	
	return flag;
}

bool CYarpCommunicationServerBarrettHand::OpenTorque( void )
{
	mutex_Command.wait();
	bool flag = BarrettHand.TorqueOpen();
	mutex_Command.post();
	return flag;
}

bool CYarpCommunicationServerBarrettHand::CloseTorqueJoint( std::vector<float> fData )
{
	mutex_Command.wait();

	int i = -1;
	do
	{
		i++;
		ivec[i] = (int) fData[i];
	}while( ivec[i] != -1 );
	
	bool flag = BarrettHand.TorqueClose( ivec );
	
	mutex_Command.post();
	
	return flag;
}
	
bool CYarpCommunicationServerBarrettHand::CloseTorque( void )
{
	mutex_Command.wait();
	bool flag = BarrettHand.TorqueClose();
	mutex_Command.post();
	return flag;
}
	
bool CYarpCommunicationServerBarrettHand::SetOpenTarget( std::vector<float> fData )
{
	mutex_Command.wait();
	
	int i = 0;
	do
	{
		vec[i] = (double) fData[i];
		i++;
	}while( i<4 );
	
	bool flag = BarrettHand.setOpenTarget( vec );
	
	mutex_Command.post();
	
	return flag;
}
	
bool CYarpCommunicationServerBarrettHand::SetCloseTarget( std::vector<float> fData )
{
	mutex_Command.wait();
	
	int i = 0;
	do
	{
		vec[i] = (double) fData[i];
		i++;
	}while( i<4 );
	
	bool flag = BarrettHand.setCloseTarget( vec );
	
	mutex_Command.post();
	
	return flag;
}

bool CYarpCommunicationServerBarrettHand::GetPositions( std::vector<float> &fData )
{
	mutex_Command.wait();
	
	double dArray[4];
	bool flag = BarrettHand.getPositions( dArray );
	
	for( int i=0 ; i<=3 ; i++ )
		fData[i] = (float) dArray[i];
	
	mutex_Command.post();
	
	return flag;
}
	
bool CYarpCommunicationServerBarrettHand::GetOpenTarget( std::vector<float> &fData )
{
	mutex_Command.wait();

	double dArray[4];
	
	bool flag = BarrettHand.getOpenTarget( dArray );
	
	for( int i=0 ; i<=3 ; i++ )
		fData[i] = (float)dArray[i];
	
	mutex_Command.post();
	
	return flag;
}
	
bool CYarpCommunicationServerBarrettHand::GetCloseTarget( std::vector<float> &fData )
{
	mutex_Command.wait();
	
	double dArray[4];
	
	bool flag = BarrettHand.getCloseTarget( dArray );
	
	for( int i=0 ; i<=3 ; i++ )
		fData[i] = (float) dArray[i];
	
	mutex_Command.post();
	
	return flag;
}

bool CYarpCommunicationServerBarrettHand::SetOpenTargetJoint( int joint, double value )
{
	mutex_Command.wait();
	bool flag = BarrettHand.setOpenTarget( joint, value );
	mutex_Command.post();
	return flag;
}
	
bool CYarpCommunicationServerBarrettHand::SetCloseTargetJoint( int joint, double value )
{
	mutex_Command.wait();
	bool flag = BarrettHand.setCloseTarget( joint, value );
	mutex_Command.post();
	return flag;
}
	
float CYarpCommunicationServerBarrettHand::GetOpenTargetJoint( int joint )
{
	mutex_Command.wait();
	double dAux = 0.0;
	bool flag = BarrettHand.getOpenTarget( joint, &dAux );
	mutex_Command.post();
	return (float) dAux;
}
	
float CYarpCommunicationServerBarrettHand::GetCloseTargetJoint( int joint )
{
	mutex_Command.wait();
	double dAux = 0.0;
	bool flag = BarrettHand.getCloseTarget( joint, &dAux );
	mutex_Command.post();
	return (float) dAux;
}
	
bool CYarpCommunicationServerBarrettHand::SetVelocity( std::vector<float> fData )
{
	mutex_Command.wait();
	
	int i = 0;
	do
	{
		vec[i] = (double) fData[i];
		i++;
	}while(i < 4);
	
	bool flag = BarrettHand.setRefSpeeds( vec );
	
	mutex_Command.post();
	
	return flag;
}
	
bool CYarpCommunicationServerBarrettHand::GetVelocity( std::vector<float> &fData )
{
	mutex_Command.wait();
	
	double dArray[4];
	
	bool flag = BarrettHand.getRefSpeeds( dArray );
	
	for( int i=0 ; i<=3 ; i++ )
		fData[i] = (float) dArray[i];
	
	mutex_Command.post();
	
	return flag;
}
	
bool CYarpCommunicationServerBarrettHand::SetOpenVelocity( std::vector<float> fData )
{
	mutex_Command.wait();
	
	int i = 0;
	do
	{
		vec[i] = (double) fData[i];
		i++;
	}while( i<4 );
	
	bool flag = BarrettHand.setRefSpeedsOpen( vec );
	
	mutex_Command.post();
	
	return flag;
}
	
bool CYarpCommunicationServerBarrettHand::SetCloseVelocity( std::vector<float> fData )
{
	mutex_Command.wait();
	
	int i = 0;
	do
	{
		vec[i] = (double) fData[i];
		i++;
	}while( i<4 );

	bool flag = BarrettHand.setRefSpeedsClose( vec );
	
	mutex_Command.post();
	
	return flag;
}
		
bool CYarpCommunicationServerBarrettHand::SetAcceleration( std::vector<float> fData )
{
	mutex_Command.wait();
	
	int i = 0;
	do
	{
		vec[i] = (double) fData[i];
		i++;
	}while(i < 4);
	
	bool flag = BarrettHand.setRefAccelerations( vec );
	
	mutex_Command.post();
	
	return flag;
}
	
bool CYarpCommunicationServerBarrettHand::GetAcceleration( std::vector<float> &fData )
{
	mutex_Command.wait();
	double dArrayAux[4];
	
	bool flag = BarrettHand.getRefAccelerations( dArrayAux );
	
	for( int i=0 ; i<=3 ; i++ )
		fData[i] = (float) dArrayAux[i];
	
	mutex_Command.post();
	
	return flag;
}

bool CYarpCommunicationServerBarrettHand::SetMaxStrain( std::vector<unsigned int> iData )
{
	mutex_Command.wait();
	
	int i = 0;
	do
	{
		ivec[i] = (int) iData[i];
		i++;
	}while(i < 4);
	
	bool flag = BarrettHand.setMaxStrain( ivec );
	
	mutex_Command.post();
	
	return flag;
}

bool CYarpCommunicationServerBarrettHand::GetMaxStrain( std::vector<unsigned int> &iData )
{
	const int nFingers = 4;
	int strain[nFingers] = { 0, 0, 0, 0 };

	iData.assign( nFingers, 0 );

	mutex_Command.wait();

	bool bSuccess = BarrettHand.getMaxStrain( strain );

	if( bSuccess )
	{
		for( int i=0 ; i<nFingers ; i++ )
		{
			iData.at( i ) = static_cast<unsigned int>( strain[i] );
		}
	}

	mutex_Command.post();

	return bSuccess;
}

bool CYarpCommunicationServerBarrettHand::GetStrainGauge( std::vector<unsigned int> &uParam )
{
	mutex_Command.wait();
	
	const int nFingers = 4;

	double strain[nFingers] = { 0.0, 0.0, 0.0, 0.0 };

	bool bSuccess = BarrettHand.getStrainGauges( strain );

	if( bSuccess )
	{
		uParam.assign( nFingers, 0 );

		for( int i=0 ; i<nFingers ; i++ )
			uParam.at( i ) = static_cast<unsigned int>( strain[i] );
	}
	
	mutex_Command.post();

	return bSuccess;
}
	
void CYarpCommunicationServerBarrettHand::SetStrainGauge( bool ispicked1 )
{
	mutex_Command.wait();
	isPicked = ispicked1;
	mutex_Command.post();
}
	
bool CYarpCommunicationServerBarrettHand::IncrementalMoveJoint( int joint, double value )
{
	mutex_Command.wait();
	bool flag = BarrettHand.relativeMove(joint, value);
	mutex_Command.post();
	return flag;
}
	
bool CYarpCommunicationServerBarrettHand::IncrementalMove( std::vector<float> fData )
{
	mutex_Command.wait();
	
	int i = 0;
	do
	{
		vec[i] = (double) fData[i];
		i++;
	}while( i<4 );
	
	bool flag = BarrettHand.relativeMove( vec );
	
	mutex_Command.post();
	
	return flag;
}
	
void CYarpCommunicationServerBarrettHand::GetHandConfig( double dHandConfig[4] )
{
	mutex_Command.wait();
	
	for( int i=0 ; i<4 ; i++ )
		dHandConfig[i] = HandConfig[i];

	mutex_Command.post();
}
	
void CYarpCommunicationServerBarrettHand::GetSpreadConfig( double &dHandConfig )
{
	mutex_Command.wait();
	dHandConfig = HandConfig[0];
	mutex_Command.post();
}
	
void CYarpCommunicationServerBarrettHand::SetNewComand( bool flag )
{
	mutex_Command.wait();
	newcomand = flag;
	mutex_Command.post();
}
	
bool CYarpCommunicationServerBarrettHand::GetNewComand( void )
{
	mutex_Command.wait();
	
	if( newcomand == true )
	{
		newcomand = false;
		mutex_Command.post();
		return true;
	}
	else
	{
		mutex_Command.post();
		return false;
	}
}
	
void CYarpCommunicationServerBarrettHand::SetNewComandTorque( bool flag )
{
	mutex_Command.wait();
	newcomandtorque = flag;
	mutex_Command.post();
}
	
bool CYarpCommunicationServerBarrettHand::GetNewComandTorque( void )
{
	mutex_Command.wait();
	
	if( newcomandtorque == true )
	{
		newcomandtorque = false;
		mutex_Command.post();
		return true;
	}
	else
	{
		mutex_Command.post();
		return false;
	}
}
	
void CYarpCommunicationServerBarrettHand::SetNewComandSpread( bool flag )
{
	mutex_Command.wait();
	newcomandspread = flag;
	mutex_Command.post();
}
	
bool CYarpCommunicationServerBarrettHand::GetNewComandSpread( void )
{
	mutex_Command.wait();
	
	if(newcomandspread == true)
	{
		newcomandspread = false;
		mutex_Command.post();
		return true;
	}
	else
	{
		mutex_Command.post();
		return false;
	}
}

void CYarpCommunicationServerBarrettHand::Process( CMessage &msgIn, CMessage &msgOut, void *private_data )
{
	bool bCommandSuccess = true;

	// Test if it is a valid command
	if( !msgIn.isValid() ) return;

	switch( msgIn.uCommand ) 
	{
		case BARRETTHAND_INITIALIZE:
			mutex_barrett.wait();
			bCommandSuccess = Initialize();
			mutex_barrett.post();
			break;

		case BARRETTHAND_INITJOINT:
			mutex_barrett.wait();
			bCommandSuccess = InitJoint( msgIn.fData );
			mutex_barrett.post();
			break;

		case BARRETTHAND_OPENJOINT :
			mutex_barrett.wait();
			bCommandSuccess = OpenJoint( msgIn.fData );
			mutex_barrett.post();
			break;

		case BARRETTHAND_OPEN :
			mutex_barrett.wait();
			bCommandSuccess = Open();
			mutex_barrett.post();
			break;

		case BARRETTHAND_CLOSEJOINT :
			mutex_barrett.wait();
			bCommandSuccess = CloseJoint( msgIn.fData );
			mutex_barrett.post();
			break;

		case BARRETTHAND_CLOSE :
			mutex_barrett.wait();
			bCommandSuccess = Close();
			mutex_barrett.post();
			break;

		case BARRETTHAND_TORQUEOPEN_JOINT :
			mutex_barrett.wait();
			bCommandSuccess = OpenTorqueJoint( msgIn.fData );
			mutex_barrett.post();
			break;

		case BARRETTHAND_TORQUEOPEN :
			mutex_barrett.wait();
			bCommandSuccess = OpenTorque();
			mutex_barrett.post();
			break;

		case BARRETTHAND_TORQUECLOSE_JOINT :
			mutex_barrett.wait();
			bCommandSuccess = CloseTorqueJoint( msgIn.fData );
			mutex_barrett.post();
			break;

		case BARRETTHAND_TORQUECLOSE :
			mutex_barrett.wait();
			bCommandSuccess = CloseTorque();
			mutex_barrett.post();
			break;

		case BARRETTHAND_SETOPENTARGET :
			mutex_barrett.wait();
			bCommandSuccess = SetOpenTarget( msgIn.fData );
			mutex_barrett.post();
			break;

		case BARRETTHAND_SETCLOSETARGET :
			mutex_barrett.wait();
			bCommandSuccess = SetCloseTarget( msgIn.fData );
			mutex_barrett.post();
			break;

		case BARRETTHAND_GETOPENTARGET :
			while( !isHandAvailable() )
			{
				yarp::os::Time::delay( 0.1 );
			}
			msgOut.fData.resize( 4 );
			mutex_barrett.wait();
			bCommandSuccess = GetOpenTarget( msgOut.fData );
			mutex_barrett.post();
			break;

		case BARRETTHAND_GETCLOSETARGET :
			while( !isHandAvailable() )
			{
				yarp::os::Time::delay( 0.1 );
			}
			msgOut.fData.resize( 4 );
			mutex_barrett.wait();
			bCommandSuccess = GetCloseTarget( msgOut.fData );
			mutex_barrett.post();
			break;

		case BARRETTHAND_SETOPENTARGETJOINT :
			mutex_barrett.wait();
			bCommandSuccess = SetOpenTargetJoint((int)msgIn.fData[0], (double)msgIn.fData[1]);
			mutex_barrett.post();
			break;

		case BARRETTHAND_SETCLOSETARGETJOINT :
			mutex_barrett.wait();
			bCommandSuccess = SetCloseTargetJoint((int)msgIn.fData[0], (double)msgIn.fData[1]);
			mutex_barrett.post();
			break;
			
		case BARRETTHAND_SETMAXSTRAINGAUGE :
			mutex_barrett.wait();
			bCommandSuccess = SetMaxStrain(msgIn.uParam);
			mutex_barrett.post();
			break;

		case BARRETTHAND_GETOPENTARGETJOINT :
			while( !isHandAvailable() )
			{
				yarp::os::Time::delay( 0.1 );
			}
			mutex_barrett.wait();
			msgOut.fData.assign( 1, GetOpenTargetJoint( (int) msgIn.fData[0] ) );
			mutex_barrett.post();
			break;

		case BARRETTHAND_GETCLOSETARGETJOINT :
			while( !isHandAvailable() )
			{
				yarp::os::Time::delay( 0.1 );
			}
			mutex_barrett.wait();
			msgOut.fData.assign( 1, GetCloseTargetJoint( (int) msgIn.fData[0] ) );
			mutex_barrett.post();
			break;

		case BARRETTHAND_SETVELOCITY :
			mutex_barrett.wait();
			bCommandSuccess = SetVelocity( msgIn.fData );
			mutex_barrett.post();
			break;

		case BARRETTHAND_GETVELOCITY :
			while( !isHandAvailable() )
			{
				yarp::os::Time::delay( 0.1 );
			}
			msgOut.fData.assign( 4, 0.0f );
			mutex_barrett.wait();
			bCommandSuccess = GetVelocity( msgOut.fData );
			mutex_barrett.post();
			break;

		case BARRETTHAND_GETPOSITIONS :
			while( !isHandAvailable() )
			{
				yarp::os::Time::delay( 0.1 );
			}
			mutex_barrett.wait();
			msgOut.fData.assign( 4, 0.0f );
			bCommandSuccess = GetPositions( msgOut.fData );
			mutex_barrett.post();
			break;

		case BARRETTHAND_SETOPENVELOCITY :
			mutex_barrett.wait();
			bCommandSuccess = SetOpenVelocity( msgIn.fData );
			mutex_barrett.post();
			break;

		case BARRETTHAND_SETCLOSEVELOCITY :
			mutex_barrett.wait();
			bCommandSuccess = SetCloseVelocity( msgIn.fData );
			mutex_barrett.post();
			break;

		case BARRETTHAND_SETACCELERATION :
			mutex_barrett.wait();
			bCommandSuccess = SetAcceleration( msgIn.fData );
			mutex_barrett.post();
			break;

		case BARRETTHAND_GETACCELERATION :
			while( !isHandAvailable() )
			{
				yarp::os::Time::delay( 0.1 );
			}
			msgOut.fData.assign( 4, 0.0f );
			mutex_barrett.wait();
			bCommandSuccess = GetAcceleration( msgOut.fData );
			mutex_barrett.post();
			break;

		case BARRETTHAND_GETSTRAINGAUGE:
			while( !isHandAvailable() )
			{
				yarp::os::Time::delay( 0.1 );
			}
			mutex_barrett.wait();
			bCommandSuccess = GetStrainGauge( msgOut.uParam );
			mutex_barrett.post();
			break;

		case BARRETTHAND_GETMAXSTRAINGAUGE:
			while( !isHandAvailable() )
			{
				yarp::os::Time::delay( 0.1 );
			}
			mutex_barrett.wait();
			bCommandSuccess = GetMaxStrain( msgOut.uParam );
			mutex_barrett.post();
			break;

		case BARRETTHAND_INCREMENTALMOVEJOINT:
			mutex_barrett.wait();
			bCommandSuccess = IncrementalMoveJoint( (int)msgIn.fData[0], (double)msgIn.fData[1] );
			mutex_barrett.post();
			break;

		case BARRETTHAND_INCREMENTALMOVEALL:
			mutex_barrett.wait();
			bCommandSuccess = IncrementalMove( msgIn.fData );
			mutex_barrett.post();
			break;

		case BARRETTHAND_MOVEJOINT:
			mutex_barrett.wait();
			switch( (int)msgIn.fData[0] )
			{
			case 0:
				Spread.push_back( msgIn.fData[1] );
				break;
			case 1:
				Finger1.push_back( msgIn.fData[1] );
				break;
			case 2:
				Finger2.push_back( msgIn.fData[1] );
				break;
			case 3:
				Finger3.push_back( msgIn.fData[1] );
				break;
			}
			mutex_barrett.post();
			break;

		case BARRETTHAND_MOVESPREAD:
			mutex_barrett.wait();
			Spread.push_back(msgIn.fData[0]);
			mutex_barrett.post();
			break;

		case BARRETTHAND_MOVEALL:
			mutex_barrett.wait();
			Spread.push_back(msgIn.fData[0]);
			Finger1.push_back(msgIn.fData[1]);
			Finger2.push_back(msgIn.fData[2]);
			Finger3.push_back(msgIn.fData[3]);
			mutex_barrett.post();
			break;

		case BARRETTHAND_MOVEALLFINGERS:
			mutex_barrett.wait();
			Finger1.push_back(msgIn.fData[1]);
			Finger2.push_back(msgIn.fData[2]);
			Finger3.push_back(msgIn.fData[3]);
			mutex_barrett.post();
			break;

		case BARRETTHAND_MOVEFINGERS:
			mutex_barrett.wait();
			Finger1.push_back(msgIn.fData[0]);
			Finger2.push_back(msgIn.fData[0]);
			Finger3.push_back(msgIn.fData[0]);
			mutex_barrett.post();
			break;

		case BARRETTHAND_MOVETWOFINGERS:
			mutex_barrett.wait();
			Finger1.push_back(msgIn.fData[0]);
			Finger2.push_back(msgIn.fData[0]);
			Finger3.push_back(msgIn.fData[1]);
			mutex_barrett.post();
			break;

		case BARRETTHAND_MOVETWOFINGERS_WAIT:
			mutex_barrett.wait();		
			HandConfig[1] = msgIn.fData[0];
			HandConfig[2] = msgIn.fData[0];
			HandConfig[3] = msgIn.fData[1];
			bCommandSuccess = BarrettHand.positionMoveTwoFingers(HandConfig);
			mutex_barrett.post();
			break;

		case BARRETTHAND_MOVEALLFINGERS_WAIT:
			mutex_barrett.wait();
			HandConfig[1] = msgIn.fData[0];
			HandConfig[2] = msgIn.fData[0];
			HandConfig[3] = msgIn.fData[0];
			bCommandSuccess = BarrettHand.positionMoveFingers(HandConfig[1]);
			mutex_barrett.post();
			break;

		case BARRETTHAND_MOVEFINGERS_WAIT:
			mutex_barrett.wait();
			HandConfig[1] = msgIn.fData[0];
			HandConfig[2] = msgIn.fData[1];
			HandConfig[3] = msgIn.fData[2];
			bCommandSuccess = BarrettHand.positionMoveFingers(HandConfig);
			mutex_barrett.post();
			break;

		case BARRETTHAND_MOVEJOINT_WAIT:
			mutex_barrett.wait();
			HandConfig[(int)msgIn.fData[0]] = msgIn.fData[1];
			bCommandSuccess = BarrettHand.positionMove( (int)msgIn.fData[0], msgIn.fData[1] );
			mutex_barrett.post();
			break;

		case BARRETTHAND_MOVESPREAD_WAIT:
			mutex_barrett.wait();
			HandConfig[0] = msgIn.fData[0];
			bCommandSuccess = BarrettHand.positionMove(0,HandConfig[0]);
			mutex_barrett.post();
			break;
		
		case BARRETTHAND_WRITE_COMMAND_RAW:
			bCommandSuccess = BarrettHand.Write_Command_Read_Reply_Clean( msgIn.strText, msgOut.strText );
			break;

		default:
			msgOut.strText = "unknown command";
			msgOut.uCommand = msgIn.uCommand;
			break;
	}

	if( !bCommandSuccess )
	{
		msgOut.iErrorCode = -1;
		Report_Error_at_Command( msgIn.uCommand );
	}
}

void CYarpCommunicationServerBarrettHand::Report_Error_at_Command( unsigned int uCommand )
{
	std::string comm_str = CommandToString( uCommand );

	std::cerr << std::endl <<
		"!!! Error in command '" << comm_str <<"' !!!" << std::endl;
}

std::string CYarpCommunicationServerBarrettHand::CommandToString( unsigned int uCommand )
{
	std::string command_str = "";

	switch( uCommand ) 
	{
	case BARRETTHAND_TERMINATE:
		command_str = "BARRETTHAND_TERMINATE";
		break;

	case BARRETTHAND_INITIALIZE:
		command_str = "BARRETTHAND_INITIALIZE";
		break;

	case BARRETTHAND_INITJOINT:
		command_str = "BARRETTHAND_INITJOINT";
		break;

	case BARRETTHAND_OPENJOINT:
		command_str = "BARRETTHAND_OPENJOINT";
		break;

	case BARRETTHAND_OPEN:
		command_str = "BARRETTHAND_OPEN";
		break;

	case BARRETTHAND_CLOSEJOINT:
		command_str = "BARRETTHAND_CLOSEJOINT";
		break;

	case BARRETTHAND_CLOSE:
		command_str = "BARRETTHAND_CLOSE";
		break;

	case BARRETTHAND_TORQUEOPEN_JOINT:
		command_str = "BARRETTHAND_TORQUEOPEN_JOINT";
		break;

	case BARRETTHAND_TORQUEOPEN:
		command_str = "BARRETTHAND_TORQUEOPEN";
		break;

	case BARRETTHAND_TORQUECLOSE_JOINT:
		command_str = "BARRETTHAND_TORQUECLOSE_JOINT";
		break;

	case BARRETTHAND_TORQUECLOSE:
		command_str = "BARRETTHAND_TORQUECLOSE";
		break;

	case BARRETTHAND_MOVEJOINT:
		command_str = "BARRETTHAND_MOVEJOINT";
		break;

	case BARRETTHAND_MOVEALL:
		command_str = "BARRETTHAND_MOVEALL";
		break;

	case BARRETTHAND_SETOPENTARGET:
		command_str = "BARRETTHAND_SETOPENTARGET";
		break;

	case BARRETTHAND_SETCLOSETARGET:
		command_str = "BARRETTHAND_SETCLOSETARGET";
		break;

	case BARRETTHAND_GETOPENTARGET:
		command_str = "BARRETTHAND_GETOPENTARGET";
		break;

	case BARRETTHAND_GETCLOSETARGET:
		command_str = "BARRETTHAND_GETCLOSETARGET";
		break;

	case BARRETTHAND_SETVELOCITY:
		command_str = "BARRETTHAND_SETVELOCITY";
		break;

	case BARRETTHAND_GETVELOCITY:
		command_str = "BARRETTHAND_GETVELOCITY";
		break;

	case BARRETTHAND_SETACCELERATION:
		command_str = "BARRETTHAND_SETACCELERATION";
		break;

	case BARRETTHAND_GETACCELERATION:
		command_str = "BARRETTHAND_GETACCELERATION";
		break;

	case BARRETTHAND_INCREMENTALMOVEJOINT:
		command_str = "BARRETTHAND_INCREMENTALMOVEJOINT";
		break;

	case BARRETTHAND_INCREMENTALMOVEALL:
		command_str = "BARRETTHAND_INCREMENTALMOVEALL";
		break;

	case BARRETTHAND_SETOPENTARGETJOINT:
		command_str = "BARRETTHAND_SETOPENTARGETJOINT";
		break;

	case BARRETTHAND_SETCLOSETARGETJOINT:
		command_str = "BARRETTHAND_SETCLOSETARGETJOINT";
		break;

	case BARRETTHAND_GETOPENTARGETJOINT:
		command_str = "BARRETTHAND_GETOPENTARGETJOINT";
		break;

	case BARRETTHAND_GETCLOSETARGETJOINT:
		command_str = "BARRETTHAND_GETCLOSETARGETJOINT";
		break;

	case BARRETTHAND_MOVEFINGERS:
		command_str = "BARRETTHAND_MOVEFINGERS";
		break;

	case BARRETTHAND_MOVESPREAD:
		command_str = "BARRETTHAND_MOVESPREAD";
		break;

	case BARRETTHAND_MOVETWOFINGERS:
		command_str = "BARRETTHAND_MOVETWOFINGERS";
		break;

	case BARRETTHAND_SETOPENVELOCITY:
		command_str = "BARRETTHAND_SETOPENVELOCITY";
		break;

	case BARRETTHAND_SETCLOSEVELOCITY:
		command_str = "BARRETTHAND_SETCLOSEVELOCITY";
		break;

	case BARRETTHAND_GETSTRAINGAUGE:
		command_str = "BARRETTHAND_GETSTRAINGAUGE";
		break;

	case BARRETTHAND_GETPOSITIONS:
		command_str = "BARRETTHAND_GETPOSITIONS";
		break;

	default:
		command_str = "Unknown";
		break;
	}

	return command_str;
}
