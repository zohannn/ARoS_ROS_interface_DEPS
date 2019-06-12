#ifndef _YARP_COMMUNICATION_SERVER_UPPERLIMB_H_
#define _YARP_COMMUNICATION_SERVER_UPPERLIMB_H_

#include <queue>

// YARP
#include <YarpCommunication.h>
#include <yarp/os/Semaphore.h>

// BOOST
#include <boost\shared_ptr.hpp>
#include <boost\make_shared.hpp>
#include <boost\scoped_ptr.hpp>
#include <boost\atomic.hpp>
#include <boost\thread.hpp>
#include <boost\chrono\thread_clock.hpp>

//Arm and hand
#include <AmtecPStepServer.h>
#include <AmtecLWA7dof.h>
#include <barrett_hand_826X.h>

#include "../Core/Joint_States.h"

// Boost logging
#include <boost\log\core.hpp>
#include <boost\log\trivial.hpp>
#include <boost\log\expressions.hpp>
#include <boost\log\sinks\text_file_backend.hpp>
#include <boost\log\utility\setup\file.hpp>
#include <boost\log\utility\setup\common_attributes.hpp>
#include <boost\log\sources\severity_logger.hpp>
#include <boost\log\sources\record_ostream.hpp>

class CYarpCommunicationServerUpperLimb : public CYarpCommunication
{

private:
		int ivec[4];
		double
			vec[4],
			HandConfig[4];

			bool
				enable_arm,
				enable_hand,
				init_hand, // true if the hand has to be initialized upon startup
				newcomand,
				newcomandtorque,
				newcomandspread,
				isPicked,
				bExit;

		std::string 
			arm_config_file, // arm configuration file 
			hand_config_file; // hand configuration file

protected:

	//Upper Limb number of joints
	const size_t arm_nr_joints; // number of joints of the arm
	const size_t hand_nr_joints; // number of joints of the hand
	const size_t nr_joints; // total number of joints ( arm + hand )

	//software enabled/disabled arm
	bool soft_enabled_arm;
	//software enabled/disabled hand
	bool soft_enabled_hand;
	
	// UpperLimb Devices
	boost::shared_ptr<yarp::dev::AmtecLWA7dof> arm; //arm device pointer
	boost::shared_ptr<yarp::dev::BarrettHand826X> hand; //hand device pointer

	// position
	//Trajectory step: contains vector of joints values and the time to execute the step
	typedef std::tuple<std::vector<float>, float> TrajectoryStep;
	//Trajectory movement: contains vector of steps and flags indicating which devices should run (arm,hand)
	typedef std::tuple<std::vector<TrajectoryStep>, bool, bool> TrajectoryMovement;

	// velocity
	// Velocity Trajectory step: contains vector of joints velocity values and the time to execute the step
	typedef std::tuple<std::vector<float>, float> VelTrajectoryStep;
	//Velocity Trajectory movement: contains vector of steps and flags indicating which devices should run (arm,hand)
	typedef std::tuple<std::vector<VelTrajectoryStep>, bool, bool> VelTrajectoryMovement;

	void TrajectoryMovementsExecutor();
	boost::thread traj_executor_thrd;
	std::queue<TrajectoryMovement> trajectory_queue; // queue of the trajectory 
	boost::condition_variable queue_not_empty_cv; // condition variable to notify that the trajectory is not empty
	boost::mutex queue_mtx; // mutex of the queue for the trajectory executor 

	void VelTrajectoryMovementsExecutor();
	boost::thread vel_traj_executor_thrd;
	std::deque<VelTrajectoryMovement> vel_trajectory_queue; // queue of the velocity trajectory 
	boost::condition_variable vel_queue_not_empty_cv; // condition variable to notify that the velocity trajectory is not empty
	boost::mutex vel_queue_mtx; // mutex of the queue for the trajectory velocity executor 

	// Barrett Hand position values
	std::vector<double> Finger1;
	std::vector<double> Finger2;
	std::vector<double> Finger3;
	std::vector<double> Spread;

	boost::mutex mutex_barrett; // barrett hand mutex

	// Hand States
	enum HandMachineState
	{
		Moving,
		Grasping,
		Releasing,
	};
	HandMachineState hand_machine_state;

	boost::mutex mutex_Command; // yarp commands mutex

	// Barrett Hand thread
	boost::thread hand_processor;
	void PrepareHandProcessor();
	void HandExecutor();

	// position
	// stop the execution of the threads
	boost::atomic<bool> stop_executor;
	boost::atomic<bool> stop_movement;
	// execution of the movements
	boost::atomic<bool> executing_movement;
	boost::atomic<bool> executing_movement_error;
	boost::condition_variable executing_movement_cv;
	boost::mutex executing_movement_mtx;
	std::string last_error_str;

	// velocity
	// stop the execution of the threads
	boost::atomic<bool> stop_vel_executor;
	boost::atomic<bool> stop_vel_movement;
	// execution of the movements
	boost::atomic<bool> executing_vel_movement;
	boost::atomic<bool> executing_vel_movement_error;
	boost::condition_variable executing_vel_movement_cv;
	boost::mutex executing_vel_movement_mtx;
	std::string last_vel_error_str;

	//Joint states 
	std::string joint_states_out_name;
	bool joint_states_out_worker_end_job;
	yarp::os::BufferedPort<Joint_States> joint_states_out_channel;
	boost::mutex joint_states_queue_mtx;
	std::queue<Joint_States> joint_states_queue;
	boost::condition_variable joint_states_queue_cv;
	boost::thread joint_states_out_worker;
	void joint_states_job();

	// Amtec Driver 
	CAmtecPStepServer PStepServer;
	
public:
	// Constructor
	CYarpCommunicationServerUpperLimb(const std::string name, const std::string arm_config, const std::string hand_config,
										bool en_arm=true, bool en_hand=true, bool ini_hand=false);	
	// Destructor
	~CYarpCommunicationServerUpperLimb();

	// Joints states
	bool start_joints_states_sender(std::string name = "/aros/upperlimb_right/joint_states");
	void stop_joints_states_sender();

	// add a trajectory for the upper-limb
	void AddTrajectory(std::vector<unsigned> &uParam, std::vector<float> &fData);
	// add a trajectory of velocities for the upper-limb
	void AddVelTrajectory(std::vector<unsigned> &uParam, std::vector<float> &fData);
	// Stop the execution and clear the trajectory
	void StopClearTrajectory(void);

	// set the arm and the hand devices
	void SetDevices(boost::shared_ptr<yarp::dev::AmtecLWA7dof> arm_, boost::shared_ptr<yarp::dev::BarrettHand826X> hand_);

	// Process commands
	void Process( CMessage &msgIn, CMessage &msgOut, void *private_data=nullptr );
	bool ArmProcessor(CMessage &in_req, CMessage &out_reply, void *private_data=nullptr );
	bool HandProcessor(CMessage &in_req, CMessage &out_reply, void *private_data=nullptr );


	// logging
	void init_logging();
	boost::log::sources::severity_logger< boost::log::trivial::severity_level > lg; // logger

	// Initialize
	bool Init( void );

	// Terminate
	void Fini( void );
	
	// get the arm device 
	boost::shared_ptr<yarp::dev::AmtecLWA7dof> getArmDevice(); 

	// get the hand device
	boost::shared_ptr<yarp::dev::BarrettHand826X> getHandDevice(); 

	//Executor state
	enum ExecutorState
	{
		Ready,
		Error
	};

	ExecutorState GetExecutorState();
	void ResetExecutor();
private:
	ExecutorState executor_state;
public:
	// Set positions of the joints
	void MovePos( std::vector<float> fData );
	// Set velocities of the joints
	void MoveVel( std::vector<float>& fData, double* hand_pos, double* hand_vel );
	// Wait the end of the trajectory
	int WaitTrajectoryEnd(int timeout_ms);
	// Wait the end of the trajectory
	int WaitVelTrajectoryEnd(int timeout_ms);

	// ----------------- ARM Specific functions ------------------------------- //

	bool ArmCheckCommand(unsigned int cmd);

	void HomeJoint(int joint);

	void Home(std::vector<unsigned int> &home_order);

	void HomeAll(void);

	void HomeGripper(void);

	std::string Get_Device_Error(void);

	bool isArmStatusOK(void);

	void freeJoint(int nJoint);

	void ResetJoint(int joint);

	void ResetAll(void);

	void Restart(void);


	void StopJoint(int joint);

	void StopAll(void);

	void SoftStop(void);

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

	// ----------------- HAND Specific functions ------------------------------- //
	bool isHandAvailable(void);

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

	std::string Report_Error_at_Command(unsigned int uCommand);

	std::string CommandToString(unsigned int uCommand);
};


#endif //_YARP_COMMUNICATION_SERVER_UPPERLIMB_H_