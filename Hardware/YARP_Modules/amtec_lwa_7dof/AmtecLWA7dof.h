#ifndef _YARP_MODULE_AMTECLWA7DOF_
#define _YARP_MODULE_AMTECLWA7DOF_

#include <vector>
#include <string>

#include <yarp/dev/ControlBoardInterfaces.h>
#include <yarp/os/Semaphore.h>

#include "Amtec_Definitions.h"

enum ARM_Side{
	RIGHT_ARM = 0,
	LEFT_ARM,
	TWO_ARMS
};

namespace yarp
{
	namespace dev
	{
		class AmtecLWA7dof;
	}
}

class yarp::dev::AmtecLWA7dof 
	: public yarp::dev::DeviceDriver
	, public yarp::dev::IPositionControl
	, public yarp::dev::IVelocityControl
{
private:
	// 0 if no error, Amtec::Error::ERROR_STATE if state error in modules, function error otherwise.
	int error_code_;
	std::string error_str_;

	//Variables to store configurations
	int 
		nDevice,
		arm_config,
		robot_nr,
		arm_nr;
	//flag defining if the device modules should wait for a sync motion command before start moving.
	bool use_sync_motion;

	std::string
		InitString,
		Conf_no_Gripper,
		Conf_Gripper,
		Conf_no_Gripper_right,
		Conf_Gripper_right,
		Conf_no_Gripper_left,
		Conf_Gripper_left;
	
	std::vector< float > 
		vec_RefSpeed,
		vec_RefAcc,
		vec_Direction,
		vec_OffSet;

	 /* Number of joints
	  *	Joints numbers goes from 0 to Number_of_Joints-1, 
	  *	witch corresponds to Modules numbers from 1 to Number_of_Joints */
	int Number_of_Joints;
	
	/*	Number of Modules, in our convention, Linear modules (gripper) are not considered joints
	 *	Modules ID goes from 0 to Number_of_Modules */
	int Number_of_Modules;
	
	bool bHasGripper;
	
protected:
	yarp::os::Semaphore mutex_PCube;

public:
	std::vector< std::string > 
		vec_str_error,
		vec_str_info,
		vec_str_conf;

	// Constructor
	AmtecLWA7dof( void );

	// Destructor
	~AmtecLWA7dof( void );

	//----------------------------------------------------------------------
	// Device Driver implementation, specific functions
	// Virtual functions from DeviceDriver class
	bool open( int nGripper = 0, int arm=0, bool use_sync_motion_ = false );
    bool open( yarp::os::Searchable& config );
	bool close( void );
	
	//----------------------------------------------------------------------
	// Specific functions for Amtec LWA arm
	bool HomeJoint( int nJoint );
	bool Home( std::vector<unsigned int> &home_order );
	bool HomeAll( void );
	bool Reset( int nJoint );
	bool ResetAll( void );
	unsigned long Get_Joint_State( int nJoint );
	unsigned char Get_Joint_Type( int nJoint );
	bool Test_Joint_Flag( int nJoint, unsigned long ulFlag );
	bool Test_Flag( unsigned long ulState, unsigned long ulFlag );

	/**
	 *	Test if specified Module number has error.
	 *	@param nModule: Module number to test.
	 *	@return: true if error no error was found, false otherwise
	 */
	bool Test_Module( int nModule );

	bool Update_State_Configuration( int nJoint );
	bool Update_State_Info( int nJoint );

	/**
	 *	Updates state error of module ( joint ).
	 *	if call succeed, vec_str_error is updated with errors strings according to state.
	 *	@param nJoint: Joint number to state error update.
	 *	@return: true if error state was updated successfully, false otherwise ( may had occurred a function error )
	 */
	bool Update_State_Error( int nJoint );

	/**
	 *	Gets error code.
	 *	if error code = 0, no error ocurred. If Amtec::Error::ERROR_STATE, state error in modules. If error 
	 *  code = Amtec::Error::ERROR_OTHER, other error occurred. Function error code otherwise.
	 *	@return: error code.
	 */
	int Get_Error_Code();

	/**
	 *	Gets error string.
	 *	if error code = 0, empty string will be returned. If error code = Amtec::Error::ERROR_STATE, state error string will be 
	 *  returned. Function error string otherwise.
	 *	@return: error string.
	 */
	std::string Get_Error_Str( void );

	std::string Get_Error_Function_String( unsigned long ulError );
	std::string Get_Error_Function_String( int error );

	bool are_Modules_OK( void );

	/**
	 *	Checks if there is any error with any module in arm.
	 *	@return: true is no error was detected, false otherwise.
	 */

	bool isArmStatusOK( void );

	/**
	 *	Checks if there is any error with the specified module.
	 *	@return: true is no error was detected, false otherwise.
	 */
	bool is_Module_OK( int joint );

	/**
	 *	Tries to restart module after an error or powerdown.
	 *	@return: true if succeeded, false otherwise.
	 */
	bool Restart_Module( void );

	//----------------------------------------------------------------------
	// Extra functions for Amtec LWA arm control
	// All the functions return 'true' if the function was sucessfull, 'false' otherwise
	// except for 'Get_Error_Function_String', for obvious reasons...
	
	// moves Joint 'j' to position 'ref' in 'time' miliseconds, if wait end is true, then function will wait for end of motion
	bool positionMove( int j, float ref, unsigned short time, bool wait_end = false );
	
	//Move all the joints to a position stored in the array 'refs' in 'time' miliseconds and wait for motion end	
	bool positionMove(float *ref, unsigned short time, bool wait_end=false );

	//Move all the joints to a position stored in the array 'refs' in 'time' miliseconds and wait for motion end	
	bool positionMoveSync(const float *ref, unsigned short time, bool wait_end = false);

	//Move some joints 'joints2move' to a position stored in the array 'refs' in 'time' miliseconds and wait for motion end	
	bool positionMove(float *refs, bool *joints2move, unsigned short time, bool wait_end );

	// moves all the joints to a position stored in the array 'refs' in the respective time (ms) stored in 'times' array
	bool positionMove( float * refs, unsigned short * times );
	
	// moves all the joints to a position stored in the array 'refs' in 'time' miliseconds
	bool positionMove( float * refs, unsigned short time, bool wait_end, float stop_margin );

	// moves all the joints to a position stored in the array 'refs' with the velocity stored in 'fVel'
	bool positionMove( float * refs, float fVel );

	// moves joint 'j' with a ramp velocity profile to position 'pos' with velocity 'vel' and acceleration 'acc'
	bool positionMoveRamp( int j, float pos, float vel, float acc );

	// moves joint 'j' with a ramp velocity profile to position 'pos' with default velocity 'vel' and reduced acceleration
	bool positionMoveRamp( int j, float pos );

	// returns position of Joint 'nJoint' in pointer 'fPos'
	bool getPos( int nJoint, float * fPos );

	// returns position of all joints in pointer 'fPos'
	bool getPosAll( float * fPos );

	// returns position of Joint 'nJoint' in pointer 'fPos' in Degrees
	bool getPosDeg( int nJoint, float * fPos );

	// returns position of all joints in pointer 'fPos' in Degrees
	bool getPosAllDeg( float * fPos );

	// returns velocity of Joint 'nJoint' in pointer 'fPos'
	bool getVel( int nJoint, float * fVel );

	// returns velocity of all the joints in pointer 'fVel'
	bool getVelAll( float * fVel );
	
	// returns Minimun limit of joint 'nJoint'
	bool getMinLimit( int nJoint, float * fPos );
	
	// returns Maximun limit of joint 'nJoint'
	bool getMaxLimit( int nJoint, float * fPos );

	// set Minimun limit of joint 'nJoint'
	bool setMinLimit( int nJoint, float fPos );

	// set Maximum limit of joint 'nJoint'
	bool setMaxLimit( int nJoint, float fPos );

	
	// returns Maximun velocity limit of joint 'nJoint' [deg/sec]
	bool getMaxVelLimit( int nJoint, float * fVel );

	// returns Default Maximun velocity limit of joint 'nJoint' [deg/sec]
	bool getMaxDefVelLimit( int nJoint, float * fVel );

	// set Maximum velocity limit of joint 'nJoint' [deg/sec]
	bool setMaxVelLimit( int nJoint, float fVel );

	// returns Maximun acceleration limit of joint 'nJoint' [deg/sec²]
	bool getMaxAccLimit( int nJoint, float * fAcc );

	// returns Default Maximun acceleration limit of joint 'nJoint' [deg/sec²]
	bool getMaxDefAccLimit( int nJoint, float * fAcc );

	// set Maximum acceleration limit of joint 'nJoint' [deg/sec²]
	bool setMaxAccLimit( int nJoint, float fAcc );
	
	// returns number of joints
	int getNumberOfJoints( void );

	// Opens the gripper (if present)
	bool openGripper( unsigned short uiTime = 5000 );	// 'uiTime' in ms (Times smaller than 4000ms may not work)

	// Closes the gripper (if present)
	bool closeGripper( unsigned short uiTime = 5000 ); 	// 'uiTime' in ms (Times smaller than 4000ms may not work)

	// Move gripper to desired position
	bool moveGripperPos( float fPos, unsigned short uiTime = 5000 ); // 'uiTime' in ms (Times smaller than 4000ms may not work)

	// Get gripper position
	bool getGripper( float *fPos );

	// Home Gripper
	bool HomeGripper( void );

	//Opens brake of specified joint
	bool freeJoint(int nJoint);
	
	bool SoftStop();

	// waits from motion end until the @param timeout has elapsed, @param state is set to 0 if success, -1 otherwise.
	bool waitMotionEnd( unsigned timeout_ms, int *state=nullptr );

	// waits until all the joints are within a margin degrees of the desired angles
	bool waitMotionEnd( float * refs, float margin_for_stop );

	//----------------------------------------------------------------------

	//----------------------------------------------------------------------
	// Extra functions to Force Torque sensor
		// All the functions return error number.
		//Get data from force torque sensor [Fx Fy Fz Tx Ty Tz]
	int get_force_torque(float *data);

	//----------------------------------------------------------------------
	//		Implemented virtual functions from IPositionControl
	//----------------------------------------------------------------------
	
    /** Stop motion, single joint
     * @param j joint number
     * @return true/false on success/failure
     */
    bool stop( int j );

    /** Stop motion, multiple joints 
     * @return true/false on success/failure
     */
    bool stop( void );
	
    /**
     * Get the number of controlled axes. This command asks the number of controlled
     * axes for the current physical interface.
     * @param ax pointer to storage
     * @return true/false.
     */
	bool getAxes( int *ax );
	 

	    /** Set new reference point for a single axis.
     * @param j joint number
     * @param ref specifies the new ref point
     * @return true/false on success/failure
     */
	bool positionMove( int j, double ref );

	
    /** Set new reference point for all axes.
     * @param refs array, new reference points.
     * @return true/false on success/failure
     */
	bool positionMove( const double *refs );

	    /** Set reference speed for a joint, this is the speed used during the
     * interpolation of the trajectory.
     * @param j joint number
     * @param sp speed value
     * @return true/false upon success/failure
     */
	bool setRefSpeed( int j, double sp );


    /** Set reference speed on all joints. These values are used during the
     * interpolation of the trajectory.
     * @param spds pointer to the array of speed values.
     * @return true/false upon success/failure
     */
	bool setRefSpeeds( const double *spds );

	
    /** Set reference acceleration for a joint. This value is used during the
     * trajectory generation.
     * @param j joint number
     * @param acc acceleration value
     * @return true/false upon success/failure
     */
	bool setRefAcceleration( int j, double acc );


    /** Set reference acceleration on all joints. This is the valure that is
     * used during the generation of the trajectory.
     * @param accs pointer to the array of acceleration values
     * @return true/false upon success/failure
     */
	bool setRefAccelerations( const double *accs );


    /** Get reference speed for a joint. Returns the speed used to 
     * generate the trajectory profile.
     * @param j joint number
     * @param ref pointer to storage for the return value
     * @return true/false on success or failure
     */
	bool getRefSpeed( int j, double *ref );


    /** Get reference speed of all joints. These are the  values used during the
     * interpolation of the trajectory.
     * @param spds pointer to the array that will store the speed values.
     */
	bool getRefSpeeds( double *spds );

	
    /** Get reference acceleration for a joint. Returns the acceleration used to 
     * generate the trajectory profile.
     * @param j joint number
     * @param acc pointer to storage for the return value
     * @return true/false on success/failure
     */
	bool getRefAcceleration( int j, double *acc );


    /** Get reference acceleration of all joints. These are the values used during the
     * interpolation of the trajectory.
     * @param accs pointer to the array that will store the acceleration values.
     * @return true/false on success or failure 
     */
	bool getRefAccelerations( double *accs );


    /** Set relative position. The command is relative to the 
     * current position of the axis.
     * @param j joint axis number
     * @param delta relative command
     * @return true/false on success/failure
     */
    bool relativeMove( int j, double delta );

	
    /** Set relative position, all joints.
     * @param deltas pointer to the relative commands
     * @return true/false on success/failure
     */
    bool relativeMove( const double *deltas );

	
    /** Check if the current trajectory is terminated. Non blocking.
     * @return true if the trajectory is terminated, false otherwise
     */
	bool checkMotionDone( int j, bool *flag );

	
    /** Check if the current trajectory is terminated. Non blocking.
     * @return true if the trajectory is terminated, false otherwise
     */
	bool checkMotionDone( bool *flag );

	
	//----------------------------------------------------------------------
	//		Not implemented functions from IPositionControl
	//----------------------------------------------------------------------

    /** Set position mode. This command
     * is required by control boards implementing different
     * control methods (e.g. velocity/torque), in some cases
     * it can be left empty.
     * return true/false on success/failure
     */
	bool setPositionMode() {	return false; }


	//----------------------------------------------------------------------
	//		virtual functions from IVelocityControl
	//----------------------------------------------------------------------
	
    /**
     * Set position mode. This command
     * is required by control boards implementing different
     * control methods (e.g. velocity/torque), in some cases
     * it can be left empty.
     * @return true/false on success failure
     */
    virtual bool setVelocityMode();

    /**
     * Start motion at a given speed, single joint.
     * @param j joint number
     * @param sp speed value
     * @return bool/false upone success/failure
     */
    virtual bool velocityMove(int j, double sp);

    /**
     * Start motion at a given speed, multiple joints.
     * @param sp pointer to the array containing the new speed values
     * @return true/false upon success/failure
     */
    virtual bool velocityMove(const double *sp);
	
	// End of virtual functions from IVelocityControl
	//----------------------------------------------------------------------
	

};

#endif // _YARP_MODULE_AMTECLWA7DOF_
