#ifndef _WRAPPER_AMTEC_ARM_H_
#define _WRAPPER_AMTEC_ARM_H_

#pragma once

#include <vector>
#include <tuple>
#include <boost\thread.hpp>
#include <boost\functional.hpp>
#include <boost\atomic.hpp>
#include <queue>

#include <AmtecLWA7dof.h>
#include <AmtecPStepServer.h>

class CWrapperAmtecArm
{
protected:
	CAmtecPStepServer PStepServer;

	typedef std::tuple<std::vector<float>, unsigned int> TrajectoryStep;
	typedef std::vector<TrajectoryStep> TrajectoryMovement;

	boost::atomic<bool> stop_executor;
	boost::atomic<bool> stop_movement;
	boost::thread traj_executor_thrd;
	std::queue<TrajectoryMovement> trajectory_queue;
	boost::condition_variable queue_not_empty_cv;
	boost::mutex queue_mtx;
	boost::condition_variable executing_movement_cv;
	boost::mutex executing_movement_mtx;
	boost::atomic<bool> executing_movement;

	void TrajectoryMovementsExecutor();

	bool success_; //Store result from last call to a function of AmtecLWArm module.

	int ant;

public:
	// Object that contains the arm
	yarp::dev::AmtecLWA7dof Amtec_Arm;

	CWrapperAmtecArm( void );
	~CWrapperAmtecArm( void );

	//--------------------------------------------------------------------------
	// Home functions

	void HomeJoint( int joint );

	void Home( std::vector<unsigned int> &home_order );

	int HomeAll( void );

	void HomeGripper( void );

	//--------------------------------------------------------------------------
	// Arm error related functions

	//Get state of last movement.
	bool Success( void );

	//Get last error string of Arm Device
	std::string Get_Device_Error( void );

	bool isArmStatusOK( void );

	// Release joint
	void freeJoint( int nJoint );

	void ResetJoint( int joint );

	void ResetAll( void );

	void Restart( void );

	//--------------------------------------------------------------------------
	// Stop functions

	void StopJoint( int joint );

	void StopAll( void );

	// Software stop of arm joints
	void SoftStop( void );

	void StopClearTrajectory( void );

	//--------------------------------------------------------------------------
	// Gripper related functions

	void CloseGripper( float value, unsigned int time );

	void OpenGripper( void );

	float GetGripperPos( void );

	//--------------------------------------------------------------------------
	// Joint information

	int GetNumberJoints( void );

	float GetPos( int joint );

	void GetPosAll( std::vector<float> &data );

	float GetPosDeg( int joint );

	std::vector<float> GetPosDegAll( void );

	void GetJointLimits( int joint, float * fLimits );

	std::vector<float> Get_Force_Torque( void );

	//--------------------------------------------------------------------------
	// Waiting functions

	/*	 Wait for motion end with a timeout
	*	@arg timeout: time to wait for motion to terminate, if the motion does not end before the
	*  specified time finish, no error is set, but @return will return -1; 0 if motion ended well.
	*	@return: 0 if motion ended before the @arg timeout, or -1 otherwise (even if error occurred).*/

	int WaitMotionEnd( unsigned int timeout_ms );

	/*	Wait for motion to approach a final value within a specified margin
	*	@arg refs: angles of the joints
	*	@arg stop_margin: margin to wait for.*/
	int WaitMotionEnd( std::vector<float> refs, float stop_margin );

	int WaitTrajectoryEnd(int timeout_ms);

	//--------------------------------------------------------------------------
	// Movement functions

	void MoveJointPosition( int joint, float value );

	void MoveJointPositionRamp( int joint, float pos );

	void MoveJointPositionRamp( int joint, float pos, float vel, float acc );

	void MoveStep( int joint, float value, unsigned short time, bool wait_end = false );

	void MoveStepAll( std::vector<float> fData );

	void MoveStepSameTime( std::vector<float> *fData, std::vector<unsigned int> *joints2move );

	void MoveStepSameTimeWait( std::vector<float> fData );

	void MoveStepAllSameTime( std::vector<float> fData );

	void MoveTrajectory(std::vector<unsigned> &uParam, std::vector<float> &fData);

	//Move @param 'joint' at velocity 'speed'
	void MoveVel( int joint, double speed );

	//Move all joints at specified speed [sp0 sp1 sp2 sp3 sp4 sp5 sp6]
	void MoveVel( std::vector<float> fData );

	void MoveAllVel( std::vector<float> fData );

	void MoveAllVelTraj( std::vector<float> fData, std::vector<unsigned int> uParam );

	//--------------------------------------------------------------------------

	void Inv_kinematics( std::vector<float> fData );
};

#endif // _WRAPPER_AMTEC_ARM_H_
