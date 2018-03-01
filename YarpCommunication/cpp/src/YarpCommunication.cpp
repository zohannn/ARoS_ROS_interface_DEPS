#include "YarpCommunication.h"
#include <yarp/os/ConnectionWriter.h>

CYarpCommunication::CYarpCommunication( const std::string name,  const std::string peerName, bool isServer, void *privateData )
: CYarpCommBase( name , peerName, isServer )
, _privateData( privateData )
{
}

CYarpCommunication::~CYarpCommunication(void)
{
}

bool CYarpCommunication::read( yarp::os::ConnectionReader &connection )
{
	yarp::os::Bottle _bottleIn;

	bool _bSuccess = _bottleIn.read( connection );
	
	//when the port is closed, and other related events(interrupted...),this callback will be invoked
	// and bottle.read will return false.
	if( !_bSuccess )
		return false;

	CMessage 
		msgIn,
		msgOut;

	msgIn.fromBottle( _bottleIn );

	//----------

	// Verify if the writer asked for an answer.
	// If the connection has no writer an answer was not asked and the writer is not waitting for an answer, 
	// otherwise the writer is waitting for an answer
	
	yarp::os::ConnectionWriter *_writer = connection.getWriter();

	if( !_writer )
	{	// process a command received without answer request
		Process( msgIn, msgOut, _privateData );
	}
	else
	{
		// reply to a command received with answer request
		yarp::os::Bottle _bottleOut;

		msgOut.clear();
		
		msgOut.uCommand = msgIn.uCommand + static_cast<unsigned int>( ACK );

		Process( msgIn, msgOut, _privateData );

		msgOut.toBottle( _bottleOut );
		_bSuccess = _bottleOut.write( *_writer );
	}

	//----------

	return _bSuccess;
}

bool CYarpCommunication::Send( CMessage &request, bool bReceiveReply )
{
	yarp::os::Bottle _bottleOut;
	request.toBottle( _bottleOut );

	bool _bSuccess = false;

	if( !bReceiveReply )
	{
		_bSuccess = port.write( _bottleOut );
	}
	else
	{
		yarp::os::Bottle _bottleIn;
		
		CMessage 
			msgIn,
			msgOut;

		_bottleIn.clear();
		_bSuccess = port.write( _bottleOut, _bottleIn );
		msgIn.fromBottle( _bottleIn );
	
		Process( msgIn, msgOut, _privateData );
	}

	if( !_bSuccess )
		strError = "Error sending the request over the network. Connection Lost.";

	return _bSuccess;
}
