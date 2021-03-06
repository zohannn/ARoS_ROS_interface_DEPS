#include "YarpConnector.h"
#include <yarp/os/ConnectionWriter.h>

YarpConnector::YarpConnector( const std::string _name, std::string _portName, const std::string _serverPortName, Mode mode )
: CYarpCommBase( _portName, _serverPortName, mode )
, name( _name )
{
	Process = nullptr;
}

YarpConnector::~YarpConnector( void )
{
}

bool YarpConnector::read( yarp::os::ConnectionReader &connection )
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
	// If the connection has no writer an answer was not asked and the writer is not waiting for an answer, 
	// otherwise the writer is waiting for an answer
	
	yarp::os::ConnectionWriter *_writer = connection.getWriter();

	if( !_writer )
	{	// process a command received without answer request
		if (Process != nullptr)
			Process(msgIn, msgOut);
	}
	else
	{
		// reply to a command received with answer request
		yarp::os::Bottle _bottleOut;

		msgOut.clear();
		
		msgOut.uCommand = msgIn.uCommand + static_cast<unsigned int>( ACK );

		if (Process != nullptr)
			Process(msgIn, msgOut);

		msgOut.toBottle( _bottleOut );
		_bSuccess = _bottleOut.write( *_writer );
	}

	//----------

	return _bSuccess;
}

void YarpConnector::SetReceiveHandler( std::function<void(CMessage&, CMessage&)> process )
{
	Process = process;
}

bool YarpConnector::Send( CMessage &request, bool bReceiveReply )
{
	yarp::os::Bottle _bottleOut;
	request.toBottle( _bottleOut );

	bool _bSuccess = false;

	if( !bReceiveReply ) {
		_bSuccess = port.write( _bottleOut );
	}
	else {
		yarp::os::Bottle _bottleIn;
		
		CMessage 
			msgIn,
			msgOut;

		_bottleIn.clear();
		_bSuccess = port.write( _bottleOut, _bottleIn );
		if (_bSuccess)
		{
			msgIn.fromBottle(_bottleIn);

			if (Process != nullptr)
				Process(msgIn, msgOut);
		}
	}

	if( !_bSuccess )
		strError = "Error sending the request over the network. Connection Lost.";

	return _bSuccess;
}

bool YarpConnector::Name( std::string _name )
{
	if (isOpened()) return false;

	name = _name;

	return true;
}

std::string YarpConnector::Name( void )
{
	return name;
}
