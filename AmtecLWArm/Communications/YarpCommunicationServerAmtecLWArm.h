#ifndef _YARP_COMMUNICATION_SERVER_AMTECLWARM_H_
#define _YARP_COMMUNICATION_SERVER_AMTECLWARM_H_

#include <YarpCommunication.h>
#include <yarp/os/Semaphore.h>
#include <AmtecPStepServer.h>
#include <AmtecLWA7dof.h>
#include "WrapperAmtecArm.h"

class CYarpCommunicationServerAmtecLWArm
	: public CYarpCommunication
	, public CWrapperAmtecArm
{
protected:
	std::string sConfig;
	
public:
	// Constructor
	CYarpCommunicationServerAmtecLWArm( const std::string name, const std::string dev_config_file );
	
	// Destructor
	~CYarpCommunicationServerAmtecLWArm( void );

	// Initialize
	bool Init( void );

	// Terminate
	void Fini( void );

	// Process commands
	void Process( CMessage &msgIn, CMessage &msgOut, void *private_data );
};

#endif //_YARP_COMMUNICATION_SERVER_AMTECLWARM_H_
