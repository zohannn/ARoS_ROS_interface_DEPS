#ifndef _YARP_COMMUNICATION_SERVER_BARRETTHAND_H_
#define _YARP_COMMUNICATION_SERVER_BARRETTHAND_H_

#include <YarpCommunication.h>
#include <yarp/os/Semaphore.h>
#include <barrett_hand_826X.h>

class CYarpCommunicationServerBarrettHand :
	public CYarpCommunication
{
public:
	yarp::dev::BarrettHand826X BarrettHand;

	std::vector<double> Finger1;
	std::vector<double> Finger2;
	std::vector<double> Finger3;
	std::vector<double> Spread;

	yarp::os::Semaphore mutex_barrett;

private:
	std::string sConfig;

	yarp::os::Semaphore mutex_Command;

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

protected:
	bool isHandAvailable( void );

protected:
	void Report_Error_at_Command( unsigned int uCommand );
	std::string CommandToString( unsigned int uCommand );

public:
	CYarpCommunicationServerBarrettHand( const std::string name, const std::string dev_config_file );
	
	~CYarpCommunicationServerBarrettHand( void ) {};

	bool Init( void );

	void Fini( void );
	
	void Process( CMessage &msgIn, CMessage &msgOut, void *private_data = nullptr );
	
	bool Initialize( void );

	bool InitJoint( std::vector<float> fData );
	
	bool OpenJoint( std::vector<float> fData );
	
	bool Open( void );
	
	bool CloseJoint( std::vector<float> fData );
	
	bool Close( void );
	
	bool OpenTorqueJoint( std::vector<float> fData );
	
	bool OpenTorque( void );
	
	bool CloseTorqueJoint( std::vector<float> fData );
	
	bool CloseTorque( void );
	
	bool SetOpenTarget( std::vector<float> fData );
	
	bool SetCloseTarget( std::vector<float> fData );

	bool GetPositions( std::vector<float> &fData );
	
	bool GetOpenTarget( std::vector<float> &fData );
	
	bool GetCloseTarget( std::vector<float> &fData );

	bool SetOpenTargetJoint( int joint, double value );
	
	bool SetCloseTargetJoint( int joint, double value );
	
	float GetOpenTargetJoint( int joint );
	
	float GetCloseTargetJoint( int joint );
	
	bool SetVelocity( std::vector<float> fData );
	
	bool GetVelocity( std::vector<float> &fData );
	
	bool SetOpenVelocity( std::vector<float> fData );
	
	bool SetCloseVelocity( std::vector<float> fData );
		
	bool SetAcceleration( std::vector<float> fData );
	
	bool GetAcceleration( std::vector<float> &fData );

	bool GetStrainGauge( std::vector<unsigned int> &uParam );
	
	void SetStrainGauge( bool ispicked1 );

	bool SetMaxStrain( std::vector<unsigned int> iData );
	
	bool GetMaxStrain( std::vector<unsigned int> &iData );

	bool IncrementalMoveJoint( int joint, double value );
	
	bool IncrementalMove( std::vector<float> fData );
	
	void GetHandConfig( double dHandConfig[4] );
	
	void GetSpreadConfig( double &dHandConfig );
	
	void SetNewComand( bool flag );
	
	bool GetNewComand( void );
	
	void SetNewComandTorque( bool flag );
	
	bool GetNewComandTorque( void );
	
	void SetNewComandSpread( bool flag );

	bool GetNewComandSpread( void );
	
};

#endif //_YARP_COMMUNICATION_SERVER_BARRETTHAND_H_
