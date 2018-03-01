#ifndef _YARP_COMMUNICATION_H_
#define _YARP_COMMUNICATION_H_

#pragma once

//------------------------------------------------------------------------------

#include "YarpCommBase.h"

//------------------------------------------------------------------------------

/// <summary>CYarpCommunication provides a two way communication channel based on YARP using a custom message format.
/// <para>It may work either as Server (waits for connections from client(s)) or as Client (establishes a two 
///	way connection to the specified server (peer).
///	When as Server, waits for connections at port 'myName'. On a message received from a client, the
///	method <see cref="CYarpCommunication::Process"/> will be called and a reply sent to the client when the latter asked for it. A call to
/// method <see cref="CYarpCommunication::Send"/> will send the specified message to all the connected clients.
///	When as a Client, tries to connect its port 'myName' to the server 'peerName' using a two way communication
///	channel. On a message received from the server, the method <see cref="CYarpCommunication::Process"/> will be called and a reply sent to the
///	server if the latter asked for it. A call to <see cref="CYarpCommunication::Send"/> will send the specified message to the server.</para>
/// </summary>
class CYarpCommunication
	: public CYarpCommBase
{
public:
	/// <summary>Creates an instance of CYarpCommunication</summary>
	/// <param name="port_name">port name for this instance (mandatory either as server or client). May also be set by <see cref="CYarpCommunication::set_PortName"/>.</param>
	/// <param name="port_name_server">port name for server where we want to connect to. (mandatory if is a client). May also be set by <see cref="CYarpCommunication::set_PortNameServer"/>.</param>
	/// <param name="isServer">true if the instance should act like a server, false otherwise.</param>
	/// <param name="privateData">Optional parameter. May be used as user data. Each call to <see cref="CYarpCommunication::Process"/> will have this value as argument.</param>
	CYarpCommunication( const std::string port_name = "", const std::string port_name_server = "", bool isServer = false, void *privateData = nullptr );

	/// <summary>Destroy the instance of CYarpCommunication</summary>
	virtual ~CYarpCommunication( void );

//------------------------------------------------------------------------------
// Message handling
protected:
	/// <summary>this is called when it receives a message (without having asked for it).</summary>
	/// <param name="connection"> Connection reader.></param>
	/// <returns>True if the message was read and processed, false otherwise. See <see cref="CYarpCommBase::get_ErrorString"/> for error details.</returns>
	bool read( yarp::os::ConnectionReader &connection );

public:
	/// <summary>Sends a specific command request over the port.
	/// <para>If bReceiveReply parameter is true, a Reply method in server will be called, otherwise Process method in server will be called.</para></summary>
	/// <param name="command">Command code to send.</param>
	/// <param name="bReceiveReply">True: request destination to send a reply to this command. False: Inform destination that this request is not interest in a reply</param>
	/// <returns>True if send is successful, false otherwise. See <see cref="CYarpCommBase::get_ErrorString"/> for error details.</returns>
	using CYarpCommBase::Send;

	/// <summary> Sends a request over the port.</summary>
	/// <param name="request">Message request to send.</param>
	/// <param name="bReceiveReply">True: request destination to send a reply to this request. False: Inform destination that this request is not interest in a reply</param>
	/// <returns>True if send is successful, false otherwise. See <see cref="CYarpCommBase::get_ErrorString"/> for error details.</returns>
	bool Send( CMessage &request, bool bReceiveReply = true );

//------------------------------------------------------------------------------
protected:
	/// <summary> To be implemented to process all the requests.
	/// This function will be invoked every time a message is received (either just to execute some command or to return a reply) or an
	/// answer from a previous request arrives.</summary>
	/// <param name="msgIn"> Message that arrives for processing.></param>
	/// <param name="msgOut"> Message that holds the reply data.></param>
	/// <param name="privateData"> User private data as provided in constructor <see cref="CYarpCommunication::CYarpCommunication"/></param>
	virtual void Process( CMessage &msgIn, CMessage &msgOut, void *privateData = nullptr ) = 0;

//------------------------------------------------------------------------------
private:
	/// <summary>Can be used to define private data.</summary>
	void *_privateData;
};

#endif //_YARP_COMMUNICATION_H_
