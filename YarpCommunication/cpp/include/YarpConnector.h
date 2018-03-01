
#pragma once

#include <functional>

#include "YarpCommBase.h"

//------------------------------------------------------------------------------

/// <summary>YarpConnector provides a two way communication channel based on YARP using a custom message format.
/// <para>It may work either as Server (waits for connections from client(s)) or as Client (establishes a two 
///	way connection to the specified server (peer).
///	When as Server, waits for connections at port 'portName'. On a message received from a client, the
///	method <see cref="YarpConnector::Process"/> will be called and a reply sent to the client when the latter asked for it. A call to
/// method <see cref="YarpConnector::Send"/> will send the specified message to all the connected clients.
///	When as a Client, tries to connect its port 'portName' to the server 'serverPortName' using a two way communication
///	channel. On a message received from the server, the method <see cref="YarpConnector::Process"/> will be called and a reply sent to the
///	server if the latter asked for it. A call to <see cref="YarpConnector::Send"/> will send the specified message to the server.</para>
/// </summary>
class YarpConnector
	: public CYarpCommBase
{
//------------------------------------------------------------------------------
public:
	/// <summary>Creates an instance of YarpConnector</summary>
	/// <param name="name"> Connector instance name</param>
	/// <param name="portName">port name for this instance (mandatory either as server or client). May also be set by <see cref="YarpConnector::PortName"/>.</param>
	/// <param name="ServerPortName">port name for server where we want to connect to. (mandatory if is a client). May also be set by <see cref="YarpConnector::ServerPortName"/>.</param>
	/// <param name="mode">The connector working mode. Mode::Client or Mode::Server</param>
	YarpConnector( const std::string name, const std::string portName = "", const std::string serverPortName = "", Mode mode = Client );

	/// <summary>Destroy the instance of YarpConnector</summary>
	virtual ~YarpConnector( void );

//------------------------------------------------------------------------------
// Member variables
protected:
	std::string name;

//------------------------------------------------------------------------------
// Accessors
public:
	/// <summary>Sets the connector name</summary>
	/// <param name="name">Name</param>
	/// <returns>True if communication is not open (and name was changed), otherwise false.</returns>
	bool Name( const std::string name );

	/// <summary>Gets this connector name.</summary>
	/// <returns>Connector name.</returns>
	std::string Name( void );
	
//------------------------------------------------------------------------------
protected:
	/// <summary>this is called when it receives a message (without having asked for it).</summary>
	bool read( yarp::os::ConnectionReader &connection );

//------------------------------------------------------------------------------
protected:
	/// <summary> Function pointer to the callback responsible for processing the asynchronously received messages
	/// and/or the replies to the Sent messages.
	/// This function will be invoked every time a message is received, or an
	/// answer from a previous request arrives.
	/// you may set the callback using the SetReceiverHandler method.</summary>
	std::function<void( CMessage&, CMessage& )> Process;

//------------------------------------------------------------------------------
public:
	///<summary> Sets the handler to the callback that will process an
	/// asynchronous message reception.</summary>
	///<param name="process"> Function that accepts two CMessage as argument and returns nothing. 
	/// This can either be a function pointer, a function object or a lambda function. </param>
	void SetReceiveHandler( std::function<void(CMessage&, CMessage&)> process );

	/// <summary>Sends a specific command request over the port.</summary>
	/// <param name="command">Command code to send.</param>
	/// <param name="bReceiveReply">True: request destination to send a reply to this command. False: Inform destination that this request is not interest in a reply</param>
	/// <returns>True if send is successful, false otherwise. See <see cref="CYarpCommBase::get_ErrorString"/> for error details.</returns>
	using CYarpCommBase::Send;

	/// <summary> Sends a request over the port.</summary>
	/// <param name="request">Message request to send.</param>
	/// <param name="bReceiveReply">True: request destination to send a reply to this request. False: Inform destination that this request is not interest in a reply</param>
	/// <returns>True if send is successful, false otherwise. See <see cref="CYarpCommBase::get_ErrorString"/> for error details.</returns>
	bool Send( CMessage &request, bool bReceiveReply = true );

	/// <summary> Sends a request over the port.</summary>
	/// <param name="request">Message request to send.</param>
	/// <param name="pred">Function that accepts a CMessage as argument and returns nothing. 
	/// This can either be a function pointer, a function object or a lambda function.</param>
	/// <returns>True if send is successful, false otherwise. See <see cref="CYarpCommBase::get_ErrorString"/> for error details.</returns>
	template<class _Pr>
	bool inline Send( CMessage &request, _Pr pred )
	{
		yarp::os::Bottle _bottleOut, _bottleIn;
		request.toBottle(_bottleOut);

		bool _bSuccess = false;

		CMessage
			msgIn,
			msgOut;

		_bottleIn.clear();
		_bSuccess = port.write(_bottleOut, _bottleIn);
		if (!_bSuccess)
			strError = "Error sending the request over the network. Connection Lost.";
		else
		{
			msgIn.fromBottle(_bottleIn);
			pred(msgIn);
		}

		return _bSuccess;
	}

	/// <summary> Sends a request over the port.</summary>
	/// <param name="command">Command code to send.</param>
	/// <param name="pred">Function that accepts a CMessage as argument and returns nothing. 
	/// This can either be a function pointer, a function object or a lambda function.</param>
	/// <returns>True if send is successful, false otherwise. See <see cref="CYarpCommBase::get_ErrorString"/> for error details.</returns>
	template<class _Pr>
	bool inline Send( unsigned  command, _Pr pred )
	{
		CMessage request;
		request.uCommand = command;
		return Send(request, pred);
	}
};