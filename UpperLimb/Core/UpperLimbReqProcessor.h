#pragma once

#include "Joint_States.h"

#include <vector>
#include <tuple>
#include <queue>

#include <boost\shared_ptr.hpp>
#include <boost\scoped_ptr.hpp>
#include <boost\atomic.hpp>
#include <boost\thread.hpp>

#include "Message.h"
#include "YarpConnector.h"
#include "AmtecLWA7dof.h"
/*
#ifndef USE_BOOST_SERIAL_ASIO
	#define USE_BOOST_SERIAL_ASIO 
#endif
*/
#include "barrett_hand_826X.h"

class UpperLimbReqProcessor
{
public:
	//Executor state
	enum ExecutorState
	{
		Ready,
		Error
	};

private:
	//Upper Limb devices
	const size_t arm_nr_joints;
	const size_t hand_nr_joints;
	const size_t nr_joints;
	//software enabled/disabled arm
	bool soft_enabled_arm;
	//software enabled/disabled hand
	bool soft_enabled_hand;
	//arm device pointer
	boost::shared_ptr<yarp::dev::AmtecLWA7dof> arm;
	//hand device pointer
	boost::shared_ptr<yarp::dev::BarrettHand826X> hand;

	//indexes for data tuples
	//Joints' vector values index
	//const size_t joints_idx = 0;
	////movement time index
	//const size_t time_idx = 1;
	////TrajectorySteps' vector index
	//const size_t steps_idx = 0;
	////index of flag indicating if arm should move in trajectory 
	//const size_t arm_mov_idx = 1;
	////index of flag indicating if hand should move in trajectory
	//const size_t hand_mov_idx = 2;
	//Trajectory step: contains vector of joints values and the time to execute the step
	typedef std::tuple<std::vector<float>, float> TrajectoryStep;
	//Trajectory movement: contains vector of steps and flags indicating which devices should run
	typedef std::tuple<std::vector<TrajectoryStep>, bool, bool> TrajectoryMovement;

	boost::atomic<bool> stop_executor;
	boost::atomic<bool> stop_movement;
	boost::thread traj_executor_thrd;
	std::queue<TrajectoryMovement> trajectory_queue;
	boost::condition_variable queue_not_empty_cv;
	boost::mutex queue_mtx;

	ExecutorState executor_state;

	boost::condition_variable executing_movement_cv;
	boost::mutex executing_movement_mtx;
	boost::atomic<bool> executing_movement;
	boost::atomic<bool> executing_movement_error;
	std::string last_error_str;

	void TrajectoryMovementsExecutor();


	//Joint states 
	std::string joint_states_out_name;
	bool joint_states_out_worker_end_job;
	yarp::os::BufferedPort<Joint_States> joint_states_out_channel;
	boost::mutex joint_states_queue_mtx;
	std::queue<Joint_States> joint_states_queue;
	boost::condition_variable joint_states_queue_cv;

	boost::thread joint_states_out_worker;

	void joint_states_job();

public:
	UpperLimbReqProcessor();
	~UpperLimbReqProcessor();

	void ProcessReq(CMessage &in_req, CMessage &out_reply, YarpConnector &con);
	void SetDevices(boost::shared_ptr<yarp::dev::AmtecLWA7dof> arm_, boost::shared_ptr<yarp::dev::BarrettHand826X> hand_);

	void AddTrajectory(std::vector<unsigned> &uParam, std::vector<float> &fData);
	int WaitTrajectoryEnd(int timeout_ms);
	void StopClearTrajectory(void);

	bool ArmProcessor(CMessage &in_req, CMessage &out_reply, YarpConnector &con);
	bool HandProcessor(CMessage &in_req, CMessage &out_reply, YarpConnector &con);

	//Resets executor state
	void ResetExecutor();

	ExecutorState GetExecutorState();

	bool start_joints_states_sender(std::string name = "/aros/upperlimb_right/joint_states");
	void stop_joints_states_sender();

	enum HandMachineState
	{
		Moving,
		Grasping,
		Releasing,
	};

	HandMachineState hand_machine_state;

public:
	//ARM Specific functions

	bool ArmCheckCommand(unsigned int cmd);
	//--------------------------------------------------------------------------
	// Home functions

	void HomeJoint(int joint);

	void Home(std::vector<unsigned int> &home_order);

	void HomeAll(void);

	void HomeGripper(void);

	//--------------------------------------------------------------------------
	// Arm error related functions

	//Get last error string of Arm Device
	std::string Get_Device_Error(void);

	bool isArmStatusOK(void);

	// Release joint
	void freeJoint(int nJoint);

	void ResetJoint(int joint);

	void ResetAll(void);

	void Restart(void);

	//--------------------------------------------------------------------------
	// Stop functions

	void StopJoint(int joint);

	void StopAll(void);

	// Software stop of arm joints
	void SoftStop(void);

	//--------------------------------------------------------------------------
	// Joint information

	int GetNumberJoints(void);

	float GetPos(int joint);

	void GetPosAll(std::vector<float> &data);

	float GetPosDeg(int joint);

	std::vector<float> GetPosDegAll(void);

	std::vector<float> Get_Force_Torque(void);

	void GetJointLimits(int joint, float * fLimits);

	//--------------------------------------------------------------------------
	// Waiting functions

	/*	 Wait for motion end with a timeout
	*	@arg timeout: time to wait for motion to terminate, if the motion does not end before the
	*  specified time finish, no error is set, but @return will return -1; 0 if motion ended well.
	*	@return: 0 if motion ended before the @arg timeout, or -1 otherwise (even if error occurred).*/

	int WaitMotionEnd(unsigned int timeout_ms);

	/*	Wait for motion to approach a final value within a specified margin
	*	@arg refs: angles of the joints
	*	@arg stop_margin: margin to wait for.*/
	void WaitMotionEnd(std::vector<float> &refs, float stop_margin);	

	//--------------------------------------------------------------------------
	// Movement functions

	void MoveJointPosition(int joint, float value);

	void MoveJointPositionRamp(int joint, float pos);

	void MoveJointPositionRamp(int joint, float pos, float vel, float acc);

	void MoveStep(int joint, float value, unsigned short time, bool wait_end = false);

	void MoveStepAll(std::vector<float> &fData);

	void MoveStepSameTime(std::vector<float> *fData, std::vector<unsigned int> *joints2move);

	void MoveStepSameTimeWait(std::vector<float> &fData);

	void MoveStepAllSameTime(std::vector<float> &fData);
	
	
	//Hand related
private:
	std::vector<double> Finger1;
	std::vector<double> Finger2;
	std::vector<double> Finger3;
	std::vector<double> Spread;

	boost::mutex mutex_barrett;

private:
	std::string sConfig;

	boost::mutex mutex_Command;

	bool
		newcomand,
		newcomandtorque,
		newcomandspread,
		isPicked,
		bExit;

	int ivec[4];

	double
		vec[4],
		HandConfig[4];

	bool isHandAvailable(void);


	std::string Report_Error_at_Command(unsigned int uCommand);
	std::string CommandToString(unsigned int uCommand);

	boost::thread hand_processor;
	void PrepareHandProcessor();
	void HandExecutor();
public:
	//Hand specific functions
	bool HandCheckCommand(unsigned int cmd);

	bool Initialize(void);

	bool InitJoint(std::vector<float> fData);

	bool OpenJoint(std::vector<float> fData);

	bool Open(void);

	bool CloseJoint(std::vector<float> fData);

	bool Close(void);

	bool OpenTorqueJoint(std::vector<float> fData);

	bool OpenTorque(void);

	bool CloseTorqueJoint(std::vector<float> fData);

	bool CloseTorque(void);

	bool SetOpenTarget(std::vector<float> fData);

	bool SetCloseTarget(std::vector<float> fData);

	bool GetPositions(std::vector<float> &fData);

	bool GetOpenTarget(std::vector<float> &fData);

	bool GetCloseTarget(std::vector<float> &fData);

	bool SetOpenTargetJoint(int joint, double value);

	bool SetCloseTargetJoint(int joint, double value);

	float GetOpenTargetJoint(int joint);

	float GetCloseTargetJoint(int joint);

	bool SetVelocity(std::vector<float> fData);

	bool GetVelocity(std::vector<float> &fData);

	bool SetOpenVelocity(std::vector<float> fData);

	bool SetCloseVelocity(std::vector<float> fData);

	bool SetAcceleration(std::vector<float> fData);

	bool GetAcceleration(std::vector<float> &fData);

	bool GetStrainGauge(std::vector<unsigned int> &uParam);

	void SetStrainGauge(bool ispicked1);

	bool SetMaxStrain(std::vector<unsigned int> iData);

	bool GetMaxStrain(std::vector<unsigned int> &iData);

	bool IncrementalMoveJoint(int joint, double value);

	bool IncrementalMove(std::vector<float> fData);

	void GetHandConfig(double dHandConfig[4]);

	void GetSpreadConfig(double &dHandConfig);

	void SetNewComand(bool flag);

	bool GetNewComand(void);

	void SetNewComandTorque(bool flag);

	bool GetNewComandTorque(void);

	void SetNewComandSpread(bool flag);

	bool GetNewComandSpread(void);
};