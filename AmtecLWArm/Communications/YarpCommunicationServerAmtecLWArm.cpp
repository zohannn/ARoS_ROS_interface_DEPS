#include "YarpCommunicationServerAmtecLWArm.h"

#include <algorithm>
#include <math.h>
#include <iostream>
#include <sstream>
#include <boost\thread\future.hpp>

using namespace std;

CYarpCommunicationServerAmtecLWArm::CYarpCommunicationServerAmtecLWArm( const std::string name, const std::string dev_config_file )
: CYarpCommunication( name, "", true )
, sConfig( dev_config_file )
{
}

CYarpCommunicationServerAmtecLWArm::~CYarpCommunicationServerAmtecLWArm( void )
{
}

bool CYarpCommunicationServerAmtecLWArm::Init( void )
{
	if( !PStepServer.Initialize() ) return false;

	yarp::os::Property prop;
	prop.fromConfigFile( sConfig.c_str() );
	
	bool bArmInit = Amtec_Arm.open( prop );

	if( !bArmInit )
	{
		PStepServer.Terminate();

		std::cerr << std::endl <<
			"Error opening Device Driver!" << std::endl;
	}

	return bArmInit;
}

void CYarpCommunicationServerAmtecLWArm::Fini( void )
{
	if( Amtec_Arm.close() )
		PStepServer.Terminate();

	else
		std::cerr << "Error closing Device Driver!" << std::endl;
}

void CYarpCommunicationServerAmtecLWArm::Process( CMessage &msgIn, CMessage &msgOut, void *private_data )
{
	success_ = true;

	unsigned time = 0;

	int
		nJoints = 0,
		returned_code = 0,
		nAux = 0;
	
	float fAux = 0.0f;

	std::stringstream joint_text;

	switch( msgIn.uCommand ) 
	{
	//--------------------------------------------------------------------------	
	case AMTEC_GET_FORCE_TORQUE:
		msgOut.fData = Get_Force_Torque();
		break;
	
	//--------------------------------------------------------------------------
	case AMTEC_GET_POS:
		if( msgIn.uParam.size() < 1 )
		{
			msgOut.iErrorCode = -1;
			msgOut.strText = "Undefined joint number!";
		}
		else
		{
			msgOut.fData.resize(1);
			msgOut.fData[0] = GetPos( (int) msgIn.uParam[0] );
		}
		break;

	//--------------------------------------------------------------------------
	case AMTEC_GET_POS_ALL:
		GetPosAll( msgOut.fData );
		break;

	//--------------------------------------------------------------------------
	case AMTEC_MOVE_VEL_JOINT:
		if( msgIn.uParam.size() < 1 || msgIn.fData.size() < 1 )
		{
			msgOut.iErrorCode = -1;
			msgOut.strText = "Move velocity joint: joint nr in uParam[0], speed in fData[0]";
		}
		else
		{
			MoveVel( msgIn.uParam[0], msgIn.fData[0] );
		}
		break;

	//--------------------------------------------------------------------------
	case AMTEC_MOVE_VEL:
		Amtec_Arm.getAxes( &nJoints );
		if( msgIn.fData.size() < (unsigned)nJoints )
		{
			joint_text<<nJoints;
			msgOut.iErrorCode = -1;
			msgOut.strText = "Move velocity: Array size to small, it must have " + joint_text.str() + "elements.";
		}
		else
		{
			MoveVel( msgIn.fData );
		}
		break;

	//--------------------------------------------------------------------------
	case AMTEC_HOME_JOINT:
		if( msgIn.uParam.size() < 1 )
		{
			msgOut.iErrorCode = -1;
			msgOut.strText = "Undefined joint number!";
		}
		else
		{
			HomeJoint( (int)msgIn.uParam[0] );
		}
			
		break;

	//--------------------------------------------------------------------------
	case AMTEC_HOME:
		Amtec_Arm.getAxes( &nJoints );
		if( msgIn.uParam.size() < (unsigned)nJoints )
		{
			joint_text<<nJoints;
			msgOut.iErrorCode = -1;
			msgOut.strText = "Home: Array size to small, it must have " + joint_text.str() + "elements.";
		}
		else
		{
			Home( msgIn.uParam );
		}
		break;

	//--------------------------------------------------------------------------
	case AMTEC_HOME_ALL:
		HomeAll();
		break;

	//--------------------------------------------------------------------------
	case AMTEC_RESET_JOINT:
		ResetJoint( (int) msgIn.uParam[0] );
		break;

	//--------------------------------------------------------------------------
	case AMTEC_RESET_ALL:
		ResetAll();
		break;

	//--------------------------------------------------------------------------
	case AMTEC_STOP_JOINT:
		StopJoint( (int) msgIn.uParam[0] );
		break;

	//--------------------------------------------------------------------------
	case AMTEC_STOP_ALL:
		StopAll();
		break;

	//--------------------------------------------------------------------------
	case AMTEC_MOVE_STEP:
		if( msgIn.fData.size() < 3 )
		{
			msgOut.iErrorCode = -1;
			msgOut.strText = "Corrupted data";
				
		}
		else
		{
			MoveStep( (int)msgIn.fData[0], msgIn.fData[1], (unsigned short) msgIn.fData[2] );
		}
			
		break;

	//--------------------------------------------------------------------------
	case AMTEC_MOVE_STEP_WAIT:
		if( msgIn.fData.size() < 3 )
		{
			msgOut.iErrorCode = -1;
			msgOut.strText = "Corrupted data";
				
		}
		else
		{
			MoveStep( (int)msgIn.fData[0], msgIn.fData[1], (unsigned short) msgIn.fData[2], true );
		}
			
		break;

	//--------------------------------------------------------------------------
	case AMTEC_FREE_JOINT:
		if( msgIn.uParam.size() < 1 )
		{
			msgOut.iErrorCode = -1;
			msgOut.strText = "Undefined joint number!";
		}
		else
		{
			freeJoint( (int)msgIn.uParam[0] );
		}
		break;

	//--------------------------------------------------------------------------
	case AMTEC_MOVE_STEP_ALL:
		MoveStepAll( msgIn.fData );
		break;

	//--------------------------------------------------------------------------
	case AMTEC_MOVE_STEP_ALL_SAME_TIME:
		MoveStepAllSameTime( msgIn.fData);
		break;

		//--------------------------------------------------------------------------
	case AMTEC_MOVE_STEP_TRAJECTORY:
		MoveTrajectory(msgIn.uParam, msgIn.fData);
		break;

	case AMTEC_WAIT_TRAJECTORY_END:
		if( msgIn.uParam.size() > 0 )
			// Perform wait based in time
			nAux = WaitTrajectoryEnd( msgIn.uParam.front() );
		else
			// Perform wait based in time
			nAux = WaitTrajectoryEnd( -1 );

		msgOut.uParam.assign(
				1,	// Assign one value to uParam, the return value of WaitMotionEnd
				static_cast<unsigned int>( nAux )
			);
		break;

	case AMTEC_WAIT_TRAJECTORY_END_ASYNC:
		boost::async(boost::launch::async,
			[&]()
		{
			int ret;
				//wait for trajectory end
			ret = WaitTrajectoryEnd(-1);

			CMessage msg;
			// Assign one value to uParam, the return value of WaitMotionEnd
			msg.uParam.push_back(static_cast<unsigned int>(ret));
			msg.uCommand = AMTEC_TRAJECTORY_ASYNC_FINISHED;
			Send(msg, false);
		});
		break;

	case AMTEC_STOP_CLEAR_TRAJECTORY:
		StopClearTrajectory();
		break;
	//--------------------------------------------------------------------------
	case AMTEC_MOVE_ALL_VEL:
		MoveAllVel( msgIn.fData );
		break;

	//--------------------------------------------------------------------------
	case AMTEC_MOVE_ALL_VEL_TRAJ:
		MoveAllVelTraj( msgIn.fData, msgIn.uParam );
		break;

	//--------------------------------------------------------------------------
	case AMTEC_GET_POS_DEG:
		msgOut.fData.resize(1);
		msgOut.fData[0]=GetPosDeg( (int) msgIn.uParam[0] );
		break;

	//--------------------------------------------------------------------------
	case AMTEC_GET_POS_DEG_ALL:
		msgOut.fData = GetPosDegAll();
		break;

	//--------------------------------------------------------------------------
	case AMTEC_CLOSEGRIPPER:
		if( msgIn.fData.size() < 1 )
		{
			msgOut.iErrorCode = -1;
			msgOut.strText = "Corrupted data";
				
		}
		else
		{
			if( msgIn.uParam.size() == 1 )
				CloseGripper( msgIn.fData[0], msgIn.uParam[0] );
			else
				CloseGripper( msgIn.fData[0], 5000 );
		}
			
		break;

	//--------------------------------------------------------------------------
	case AMTEC_OPENGRIPPER:
		OpenGripper();
		break;

	//--------------------------------------------------------------------------
	case AMTEC_GET_GRIPPER:
		msgOut.fData.resize(1);
		msgOut.fData[0] = GetGripperPos();
		break;

	//--------------------------------------------------------------------------
	case AMTEC_HOME_GRIPPER:
		HomeGripper();
		break;

	//--------------------------------------------------------------------------
	case AMTEC_RESTART_MODULE:
		Restart();
		break;

	//--------------------------------------------------------------------------
	case AMTEC_INV_KIN:
		Inv_kinematics(msgIn.fData);
		break;

	//--------------------------------------------------------------------------
	case AMTEC_SOFT_STOP:
		SoftStop();
		break;

	//--------------------------------------------------------------------------
	case AMTEC_MOVE_STEP_SAME_TIME_WAIT:
		MoveStepSameTimeWait( msgIn.fData);
		break;

	//--------------------------------------------------------------------------
	case AMTEC_WAIT_MOTION_END:
		if( msgIn.fData.size() > 0 )
		{
			// Perform wait based in angle margin
			fAux = msgIn.fData.back();
			msgIn.fData.pop_back();
			WaitMotionEnd( msgIn.fData, fAux );
		}

		else if( msgIn.uParam.size() > 0 )
		{
			// Perform wait based in time
			nAux = WaitMotionEnd( msgIn.uParam.front() );

			msgOut.uParam.assign(
				1,	// Assign one value to uParam, the return value of WaitMotionEnd
				static_cast<unsigned int>( nAux )
			);
		}
		
		else
		{
			// Error in command
			msgOut.iErrorCode = -1;
			msgOut.strText += "uParam[0]: wait time (optional).";
			msgOut.strText += "\nfData: target angles for the arms (optional).";
		}

		break;
				
	//--------------------------------------------------------------------------
	case AMTEC_MOVE_STEP_SAME_TIME_SEL:
		Amtec_Arm.getAxes( &nJoints );
		if( msgIn.fData.size() < (unsigned)nJoints || msgIn.uParam.size() < (unsigned)nJoints)
		{
			joint_text<<nJoints;
			msgOut.iErrorCode = -1;
			msgOut.strText = "Move step some joints same tiime: Array size to small, fData and uParam must both have " + joint_text.str() + "elements.";
		}
		else
		{
			MoveStepSameTime( &msgIn.fData, &msgIn.uParam );
		}
		break;

	//--------------------------------------------------------------------------
	default:
			msgOut.strText = "unknown command";
			msgOut.iErrorCode = 1;
		break;
	}	

	if( !success_ )
	{
		msgOut.iErrorCode = Amtec_Arm.Get_Error_Code();
		msgOut.strText = Amtec_Arm.Get_Error_Str();
	}
}
