#include "AmtecLWA7dof.h"
#include <algorithm>
#include <sstream>

#ifndef DEG_TO_RAD_F
#define DEG_TO_RAD_F 0.017453292519943295769236907684886f
#endif

#ifndef RAD_TO_DEG_F
#define RAD_TO_DEG_F 57.295779513082320876798154814105f
#endif

yarp::dev::AmtecLWA7dof::AmtecLWA7dof( void )
: InitString( "ESD:0,500" )
, nDevice( -1 )
, mutex_PCube( 1 )
, bHasGripper( false )
, Number_of_Joints( 0 )
, Conf_Gripper( "" )
, Conf_no_Gripper( "" )
, use_sync_motion( false )
{
	// Configuration files at: %MAINPROJECT%/Hardware/YARP_Modules/amtec_lwa_7dof/Init_Files

	std::string file_path = "";
	char *env = getenv( "MAINPROJECT" );
	if( env != NULL )
	{
		file_path = env;
		int pos = file_path.find_first_of( '\\', 0 );

		if( pos != std::string::npos ) file_path[pos] = '/';
	
		file_path += "/Hardware/YARP_Modules/amtec_lwa_7dof/Init_Files/";
	}
	else
	{	//When no File system based on MARL repository layout exists
		file_path = "amtec_lwa_7dof/Init_Files/";
	}

	Conf_Gripper = file_path;
	Conf_no_Gripper = file_path;

	Conf_Gripper += "LWAFTS6_Schunk_Spain_Robot2.ini";
	Conf_no_Gripper += "LWAFTS5_Schunk_Spain_Robot2.ini";

	Conf_Gripper_right = file_path;
	Conf_no_Gripper_right = file_path;
	Conf_Gripper_left = file_path;
	Conf_no_Gripper_left = file_path;

	Conf_Gripper_right += "LWAFTS6_Schunk_Spain_Robot2.ini";
	Conf_no_Gripper_right += "LWAFTS5_Schunk_Spain_Robot2.ini";
	Conf_Gripper_left += "LWAFTS6_Schunk_Spain_Robot2.ini";
	Conf_no_Gripper_left += "LWAFTS5_Schunk_Spain_Robot2_left.ini";
}

yarp::dev::AmtecLWA7dof::~AmtecLWA7dof( void )
{
	// Ensure device is closed
	close();
}

//------------------------------------------------------------------------------
// Device Driver implementation, specific functions
// Virtual functions from DeviceDriver class

bool yarp::dev::AmtecLWA7dof::open( int nGripper, int narm, bool use_sync_motion_ )
{
	if( narm == LEFT_ARM )
		InitString = "ESD:2,500";
	else
		InitString = "ESD:0,500";
	
	mutex_PCube.wait();
	error_code_ = PCube_openDevice( &nDevice, InitString.c_str() );
	mutex_PCube.post();

	if( error_code_ < 0 ) 
	{
		error_str_ = Get_Error_Function_String( error_code_ );
		std::cout << "Error: " << error_str_ << std::endl;
		return false;
	}

	mutex_PCube.wait();

	if( nGripper )
	{
		error_code_ = PCube_configFromFile( Conf_Gripper.c_str() );
	}
	else if(narm==RIGHT_ARM)
	{
		error_code_ = PCube_configFromFile( Conf_no_Gripper_right.c_str() );
	}
	else
	{
		error_code_ = PCube_configFromFile( Conf_no_Gripper_left.c_str() );
	}
	
	mutex_PCube.post();

	if( error_code_ < 0 ) 
	{
		error_str_ = Get_Error_Function_String( error_code_ );
		std::cout << "Error: " << error_str_ << std::endl;
		return false;
	}
	
	else
	{
		// this function also updates class member 'Number_of_Joints' and 'bHasGripper'
		int num_joints = getNumberOfJoints();

		if( num_joints < 6 )
		{
			//set error_str and error_code_ to other
			error_str_ = "Could not find joints! Power down?: ";
			error_str_ += Get_Error_Function_String( error_code_ );
			error_code_ = Amtec::Error::ERROR_OTHER;
			std::cout << error_str_ << std::endl;
			return false;
		}

		//even if we could get Number of joints, there may exists an error reading data from modules
		if( !isArmStatusOK() )
		{
			std::cout << "Error: " << Get_Error_Str() << std::endl;
			return false;
		}

		vec_RefSpeed.assign( num_joints, 0.0f );
		vec_RefAcc.assign( num_joints, 0.0f );
		vec_Direction.assign( num_joints, 1.0f );
		vec_OffSet.assign( num_joints, 0.0f );

		unsigned long ulSerialNo = 0;

		// Get the serial number of the first module in each arm,
		// this allows us to identify which arm we are working with
		// and apply the correct offsets and direction corrections
		mutex_PCube.wait();
		error_code_ = PCube_getModuleSerialNo( nDevice, 1, &ulSerialNo );
		mutex_PCube.post();

		if( error_code_ < 0 ) return false;

		bool bSuccess = true;

		switch( ulSerialNo )
		{
		//LWA7DOF no 1: for repair
		/*case :
			vec_Direction.at( 5 ) = -1.0f;
			break;*/

		// LWA7DOF no 2: 3132, 3138, 3139, 3135, 3141, 3163, 3164
		case 3132:
			vec_Direction.at( 5 ) = -1.0f;
			break;

		// LWA7DOF no 3: 3351, 3347, 3346, 3345, 3344, 3268, 3269
		case 3351:
			vec_OffSet.at( 0 ) = 1.3f*DEG_TO_RAD_F;
			vec_OffSet.at( 1 ) = -0.5f*DEG_TO_RAD_F;
			vec_OffSet.at( 2 ) = 3.0f*DEG_TO_RAD_F;
			vec_OffSet.at( 3 ) = -2.0f*DEG_TO_RAD_F;
			break;

		default:
			bSuccess = false;
			break;
		}

		vec_Direction.at( 6 ) = -1.0f;

		if(use_sync_motion_)
		{
			use_sync_motion = true;
			for (int modulenr = 1; modulenr <= num_joints; modulenr++)
			{
				unsigned long config;
				error_code_ = PCube_getConfig(nDevice, modulenr, &config);
				if (error_code_ < 0) return false;
				config |= CONFIGID_MOD_SYNC_MOTION;
				error_code_ = PCube_setConfig(nDevice, modulenr, config);
				if (error_code_ < 0) return false;
			}
		}
		else
		{
			use_sync_motion = false;
			for (int modulenr = 1; modulenr <= num_joints; modulenr++)
			{
				unsigned long config;
				error_code_ = PCube_getConfig(nDevice, modulenr, &config);
				if (error_code_ < 0) return false;
				config &= ~CONFIGID_MOD_SYNC_MOTION;
				error_code_ = PCube_setConfig(nDevice, modulenr, config);
				if (error_code_ < 0) return false;
			}
		}


		return bSuccess;
	}
}

bool yarp::dev::AmtecLWA7dof::open( yarp::os::Searchable& config )
{
	arm_config = config.check( "gripper", yarp::os::Value( 0 ), "'1' if you want to use the Amtec gripper, otherwise '0'" ).asInt();

	arm_nr = config.check( "arm", yarp::os::Value( 0 ), "'0' if the arm is mounted on the right side, '0' if the arm is mounted on the left side" ).asInt();

	bool use_sync = config.check("use_sync_motion") ? config.find("use_sync_motion").asBool() : false;
	/*
	std::string sRobot = config.check( "robot", yarp::os::Value("aros"), "the robot where the arm is mounted: aros, jumbo or dumbo" ).asString();

	if( sRobot.compare( "jumbo" ) == 0 ) robot_nr = AMTEC_AT_JUMBO;
	else if( sRobot.compare( "dumbo" ) == 0 ) robot_nr = AMTEC_AT_DUMBO;
	else robot_nr = AMTEC_AT_AROS;
	*/

	return open(arm_config, arm_nr, use_sync);
}

bool yarp::dev::AmtecLWA7dof::close( void )
{
	if( nDevice != -1 )
	{
		if( !stop() ) return false;

		mutex_PCube.wait();
		PCube_closeDevice( nDevice );
		mutex_PCube.post();

		nDevice = -1;

		return true;
	}
	else 
		return false;
}

// End of Device Driver implementation, specific functions
// -----------------------------------------------------------------------------


//----------------------------------------------------------------------
//		Implemented virtual functions from IPositionControl

bool yarp::dev::AmtecLWA7dof::stop( int j )
{
	mutex_PCube.wait();
	error_code_ = PCube_haltModule( nDevice, j + 1 );
	mutex_PCube.post();

	return ( error_code_ >= 0 );
}

bool yarp::dev::AmtecLWA7dof::stop( void )
{
	mutex_PCube.wait();
	error_code_ = PCube_haltAll( nDevice );
	mutex_PCube.post();

	return ( error_code_ >= 0 );
}

bool yarp::dev::AmtecLWA7dof::getAxes( int *ax )
 {
	*ax = Number_of_Joints;
	return true;
 }

bool yarp::dev::AmtecLWA7dof::positionMove( int j, double ref )
{
	mutex_PCube.wait();
	error_code_ = PCube_movePos( nDevice, j + 1, static_cast<float>( ref ) * vec_Direction[j] + vec_OffSet[j] );
	mutex_PCube.post();

	return ( error_code_ >= 0 );
}

bool yarp::dev::AmtecLWA7dof::positionMove( const double *refs )
{
	bool bSuccess = true;
	
	mutex_PCube.wait();
	for( int iJoint = 0; iJoint<Number_of_Joints; iJoint++ )
	{
		error_code_ = PCube_movePos( nDevice, iJoint+1, static_cast<float>( refs[iJoint] ) * vec_Direction[iJoint] + vec_OffSet[iJoint] );
		if( error_code_ < 0 ) bSuccess = false;
	}
	mutex_PCube.post();

	return bSuccess;
}

bool yarp::dev::AmtecLWA7dof::relativeMove( int j, double delta )
{
	float fPos = 0.0f;
	if( !getPos( j, &fPos ) ) return false;

	return positionMove( j, static_cast<double>( fPos ) + delta );
}

bool yarp::dev::AmtecLWA7dof::relativeMove( const double *deltas )
{
	bool bSuccess = true;

	for( int iJoint=0 ; iJoint<Number_of_Joints ; iJoint++ )
	{
		if( !relativeMove( iJoint, deltas[iJoint] ) ) 
			bSuccess = false;
	}

	return bSuccess;
}

bool yarp::dev::AmtecLWA7dof::setRefSpeed( int j, double sp )
{
	mutex_PCube.wait();
	error_code_ = PCube_setRampVel( nDevice, j, static_cast<float>( sp ) );
	mutex_PCube.post();
	
	if( error_code_ >= 0 )
	{
		vec_RefSpeed[j] = static_cast<float>( sp );
		return true;
	}
	else
		return false;
}

bool yarp::dev::AmtecLWA7dof::setRefSpeeds( const double *spds )
{
	bool bSuccess = true;
	
	for( int iJoint=0 ; iJoint<Number_of_Joints ; iJoint++ )
	{
		if( !setRefSpeed( iJoint, spds[iJoint] ) ) 
			bSuccess = false;
	}
	
	return bSuccess;
}

bool yarp::dev::AmtecLWA7dof::setRefAcceleration( int j, double acc )
{
	mutex_PCube.wait();
	error_code_ = PCube_setRampAcc( nDevice, j, static_cast<float>( acc ) );
	mutex_PCube.post();
	
	if( error_code_ >= 0 )
	{
		vec_RefAcc[j] = static_cast<float>( acc );
		return true;
	}
	else
		return false;
}

bool yarp::dev::AmtecLWA7dof::setRefAccelerations( const double *accs )
{
	bool bSuccess = true;
	
	for( int iJoint=0 ; iJoint<Number_of_Joints ; iJoint++ )
	{
		if( !setRefAcceleration( iJoint, accs[iJoint] ) )
			bSuccess = false;
	}
	
	return bSuccess;
}

bool yarp::dev::AmtecLWA7dof::getRefSpeed( int j, double *ref )
{
	*ref = static_cast<double>( vec_RefSpeed[j] );
	return true;
}

bool yarp::dev::AmtecLWA7dof::getRefSpeeds( double *spds )
{
	double dVal = 0.0;
	
	for( int iJoint=0 ; iJoint<Number_of_Joints ; iJoint++ )
	{
		getRefSpeed( iJoint, &dVal );
		spds[iJoint] = dVal;
	}
	
	return true;
}

bool yarp::dev::AmtecLWA7dof::getRefAcceleration( int j, double *acc )
{
	*acc = static_cast<double>( vec_RefAcc[j] );
	return true;
}

bool yarp::dev::AmtecLWA7dof::getRefAccelerations( double *accs )
{
	double dVal = 0.0;
	
	for( int iJoint=0 ; iJoint<Number_of_Joints ; iJoint++ )
	{
		getRefAcceleration( iJoint, &dVal );
		accs[iJoint] = dVal;
	}
	
	return true;
}

bool yarp::dev::AmtecLWA7dof::checkMotionDone( int j, bool *flag )
{
	*flag = Test_Joint_Flag( j, Amtec::Module::State::STAT_MOTION );
	return *flag;
}

bool yarp::dev::AmtecLWA7dof::checkMotionDone( bool *flag )
{
	bool bDone = true;

	for( int iJoint=0 ; iJoint<Number_of_Joints ; iJoint++ )
	{
		if( !checkMotionDone( iJoint, flag ) ) bDone = false;
	}

	return bDone;
}

//		End of implemented virtual functions from IPositionControl
//----------------------------------------------------------------------

//----------------------------------------------------------------------
//		virtual functions from IVelocityControl
//----------------------------------------------------------------------

bool yarp::dev::AmtecLWA7dof::setVelocityMode()
{
	return false;
}

bool yarp::dev::AmtecLWA7dof::velocityMove( int j, double sp )
{
	mutex_PCube.wait();
	error_code_ = PCube_moveVel( nDevice, j + 1, static_cast<float>( sp ) * vec_Direction[j] );
	mutex_PCube.post();

	return ( error_code_ >= 0 );
}

bool yarp::dev::AmtecLWA7dof::velocityMove( const double *sp )
{
	bool success = true;

	for( int iJoint = 0; iJoint < Number_of_Joints && success; iJoint++) 
	{
		success = velocityMove( iJoint, sp[iJoint] );

		//if we could not set velocity for one joint, stop all others!
		if( !success ) stop();
	}
	if (use_sync_motion)
	{
		mutex_PCube.wait();
		error_code_ = PCube_startMotionAll(nDevice);
		mutex_PCube.post();
	}

	return are_Modules_OK();
}
	
// End of virtual functions from IVelocityControl
//----------------------------------------------------------------------


//----------------------------------------------------------------------
// Extra functions for Amtec Light Weight Arm control
// All the 'bool' functions return 'true' if the function was sucessfull, 'false' otherwise

bool yarp::dev::AmtecLWA7dof::positionMove( int j, float ref, unsigned short time, bool wait_end )
{
	float value = ref * vec_Direction[j] + vec_OffSet[j];

	error_code_ = PCube_moveStep( nDevice, j + 1, value, time );
	if( error_code_ < 0 )
		return false;

	if( wait_end )
	{
		mutex_PCube.wait();
		error_code_ = PCube_waitForMotionEnd( nDevice, j + 1, time );
		mutex_PCube.post();
	}

	return ( error_code_ >= 0 );
}

bool yarp::dev::AmtecLWA7dof::positionMoveSync(const float *refs, unsigned short time, bool wait_end)
{
	bool
		bSuccess = true;

	//do not allow 0ms, because it would make the modules to simply open the brake
	if(time == 0)
		time = 1;

	int iJoint = 0;

	for (iJoint = 0; iJoint<Number_of_Joints; iJoint++)
	{
		float value = refs[iJoint] * vec_Direction[iJoint] + vec_OffSet[iJoint];

		error_code_ = PCube_moveStep(nDevice, iJoint + 1, value, time);
		if (error_code_ < 0)
		{
			bSuccess = false;
			break;
		}
	}

	if (!bSuccess)
	{
		stop();
		return false;
	}
	if (use_sync_motion)
	{
		error_code_ = PCube_startMotionAll(nDevice);
		if (error_code_ < 0)
			return false;
	}

	return wait_end ? waitMotionEnd(10000) : true;
}

bool yarp::dev::AmtecLWA7dof::positionMove( float *refs, unsigned short time, bool wait_end )
{	
	bool 
		bSuccess = true,
		bEnd = false;

	int iJoint = 0;
	
	for( iJoint=0 ; iJoint<Number_of_Joints ; iJoint++ )
	{
		if( !positionMove( iJoint, refs[iJoint], time ) )
			bSuccess = false;
	}
	
	if( !bSuccess )
	{
		stop();
		return false;
	}

	if( !are_Modules_OK() )
		return false;

	if (use_sync_motion)
	{
		error_code_ = PCube_startMotionAll(nDevice);
		if (error_code_ < 0)
			return false;
	}

	return wait_end ? waitMotionEnd( 10000 ) : bSuccess;
}

bool yarp::dev::AmtecLWA7dof::positionMove( float *refs, bool *joints2move, unsigned short time, bool wait_end )
{
	bool bSuccess = true;

	int 
		end = 0,
		iJoint = 0;

		//move only selected joints
	for( iJoint=0 ; iJoint<Number_of_Joints; iJoint++ )
	{
		if( joints2move[iJoint] )
		{
			if(!positionMove( iJoint, refs[iJoint], time, wait_end ) ) 
				bSuccess = false;
		}
	}

	if( !bSuccess )
	{
		stop();
		return false;
	}

	if (use_sync_motion)
	{
		error_code_ = PCube_startMotionAll(nDevice);
		if (error_code_ < 0)
			return false;
	}
	
	return are_Modules_OK();
}

bool yarp::dev::AmtecLWA7dof::positionMove( float * refs, unsigned short * times )
{
	bool bSuccess = true;

	for( int iJoint=0 ; iJoint<Number_of_Joints ; iJoint++ )
	{
		if( !positionMove( iJoint, refs[iJoint], times[iJoint] ) ) 
			bSuccess = false;
	}

	if( !bSuccess )
	{
		stop();
		return false;
	}

	if (use_sync_motion)
	{
		error_code_ = PCube_startMotionAll(nDevice);
		if (error_code_ < 0)
			return false;
	}

	mutex_PCube.wait();
	error_code_ = PCube_waitForMotionEndAll( nDevice, 10000 ); //miliseconds
	mutex_PCube.post();

	return ( error_code_ >= 0 );
}

/*inline bool yarp::dev::AmtecLWA7dof::positionMove( float * refs, unsigned short time )
{
	bool bSuccess = true;
	double value=0.0;

	for( int iJoint=0 ; iJoint<Number_of_Joints ; iJoint++ )
	{
		if( !positionMove( iJoint, refs[iJoint], time ) ) 
			bSuccess = false;
	}

	unsigned timeout = 0.9*time;

	mutex_PCube.wait();
	int error = PCube_waitForMotionEndAll( nDevice, (unsigned long) timeout);
	mutex_PCube.post();

	if( error<0 ) 
		bSuccess = false;

	return bSuccess;
}*/

bool yarp::dev::AmtecLWA7dof::positionMove( float * refs, unsigned short time, bool wait_end, float stop_margin )
{
	bool bSuccess = true;
	
	int iJoint = 0;

	for( iJoint=0 ; iJoint<Number_of_Joints ; iJoint++ )
	{
		if( !positionMove( iJoint, refs[iJoint], time ) ) 
			bSuccess = false;
	}

	if( !bSuccess )
	{
		stop();
		return false;
	}

	if( !are_Modules_OK() ) return false;

	if (use_sync_motion)
	{
		error_code_ = PCube_startMotionAll(nDevice);
		if (error_code_ < 0)
			return false;
	}

	// Wait motion to be within 'stop_margin' degrees of completion
	return wait_end ? waitMotionEnd( refs, stop_margin ) : true;
}

bool yarp::dev::AmtecLWA7dof::positionMove( float * refs, float fVel )
{
	float 
		fPos = 0.0f,
		fAux = 0.0f;
		
	std::vector< float > vf_Times;
	std::vector< float >::iterator it_max;

	for( int i=0 ; i<Number_of_Joints ; i++ )
	{
		getPos( i, &fPos );
		fAux = fabs( refs[i] - fPos ) / fVel;
		vf_Times.push_back( fAux );
	}

	it_max = std::max_element( vf_Times.begin(), vf_Times.end() );
	
	if( it_max == vf_Times.end() ) return false;

	unsigned short usTime = static_cast<unsigned short>( *it_max * 1000.0f );

	if( usTime < 200 )
		usTime = 200;

	return positionMove( refs, usTime );
}

bool yarp::dev::AmtecLWA7dof::positionMoveRamp( int j, float pos, float vel, float acc )
{
	float actual_pos = 0.0f;

	mutex_PCube.wait();
	error_code_ = PCube_getPos( nDevice, j + 1, &actual_pos );
	mutex_PCube.post();
	
	if( error_code_ < 0 )
		return false;

	if (use_sync_motion)
	{
		//disable sync motion command for this module
		unsigned long config;
		error_code_ = PCube_getConfig(nDevice, j + 1, &config);
		if (error_code_ < 0) return false;
		config &= ~CONFIGID_MOD_SYNC_MOTION;
		error_code_ = PCube_setConfig(nDevice, j + 1, config);
		if (error_code_ < 0) return false;
	}
	
	mutex_PCube.wait();
	error_code_ = PCube_moveRamp( nDevice, j + 1, pos, vel, acc );
	mutex_PCube.post();
	
	if( error_code_ < 0 )
		return false;

	if (use_sync_motion)
	{
		//disable sync motion command for this module
		unsigned long config;
		error_code_ = PCube_getConfig(nDevice, j + 1, &config);
		if (error_code_ < 0) return false;
		config |= CONFIGID_MOD_SYNC_MOTION;
		error_code_ = PCube_setConfig(nDevice, j + 1, config);
		if (error_code_ < 0) return false;
	}

	unsigned long time = 45000;
	
	mutex_PCube.wait();
	error_code_ = PCube_waitForMotionEnd( nDevice, j + 1, time );
	mutex_PCube.post();
	
	return ( error_code_ >= 0 );
}


bool yarp::dev::AmtecLWA7dof::positionMoveRamp( int j, float pos )
{
	float 
		fVel = 0.0f,
		fAcc = 0.0f;
	
	mutex_PCube.wait();
	error_code_ = PCube_getHomeVel( nDevice, 7, &fVel );
	mutex_PCube.post();

	if( error_code_ < 0 )
		return false;

	mutex_PCube.wait();
	error_code_ = PCube_getMaxAcc( nDevice, 7, &fAcc );
	mutex_PCube.post();

	if( error_code_ < 0 )
		return false;
	else
		return positionMoveRamp( j, pos, fVel, fAcc/10.0f );
}

bool yarp::dev::AmtecLWA7dof::getPos( int nJoint, float * fPos )
{
	mutex_PCube.wait();
	error_code_ = PCube_getPos( nDevice, nJoint + 1, fPos );
	mutex_PCube.post();

	*fPos = *fPos * vec_Direction[nJoint] - vec_OffSet[nJoint];

	return ( error_code_ >= 0 );
}

bool yarp::dev::AmtecLWA7dof::getPosAll( float * fPos )
{
	mutex_PCube.wait();

	error_code_ = PCube_savePosAll( nDevice );

	if( error_code_ < 0 ) return false;

	bool bSuccess = true;
	
	for( int iJoint = 0; iJoint<Number_of_Joints; iJoint++ )
	{
		error_code_ = PCube_getSavePos( nDevice, iJoint + 1, &fPos[iJoint] );
		fPos[iJoint] = fPos[iJoint] * vec_Direction[iJoint] - vec_OffSet[iJoint];
		if( error_code_ < 0 ) 
			bSuccess = false;
	}

	mutex_PCube.post();

	return bSuccess;
}

bool yarp::dev::AmtecLWA7dof::getPosDeg( int nJoint, float * fPos )
{
	bool bSuccess = getPos( nJoint, fPos );

	if( bSuccess )
		*fPos *= RAD_TO_DEG_F;

	return bSuccess;
}

bool yarp::dev::AmtecLWA7dof::getPosAllDeg( float * fPos )
{
	bool bSuccess = getPosAll( fPos );

	if( bSuccess )
	{
		for( int iJoint = 0; iJoint < Number_of_Joints; iJoint++ )
		{
			fPos[iJoint] *= RAD_TO_DEG_F;
		}
	}

	return bSuccess;
}

bool yarp::dev::AmtecLWA7dof::getVel( int nJoint, float * fVel )
{
	mutex_PCube.wait();
	error_code_ = PCube_getVel( nDevice, nJoint + 1, fVel );
	mutex_PCube.post();

	return ( error_code_ >= 0 );
}

bool yarp::dev::AmtecLWA7dof::getMinLimit( int nJoint, float * fPos )
{
	mutex_PCube.wait();
	error_code_ = PCube_getMinPos( nDevice, nJoint + 1, fPos );
	mutex_PCube.post();

	if( error_code_ >= 0 )
	{
		if( nJoint < static_cast<int>( vec_Direction.size() ) )
		{
			*fPos = *fPos * vec_Direction[nJoint] - vec_OffSet[nJoint];
		}

		return true;
	}
	else
		return false;
}
	
bool yarp::dev::AmtecLWA7dof::getMaxLimit( int nJoint, float * fPos )
{
	mutex_PCube.wait();
	error_code_ = PCube_getMaxPos( nDevice, nJoint + 1, fPos );
	mutex_PCube.post();

	if( error_code_ >= 0 )
	{
		if( nJoint < static_cast<int>( vec_Direction.size() ) )
		{
			*fPos = *fPos * vec_Direction[nJoint] - vec_OffSet[nJoint];
		}
		return true;
	}
	else 
		return false;
}

int yarp::dev::AmtecLWA7dof::getNumberOfJoints( void )
{
	mutex_PCube.wait();
	int nCount = PCube_getModuleCount( nDevice );
	mutex_PCube.post();

	//Module count cannot be inferior to zero, it means that an error occurred.
	if( nCount <= 0 )
		return -1;

	unsigned char joint_type = Get_Joint_Type( nCount-1 );	// joints begin at '0'

		//Get_Joint_Type sets error_code_, so we must check it for errors.
	if( error_code_ < 0 )
		return -1;
	
	if( joint_type == Amtec::Module::Type::TYPE_LINEAR ) // test the type of the last found module
	{
		bHasGripper = true;
		Number_of_Joints = nCount - 1;
	}
	
	else Number_of_Joints = nCount;

	Number_of_Modules = nCount;

	return Number_of_Joints;
}

bool yarp::dev::AmtecLWA7dof::openGripper( unsigned short uiTime )
{
	int iGripper = 0;
	if( bHasGripper ) iGripper = Number_of_Joints;
	else return false;

	float fPos = 0.0f;
	getMaxLimit( iGripper, &fPos );
	
	mutex_PCube.wait();
	error_code_ = PCube_moveStep( nDevice, iGripper + 1, fPos, uiTime );
	mutex_PCube.post();
	
	return ( error_code_ >= 0 );
}

bool yarp::dev::AmtecLWA7dof::closeGripper( unsigned short uiTime )
{
	int iGripper = 0;
	if( bHasGripper ) iGripper = Number_of_Joints;
	else return false;

	float fPos = 0.0f;
	getMinLimit( iGripper, &fPos );

	mutex_PCube.wait();
	error_code_ = PCube_moveStep( nDevice, iGripper + 1, fPos, uiTime );
	mutex_PCube.post();

	if( error_code_ < 0 )
		return false;

	mutex_PCube.wait();
	error_code_ = PCube_waitForMotionEnd( nDevice, iGripper, 10000 );
	mutex_PCube.post();

	return ( error_code_ >= 0 );
}

bool yarp::dev::AmtecLWA7dof::moveGripperPos( float fPos, unsigned short uiTime )
{
	int iGripper = 0;
	if( bHasGripper ) iGripper = Number_of_Joints + 1;
	else return false;

	mutex_PCube.wait();
	error_code_ = PCube_moveStep( nDevice, iGripper, fPos, uiTime );
	mutex_PCube.post();

	if( error_code_ < 0 )
		return false;

	mutex_PCube.wait();
	error_code_ = PCube_waitForMotionEnd( nDevice, iGripper, 10000 );
	mutex_PCube.post();
	
	return ( error_code_ >= 0 );
}

bool yarp::dev::AmtecLWA7dof::getGripper( float *fPos )
{
	if( !bHasGripper )
		return false;

	mutex_PCube.wait();
	error_code_ = PCube_getPos( nDevice, Number_of_Joints + 1, fPos );
	mutex_PCube.post();
	
	return ( error_code_ >= 0 );
}

bool yarp::dev::AmtecLWA7dof::freeJoint(int nJoint)
{
	mutex_PCube.wait();
	error_code_ = PCube_moveCur( nDevice, nJoint + 1, 0.0 );
	mutex_PCube.post();

	return ( error_code_ >= 0 );
}

bool yarp::dev::AmtecLWA7dof::waitMotionEnd( unsigned timeout_ms, int *state )
{
	int st = 0;

	mutex_PCube.wait();
	error_code_ = PCube_waitForMotionEndAll( nDevice, timeout_ms );
	mutex_PCube.post();

	if( error_code_ == ERRID_DEV_WAITTIMEOUT )
	{
		//in this case we do not consider it an error!
		error_code_ = 0;
		st = -1;
	}
	
	if( state != nullptr )
		*state = st;

	return ( error_code_ >= 0 );
}

bool yarp::dev::AmtecLWA7dof::waitMotionEnd( float * refs, float stop_margin )
{
	bool 
		bEnd = false,
		bSuccess = true;

	float
		fPos = 0.0f,
		fMargin_Low = 0.0f,
		fMargin_High = 0.0f;

	for( int iJoint=0 ; iJoint<Number_of_Joints ; iJoint++ )
	{
		bEnd = false;
		
		fMargin_Low = refs[iJoint] - stop_margin;
		fMargin_High = refs[iJoint] + stop_margin;

		do
		{
			if( getPos( iJoint, &fPos ) )
			{
				if( ( fPos >= fMargin_Low ) && ( fPos <= fMargin_High ) )
				{
					bEnd = true;
				}
			}
			else
			{
				//error occurred getting position
				bSuccess = false;
				bEnd = true;
			}

		}while( !bEnd );
	}

	return bSuccess;
}

// End of extra functions for Amtec LWA arm control
//----------------------------------------------------------------------


//----------------------------------------------------------------------
	// Extra functions to Force Torque sensor
	// All the functions return error number.
int yarp::dev::AmtecLWA7dof::get_force_torque(float *data)
{
	short state = 0;
	
	// ModuleId must be ID found in label in FTC sensor. ChannelTypeId must be 0, or other seen in PStepDemo
	
	mutex_PCube.wait();
	error_code_ = PCube_getDataSCHUNK_FTC( nDevice, 5, 0, &data[0], &data[1], &data[2], &data[3], &data[4], &data[5], &state );
	mutex_PCube.post();

	return error_code_;
}

// End of extra functions to Force Torque sensor
//----------------------------------------------------------------------


//----------------------------------------------------------------------
// Specific functions for Amtec Light Weight Arm

bool yarp::dev::AmtecLWA7dof::HomeJoint( int nJoint )
{
	mutex_PCube.wait();
	int nCount = PCube_getModuleCount( nDevice );
	mutex_PCube.post();
	
	if( ( nDevice < 0 ) || ( nJoint >= nCount ) || ( nJoint < 0 ) ) return false;

	unsigned long timeOut = 30;		//seconds
	
	mutex_PCube.wait();
	error_code_ = PCube_homeModule( nDevice, nJoint+1 );
	mutex_PCube.post();

	if( error_code_ < 0 )
		return false;

	mutex_PCube.wait();
	error_code_ = PCube_waitForHomeEnd( nDevice, nJoint + 1, timeOut * 1000 );	//this timeout assumes the unit is 'ms'. Have to check this!!!
	mutex_PCube.post();

	if( error_code_ < 0 )
		return false;

	float 
		fVel = 0.0f,// = 4.0f*(3.1415f/180.0f);
		fPos = 0.0f;
	
	mutex_PCube.wait();
	error_code_ = PCube_getHomeVel( nDevice, nJoint + 1, &fVel );
	mutex_PCube.post();

	if( error_code_ < 0 )
		return false;

	mutex_PCube.wait();
	error_code_ = PCube_getPos( nDevice, nJoint + 1, &fPos );
	mutex_PCube.post();

	if( error_code_ < 0 )
		return false;

	int time = static_cast<int>( 1000 * ( fabs( fPos ) / fVel ) );

	if(use_sync_motion)
	{
		//disable sync motion command for this module
		unsigned long config;
		error_code_ = PCube_getConfig(nDevice, nJoint + 1, &config);
		if (error_code_ < 0) return false;
		config &= ~CONFIGID_MOD_SYNC_MOTION;
		error_code_ = PCube_setConfig(nDevice, nJoint + 1, config);
		if (error_code_ < 0) return false;
	}

	mutex_PCube.wait();
	error_code_ = PCube_moveStep( nDevice, nJoint + 1, 0.0f, time );
	mutex_PCube.post();
	
	if( error_code_ < 0 )
		return false;

	mutex_PCube.wait();
	error_code_ = PCube_waitForMotionEnd( nDevice, nJoint + 1, timeOut * 1000 );
	mutex_PCube.post();

	if(use_sync_motion)
	{
		//disable sync motion command for this module
		unsigned long config;
		error_code_ = PCube_getConfig(nDevice, nJoint + 1, &config);
		if (error_code_ < 0) return false;
		config |= CONFIGID_MOD_SYNC_MOTION;
		error_code_ = PCube_setConfig(nDevice, nJoint + 1, config);
		if (error_code_ < 0) return false;
	}

	return ( error_code_ >= 0 );
}

bool yarp::dev::AmtecLWA7dof::Home( std::vector<unsigned int> &home_order )
{
	bool 
		bHome = true,
		bAux = false;
		
	size_t v_size = home_order.size();
	
	for( size_t iJoint=0 ; iJoint<v_size ; iJoint++ )
	{
		if( !HomeJoint( home_order[iJoint] ) )
			bHome = false;
	}

	return bHome;
}

bool yarp::dev::AmtecLWA7dof::HomeAll( void )
{
	mutex_PCube.wait();
	error_code_ = PCube_homeAll( nDevice );
	mutex_PCube.post();

	if( error_code_ < 0 )
		return false;

	mutex_PCube.wait();
	error_code_ = PCube_waitForHomeEndAll( nDevice, 60000 ); //Wait 60secs
	mutex_PCube.post();

	return ( error_code_ >= 0 );
}

bool yarp::dev::AmtecLWA7dof::HomeGripper( void )
{
	if( nDevice < 0 ) 
		return false;

	int iGripper = 0;
	
	if( bHasGripper ) 
		iGripper = Number_of_Joints + 1;
	else 
		return false;

	mutex_PCube.wait();
	error_code_ = PCube_homeModule( nDevice, iGripper );
	mutex_PCube.post();

	if( error_code_ < 0 )
		return false;

	mutex_PCube.wait();
	error_code_ = PCube_waitForMotionEnd( nDevice, iGripper, 10000 );
	mutex_PCube.post();

	return ( error_code_ >= 0 );
}

bool yarp::dev::AmtecLWA7dof::Reset( int nJoint )
{
	mutex_PCube.wait();
	error_code_ = PCube_resetModule( nDevice, nJoint + 1 );
	mutex_PCube.post();

	return ( error_code_ >= 0 );
}

bool yarp::dev::AmtecLWA7dof::ResetAll( void )
{
	mutex_PCube.wait();
	error_code_ = PCube_resetAll( nDevice );
	mutex_PCube.post();

	return ( error_code_ >= 0 );
}

unsigned long yarp::dev::AmtecLWA7dof::Get_Joint_State( int nJoint )
{
	unsigned long state = 0;

	mutex_PCube.wait();
	error_code_ = PCube_getModuleState( nDevice, nJoint + 1, &state );
	mutex_PCube.post();

	if( error_code_ < 0 )
		return -1;
	else
		return state;
}

unsigned char yarp::dev::AmtecLWA7dof::Get_Joint_Type( int nJoint )
{
	unsigned char type = 0;

	mutex_PCube.wait();
	error_code_ = PCube_getModuleType( nDevice, nJoint + 1, &type );
	mutex_PCube.post();

	return type;
}

bool yarp::dev::AmtecLWA7dof::Test_Joint_Flag( int nJoint, unsigned long ulFlag )
{
	unsigned long state = 0;

	mutex_PCube.wait();
	error_code_ = PCube_getModuleState( nDevice, nJoint + 1, &state );
	mutex_PCube.post();

	return ( error_code_ < 0 ) ? false : Test_Flag( state, ulFlag );
}

bool yarp::dev::AmtecLWA7dof::Test_Flag( unsigned long ulState, unsigned long ulFlag )
{
	return ( ( ulState & ulFlag ) == ulFlag );
}

bool yarp::dev::AmtecLWA7dof::Test_Module( int nModule )
{
	//update Module state

	bool success = Update_State_Error( nModule - 1 );
	
	//may had occurred a function call error. If a function call error ocurred, state should not be tested.
	if( success )
	{
		//any state error?
		success = vec_str_error.size() == 0;
	}
	
	return success;
}

bool yarp::dev::AmtecLWA7dof::isArmStatusOK( void )
{
	//reset flags
	if( !ResetAll() )
		return false;
	
	for( int iModule = 1 ; iModule <= Number_of_Modules ; iModule++ )
	{
//		if( Test_Joint_Flag( iJoint, Amtec::Module::State::STAT_ERROR ) ) return false;
			//check for any error
		if( !Test_Module( iModule ) ) return false;
	}
	return true;
}

bool yarp::dev::AmtecLWA7dof::are_Modules_OK( void )
{
	//check if there is any module error
	int nJoints = getNumberOfJoints();

	error_code_ = 0;
	
	for( int iJoint = 0; iJoint<nJoints; iJoint++ )
	{
		if( !Update_State_Error( iJoint ) || error_code_ != 0 )
		{
			//temporarely save error code because stop may overwrite it!
			int err = error_code_;
			stop();
			error_code_ = err;
			return false;
		}
	}

	return true;
}


bool yarp::dev::AmtecLWA7dof::is_Module_OK( int joint )
{
	using namespace Amtec::Module::State;
	//check if module joint has a module error
	unsigned long ulError;
	error_code_ = PCube_getModuleState( nDevice, joint + 1, &ulError );

	const unsigned error_flag = STAT_ERROR | STAT_POWERFAULT | STAT_TOW_ERROR | STAT_COMM_ERROR | STAT_POW_VOLT_ERR |STAT_POW_FET_TEMP | STAT_POW_WDG_TEMP | STAT_POW_SHORTCUR | STAT_POW_HALLERR | STAT_POW_INTEGRALERR |	STAT_CPU_OVERLOAD  | STAT_BEYOND_HARD | STAT_BEYOND_SOFT | STAT_POW_SETUP_ERR;

	if( error_code_ < 0 || ((ulError & error_flag) != 0))
		return false;

	return true;
}

bool yarp::dev::AmtecLWA7dof::SoftStop( void )
{
	if( !stop() )
		return false;

	return ResetAll();
}

int yarp::dev::AmtecLWA7dof::Get_Error_Code( void )
{
	return error_code_;
}

std::string yarp::dev::AmtecLWA7dof::Get_Error_Str( void )
{
	switch ( error_code_ )
	{
	case Amtec::Error::ERROR_NONE:
		error_str_ = "";
		break;

	case Amtec::Error::ERROR_OTHER:
		//error_str_ is set automatically
		break;

	case Amtec::Error::ERROR_STATE:
		//get state errors
		error_str_ = "";
		for ( size_t i = 0; i < vec_str_error.size(); i++ )
			error_str_ += vec_str_error[i]; 
		break;

	default:
		//get function errors
		error_str_ = Get_Error_Function_String( error_code_ );
		break;
	}

	return error_str_;
}

bool yarp::dev::AmtecLWA7dof::Restart_Module()
{
	//there is no problem in close returns failure.
	close();

	return open( arm_config, robot_nr );
}
std::string yarp::dev::AmtecLWA7dof::Get_Error_Function_String( unsigned long ulError )
{
	return Get_Error_Function_String( static_cast<int>( ulError ) );
}

std::string yarp::dev::AmtecLWA7dof::Get_Error_Function_String( int error )
{
	std::string error_str = "";

	switch( error )
	{
	case Amtec::Function_Error::ERROR_FUNCTIONNOTAVAILABLE:
		error_str = "The function called is not available." ;
		break;
		
	case Amtec::Function_Error::ERROR_NOINITSTRING:
		error_str = "The InitString is missing during initialization.";
		break;

	case Amtec::Function_Error::ERROR_NODEVICENAME:
		error_str =  "The device name specified in InitString is wrong or invalid.";
		break;

	case Amtec::Function_Error::ERROR_BADINITSTRING:
		error_str = "The InitString is incomplete or wrong.";
		break;

	case Amtec::Function_Error::ERROR_INITERROR:
		error_str = "Initialization of the interface failed, check hardware and driver setup.";
		break;

	case Amtec::Function_Error::ERROR_NOTINITIALIZED:
		error_str = "The function was called before initializing the device.";
		break;

	case Amtec::Function_Error::ERROR_WRITEERROR:
		error_str = "Error during an attempt to write data to the interface.";
		break;

	case Amtec::Function_Error::ERROR_READERROR:
		error_str = "Error during an attempt to read data from the interface.";
		break;

	case Amtec::Function_Error::ERROR_WRITETIMEOUT:
		error_str = "Timeout while sending data on the bus.";
		break;

	case Amtec::Function_Error::ERROR_READTIMEOUT:
		error_str = "Timeout while reading data from a module.";
		break;

	case Amtec::Function_Error::ERROR_WRONGMESSAGEID:
		error_str = "The message received has an unexpected MessageID.";
		break;

	case Amtec::Function_Error::ERROR_WRONGCOMMANDID:
		error_str = "The message received has an unexpected CommandID.";
		break;

	case Amtec::Function_Error::ERROR_WRONGPARAMETERID:
		error_str = "The message received has an unexpected ParameterID.";
		break;

	case Amtec::Function_Error::ERROR_EXITERROR:
		error_str = "Error occured while closing the interface.";
		break;

	case Amtec::Function_Error::ERROR_NOMODULES:
		error_str = "No module found during initialization of the interface.";
		break;

	case Amtec::Function_Error::ERROR_WRONGDEVICEID:
		error_str = "The given DeviceID is wrong.";
		break;

	case Amtec::Function_Error::ERROR_NOLIBRARY:
		error_str = "A DLL file is missing to execute the function call.";
		break;

	case Amtec::Function_Error::ERROR_ISINITIALIZED:
		error_str = "The Interface has been already initialized.";
		break;

	case Amtec::Function_Error::ERROR_WRONGEMSMODULEID:
		error_str = "The given EMS module ID does not exist.";
		break;

	case Amtec::Function_Error::ERROR_EMSNOTINITIALIZED:
		error_str = "The EMS module has not been initialized.";
		break;

	case Amtec::Function_Error::ERROR_EMSMAXNUMBER:
		error_str = "The maximum number of EMS modules has been reached.";
		break;

	case Amtec::Function_Error::ERROR_EMSINITERROR:
		error_str = "Error initializing an EMS module.";
		break;

	case Amtec::Function_Error::ERROR_WRONGEMSTYPE:
		error_str = "This function is intended to use with a different EMS module type.";
		break;
	
	case Amtec::Function_Error::ERROR_WRONGEMSCHANNELID:
		error_str = "The given channel ID of the EMS module does not exist.";
		break;

	case Amtec::Function_Error::ERROR_WRONGMP55MODULEID:
		error_str = "The given MP55 module ID does not exist.";
		break;

	case Amtec::Function_Error::ERROR_WRONGSCHUNKMODULEID:
		error_str = "The given SCHUNK module ID does not exist.";
		break;

	case Amtec::Function_Error::ERROR_WRONGMODULEID:
		error_str = "Wrong Module ID.";
		break;

	case Amtec::Function_Error::ERROR_MODULEERROR:
		error_str = "Module Error.";
		break;

	case Amtec::Function_Error::ERROR_WAITTIMEOUT:
		error_str = "Wait Timeout for a end of motion.";
		break;

	case Amtec::Function_Error::ERROR_OPENINIFILE:
		error_str = "Error on Open init file.";
		break;

	default:
		error_str = "No error.";
		break;
	}

	return error_str;
}

bool yarp::dev::AmtecLWA7dof::Update_State_Configuration( int nJoint )
{
	unsigned long ulConf = 0;

	mutex_PCube.wait();
	error_code_ = PCube_getConfig( nDevice, nJoint + 1, &ulConf );
	mutex_PCube.post();

	if( error_code_ < 0 ) return false;

	bool bTest = false;

	vec_str_conf.clear();

	using namespace Amtec::Module::Configuration;

	bTest = Test_Flag( ulConf, CONF_BRAKE_PRESENT );
	if( bTest ) vec_str_conf.push_back( "Brake present" );

	bTest = Test_Flag( ulConf, CONF_BRAKE_AT_POWERON );
	if( !bTest ) vec_str_conf.push_back( "Brake will be released at Power On" );

	bTest = Test_Flag( ulConf, CONF_SWR_WITH_ENCODERZERO );
	if( bTest ) vec_str_conf.push_back( "Encoder Index used for Homing" );

	bTest = Test_Flag( ulConf, CONF_SWR_AT_FALLING_EDGE );
	if( bTest ) vec_str_conf.push_back( "Homing finishes on falling edge of homing switch" );

	bTest = Test_Flag( ulConf, CONF_CHANGE_SWR_TO_LIMIT );
	if( bTest ) vec_str_conf.push_back( "Homing switch converts to limit switch after Homing is finished" );

	bTest = Test_Flag( ulConf, CONF_SWR_ENABLED	);
	if( bTest ) vec_str_conf.push_back( "Homing switch is enabled" );

	bTest = Test_Flag( ulConf, CONF_SWR_LOW_ACTIVE );
	if( bTest ) vec_str_conf.push_back( "Homing switch is low active" );

	bTest = Test_Flag( ulConf, CONF_SWR_USE_EXTERNAL );
	if( bTest ) vec_str_conf.push_back( "The external homing switch will be used" );
	
	bTest = Test_Flag( ulConf, CONF_SW1_ENABLED );
	if( bTest ) vec_str_conf.push_back( "Limit switch 1 is enabled" );
	
	bTest = Test_Flag( ulConf, CONF_SW1_LOW_ACTIVE );
	if( bTest ) vec_str_conf.push_back( "Limit switch 1 is low active" );
	
	bTest = Test_Flag( ulConf, CONF_SW1_USE_EXTERNAL );
	if( bTest ) vec_str_conf.push_back( "The external limit switch 1 will be used" );

	bTest = Test_Flag( ulConf, CONF_SW2_ENABLED );
	if( bTest ) vec_str_conf.push_back( "Limit switch 2 is enabled" );

	bTest = Test_Flag( ulConf, CONF_SW2_LOW_ACTIVE );
	if( bTest ) vec_str_conf.push_back( "Limit switch 2 is low active" );
	
	bTest = Test_Flag( ulConf, CONF_SW2_USE_EXTERNAL );
	if( bTest ) vec_str_conf.push_back( "The external limit switch 2 will be used" );

	bTest = Test_Flag( ulConf, CONF_LINEAR );
	if( bTest ) vec_str_conf.push_back( "Module is of linear type" );
	
	bTest = Test_Flag( ulConf, CONF_ALLOW_FULL_CUR );
	if( !bTest ) vec_str_conf.push_back( "The max. cur commanded with PCube_moveCur will be limited to the nominal current." );

	bTest = Test_Flag( ulConf, CONF_M3_COMPATIBLE );
	if( bTest ) vec_str_conf.push_back( "Module is MoRSE3 compatible. The module does not accept motion commands unless Homing is finished successfully." );

	bTest = Test_Flag( ulConf, CONF_LINEAR_SCREW );
	if( bTest ) vec_str_conf.push_back( "Module is linear module with ball screw actuator." );

	bTest = Test_Flag( ulConf, CONF_DISABLE_ON_HALT );
	if( bTest ) vec_str_conf.push_back( "On error the motor is set to zero current." );

	bTest = Test_Flag( ulConf, CONF_WATCHDOG_ENABLE );
	if( bTest ) vec_str_conf.push_back( "Watchdog is enabled." );

	bTest = Test_Flag( ulConf, CONF_ZERO_MOVE_AFTER_HOK );
	if( bTest ) vec_str_conf.push_back( "After Homing is finished the module automatically moves to ist zero position" );
	
	bTest = Test_Flag( ulConf, CONF_DISABLE_ACK );
	if( bTest ) vec_str_conf.push_back( "Messages are not acknowledged anymore. Get commands will still be answered. Valid only for CAN-Bus." );

	bTest = Test_Flag( ulConf, CONF_SYNC_MOTION	);
	if( bTest ) vec_str_conf.push_back( "Enables synchronized Motion commands. After sending the motion command the a special Start Motion broadcast is expected (PCube_startMotionAll)." );
	
	return true;
}

bool yarp::dev::AmtecLWA7dof::Update_State_Info( int nJoint )
{
	unsigned long ulInfo = 0;

	mutex_PCube.wait();
	error_code_ = PCube_getModuleState( nDevice, nJoint+1, &ulInfo );
	mutex_PCube.post();

	if( error_code_ < 0 ) return false;

	vec_str_info.clear();

	using namespace Amtec::Module::State;

	bool bTest = Test_Flag( ulInfo, STAT_HOME );
	if( bTest ) vec_str_info.push_back( "The module was homed (Info)." );

	bTest = Test_Flag( ulInfo, STAT_HALT );
	if( bTest ) vec_str_info.push_back( "The module is in HALT state. Motion commands are not processed (Info)." );

	bTest = Test_Flag( ulInfo, STAT_SWR );
	if( bTest ) vec_str_info.push_back( "The Reference switch is active." );
	
	bTest = Test_Flag( ulInfo, STAT_SW1 );
	if( bTest ) vec_str_info.push_back( "Limit switch 1 is active." );
	
	bTest = Test_Flag( ulInfo, STAT_SW2 );
	if( bTest ) vec_str_info.push_back( "Limit switch 2 is active." );
	
	bTest = Test_Flag( ulInfo, STAT_BRAKEACTIVE );
	if( bTest ) vec_str_info.push_back( "The brake is active." );
	
	bTest = Test_Flag( ulInfo, STAT_CURLIMIT );
	if( bTest ) vec_str_info.push_back( "The PID loop output has reached the limit." );
	
	bTest = Test_Flag( ulInfo, STAT_MOTION );
	if( bTest ) vec_str_info.push_back( "The module is in motion." );
	
	bTest = Test_Flag( ulInfo, STAT_RAMP_ACC );
	if( bTest ) vec_str_info.push_back( "The module is in a phase of acceleration. Only valid for ramp motion commands." );
	
	bTest = Test_Flag( ulInfo, STAT_RAMP_STEADY );
	if( bTest ) vec_str_info.push_back( "The module is in a phase of steady speed. Only valid for ramp motion commands." );
	
	bTest = Test_Flag( ulInfo, STAT_RAMP_DEC );
	if( bTest ) vec_str_info.push_back( "The module is in a phase of deceleration. Only valid for ramp motion commands." );
	
	bTest = Test_Flag( ulInfo, STAT_RAMP_END );
	if( bTest ) vec_str_info.push_back( "The module has reached the target position. Only valid for ramp motion commands." );
	
	bTest = Test_Flag( ulInfo, STAT_INPROGRESS );
	if( bTest ) vec_str_info.push_back( "The module processes an actual Step motion command." );
	
	bTest = Test_Flag( ulInfo, STAT_FULLBUFFER );
	if( bTest ) vec_str_info.push_back( "A Step motion command is waiting in the message buffer." );

	return true;
}

bool yarp::dev::AmtecLWA7dof::Update_State_Error( int nJoint )
{
	unsigned long ulError = 0;

	mutex_PCube.wait();
	error_code_ = PCube_getModuleState( nDevice, nJoint + 1, &ulError );
	mutex_PCube.post();

	if( error_code_ < 0 ) return false;

	vec_str_error.clear();

	using namespace Amtec::Module::State;

	bool bTest = Test_Flag( ulError, STAT_ERROR );
	if( bTest ) vec_str_error.push_back( "An error occured in the module (Generic Error flag)." );
	
	bTest = Test_Flag( ulError, STAT_POWERFAULT );
	if( bTest ) vec_str_error.push_back( "An error occured in the power bridge." );
	
	bTest = Test_Flag( ulError, STAT_TOW_ERROR );
	if( bTest ) vec_str_error.push_back( "The drive was unable to follow the interpolated position." );
	
	bTest = Test_Flag( ulError, STAT_COMM_ERROR );
	if( bTest ) vec_str_error.push_back( "Problems in communication occured ." );
	
	bTest = Test_Flag( ulError, STAT_POW_VOLT_ERR );
	if( bTest ) vec_str_error.push_back( "A voltage drop occured." );
	
	bTest = Test_Flag( ulError, STAT_POW_FET_TEMP );
	if( bTest ) vec_str_error.push_back( "A temperature problem occured in the power bridge." );
	
	bTest = Test_Flag( ulError, STAT_POW_WDG_TEMP );
	if( bTest ) vec_str_error.push_back( "A temperature problem occured in the motor." );
	
	bTest = Test_Flag( ulError, STAT_POW_SHORTCUR );
	if( bTest ) vec_str_error.push_back( "A short current occured." );
	
	bTest = Test_Flag( ulError, STAT_POW_HALLERR );
	if( bTest ) vec_str_error.push_back( "A commutation error occured." );
	
	bTest = Test_Flag( ulError, STAT_POW_INTEGRALERR );
	if( bTest ) vec_str_error.push_back( "An error according to the integral criteria occured (current too high over time)." );
	
	bTest = Test_Flag( ulError, STAT_CPU_OVERLOAD );
	if( bTest ) vec_str_error.push_back( "The CPU is too busy." );
	
	bTest = Test_Flag( ulError, STAT_BEYOND_HARD );
	if( bTest ) vec_str_error.push_back( "The module has reached the hard limit switch." );
	
	bTest = Test_Flag( ulError, STAT_BEYOND_SOFT );
	if( bTest ) vec_str_error.push_back( "The module has reached the soft limit switch." );
	
	bTest = Test_Flag( ulError, STAT_POW_SETUP_ERR );
	if( bTest ) vec_str_error.push_back( "Error in initializing the current controller. Power must be switched off." );

	if( vec_str_error.size() != 0 )
	{
		std::stringstream nr;
		nr << nJoint;
		vec_str_error.insert( vec_str_error.begin(), "Joint " + nr.str() + " " );
		error_code_ = Amtec::Error::ERROR_STATE;	//state error;
	}

	return true;
}

// End of specific functions for Amtec LWA arm
//----------------------------------------------------------------------
