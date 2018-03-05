#include "YarpCommunicationServerPanTilt.h"

#include <iostream>

#include "MathDefines.h"
#include "pantilt_definitions.h"
#include "PanTiltWrapper_defs.h"
#include <iostream>

CYarpCommunicationServerPanTilt::CYarpCommunicationServerPanTilt( const std::string name, const std::string dev_config_file )
: CYarpCommunication( name, "", true )
, sConfig( dev_config_file )
, dVal( 0.0 )
{
	dVec[0] = 0.0;
	dVec[1] = 0.0;
	dVec[2] = 0.0;
}

CYarpCommunicationServerPanTilt::~CYarpCommunicationServerPanTilt( void )
{
}

bool CYarpCommunicationServerPanTilt::Init( void )
{

	if( !PanTilt_Properties.fromConfigFile( sConfig.c_str() ) )
	{
		Print_Wrapper_Error( PanTiltWrapper::Error::Code::CONFIG_FILE, sConfig );
		return false;
	}
	
	bool bPanTiltInit = PanTilt.open( PanTilt_Properties );

	// Try to initialize the driver
	if( !bPanTiltInit)
	{
		// Device driver opening failed
		Print_Wrapper_Error( PanTiltWrapper::Error::Code::DRIVER_OPEN );
	}

	// Success opening the driver?
	return bPanTiltInit;
}

void CYarpCommunicationServerPanTilt::Fini( void )
{
	PanTilt.close();
	PanTilt_Properties.clear();
}

void CYarpCommunicationServerPanTilt::ConvertCoordinatestoPanTilt( float fCoord[3], float *fPanTilt )
{
	const float
		fX = fCoord[0], 
		fY = fCoord[1], 
		fZ = fCoord[2],
		fNE = 7.5f,	// cm Distance from the Neck to the Eyes
		fLowerLimit = -47.0f; // -47º is the lower limit for tilt position

	float
		// Distance from the robot to the Object
		fRO = sqrt( pow( fX, 2.0f ) + pow( fY, 2.0f ) ),
		// Distance from the Table to the Neck
		fTN = 176.5694f - fZ, //Corrected from 174.27
		// Distance from Neck to the Object
		fNO = sqrt( pow( fRO, 2.0f ) + pow( fTN, 2.0f ) ),
		//Auxiliary angle
		fAlpha = atan2( fRO, fTN ),
		//Auxiliary angle
		fBeta = acos( fNE/fNO ),
		fTilt_aux = 0.0f;

	// Pan angle
	fPanTilt[PanTilt::Axis::ID::PAN] = atan2( fY, fX ) * RAD_TO_DEG_F;

	// The tilt angle is negative on the pan-tilt, hence, the changing in the calculation
	fTilt_aux = ( fAlpha + fBeta - M_PI_F ) * RAD_TO_DEG_F;
	
	// Tilt angle, test calculated tilt against lower limit
	fPanTilt[PanTilt::Axis::ID::TILT] = ( fTilt_aux < fLowerLimit ) ? fLowerLimit : fTilt_aux;
}

void CYarpCommunicationServerPanTilt::Prepare_ReplyMessage_2Ax( CMessage * msgOut )
{
	msgOut->uParam.assign( PanTilt::Axis::ID::PANTILT_NUMBER_OF_AXES, 0 );
	msgOut->fData.assign( PanTilt::Axis::ID::PANTILT_NUMBER_OF_AXES, 0.0f );
}

void CYarpCommunicationServerPanTilt::Process( CMessage &msgIn, CMessage &msgOut, void *private_data )
{
	if( !PanTilt.isOpen() ) return;

	bool 
		bSuccess = false, // Assume worst case scenario
		bInvalid = !msgIn.isValid(),	// Test if it is a valid command
		bOdd = ( ( msgIn.uCommand % 2 ) != 0 );	// if uCommand is odd is invalid
		
	if( bInvalid || bOdd )
	{
		Print_Wrapper_Error( PanTiltWrapper::Error::Code::INVALID_COMMAND );
		return;
	}

	unsigned int i = 0;

	float 
		coord[3] = { 0.0f, 0.0f, 0.0f },
		pantilt[PanTilt::Axis::ID::PANTILT_NUMBER_OF_AXES];

	for( i=0 ; i<PanTilt::Axis::ID::PANTILT_NUMBER_OF_AXES ; i++ )
		pantilt[i] = 0.0f;

	switch( msgIn.uCommand ) 
	{
		case PANTILT_ISACTIVE:
			bSuccess = true; // No action to perform, hence no errors will occur.
			break;
				
		case PANTILT_RESET_PAN:
			bSuccess = PanTilt.reset( PanTilt::Axis::ID::PAN );
			break;

		case PANTILT_RESET_TILT:
			bSuccess = PanTilt.reset( PanTilt::Axis::ID::TILT );
			break;

		case PANTILT_INITIALIZE:
		case PANTILT_RESET_PANTILT:
			bSuccess = PanTilt.reset();
			break;

		case PANTILT_HOME:
			dVec[PanTilt::Axis::ID::PAN] = 0.0;
			dVec[PanTilt::Axis::ID::TILT] = 0.0;
			bSuccess = PanTilt.positionMove( dVec );
			if( bSuccess )
				msgOut.fData.assign( 2, 0.0f );
			break;

		case PANTILT_STOP_PAN:
			bSuccess = PanTilt.stop( PanTilt::Axis::ID::PAN );
			break;

		case PANTILT_STOP_TILT:
			bSuccess = PanTilt.stop( PanTilt::Axis::ID::TILT );
			break;

		case PANTILT_STOP_PANTILT:
			bSuccess = PanTilt.stop();
			break;

		case PANTILT_USE_POSITION_CONTROL:
			bSuccess = PanTilt.setPositionMode();
			if( !bSuccess) break;
			
			// Proceed with retrieving current angles
			Prepare_ReplyMessage_2Ax( &msgOut );
			
			for( i=0 ; i<PanTilt::Axis::ID::PANTILT_NUMBER_OF_AXES ; i++ )
			{
				if( PanTilt.getPosition( i, &dVal ) )
				{
					msgOut.uParam[i] = 1;
					msgOut.fData[i] = static_cast<float>( dVal );
				}
			}
			break;

		case PANTILT_USE_VELOCITY_CONTROL:
			bSuccess = PanTilt.setVelocityMode();
			if( !bSuccess ) break;
			
			// Proceed with retrieving current reference speeds
			Prepare_ReplyMessage_2Ax( &msgOut );

			for( i=0 ; i<PanTilt::Axis::ID::PANTILT_NUMBER_OF_AXES ; i++ )
			{
				if( PanTilt.getRefSpeed( i, &dVal ) )
				{
					msgOut.uParam[i] = 1;
					msgOut.fData[i] = static_cast<float>( dVal );
				}
			}
			break;

		case PANTILT_GET_ANGLE_PAN:
			bSuccess = PanTilt.getPosition( PanTilt::Axis::ID::PAN, &dVal );
			if( bSuccess )
				msgOut.fData.assign( 1, static_cast<float>( dVal ) );
			break;
				
		case PANTILT_GET_ANGLE_TILT:
			bSuccess = PanTilt.getPosition( PanTilt::Axis::ID::TILT, &dVal );
			if( bSuccess )
				msgOut.fData.assign( 1, static_cast<float>( dVal ) );
			break;

		case PANTILT_GET_ANGLE_PANTILT:
			Prepare_ReplyMessage_2Ax( &msgOut );
			
			bSuccess = true;
			
			for( i=0 ; i<PanTilt::Axis::ID::PANTILT_NUMBER_OF_AXES ; i++ )
			{
				if( PanTilt.getPosition( i, &dVal ) )
				{
					msgOut.uParam[i] = 1;
					msgOut.fData[i] = static_cast<float>( dVal );
				}
				else
					bSuccess = false;
			}
			break;

		case PANTILT_MOVE_ANGLE_ABS_PAN:
			if( msgIn.fData.size() < 1 ) 
			{
				Print_Wrapper_Error( PanTiltWrapper::Error::Code::INSUFICIENT_FDATA );
				break;
			}
			bSuccess = PanTilt.positionMove( PanTilt::Axis::ID::PAN, msgIn.fData.front() );
			if( bSuccess )
				msgOut.fData.assign( 1, msgIn.fData.front() );
			break;
		
		case PANTILT_MOVE_ANGLE_ABS_TILT:
			if( msgIn.fData.size() < 1 )
			{
				Print_Wrapper_Error( PanTiltWrapper::Error::Code::INSUFICIENT_FDATA );
				break;
			}
			bSuccess = PanTilt.positionMove( PanTilt::Axis::ID::TILT, msgIn.fData.front() );
			if( bSuccess )
				msgOut.fData.assign( 1, msgIn.fData.front() );
			break;
		
		case PANTILT_MOVE_ANGLE_ABS_PANTILT:
			if( msgIn.fData.size() < 2 )
			{
				Print_Wrapper_Error( PanTiltWrapper::Error::Code::INSUFICIENT_FDATA );
				break;
			}

			Prepare_ReplyMessage_2Ax( &msgOut );

			bSuccess = true;

			for( i=0 ; i<PanTilt::Axis::ID::PANTILT_NUMBER_OF_AXES ; i++ )
			{
				if( PanTilt.positionMove( i, msgIn.fData[i] ) )
				{
					msgOut.uParam[i] = 1;
					msgOut.fData[i] = msgIn.fData[i];
				}
				else
					bSuccess = false;
			}
			break;

		case PANTILT_MOVE_ANGLE_REL_PAN:
			if( msgIn.fData.size() < 1 )
			{
				Print_Wrapper_Error( PanTiltWrapper::Error::Code::INSUFICIENT_FDATA );
				break;
			}
			bSuccess = PanTilt.relativeMove( PanTilt::Axis::ID::PAN, msgIn.fData.front() );
			if( bSuccess )
				msgOut.fData.assign( 1, msgIn.fData.front() );
			break;
				
		case PANTILT_MOVE_ANGLE_REL_TILT:
			if( msgIn.fData.size() < 1 )
			{
				Print_Wrapper_Error( PanTiltWrapper::Error::Code::INSUFICIENT_FDATA );
				break;
			}
			bSuccess = PanTilt.relativeMove( PanTilt::Axis::ID::TILT, msgIn.fData.front() );
			if( bSuccess )
				msgOut.fData.assign( 1, msgIn.fData.front() );
			break;
	
		case PANTILT_MOVE_ANGLE_REL_PANTILT:
			if( msgIn.fData.size() < 2 )
			{
				Print_Wrapper_Error( PanTiltWrapper::Error::Code::INSUFICIENT_FDATA );
				break;
			}
			
			Prepare_ReplyMessage_2Ax( &msgOut );

			bSuccess = true;

			for( i=0 ; i<PanTilt::Axis::ID::PANTILT_NUMBER_OF_AXES ; i++ )
			{
				if( PanTilt.relativeMove( i, msgIn.fData[i] ) )
				{
					msgOut.uParam[i] = 1;
					msgOut.fData[i] = msgIn.fData[i];
				}
				else
					bSuccess = false;
			}
			break;
	
		case PANTILT_GET_REF_SPEED_PAN:
			bSuccess = PanTilt.getRefSpeed( PanTilt::Axis::ID::PAN, &dVal );
			if( bSuccess )
				msgOut.fData.assign( 1, static_cast<float>( dVal ) );
			break;
				
		case PANTILT_GET_REF_SPEED_TILT:
			bSuccess = PanTilt.getRefSpeed( PanTilt::Axis::ID::TILT, &dVal );
			if( bSuccess )
				msgOut.fData.assign( 1, static_cast<float>( dVal ) );
			break;
	
		case PANTILT_GET_REF_SPEED_PANTILT:
			Prepare_ReplyMessage_2Ax( &msgOut );

			bSuccess = true;
			
			for( i=0 ; i<PanTilt::Axis::ID::PANTILT_NUMBER_OF_AXES ; i++ )
			{
				if( PanTilt.getRefSpeed( i, &dVal ) )
				{
					msgOut.uParam[i] = 1;
					msgOut.fData[i] = static_cast<float>( dVal );
				}
				else
					bSuccess = false;
			}
			break;
	
		case PANTILT_SET_REF_SPEED_PAN:
			if( msgIn.fData.size() < 1 )
			{
				Print_Wrapper_Error( PanTiltWrapper::Error::Code::INSUFICIENT_FDATA );
				break;
			}
			bSuccess = PanTilt.setRefSpeed( PanTilt::Axis::ID::PAN, msgIn.fData.front() );
			if( bSuccess )
				msgOut.fData.assign( 1, msgIn.fData.front() );
			break;
	
		case PANTILT_SET_REF_SPEED_TILT:
			if( msgIn.fData.size() < 1 )
			{
				Print_Wrapper_Error( PanTiltWrapper::Error::Code::INSUFICIENT_FDATA );
				break;
			}
			bSuccess = PanTilt.setRefSpeed( PanTilt::Axis::ID::TILT, msgIn.fData.front() );
			if( bSuccess )
				msgOut.fData.assign( 1, msgIn.fData.front() );
			break;
	
		case PANTILT_SET_REF_SPEED_PANTILT:
			if( msgIn.fData.size() < 2 )
			{
				Print_Wrapper_Error( PanTiltWrapper::Error::Code::INSUFICIENT_FDATA );
				break;
			}

			Prepare_ReplyMessage_2Ax( &msgOut );

			bSuccess = true;

			for( i=0 ; i<PanTilt::Axis::ID::PANTILT_NUMBER_OF_AXES ; i++ )
			{
				if( PanTilt.setRefSpeed( i, msgIn.fData[i] ) )
				{
					msgOut.uParam[i] = 1;
					msgOut.fData[i] = msgIn.fData[i];
				}
				else
					bSuccess = false;
			}
			break;
	
		case PANTILT_MOVE_VELOCITY_ABS_PAN:
			if( msgIn.fData.size() < 1 )
			{
				Print_Wrapper_Error( PanTiltWrapper::Error::Code::INSUFICIENT_FDATA );
				break;
			}
			bSuccess = PanTilt.velocityMove( PanTilt::Axis::ID::PAN, msgIn.fData.front() );
			if( bSuccess )
				msgOut.fData.assign( 1, msgIn.fData.front() );
			break;
		
		case PANTILT_MOVE_VELOCITY_ABS_TILT:
			if( msgIn.fData.size() < 1 )
			{
				Print_Wrapper_Error( PanTiltWrapper::Error::Code::INSUFICIENT_FDATA );
				break;
			}
			bSuccess = PanTilt.velocityMove( PanTilt::Axis::ID::TILT, msgIn.fData.front() );
			if( bSuccess )
				msgOut.fData.assign( 1, msgIn.fData.front() );
			break;
		
		case PANTILT_MOVE_VELOCITY_ABS_PANTILT:
			if( msgIn.fData.size() < 2 )
			{
				Print_Wrapper_Error( PanTiltWrapper::Error::Code::INSUFICIENT_FDATA );
				break;
			}

			Prepare_ReplyMessage_2Ax( &msgOut );

			bSuccess = true;

			for( i=0 ; i<PanTilt::Axis::ID::PANTILT_NUMBER_OF_AXES ; i++ )
			{
				if( PanTilt.velocityMove( i, msgIn.fData[i] ) )
				{
					msgOut.uParam[i] = 1;
					msgOut.fData[i] = msgIn.fData[i];
				}
				else
					bSuccess = false;
			}
			break;

		case PANTILT_MOVE_VELOCITY_REL_PAN:
			if( msgIn.fData.size() < 1 )
			{
				Print_Wrapper_Error( PanTiltWrapper::Error::Code::INSUFICIENT_FDATA );
				break;
			}
			bSuccess = PanTilt.velocityMoveRelative( PanTilt::Axis::ID::PAN, msgIn.fData.front() );
			if( bSuccess )
				msgOut.fData.assign( 1, msgIn.fData.front() );
			break;
				
		case PANTILT_MOVE_VELOCITY_REL_TILT:
			if( msgIn.fData.size() < 1 )
			{
				Print_Wrapper_Error( PanTiltWrapper::Error::Code::INSUFICIENT_FDATA );
				break;
			}
			bSuccess = PanTilt.velocityMoveRelative( PanTilt::Axis::ID::TILT, msgIn.fData.front() );
			if( bSuccess )
				msgOut.fData.assign( 1, msgIn.fData.front() );
			break;
	
		case PANTILT_MOVE_VELOCITY_REL_PANTILT:
			if( msgIn.fData.size() < 2 )
			{
				Print_Wrapper_Error( PanTiltWrapper::Error::Code::INSUFICIENT_FDATA );
				break;
			}
			
			Prepare_ReplyMessage_2Ax( &msgOut );

			bSuccess = true;

			for( i=0 ; i<PanTilt::Axis::ID::PANTILT_NUMBER_OF_AXES ; i++ )
			{
				if( PanTilt.velocityMoveRelative( i, msgIn.fData[i] ) )
				{
					msgOut.uParam[i] = 1;
					msgOut.fData[i] = msgIn.fData[i];
				}
				else
					bSuccess = false;
			}
			break;
	
		case PANTILT_GET_REF_ACCELERATION_PAN:
			bSuccess = PanTilt.getRefAcceleration( PanTilt::Axis::ID::PAN, &dVal );
			if( bSuccess )
				msgOut.fData.assign( 1, static_cast<float>( dVal ) );
			break;
				
		case PANTILT_GET_REF_ACCELERATION_TILT:
			bSuccess = PanTilt.getRefAcceleration( PanTilt::Axis::ID::TILT, &dVal );
			if( bSuccess )
				msgOut.fData.assign( 1, static_cast<float>( dVal ) );
			break;
	
		case PANTILT_GET_REF_ACCELERATION_PANTILT:
			Prepare_ReplyMessage_2Ax( &msgOut );

			bSuccess = true;
			
			for( i=0 ; i<PanTilt::Axis::ID::PANTILT_NUMBER_OF_AXES ; i++ )
			{
				if( PanTilt.getRefAcceleration( i, &dVal ) )
				{
					msgOut.uParam[i] = 1;
					msgOut.fData[i] = static_cast<float>( dVal );
				}
				else
					bSuccess = false;
			}
			break;
	
		case PANTILT_SET_REF_ACCELERATION_PAN:
			if( msgIn.fData.size() < 1 )
			{
				Print_Wrapper_Error( PanTiltWrapper::Error::Code::INSUFICIENT_FDATA );
				break;
			}
			bSuccess = PanTilt.setRefAcceleration( PanTilt::Axis::ID::PAN, msgIn.fData.front() );
			if( bSuccess )
				msgOut.fData.assign( 1, msgIn.fData.front() );
			break;
	
		case PANTILT_SET_REF_ACCELERATION_TILT:
			if( msgIn.fData.size() < 1 )
			{
				Print_Wrapper_Error( PanTiltWrapper::Error::Code::INSUFICIENT_FDATA );
				break;
			}
			bSuccess = PanTilt.setRefAcceleration( PanTilt::Axis::ID::TILT, msgIn.fData.front() );
			if( bSuccess )
				msgOut.fData.assign( 1, msgIn.fData.front() );
			break;
	
		case PANTILT_SET_REF_ACCELERATION_PANTILT:
			if( msgIn.fData.size() < 2 )
			{
				Print_Wrapper_Error( PanTiltWrapper::Error::Code::INSUFICIENT_FDATA );
				break;
			}

			Prepare_ReplyMessage_2Ax( &msgOut );

			bSuccess = true;

			for( i=0 ; i<PanTilt::Axis::ID::PANTILT_NUMBER_OF_AXES ; i++ )
			{
				if( PanTilt.setRefAcceleration( i, msgIn.fData[i] ) )
				{
					msgOut.uParam[i] = 1;
					msgOut.fData[i] = msgIn.fData[i];
				}
				else
					bSuccess = false;
			}
			break;	
	
		case PANTILT_AROS_LOOK_AT_POSITION:
			if( msgIn.fData.size() < 3 )
			{
				Print_Wrapper_Error( PanTiltWrapper::Error::Code::INSUFICIENT_FDATA );
				break;
			}
			
			coord[0] = msgIn.fData[0];
			coord[1] = msgIn.fData[1];
			coord[2] = msgIn.fData[2];

			ConvertCoordinatestoPanTilt( coord, pantilt );
		
			Prepare_ReplyMessage_2Ax( &msgOut );

			bSuccess = true;

			for( i=0 ; i<PanTilt::Axis::ID::PANTILT_NUMBER_OF_AXES ; i++ )
			{
				if( PanTilt.positionMove( i, pantilt[i] ) )
				{
					msgOut.uParam[i] = 1;
					msgOut.fData[i] = pantilt[i];
				}
				else
					bSuccess = false;
			}
			break;

		case PANTILT_WAIT_MOTION_END:
			bSuccess = PanTilt.waitMotionDone();
			break;

		default:
			msgOut.strText = "Unknown command";
			msgOut.uCommand = msgIn.uCommand;
			break;
	}

	if( bSuccess )
	{
		msgOut.iErrorCode = 0;
	}
	else
	{
		msgOut.iErrorCode = -1;
		Print_Wrapper_Error( PanTiltWrapper::Error::Code::COMMAND_EXEC, msgIn.uCommand );
	}
}

inline void CYarpCommunicationServerPanTilt::Print_Wrapper_Error( int error_code )
{
	std::cerr << PanTiltWrapper::Error::Wrapper_Error_Prefix << PanTiltWrapper::Error::Text[ error_code ] << std::endl;
}

inline void CYarpCommunicationServerPanTilt::Print_Wrapper_Error( int error_code, std::string strExtra )
{
	std::cerr << PanTiltWrapper::Error::Wrapper_Error_Prefix << PanTiltWrapper::Error::Text[ error_code ] << strExtra << std::endl;
}

inline void CYarpCommunicationServerPanTilt::Print_Wrapper_Error( int error_code, int command )
{
	Print_Wrapper_Error( error_code, CommandToString( command ) );
}

std::string CYarpCommunicationServerPanTilt::CommandToString( unsigned int uCommand )
{
	std::string command_str = "";

	switch( uCommand ) 
	{
		case PANTILT_ISACTIVE:
			command_str = "PANTILT_ISACTIVE";
			break;
				
		case PANTILT_INITIALIZE:
			command_str = "PANTILT_INITIALIZE";
			break;

		case PANTILT_RESET_PAN:
			command_str = "PANTILT_RESET_PAN";
			break;

		case PANTILT_RESET_TILT:
			command_str = "PANTILT_RESET_TILT";
			break;

		case PANTILT_RESET_PANTILT:
			command_str = "PANTILT_RESET_PANTILT";
			break;

		case PANTILT_HOME:
			command_str = "PANTILT_HOME";
			break;

		case PANTILT_STOP_PANTILT:
			command_str = "PANTILT_STOP_PANTILT";
			break;

		case PANTILT_STOP_PAN:
			command_str = "PANTILT_STOP_PAN";
			break;

		case PANTILT_STOP_TILT:
			command_str = "PANTILT_STOP_TILT";
			break;

		case PANTILT_USE_POSITION_CONTROL:
			command_str = "PANTILT_USE_POSITION_CONTROL";
			break;

		case PANTILT_USE_VELOCITY_CONTROL:
			command_str = "PANTILT_USE_VELOCITY_CONTROL";
			break;

		case PANTILT_GET_ANGLE_PAN:
			command_str = "PANTILT_GET_ANGLE_PAN";
			break;
				
		case PANTILT_GET_ANGLE_TILT:
			command_str = "PANTILT_GET_ANGLE_TILT";
			break;

		case PANTILT_GET_ANGLE_PANTILT:
			command_str = "PANTILT_GET_ANGLE_PANTILT";
			break;

		case PANTILT_MOVE_ANGLE_ABS_PAN:
			command_str = "PANTILT_MOVE_ANGLE_ABS_PAN";
			break;
		
		case PANTILT_MOVE_ANGLE_ABS_TILT:
			command_str = "PANTILT_MOVE_ANGLE_ABS_TILT";
			break;
		
		case PANTILT_MOVE_ANGLE_ABS_PANTILT:
			command_str = "PANTILT_MOVE_ANGLE_ABS_PANTILT";
			break;

		case PANTILT_MOVE_ANGLE_REL_PAN:
			command_str = "PANTILT_MOVE_ANGLE_REL_PAN";
			break;
				
		case PANTILT_MOVE_ANGLE_REL_TILT:
			command_str = "PANTILT_MOVE_ANGLE_REL_TILT";
			break;
	
		case PANTILT_MOVE_ANGLE_REL_PANTILT:
			command_str = "PANTILT_MOVE_ANGLE_REL_PANTILT";
			break;
	
		case PANTILT_GET_REF_SPEED_PAN:
			command_str = "PANTILT_GET_REF_SPEED_PAN";
			break;
				
		case PANTILT_GET_REF_SPEED_TILT:
			command_str = "PANTILT_GET_REF_SPEED_TILT";
			break;
	
		case PANTILT_GET_REF_SPEED_PANTILT:
			command_str = "PANTILT_GET_REF_SPEED_PANTILT";
			break;
	
		case PANTILT_SET_REF_SPEED_PAN:
			command_str = "PANTILT_SET_REF_SPEED_PAN";
			break;
	
		case PANTILT_SET_REF_SPEED_TILT:
			command_str = "PANTILT_SET_REF_SPEED_TILT";
			break;
	
		case PANTILT_SET_REF_SPEED_PANTILT:
			command_str = "PANTILT_SET_REF_SPEED_PANTILT";
			break;
	
		case PANTILT_MOVE_VELOCITY_ABS_PAN:
			command_str = "PANTILT_MOVE_VELOCITY_ABS_PAN";
			break;
		
		case PANTILT_MOVE_VELOCITY_ABS_TILT:
			command_str = "PANTILT_MOVE_VELOCITY_ABS_TILT";
			break;
		
		case PANTILT_MOVE_VELOCITY_ABS_PANTILT:
			command_str = "PANTILT_MOVE_VELOCITY_ABS_PANTILT";
			break;

		case PANTILT_MOVE_VELOCITY_REL_PAN:
			command_str = "PANTILT_MOVE_VELOCITY_REL_PAN";
			break;
				
		case PANTILT_MOVE_VELOCITY_REL_TILT:
			command_str = "PANTILT_MOVE_VELOCITY_REL_TILT";
			break;
	
		case PANTILT_MOVE_VELOCITY_REL_PANTILT:
			command_str = "PANTILT_MOVE_VELOCITY_REL_PANTILT";
			break;
	
		case PANTILT_GET_REF_ACCELERATION_PAN:
			command_str = "PANTILT_GET_REF_ACCELERATION_PAN";
			break;
				
		case PANTILT_GET_REF_ACCELERATION_TILT:
			command_str = "PANTILT_GET_REF_ACCELERATION_TILT";
			break;
	
		case PANTILT_GET_REF_ACCELERATION_PANTILT:
			command_str = "PANTILT_GET_REF_ACCELERATION_PANTILT";
			break;
	
		case PANTILT_SET_REF_ACCELERATION_PAN:
			command_str = "PANTILT_SET_REF_ACCELERATION_PAN";
			break;
	
		case PANTILT_SET_REF_ACCELERATION_TILT:
			command_str = "PANTILT_SET_REF_ACCELERATION_TILT";
			break;
	
		case PANTILT_SET_REF_ACCELERATION_PANTILT:
			command_str = "PANTILT_SET_REF_ACCELERATION_PANTILT";
			break;	
	
		case PANTILT_AROS_LOOK_AT_POSITION:
			command_str = "PANTILT_AROS_LOOK_AT_POSITION";
			break;

		case PANTILT_WAIT_MOTION_END:
			command_str = "PANTILT_WAIT_MOTION_END";
			break;
		
		default:
			command_str = "UNKNOWN";
			break;
	}

	return command_str;
}
