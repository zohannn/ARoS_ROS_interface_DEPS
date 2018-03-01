#include <pantilt_ptud46.h>

#include <string>
#include <sstream>
#include <iostream>

#include <yarp/os/Bottle.h>

yarp::dev::PanTilt_PTUD46::PanTilt_PTUD46( void )
: nAxis( PanTilt::Axis::ID::PANTILT_NUMBER_OF_AXES )
, nControlMode( PanTilt::ControlMode::POSITION )
, bOpened( false )
{
	//Default values for resolution
	dResolution.assign( nAxis, 190.5882/3600.0 );

	Motor_ID.assign( nAxis, ' ' );
	Motor_ID.at( PanTilt::Axis::ID::PAN ) = 'P';
	Motor_ID.at( PanTilt::Axis::ID::TILT ) = 'T';

	dAngleLimit_Pan.assign( nAxis, 0.0 );
	dAngleLimit_Tilt.assign( nAxis, 0.0 );
	dSpeedLimit_Pan.assign( nAxis, 0.0 );
	dSpeedLimit_Tilt.assign( nAxis, 0.0 );
}

yarp::dev::PanTilt_PTUD46::~PanTilt_PTUD46( void )
{
	close();

	dResolution.clear();

	Motor_ID.clear();

	dAngleLimit_Pan.clear();
	dAngleLimit_Tilt.clear();
	dSpeedLimit_Pan.clear();
	dSpeedLimit_Tilt.clear();
}

bool yarp::dev::PanTilt_PTUD46::isOpen( void )
{
	return bOpened;
}

inline void yarp::dev::PanTilt_PTUD46::Print_Error( int error_code )
{
	std::cerr << PanTilt::Error::Text[ error_code ] << std::endl;
}

inline void yarp::dev::PanTilt_PTUD46::Print_Error_str( int error_code, std::string strExtra )
{
	std::cerr << PanTilt::Error::Text[ error_code ] << strExtra << std::endl;
}

inline void yarp::dev::PanTilt_PTUD46::Print_Error_int( int error_code, int val )
{
	std::cerr << PanTilt::Error::Text[ error_code ] << val << std::endl;
}

bool yarp::dev::PanTilt_PTUD46::Read_Reply( std::string& sReply )
{
	std::string strWait = PANTILT_WAIT_STRING;

	int nBytesCount = Serial_Port.Read_Wait_String( sReply, strWait, 1000 );
	
	if( ( nBytesCount <= 0 ) || ( sReply.size() <= 0 ) )
	{
		Print_Error( PanTilt::Error::Code::SERIAL_PORT_READ );
		return false;
	}

	if( sReply.front() == '!' )
	{
		Print_Error_str( PanTilt::Error::Code::PANTILT_REPLY, sReply );
		return false;
	}

	return true;
}

bool yarp::dev::PanTilt_PTUD46::Read_Reply_Discard( void )
{
	// When the pan-tilt's response doesn't matter, the serial port buffer is read to be clean
	std::string reply = "";

	return Read_Reply( reply );
}

bool yarp::dev::PanTilt_PTUD46::Write_Command( std::string command )
{
	bool bSuccess = Serial_Port.Write( command );

	if( !bSuccess )
		Print_Error( PanTilt::Error::Code::SERIAL_PORT_WRITE );

	return bSuccess;
}

inline bool yarp::dev::PanTilt_PTUD46::Write_Command_Discard_Reply( std::string command )
{
	if( !Write_Command( command ) ) return false;

	return Read_Reply_Discard();
}

inline bool yarp::dev::PanTilt_PTUD46::Write_Command_Discard_Reply( int axis, char command_ID )
{
	if( !isValidAxis( axis ) ) return false;

	std::stringstream command( "" );

	// Build command string
	command << command_ID << Motor_ID.at( axis ) << '\r';

	return Write_Command_Discard_Reply( command.str() );
}

inline bool yarp::dev::PanTilt_PTUD46::Write_Command_send_1_Parameter( int axis, char command_ID, double value )
{
	if( !isValidAxis( axis ) ) return false;

	std::stringstream command( "" );

	// Build command string
	command << Motor_ID.at( axis ) << command_ID << static_cast<int>( value ) << '\r';

	return Write_Command_Discard_Reply( command.str() );
}

inline bool yarp::dev::PanTilt_PTUD46::Write_Command_receive_1_Parameter( int axis, char command_ID, double * value, std::string find_str )
{
	if( !isValidAxis( axis ) ) return false;

	std::stringstream query( "" );
	query << Motor_ID.at( axis ) << command_ID << '\r';

	if( !Write_Command( query.str() ) ) return false;

	std::string reply = "";
	if( !Read_Reply( reply ) ) return false;

	yarp::os::Bottle bot;
	bot.fromString( reply.c_str() );

	if( bot.size() > 0 )
	{
		*value = bot.find( find_str.c_str() ).asDouble();
		return true;
	}
	else
		return false;
}

bool yarp::dev::PanTilt_PTUD46::isValidAxis( int axis )
{
	if( axis>=0 && axis<nAxis ) return true;

	else
	{
		Print_Error_int( PanTilt::Error::Code::INVALID_AXIS_INDEX, axis );
		return false;
	}
}

bool yarp::dev::PanTilt_PTUD46::open( std::string sCom, int nBaudRate )
{
	//Default values for resolution
	dResolution.assign( nAxis, 190.5882 / 3600.0 );

	Motor_ID.assign( nAxis, ' ' );
	Motor_ID.at( PanTilt::Axis::ID::PAN ) = 'P';
	Motor_ID.at( PanTilt::Axis::ID::TILT ) = 'T';

	dAngleLimit_Pan.assign( nAxis, 0.0 );
	dAngleLimit_Tilt.assign( nAxis, 0.0 );
	dSpeedLimit_Pan.assign( nAxis, 0.0 );
	dSpeedLimit_Tilt.assign( nAxis, 0.0 );

	bOpened = false;

	if( !Serial_Port.open( sCom, nBaudRate ) )
	{
		Print_Error( PanTilt::Error::Code::SERIAL_PORT_OPEN );
		return false;
	}

	// Disable command echoing
	if( !Write_Command_Discard_Reply( "ED\r" ) ) return false;

	double dVal = 0.0;
	
	// ---------------------------------------
	// Get Pan and Tilt resolution
	for( int i=0 ; i<nAxis ; i++ )
	{
		if( !Write_Command_receive_1_Parameter( i, 'R', &dVal, "*" ) ) return false;

		dResolution.at( i ) = dVal / 3600.0;
	}

	// ---------------------------------------
	// Get Pan and Tilt minimum angle position
	for( int i=0 ; i<nAxis ; i++ )
	{
		if( !Write_Command_receive_1_Parameter( i, 'N', &dVal ) ) return false;

		dVal *= dResolution.at( i );

		switch( i )
		{
		case PanTilt::Axis::ID::PAN:
			set_AngleLimitMin_Pan( dVal );
			break;

		case PanTilt::Axis::ID::TILT:
			set_AngleLimitMin_Tilt( dVal );
			break;
		}
	}

	// ---------------------------------------
	// Get Pan and Tilt maximum angle position
	for( int i=0 ; i<nAxis ; i++ )
	{
		if( !Write_Command_receive_1_Parameter( i, 'X', &dVal ) ) return false;

		dVal *= dResolution.at( i );
		
		switch( i )
		{
		case PanTilt::Axis::ID::PAN:
			set_AngleLimitMax_Pan( dVal );
			break;

		case PanTilt::Axis::ID::TILT:
			set_AngleLimitMax_Tilt( dVal );
			break;
		}
	}	
	
	// ---------------------------------------
	// Get Pan and Tilt minimum speed
	for( int i=0 ; i<nAxis ; i++ )
	{
		if( !Write_Command_receive_1_Parameter( i, 'L', &dVal ) ) return false;

		dVal *= dResolution.at( i );

		switch( i )
		{
		case PanTilt::Axis::ID::PAN:
			set_SpeedLimitMin_Pan( dVal );
			break;

		case PanTilt::Axis::ID::TILT:
			set_SpeedLimitMin_Tilt( dVal );
			break;
		}
	}

	// ---------------------------------------
	// Get Pan and Tilt maximum speed
	for( int i=0 ; i<nAxis ; i++ )
	{
		if( !Write_Command_receive_1_Parameter( i, 'U', &dVal ) ) return false;

		dVal *= dResolution.at( i );
		
		switch( i )
		{
		case PanTilt::Axis::ID::PAN:
			set_SpeedLimitMax_Pan( dVal );
			break;

		case PanTilt::Axis::ID::TILT:
			set_SpeedLimitMax_Tilt( dVal );
			break;
		}
	}

	bOpened = true;

	return bOpened;
}

bool yarp::dev::PanTilt_PTUD46::open( yarp::os::Searchable& config )
{
	std::string sCom = config.check( "com", yarp::os::Value( "COM1" ), "Com Port: COM1, COM2, COM3, COM4" ).asString();
	int nBaudRate = config.check( "baud", yarp::os::Value( 38400 ), "BaudRate: 600, 1200, 2400, 4800, 9600, 19200, 38400" ).asInt();
	
	return open( sCom, nBaudRate );
}

bool yarp::dev::PanTilt_PTUD46::close( void )
{
	bOpened = false;
	return Serial_Port.close();
}

//-------------------------------------------------------------------------------------------------
// Limit accessors
void yarp::dev::PanTilt_PTUD46::set_AngleLimitMax_Pan( double val )
{
	mutex_limits.wait();
	dAngleLimit_Pan.at( MAX ) = val;
	mutex_limits.post();
}

void yarp::dev::PanTilt_PTUD46::set_AngleLimitMin_Pan( double val )
{
	mutex_limits.wait();
	dAngleLimit_Pan.at( MIN ) = val;
	mutex_limits.post();
}

void yarp::dev::PanTilt_PTUD46::set_AngleLimitMax_Tilt( double val )
{
	mutex_limits.wait();
	dAngleLimit_Tilt.at( MAX ) = val;
	mutex_limits.post();
}

void yarp::dev::PanTilt_PTUD46::set_AngleLimitMin_Tilt( double val )
{
	mutex_limits.wait();
	dAngleLimit_Tilt.at( MIN ) = val;
	mutex_limits.post();
}

void yarp::dev::PanTilt_PTUD46::set_SpeedLimitMax_Pan( double val )
{
	mutex_limits.wait();
	dSpeedLimit_Pan.at( MAX ) = val;
	mutex_limits.post();
}

void yarp::dev::PanTilt_PTUD46::set_SpeedLimitMin_Pan( double val )
{
	mutex_limits.wait();
	dSpeedLimit_Pan.at( MIN ) = val;
	mutex_limits.post();
}

void yarp::dev::PanTilt_PTUD46::set_SpeedLimitMax_Tilt( double val )
{
	mutex_limits.wait();
	dSpeedLimit_Tilt.at( MAX ) = val;
	mutex_limits.post();
}

void yarp::dev::PanTilt_PTUD46::set_SpeedLimitMin_Tilt( double val )
{
	mutex_limits.wait();
	dSpeedLimit_Tilt.at( MIN ) = val;
	mutex_limits.post();
}

double yarp::dev::PanTilt_PTUD46::get_AngleLimitMax_Pan( void )
{
	mutex_limits.wait();
	double val = dAngleLimit_Pan.at( MAX );
	mutex_limits.post();
	return val;
}

double yarp::dev::PanTilt_PTUD46::get_AngleLimitMin_Pan( void )
{
	mutex_limits.wait();
	double val = dAngleLimit_Pan.at( MIN );
	mutex_limits.post();
	return val;
}

double yarp::dev::PanTilt_PTUD46::get_AngleLimitMax_Tilt( void )
{
	mutex_limits.wait();
	double val = dAngleLimit_Tilt.at( MAX );
	mutex_limits.post();
	return val;
}

double yarp::dev::PanTilt_PTUD46::get_AngleLimitMin_Tilt( void )
{
	mutex_limits.wait();
	double val = dAngleLimit_Tilt.at( MIN );
	mutex_limits.post();
	return val;
}

double yarp::dev::PanTilt_PTUD46::get_SpeedLimitMax_Pan( void )
{
	mutex_limits.wait();
	double val = dSpeedLimit_Pan.at( MAX );
	mutex_limits.post();
	return val;
}

double yarp::dev::PanTilt_PTUD46::get_SpeedLimitMin_Pan( void )
{
	mutex_limits.wait();
	double val = dSpeedLimit_Pan.at( MIN );
	mutex_limits.post();
	return val;
}

double yarp::dev::PanTilt_PTUD46::get_SpeedLimitMax_Tilt( void )
{
	mutex_limits.wait();
	double val = dSpeedLimit_Tilt.at( MAX );
	mutex_limits.post();
	return val;
}

double yarp::dev::PanTilt_PTUD46::get_SpeedLimitMin_Tilt( void )
{
	mutex_limits.wait();
	double val = dSpeedLimit_Tilt.at( MIN );
	mutex_limits.post();
	return val;
}

//-------------------------------------------------------------------------------------------------
// Axis Info

bool yarp::dev::PanTilt_PTUD46::getAxes( int *ax )
{
	*ax = nAxis;
	return true;
}

bool yarp::dev::PanTilt_PTUD46::getAxisName( int axis, yarp::os::ConstString& name )
{
	if( !isValidAxis( axis ) ) return false;
	
	name = yarp::os::ConstString( PanTilt::Axis::Name[axis].c_str(), PanTilt::Axis::Name[axis].size() );
	
	return true;
}

bool yarp::dev::PanTilt_PTUD46::getAxisName( int axis, std::string& name )
{
	if( !isValidAxis( axis ) ) return false;

	name = PanTilt::Axis::Name[axis];
	return true;
}

//-------------------------------------------------------------------------------------------------
// Raw position control

bool yarp::dev::PanTilt_PTUD46::setPositionModeRaw( void )
{
	if( !Write_Command_Discard_Reply( "CI\r" ) ) return false;
	
	nControlMode = PanTilt::ControlMode::POSITION;
	return true;
}

bool yarp::dev::PanTilt_PTUD46::positionMoveRaw( int j, double ref )
{
	return Write_Command_send_1_Parameter( j, 'P', ref );
}

bool yarp::dev::PanTilt_PTUD46::positionMoveRaw( const double *refs )
{
	for( int i=0 ; i<nAxis ; i++ ) 
		if( !positionMoveRaw( i, refs[i] ) ) return false;

	return true;
}

bool yarp::dev::PanTilt_PTUD46::relativeMoveRaw( int j, double delta )
{
	return Write_Command_send_1_Parameter( j, 'O', delta );
}

bool yarp::dev::PanTilt_PTUD46::relativeMoveRaw( const double *deltas )
{
	for( int i=0 ; i<nAxis ; i++ ) 
		if( !relativeMoveRaw( i, deltas[i] ) ) return false;

	return true;
}

bool yarp::dev::PanTilt_PTUD46::checkMotionDoneRaw( int j, bool *flag )
{
	// Not available
	return false;
}

bool yarp::dev::PanTilt_PTUD46::checkMotionDoneRaw( bool *flag )
{
	bool bFlag = true;

	*flag = true;

	for( int i=0 ; i<nAxis ; i++ ) 
	{
		if( !checkMotionDoneRaw( i, &bFlag ) ) return false;
		if( !bFlag ) *flag = false;
	}

	return true;
}

bool yarp::dev::PanTilt_PTUD46::waitMotionDone( void )
{
	if( nControlMode != PanTilt::ControlMode::POSITION ) return false;

	else return Write_Command_Discard_Reply( "A\r" );
}

bool yarp::dev::PanTilt_PTUD46::setRefSpeedRaw( int j, double sp )
{
	return Write_Command_send_1_Parameter( j, 'S', sp );
}

bool yarp::dev::PanTilt_PTUD46::setRefSpeedsRaw( const double *spds )
{
	for( int i=0 ; i<nAxis ; i++ ) 
		if( !setRefSpeedRaw( i, spds[i] ) ) return false;

	return true;
}

bool yarp::dev::PanTilt_PTUD46::setRefAccelerationRaw( int j, double acc )
{
	return Write_Command_send_1_Parameter( j, 'A', acc );
}

bool yarp::dev::PanTilt_PTUD46::setRefAccelerationsRaw( const double *accs )
{
	for( int i=0 ; i<nAxis ; i++ ) 
		if( !setRefAccelerationRaw( i, accs[i] ) ) return false;

	return true;
}

bool yarp::dev::PanTilt_PTUD46::getPositionRaw( int j, double *ref )
{
	return Write_Command_receive_1_Parameter( j, 'P', ref );
}

bool yarp::dev::PanTilt_PTUD46::getPositionsRaw( double *refs )
{
	for( int i=0 ; i<nAxis ; i++ ) 
		if( !getPositionRaw( i, &refs[i] ) ) return false;

	return true;
}

bool yarp::dev::PanTilt_PTUD46::getRefSpeedRaw( int j, double *ref )
{
	return Write_Command_receive_1_Parameter( j, 'S', ref );
}

bool yarp::dev::PanTilt_PTUD46::getRefSpeedsRaw( double *spds )
{
	for( int i=0 ; i<nAxis ; i++ ) 
		if( !getRefSpeedRaw( i, &spds[i] ) ) return false;

	return true;
}

bool yarp::dev::PanTilt_PTUD46::getRefAccelerationRaw( int j, double *acc )
{
	return Write_Command_receive_1_Parameter( j, 'A', acc );
}

bool yarp::dev::PanTilt_PTUD46::getRefAccelerationsRaw( double *accs )
{
	for( int i=0 ; i<nAxis ; i++ ) 
		if( !getRefAccelerationRaw( i, &accs[i] ) ) return false;

	return true;
}

bool yarp::dev::PanTilt_PTUD46::stopRaw( int j )
{
	return Write_Command_Discard_Reply( j, 'H' );
}

bool yarp::dev::PanTilt_PTUD46::stopRaw( void )
{
	return Write_Command_Discard_Reply( "H\r" );
}

//-------------------------------------------------------------------------------------------------
// Position control

bool yarp::dev::PanTilt_PTUD46::setPositionMode()
{
	return setPositionModeRaw();
}

bool yarp::dev::PanTilt_PTUD46::positionMove( int j, double ref )
{
	return positionMoveRaw( j, ref/dResolution.at( j ) );
}

bool yarp::dev::PanTilt_PTUD46::positionMove( const double *refs )
{
	double dRefs[PanTilt::Axis::ID::PANTILT_NUMBER_OF_AXES];

	for( int i=0 ; i<nAxis ; i++ )
		dRefs[i] = refs[i]/dResolution.at( i );

	return positionMoveRaw( dRefs );
}

bool yarp::dev::PanTilt_PTUD46::relativeMove( int j, double delta )
{
	return relativeMoveRaw( j, delta/dResolution.at( j ) );
}

bool yarp::dev::PanTilt_PTUD46::relativeMove( const double *deltas )
{
	double dDeltas[PanTilt::Axis::ID::PANTILT_NUMBER_OF_AXES];

	for( int i=0 ; i<nAxis ; i++ )
		dDeltas[i] = dDeltas[i]/dResolution.at( i );

	return relativeMoveRaw( dDeltas );
}

bool yarp::dev::PanTilt_PTUD46::checkMotionDone( int j, bool *flag )
{
	return checkMotionDoneRaw( j, flag );
}

bool yarp::dev::PanTilt_PTUD46::checkMotionDone( bool *flag )
{
	return checkMotionDoneRaw( flag );
}

bool yarp::dev::PanTilt_PTUD46::setRefSpeed( int j, double sp )
{
	return setRefSpeedRaw( j, sp/dResolution.at( j ) );
}

bool yarp::dev::PanTilt_PTUD46::setRefSpeeds( const double *spds )
{
	double dSpds[PanTilt::Axis::ID::PANTILT_NUMBER_OF_AXES];

	for( int i=0 ; i<nAxis ; i++ )
		dSpds[i] = spds[i]/dResolution.at( i );

	return setRefSpeedsRaw( dSpds );
}

bool yarp::dev::PanTilt_PTUD46::setRefAcceleration( int j, double acc )
{
	return setRefAccelerationRaw( j, acc/dResolution.at( j ) );
}

bool yarp::dev::PanTilt_PTUD46::setRefAccelerations( const double *accs )
{
	double dAccs[PanTilt::Axis::ID::PANTILT_NUMBER_OF_AXES];

	for( int i=0 ; i<nAxis ; i++ )
		dAccs[i] = accs[i]/dResolution.at( i );

	return setRefAccelerationsRaw( dAccs );
}

bool yarp::dev::PanTilt_PTUD46::getPosition( int j, double *ref )
{
	if( !getPositionRaw( j, ref ) ) return false;

	*ref *= dResolution.at( j );
	return true;
}

bool yarp::dev::PanTilt_PTUD46::getPositions( double *refs)
{
	if( !getPositionsRaw( refs ) ) return false;

	for( int i=0; i<nAxis ; i++ )
		refs[i] *= dResolution.at( i );

	return true;
}

bool yarp::dev::PanTilt_PTUD46::getRefSpeed( int j, double *ref )
{
	if( !getRefSpeedRaw( j, ref ) ) return false;

	*ref *= dResolution.at( j );
	return true;
}

bool yarp::dev::PanTilt_PTUD46::getRefSpeeds( double *spds)
{
	if( !getRefSpeedsRaw( spds ) ) return false;

	for( int i=0; i<nAxis ; i++ )
		spds[i] *= dResolution.at( i );

	return true;
}

bool yarp::dev::PanTilt_PTUD46::getRefAcceleration( int j, double *acc )
{
	if( !getRefSpeedRaw( j, acc ) ) return false;

	*acc *= dResolution.at( j );
	return true;
}

bool yarp::dev::PanTilt_PTUD46::getRefAccelerations( double *accs )
{
	if( !getRefAccelerationsRaw( accs ) ) return false;

	for( int i=0; i<nAxis ; i++ )
		accs[i] *= dResolution.at( i );

	return true;
}

bool yarp::dev::PanTilt_PTUD46::stop( int j )
{
	return stopRaw( j );
}

bool yarp::dev::PanTilt_PTUD46::stop( void )
{
	return stopRaw();
}

//-------------------------------------------------------------------------------------------------
// Raw velocity control

bool yarp::dev::PanTilt_PTUD46::setVelocityModeRaw( void )
{
	if( !Write_Command_Discard_Reply( "CV\r" ) ) return false;

	nControlMode = PanTilt::ControlMode::VELOCITY;
	return true;
}

bool yarp::dev::PanTilt_PTUD46::velocityMoveRaw( int j, double sp )
{
	if( nControlMode == PanTilt::ControlMode::VELOCITY ) 
		setVelocityModeRaw();

	return setRefSpeedRaw( j, sp );
}

bool yarp::dev::PanTilt_PTUD46::velocityMoveRaw( const double *sp )
{
	if( nControlMode != PanTilt::ControlMode::VELOCITY )
	{
		if( !setVelocityModeRaw() ) return false;
	}

	return setRefSpeedsRaw( sp );
}

bool yarp::dev::PanTilt_PTUD46::velocityMoveRelativeRaw( int j, double sp )
{
	if( nControlMode != PanTilt::ControlMode::VELOCITY )
		if( !setVelocityModeRaw() ) return false;

	return Write_Command_send_1_Parameter( j, 'D', sp );
}

bool yarp::dev::PanTilt_PTUD46::velocityMoveRelativeRaw( const double *spds )
{
	for( int i=0 ; i<nAxis ; i++ ) 
		if( !velocityMoveRelativeRaw( i, spds[i] ) ) return false;

	return true;
}

//-------------------------------------------------------------------------------------------------
// Velocity control

bool yarp::dev::PanTilt_PTUD46::setVelocityMode( void )
{
	return setVelocityModeRaw();
}

bool yarp::dev::PanTilt_PTUD46::velocityMove( int j, double sp )
{
	return velocityMoveRaw( j, sp/dResolution.at( j ) );
}

bool yarp::dev::PanTilt_PTUD46::velocityMove( const double *spds )
{
	double dSpds[PanTilt::Axis::ID::PANTILT_NUMBER_OF_AXES];

	for( int i=0 ; i<nAxis ; i++ )
		dSpds[i] = spds[i]/dResolution.at( i );

	return velocityMoveRaw( dSpds );
}

bool yarp::dev::PanTilt_PTUD46::velocityMoveRelative( int j, double sp )
{
	return velocityMoveRelativeRaw( j, sp );
}

bool yarp::dev::PanTilt_PTUD46::velocityMoveRelative( const double *spds )
{
	double dSpds[PanTilt::Axis::ID::PANTILT_NUMBER_OF_AXES];

	for( int i=0 ; i<nAxis ; i++ )
		dSpds[i] = spds[i]/dResolution.at( i );

	return velocityMoveRelativeRaw( dSpds );
}

//-------------------------------------------------------------------------------------------------

bool yarp::dev::PanTilt_PTUD46::reset( int j )
{
	return Write_Command_Discard_Reply( j, 'R' );
}

bool yarp::dev::PanTilt_PTUD46::reset()
{
	return Write_Command_Discard_Reply( "R\r" );
}
