#ifndef YARP_MODULES_BARRETTHAND826X
#define YARP_MODULES_BARRETTHAND826X

#include <yarp/dev/DeviceDriver.h>
#include <yarp/dev/ControlBoardInterfaces.h>

#include <serial_asio.h>

/*
#if defined( USE_BOOST_SERIAL_ASIO )
	#include <serial_asio.h>
#elif defined( USE_ACE_SERIAL_IO )
	#include <Serial_IO.h>
#else
	#error Serial port in barrett_hand_826X module, incorrectly configured.
#endif
*/
#include "barrett_hand_definitions.h"

namespace yarp
{
	namespace dev
	{
		class BarrettHand826X;
	}
}

class yarp::dev::BarrettHand826X
	: public yarp::dev::DeviceDriver
	, public yarp::dev::IAxisInfo
	, public yarp::dev::IPositionControl
	, public yarp::dev::IPositionControlRaw
{
public:
	BarrettHand826X( void );

protected:
	yarp::dev::serial_asio serial_port;
/*
#if defined( USE_BOOST_SERIAL_ASIO )
	yarp::dev::serial_asio serial_port;
#elif defined( USE_ACE_SERIAL_IO )
	yarp::dev::Serial_IO serial_port;
#endif
*/
private:
	char BHand_Finger_ID[ BarrettHand::Finger::BARRETT_HAND_NUMBER_OF_DOF ];
	
	double
		BHand_SetParam_Ratio[ BarrettHand::Finger::BARRETT_HAND_NUMBER_OF_DOF ],
		BHand_GetParam_Ratio[ BarrettHand::Finger::BARRETT_HAND_NUMBER_OF_DOF ];

	size_t BHand_Device_2_Module_Idx[BarrettHand::Finger::BARRETT_HAND_NUMBER_OF_DOF];

	std::string strStop;
		
public:
	//----------------------------------------------------------------------
	// Device Driver implementation, specific functions
	// Virtual functions from DeviceDriver class
	bool open( std::string cCom, long lBaudRate, double dCTfingers, double dCTspread, double dOTfingers, double dOTspread, double dSpeedfingers, double dSpeedspread, double dAccelfingers, double dAccelspread , long lMaxStrainfingers, long lMaxStrainspread, long iInitialize);
	bool open( yarp::os::Searchable& config );
	bool close( void );

	//----------------------------------------------------------------------
	// Module's core functions
protected:
	bool isValidAxis( int axis );

	bool Read_Reply( std::string& sReply, int timeout_ms=10000 );
	bool Read_Reply_Discard( int timeout_ms=10000 );
	bool Read_Reply_Wait_Char( std::string &sReply, char waitChar, int timeout_ms=10000 );

	bool Write_Command( std::string command );
	bool Write_Command_Discard_Reply( std::string command, int timeout_ms=10000 );
	bool Write_Command_Read_Reply( std::string command, std::string &sReply, int timeout_ms=10000 );

	bool Parse_int_from_command_reply( std::string reply, std::string command, int * val, int conv=1 );
	bool Parse_double_from_command_reply( std::string reply, std::string command, double * val, double conv=1.0 );
	
	void yarp::dev::BarrettHand826X::serial_write_handler(const boost::system::error_code& ec,
														std::size_t bytes_transferred);

public:
	boost::asio::io_service ioService; // service to handle the asynchronous readings
	bool Write_Command_Read_Reply_Clean( std::string command, std::string &sReply, int timeout_ms=10000 );

public:

	// Real Time (RT) mode functions
	bool rt_mode_on;
	
	/** start the RT mode of the barrett hand 
	* @return true/false on success/failure
	*/
	bool startRTmode();
	
	/** stop the RT mode of the barrett hand 
	* @return true/false on success/failure
	*/
	bool stopRTmode();

	/** get the current velocities of the barrett hand 
	* @param  vel [1,2,3], spread
	* @return true/false on success/failure
	*/
	bool getRTVelocities(double *vel);

	/** get the current positions of the barrett hand 
	* @param  pos [1,2,3], spread
	* @return true/false on success/failure
	*/
	bool getRTPositions(double *pos);

	/** set the current velocities of the barrett hand 
	* @param  vel [1,2,3], spread
	* @return true/false on success/failure
	*/
	bool setRTVelocities(const std::vector<double>& vel);

	int getGainVelocity(double vel);


	// Supervisory mode functions

	/** Initialize all the barrett hand
	* @return true/false on success/failure
	*/
	bool Initialize( void );
	
	/** Initialize one finger/spread of barrett hand
	  * @param j finger[1,2,3]/spread[0] number
	* @return true/false on success/failure
	*/
	bool Initialize( int *vec );
	
	/** Open all the fingers of Barrett Hand
	* @return true/false on success/failure
	*/
	bool OpenHand( void );
	
	/** Close all the fingers of Barrett Hand
	* @return true/false on success/failure
	*/
	bool CloseHand( void );
	
	/** Open a finger/spread of Barrett Hand
	  * @param j finger[1,2,3]/spread[0] number
	* @return true/false on success/failure
	*/
	bool OpenHand( int *vec );
	
	/** CLose a finger/spread of Barrett Hand
	  * @param j finger[1,2,3]/spread[0] number
	* @return true/false on success/failure
	*/
	bool CloseHand( int *vec );
	
	/** Open fingers with control of motor torque at stall
	* @return true/false on success/failure
	*/
	bool TorqueOpen( void );
	
	/** CLose fingers with control of motor torque at stall
	* @return true/false on success/failure
	*/
	bool TorqueClose( void );
	
	/** Open a finger/spread with control of motor torque at stall
	  * @param j finger[1,2,3]/spread[0] number
	* @return true/false on success/failure
	*/
	bool TorqueOpen( int *vec );
	
	/** Close a finger/spread with control of motor torque at stall
	  * @param j finger[1,2,3]/spread[0] number
	* @return true/false on success/failure
	*/
	bool TorqueClose( int *vec );

	/** Get position of all joints. These are the  values used during the
	* interpolation of the trajectory.
	* @param spds pointer to the array that will store the speed values.
	*/
	bool getPositionsRaw( double *pos );

	/** Get position for finger and spread.
	* @param j finger[1,2,3]/spread[0] number
	* @param ref pointer to storage for the return value
	* @return true/false on success/failure
	*/
	bool getPositionRaw( int j, double *ref );

	/** Get position of all joints. These are the  values used during the
	* interpolation of the trajectory.
	* @param spds pointer to the array that will store the speed values.
	*/
	bool getPositions( double *pos );

	/** Get position for finger and spread.
	* @param j finger[1,2,3]/spread[0] number
	* @param ref pointer to storage for the return value
	* @return true/false on success/failure
	*/
	bool getPosition( int j, double *ref );
	
	/** Set the position gone to by a Open command
	* @param refs pointer to the array of open target values.
	* @return true/false on success/failure
	*/
	bool setOpenTarget( double *refs );
	
	/** Set the position gone to by a Open command
	* @param j finger[1,2,3]/spread[0] number
	* @param ref open target value
	* @return true/false upon success/failure
	*/
	bool setOpenTarget( int j, double ref );

	/** Set the max strain for torque
	* @param lrefs pointer to the array of max strain.
	* @return true/false on success/failure
	*/
	bool setMaxStrain( int * refs );
	
	/** Set the max strain for torque
	* @param j finger[1,2,3]/spread[0] number
	* @param ref max strain value
	* @return true/false upon success/failure
	*/
	bool setMaxStrain( int j, int ref );
	
	/** Set the max strain for torque
	* @param lrefs pointer to the array of max strain.
	* @return true/false on success/failure
	*/
	bool getMaxStrain( int * refs );
	
	/** Set the max strain for torque
	* @param j finger[1,2,3]/spread[0] number
	* @param ref max strain value
	* @return true/false upon success/failure
	*/
	bool getMaxStrain( int j, int * ref );

	/** Set the position gone to by a Close command
	* @param refs pointer to the array of close target values.
	* @return true/false on success/failure
	*/
	bool setCloseTarget( double *refs );
	
	/** Set the position gone to by a Close command
	* @param j finger[1,2,3]/spread[0] number
	* @param ref close target value
	* @return true/false upon success/failure
	*/
	bool setCloseTarget( int j, double ref );
	
	/** Get open target of all the fingers and spread. 
	* @param refs pointer to the array that will store the open target values.
	* @return true/false on success/failure
	*/
	bool getOpenTarget( double *refs );
	
	/** Get open target for a finger or spread.
	* @param j finger[1,2,3]/spread[0] number
	* @param ref pointer to storage for the return value
	* @return true/false on success/failure
	*/
	bool getOpenTarget( int j, double *ref );
	
	/** Get close target of all the fingers and spread. 
	* @param refs pointer to the array that will store the close target values.
	* @return true/false on success/failure
	*/
	bool getCloseTarget( double *refs );
	
	/** Get close target for a finger or spread.
	* @param j finger[1,2,3]/spread[0] number
	* @param ref pointer to storage for the return value
	* @return true/false on success/failure
	*/
	bool getCloseTarget( int j, double *ref );

	/** Set reference speed for a joint, this is the speed used during the
	* interpolation of the trajectory.
	* @param j finger[1,2,3]/spread[0] number
	* @param sp speed value
	* @return true/false upon success/failure
	*/
	bool setRefSpeedOpen( int j, double sp );

	/** Set reference speed on all joints. These values are used during the
	* interpolation of the trajectory.
	* @param spds pointer to the array of speed values.
	* @return true/false upon success/failure
	*/
	bool setRefSpeedsOpen( const double *spds );

	/** Set reference speed for a joint, this is the speed used during the
	* interpolation of the trajectory.
	* @param j finger[1,2,3]/spread[0] number
	* @param sp speed value
	* @return true/false upon success/failure
	*/
	bool setRefSpeedClose( int j, double sp );

	/** Set reference speed on all joints. These values are used during the
	* interpolation of the trajectory.
	* @param spds pointer to the array of speed values.
	* @return true/false upon success/failure
	*/
	bool setRefSpeedsClose( const double *spds );

	//----------------------------------------------------------------------
	// Accessors for strain gauge
	/** Get strain gauge for finger 'j'
	* @param j finger[1,2,3]/spread[0] number
	* @param ref pointer to storage for the return value
	* @return true/false on success/failure
	*/
	bool getStrainGauge( int j, double *strain );

	/** Get strain gauge for all fingers
	* @param refs pointer to storage for the return value
	* @return true/false on success/failure
	*/
	bool getStrainGauges( double *strain );

	//----------------------------------------------------------------------
	// Custom functions for finger control
	
	/** Move all fingers to position.
	* @param ref specifies the new position in encoder units
	* @return true/false on success/failure
	*/
	bool positionMoveFingersRaw( double ref );

	/** Move all fingers to position.
	* @param ref specifies the new position
	* @return true/false on success/failure
	*/
	bool positionMoveFingers( double ref );

	/** Move all fingers to specified positions.
	* @param refs specifies the new positions for each finger
	* @return true/false on success/failure
	*/
	bool positionMoveFingers( const double *refs );

	/** Move fingers 1 and 3 to specified positions.
	* @param refs specifies the new positions for each finger in encoder units
	* @return true/false on success/failure
	*/
	bool positionMoveTwoFingersRaw( double *ref );
	
	/** Move fingers 1 and 3 to specified positions.
	* @param refs specifies the new positions for each finger
	* @return true/false on success/failure
	*/
	bool positionMoveTwoFingers( double *ref );

	//----------------------------------------------------------------------
	// Common functions from yarp::dev::I<*>Control classes
	/**
	 * Get the number of controlled axes. This command asks the number of controlled
	 * axes for the current physical interface.
	 * @param ax pointer to storage
	 * @return true/false.
	 */
	bool getAxes( int *ax );

	int getAxes( void );

	//------------------------------------------------------------------------
	// yarp::dev::IAxisInfo functions
	
	//* Interface for getting information about specific axes, if available.
	bool getAxisName( int axis, yarp::os::ConstString& name );
	
	// same as above but accepts a std::string
	bool getAxisName( int axis, std::string& name );

	//------------------------------------------------------------------------

	//Virtual functions from IPositionControl

	/** Set position mode. This command
	* is required by control boards implementing different
	* control methods (e.g. velocity/torque), in some cases
	* it can be left empty.
	* return true/false on success/failure
	*/
	bool setPositionMode( void );

	/** Set new reference point for a single axis.
	* @param j finger[1,2,3]/spread[0] number
	* @param ref specifies the new ref point
	* @return true/false on success/failure
	*/
	bool positionMove( int j, double ref );

	/** Set new reference point for all axes.
	* @param refs array, new reference points.
	* @return true/false on success/failure
	*/
	bool positionMove( const double *refs );

	/** Set relative position. The command is relative to the 
	* current position of the axis.
	* @param j finger[1,2,3]/spread[0] number
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

	/** Set reference speed for a joint, this is the speed used during the
	* interpolation of the trajectory.
	* @param j finger[1,2,3]/spread[0] number
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
	* @param j finger[1,2,3]/spread[0] number
	* @param acc acceleration value
	* @return true/false upon success/failure
	*/
	bool setRefAcceleration( int j, double acc);

	/** Set reference acceleration on all joints. This is the valure that is
	* used during the generation of the trajectory.
	* @param accs pointer to the array of acceleration values
	* @return true/false upon success/failure
	*/
	bool setRefAccelerations( const double *accs);

	/** Get reference speed for a joint. Returns the speed used to 
	* generate the trajectory profile.
	* @param j finger[1,2,3]/spread[0] number
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
	* @param j finger[1,2,3]/spread[0] number
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

	/** Stop motion, single joint.
	* @param j joint number.
	* @return true/false on success/failure*/
    bool stop( int j );

	 /** Stop motion, all the joints
	* @return true/false on success/failure */
	bool stop( void );	

	//------------------------------------------------------------------------

	//Virtual functions from IPositionControlRaw

	/** Set position mode. This command
	* is required by control boards implementing different
	* control methods (e.g. velocity/torque), in some cases
	* it can be left empty.
	* return true/false on success failure
	*/
	bool setPositionModeRaw( void );

	/** Set new reference point for a single axis.
	* @param j joint number
	* @param ref specifies the new ref point
	* @return true/false on success/failure
	*/
	bool positionMoveRaw( int j, double ref );

	/** Set new reference point for all axes.
	* @param refs array, new reference points.
	* @return true/false on success/failure
	*/
	bool positionMoveRaw( const double *refs );

	/** Set relative position. The command is relative to the 
	* current position of the axis.
	* @param j joint axis number
	* @param delta relative command
	* @return true/false on success/failure
	*/
	bool relativeMoveRaw( int j, double delta );

	/** Set relative position, all joints.
	* @param deltas pointer to the relative commands
	* @return true/false on success/failure
	*/
	bool relativeMoveRaw( const double *deltas );

	/** Check if the current trajectory is terminated. Non blocking.
	* @param j is the axis number
	* @param flag is a pointer to return value
	* @return true/false on network communication ( value you actually want
	is stored in *flag )
	*/
	bool checkMotionDoneRaw( int j, bool *flag );

	/** Check if the current trajectory is terminated. Non blocking.
	* @param flag is a pointer to return value ( "and" of all joints )
	* @return true/false on network communication ( value you actually want
	is stored in *flag )
	*/
	bool checkMotionDoneRaw( bool *flag );

	/** Set reference speed for a joint, this is the speed used during the
	* interpolation of the trajectory.
	* @param j joint number
	* @param sp speed value
	* @return true/false upon success/failure
	*/
	bool setRefSpeedRaw( int j, double sp );

	/** Set reference speed on all joints. These values are used during the
	* interpolation of the trajectory.
	* @param spds pointer to the array of speed values.
	* @return true/false upon success/failure
	*/
	bool setRefSpeedsRaw( const double *spds );

	/** Set reference acceleration for a joint. This value is used during the
	* trajectory generation.
	* @param j joint number
	* @param acc acceleration value
	* @return true/false upon success/failure
	*/
	bool setRefAccelerationRaw( int j, double acc );

	/** Set reference acceleration on all joints. This is the valure that is
	* used during the generation of the trajectory.
	* @param accs pointer to the array of acceleration values
	* @return true/false upon success/failure
	*/
	bool setRefAccelerationsRaw( const double *accs );

	/** Get reference speed for a joint. Returns the speed used to 
	* generate the trajectory profile.
	* @param j joint number
	* @param ref pointer to storage for the return value
	* @return true/false on success or failure
	*/
	bool getRefSpeedRaw( int j, double *ref );

	/** Get reference speed of all joints. These are the  values used during the
	* interpolation of the trajectory.
	* @param spds pointer to the array that will store the speed values.
	*/
	bool getRefSpeedsRaw( double *spds );

	/** Get reference acceleration for a joint. Returns the acceleration used to 
	* generate the trajectory profile.
	* @param j joint number
	* @param acc pointer to storage for the return value
	* @return true/false on success/failure
	*/
	bool getRefAccelerationRaw( int j, double *acc );

	/** Get reference acceleration of all joints. These are the values used during the
	* interpolation of the trajectory.
	* @param accs pointer to the array that will store the acceleration values.
	* @return true/false on success or failure 
	*/
	bool getRefAccelerationsRaw( double *accs );

	/** Stop motion, single joint
	* @param j joint number
	* @return true/false on success/failure
	*/
	bool stopRaw( int j );

	/** Stop motion, multiple joints 
	* @return true/false on success/failure
	*/
	bool stopRaw( void );

	/** Retrieve status of finger 'j'
	* @return true/false on success/failure
	*/
	bool getStatus( int j, int * status );

	/** Retrieve status of finger 'j' and respective string
	* @return true/false on success/failure
	*/
	bool getStatus( int j, int * status, std::string &str_status );

	/** Retrieve status of all fingers
	* @return true/false on success/failure
	*/
	bool getStatus( int * status );

	/** Retrieve a human readable string of the status
	* @return true/false on success/failure
	*/
	bool getStatus_str( int status, std::string &str_status );
};

#endif //YARP_MODULES_BARRETTHAND826X
