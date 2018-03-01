#ifndef _PAN_TILT_DRIVER_H
#define _PAN_TILT_DRIVER_H

#include <yarp/dev/DeviceDriver.h>
#include <yarp/dev/ControlBoardInterfaces.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/os/Property.h>
#include <yarp/os/Semaphore.h>

#if defined( USE_BOOST_SERIAL_ASIO )
	#include <serial_asio.h>
#elif defined( USE_ACE_SERIAL_IO )
	#include <Serial_IO.h>
#else
	#error Serial port in pantilt_ptud46 module, incorrectly configured.
#endif

#include "pantilt_definitions.h"

namespace yarp
{
	namespace dev
	{
		class PanTilt_PTUD46;
	}
}

//------------------------------------------------------------------------------
// Define appropriate type of serial port object to use
#if defined( USE_BOOST_SERIAL_ASIO )
	typedef yarp::dev::serial_asio YARP_DEV_SerialPort;
#elif defined( USE_ACE_SERIAL_IO )
	typedef yarp::dev::Serial_IO YARP_DEV_SerialPort;
#endif

//------------------------------------------------------------------------------

class yarp::dev::PanTilt_PTUD46
	: public yarp::dev::DeviceDriver
	, public yarp::dev::IAxisInfo
	, public yarp::dev::IPositionControlRaw
	, public yarp::dev::IPositionControl
	, public yarp::dev::IVelocityControlRaw
	, public yarp::dev::IVelocityControl
{
private:
// Member variables
	bool bOpened;

	const int nAxis;

	int nControlMode;

	std::vector<char> Motor_ID;

	std::vector<double>
		dResolution,
		dAngleLimit_Pan,
		dAngleLimit_Tilt,
		dSpeedLimit_Pan,
		dSpeedLimit_Tilt;

	yarp::os::Semaphore mutex_limits;

	YARP_DEV_SerialPort Serial_Port;

public:
	PanTilt_PTUD46( void );
	~PanTilt_PTUD46( void );

	//--------------------------------------------------------------------------
	// Device Driver implementation, specific functions
	// Virtual functions from DeviceDriver class
	bool open( std::string sCom="COM1", int nBaudRate=38400 );
	bool open( yarp::os::Searchable& config );
	bool close( void );
	bool isOpen( void );

	//--------------------------------------------------------------------------
	// Module's core functions
protected:
	inline bool Read_Reply( std::string& sReply );
	inline bool Read_Reply_Discard( void );

	inline bool Write_Command( std::string command );

	inline bool Write_Command_Discard_Reply( std::string command );

	inline bool Write_Command_Discard_Reply( int axis, char command_ID );

	inline bool Write_Command_send_1_Parameter( int axis, char command_ID, double value );
	
	inline bool Write_Command_receive_1_Parameter( int axis, char command_ID, double * value, std::string find_str="is" );

	inline bool isValidAxis( int axis );

	//--------------------------------------------------------------------------
protected:
	void Print_Error( int error_code );
	void Print_Error_str( int error_code, std::string strExtra = "" );
	void Print_Error_int( int error_code, int val );
	
	//--------------------------------------------------------------------------

protected:
	void set_AngleLimitMax_Pan( double val );
	void set_AngleLimitMin_Pan( double val );

	void set_AngleLimitMax_Tilt( double val );
	void set_AngleLimitMin_Tilt( double val );

	void set_SpeedLimitMax_Pan( double val );
	void set_SpeedLimitMin_Pan( double val );

	void set_SpeedLimitMax_Tilt( double val );
	void set_SpeedLimitMin_Tilt( double val );

public:
	double get_AngleLimitMax_Pan( void );
	double get_AngleLimitMin_Pan( void );

	double get_AngleLimitMax_Tilt( void );
	double get_AngleLimitMin_Tilt( void );

	double get_SpeedLimitMax_Pan( void );
	double get_SpeedLimitMax_Tilt( void );

	double get_SpeedLimitMin_Pan( void );
	double get_SpeedLimitMin_Tilt( void );

	//--------------------------------------------------------------------------
public:
	bool waitMotionDone( void );

public:
	//########################################################################################
	// yarp::dev::IAxisInfo functions
	
	//* Interface for getting information about specific axes, if available.
	bool getAxisName( int axis, yarp::os::ConstString& name );
	
	// same as above but accepts a std::string
	bool getAxisName( int axis, std::string& name );

public:
	//########################################################################################
	// Common function for <*>Control<*> classes

	/** Get the number of controlled axes. This command asks the number of controlled
	* axes for the current physical interface.
	* @param ax pointer to storage.
	* @return true/false on success/failure */
    bool getAxes( int *ax );

	//########################################################################################
	// Common function for <*>ControlRaw classes

	/** Stop motion, single joint.
	* @param j joint number.
	* @return true/false on success/failure */
	bool stopRaw( int j );

	/** Stop motion, multiple joints 
	* @return true/false on success/failure */
	bool stopRaw( void );

	//########################################################################################
	// Common function for <*>Control classes

	/** Stop motion, single joint.
	* @param j joint number.
	* @return true/false on success/failure*/
    bool stop( int j );

	 /** Stop motion, all the joints
	* @return true/false on success/failure */
	bool stop( void );

 	//########################################################################################
	// yarp::dev::IPositionControlRaw

	/** Set position mode. This command
	* is required by control boards implementing different
	* control methods (e.g. velocity/torque), in some cases
	* it can be left empty.
	* return true/false on success failure */
	bool setPositionModeRaw( void );

	/** Set new reference point for a single axis.
	* @param j joint number
	* @param ref specifies the new ref point
	* @return true/false on success/failure	*/
	bool positionMoveRaw( int j, double ref );

	/** Set new reference point for all axes.
	* @param refs array, new reference points.
	* @return true/false on success/failure	*/
	bool positionMoveRaw( const double *refs );

	/** Set relative position. The command is relative to the 
	* current position of the axis.
	* @param j joint axis number
	* @param delta relative command
	* @return true/false on success/failure	*/
	bool relativeMoveRaw( int j, double delta );

	/** Set relative position, all joints.
	* @param deltas pointer to the relative commands
	* @return true/false on success/failure */
	bool relativeMoveRaw( const double *deltas );

	/** Check if the current trajectory is terminated. Non blocking.
	* @param j is the axis number
	* @param flag is a pointer to return value
	* @return true/false on success/failure	*/
	bool checkMotionDoneRaw( int j, bool *flag );

	/** Check if the current trajectory is terminated. Non blocking.
	* @param flag is a pointer to return value ("and" of all joints)
	* @return true/false on success/failure */
	bool checkMotionDoneRaw( bool *flag );

	/** Set reference speed for a joint, this is the speed used during the
	* interpolation of the trajectory.
	* @param j joint number
	* @param sp speed value
	* @return true/false upon success/failure */
	bool setRefSpeedRaw( int j, double sp );

	/** Set reference speed on all joints. These values are used during the
	* interpolation of the trajectory.
	* @param spds pointer to the array of speed values.
	* @return true/false upon success/failure */
	bool setRefSpeedsRaw( const double *spds );

	/** Set reference acceleration for a joint. This value is used during the
	* trajectory generation.
	* @param j joint number
	* @param acc acceleration value
	* @return true/false upon success/failure */
	bool setRefAccelerationRaw( int j, double acc );

	/** Set reference acceleration on all joints. This is the valure that is
	* used during the generation of the trajectory.
	* @param accs pointer to the array of acceleration values
	* @return true/false upon success/failure */
	bool setRefAccelerationsRaw( const double *accs );

	/** Get reference speed for a joint. Returns the speed used to 
	* generate the trajectory profile.
	* @param j joint number
	* @param ref pointer to storage for the return value
	* @return true/false on success or failure */
	bool getRefSpeedRaw( int j, double *ref );

	/** Get reference speed of all joints. These are the  values used during the
	* interpolation of the trajectory.
	* @param spds pointer to the array that will store the speed values. */
	bool getRefSpeedsRaw( double *spds );

	/** Get reference acceleration for a joint. Returns the acceleration used to 
	* generate the trajectory profile.
	* @param j joint number
	* @param acc pointer to storage for the return value
	* @return true/false on success/failure */
	bool getRefAccelerationRaw( int j, double *acc );

	/** Get reference acceleration of all joints. These are the values used during the
	* interpolation of the trajectory.
	* @param accs pointer to the array that will store the acceleration values.
	* @return true/false on success or failure */
	bool getRefAccelerationsRaw( double *accs );

	//---------------------------------------------------------------------
	// Functions that should be present in IPositionControlRaw, but aren't

	/** Get current position for a joint.
	* @param j joint number
	* @param ref pointer to storage for the return value
	* @return true/false on success or failure */
	bool getPositionRaw( int j, double *ref );

	/** Get current position of all joints.
	* @param spds pointer to the array that will store the speed values. 
	* @return true/false on success or failure */
	bool getPositionsRaw( double *spds );

	//########################################################################################
	// yarp::dev::IPositionControl
	
	/** Set position mode. This command
	* is required by control boards implementing different
	* control methods (e.g. velocity/torque), in some cases
	* it can be left empty.
	* return true/false on success/failure 	*/
    bool setPositionMode( void );

    /** Set new reference point for a single axis.
	* @param j joint number
	* @param ref specifies the new ref point
	* @return true/false on success/failure */
    bool positionMove( int j, double ref );

    /** Set new reference point for all axes.
	* @param refs array, new reference points.
	* @return true/false on success/failure */
    bool positionMove( const double *refs );

    /** Set relative position. The command is relative to the 
	* current position of the axis.
	* @param j joint axis number
	* @param delta relative command
	* @return true/false on success/failure */
    bool relativeMove( int j, double delta );

    /** Set relative position, all joints.
	* @param deltas pointer to the relative commands
	* @return true/false on success/failure */
    bool relativeMove( const double *deltas );

    /** Check if the current trajectory is terminated. Non blocking.
	* @return true if the trajectory is terminated, false otherwise */
    bool checkMotionDone( int j, bool *flag );

    /** Check if the current trajectory is terminated. Non blocking.
	* @return true if the trajectory is terminated, false otherwise */
    bool checkMotionDone( bool *flag );

    /** Set reference speed for a joint, this is the speed used during the
	* interpolation of the trajectory.
	* @param j joint number
	* @param sp speed value
	* @return true/false upon success/failure */
    bool setRefSpeed( int j, double sp );

    /** Set reference speed on all joints. These values are used during the
	* interpolation of the trajectory.
	* @param spds pointer to the array of speed values.
	* @return true/false upon success/failure */
    bool setRefSpeeds( const double *spds );

    /** Set reference acceleration for a joint. This value is used during the
	* trajectory generation.
	* @param j joint number
	* @param acc acceleration value
	* @return true/false upon success/failure */
    bool setRefAcceleration( int j, double acc );

    /** Set reference acceleration on all joints. This is the valure that is
	* used during the generation of the trajectory.
	* @param accs pointer to the array of acceleration values
	* @return true/false upon success/failure */
    bool setRefAccelerations( const double *accs );

    /** Get reference speed for a joint. Returns the speed used to 
	* generate the trajectory profile.
	* @param j joint number
	* @param ref pointer to storage for the return value
	* @return true/false on success or failure */
    bool getRefSpeed( int j, double *ref );

    /** Get reference speed of all joints. These are the  values used during the
	* interpolation of the trajectory.
	* @param spds pointer to the array that will store the speed values. */
    bool getRefSpeeds( double *spds );

    /** Get reference acceleration for a joint. Returns the acceleration used to 
	* generate the trajectory profile.
	* @param j joint number
	* @param acc pointer to storage for the return value
	* @return true/false on success/failure */
    bool getRefAcceleration( int j, double *acc );

    /** Get reference acceleration of all joints. These are the values used during the
	* interpolation of the trajectory.
	* @param accs pointer to the array that will store the acceleration values.
	* @return true/false on success or failure */
    bool getRefAccelerations( double *accs );

	//---------------------------------------------------------------------
	// Functions that should be present in IPositionControl, but aren't

	/** Get current position for a joint.
	* @param j joint number
	* @param ref pointer to storage for the return value
	* @return true/false on success or failure */
    bool getPosition( int j, double *ref );

    /** Get position of all joints.
	* @param refs pointer to the array that will store the positions values. */
    bool getPositions( double *refs );


	//########################################################################################
	// yarp::dev::IVelocityControlRaw

	/** Set position mode. This command
	* is required by control boards implementing different
	* control methods (e.g. velocity/torque), in some cases
	* it can be left empty.
	* @return true/false on success failure */
	bool setVelocityModeRaw( void );

	/** Start motion at a given speed, single joint.
	* @param j joint number
	* @param sp speed value
	* @return bool/false upone success/failure */
	bool velocityMoveRaw( int j, double sp );

	/** Start motion at a given speed, multiple joints.
	* @param sp pointer to the array containing the new speed values
	* @return true/false upon success/failure */
	bool velocityMoveRaw( const double *sp );

	//---------------------------------------------------------------------
	// Functions that should be present in IVelocityControlRaw, but aren't

	/** Start motion at a given offset speed, single joint.
	* @param j joint number
	* @param sp speed value
	* @return bool/false upone success/failure */
	bool velocityMoveRelativeRaw( int j, double sp );


	/** Start motion at a given offset speed, multiple joints.
	* @param sp pointer to the array containing the new speed values
	* @return true/false upon success/failure */
	bool velocityMoveRelativeRaw( const double *spds );

	//#######################################################################################
	// yarp::dev::VelocityControl

	/** Set position mode. This command
	* is required by control boards implementing different
	* control methods (e.g. velocity/torque), in some cases
	* it can be left empty.
	* @return true/false on success failure */
	bool setVelocityMode( void );

	/** Start motion at a given speed, single joint.
	* @param j joint number
	* @param sp speed value
	* @return bool/false upone success/failure */
	bool velocityMove( int j, double sp );

	/** Start motion at a given speed, multiple joints.
	* @param sp pointer to the array containing the new speed values
	* @return true/false upon success/failure */
	bool velocityMove( const double *spds );

	//---------------------------------------------------------------------
	// Functions that should be present in IVelocityControl, but aren't

	/** Start motion at a given offset speed, single joint.
	* @param j joint number
	* @param sp speed value
	* @return bool/false upone success/failure */
	bool velocityMoveRelative( int j, double sp );

	/** Start motion at a given offset speed, multiple joints.
	* @param sp pointer to the array containing the new speed values
	* @return true/false upon success/failure */
	bool velocityMoveRelative( const double *spds );

	//---------------------------------------------------------------------

	/** Reset, single joint
	* @param j joint number
	* @return true/false on success/failure */
    bool reset( int j );

	/**Reset PanTilt
    * @return true/false on success/failure */
	bool reset( void );

};

#endif // _PAN_TILT_DRIVER_H
