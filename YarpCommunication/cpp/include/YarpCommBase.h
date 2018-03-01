#ifndef _YARP_COMM_BASE_H_
#define _YARP_COMM_BASE_H_

#pragma once

//------------------------------------------------------------------------------
// YARP
#include <yarp/os/PortReader.h>
#include <yarp/os/BufferedPort.h>

//------------------------------------------------------------------------------
#include "Message.h"
#include "commands.h"

//------------------------------------------------------------------------------

/// <summary>CYarpCommBase provides a base for communication objects.</summary>
class CYarpCommBase
	: public yarp::os::PortReader
{
public:
	enum Mode
	{
		Client,
		Server
	};

//------------------------------------------------------------------------------
// Construction / Destruction
public:
	/// <summary>Creates an instance of CYarpCommunication</summary>
	/// <param name="port_name">Port name for this instance (mandatory either as server or client). May also be set by <see cref="CYarpCommunication::set_PortName"/>.</param>
	/// <param name="port_name_server">Port name for server where we want to connect to. (mandatory if is a client). May also be set by <see cref="CYarpCommunication::set_PortNameServer"/>.</param>
	/// <param name="mode">The connector working mode. CYarpCommBase::Client or CYarpCommBase::Server</param>
	CYarpCommBase( const std::string port_name, const std::string port_name_server, Mode mode );

	/// <summary>Creates an instance of CYarpCommunication</summary>
	/// <param name="port_name">Port name for this instance (mandatory either as server or client). May also be set by <see cref="CYarpCommunication::set_PortName"/>.</param>
	/// <param name="port_name_server">Port name for server where we want to connect to. (mandatory if is a client). May also be set by <see cref="CYarpCommunication::set_PortNameServer"/>.</param>
	/// <param name="isServer">true if the instance should act like a server, false otherwise.</param>
	CYarpCommBase( const std::string port_name, const std::string port_name_server = "", bool isServer = false );
	
	/// <summary>Destroy the instance of CYarpCommBase</summary>
	virtual ~CYarpCommBase( void );

//------------------------------------------------------------------------------
// Member variables
protected:
	std::string 
		/// <value>Local port name.</value>
		strPortName,
		/// <value>Server port name.</value>
		strPortNameServer;

	/// <summary>The connection mode this object is working in.</summary>
	Mode connectionMode;

	/// <summary>Network port used by this object.</summary>
	yarp::os::Port port;

	/// <summary>True if the communication is opened.</summary>
	bool _bOpened;

	/// <summary>Error message.</summary>
	std::string strError;

//------------------------------------------------------------------------------
// Accessors
public:
	/// <summary>Gets the connector working mode</summary>
	/// <returns>The connector mode. <see cref="YarpCommBase::Mode::Server"/> or  <see cref="YarpCommBase::Mode::Client"/></returns>
	Mode get_ConnectionMode( void );

	/// <summary>Sets local port name.</summary>
	/// <param name="port_name">Name for local port.</param>
	/// <returns>True if communication is not open (and name was changed), otherwise false.</returns>
	bool set_PortName( const std::string port_name );

	/// <summary>Sets server port name.</summary>
	/// <param name="port_name_server">Name for server port.</param>
	/// <returns>True if communication is not open (and name was changed), otherwise false.</returns>
	bool set_PortNameServer( const std::string port_name_server );

	/// <summary>Gets local name.</summary>
	/// <returns>Local port name.</returns>
	std::string get_PortName( void );

	/// <summary>Gets the server port name.</summary>
	/// <returns>Server port name, if this object is a server it returns its own port name.</returns>
	std::string get_PortNameServer( void );

	/// <summary>Checks whether the port is opened.</summary>
	/// <returns>True when port is opened, false otherwise.</returns>
	bool isOpened( void );

	/// <summary>Gets last error string.</summary>
	/// <returns>Error string.</returns>
	std::string get_ErrorString( void );

//------------------------------------------------------------------------------
// Initialization / termination
public:
	/// <summary>Opens the communication port.
	/// <para>When as server, only opens the YARP port.
	///	As client, opens the port and connects to the server</para>.
	///	<para>See also <seealso cref="CYarpCommBase::close"/></para></summary>
	/// <returns>true if everything went ok. false otherwise. Call <see cref="CYarpCommBase::get_ErrorString"/> for further error information.</returns>
	bool open( void );

	/// <summary>Closes the communication port. 
	/// <para>When as server, only closes the YARP port.
	///	As client, disconnects from the server and closes the port.</para>
	///	<para>See also <seealso cref="CYarpCommBase::open"/></para></summary>
	void close( void );

	/// <summary>Checks whether the connection is alive and working.
	///	See also <seealso cref="CYarpCommBase::open"/> and <seealso cref="CYarpCommBase::close"/></summary>
	/// <returns>true if everything went ok. false otherwise. Call <see cref="CYarpCommBase::get_ErrorString"/> for further error information.</returns>
	bool isOK( void );
	
//------------------------------------------------------------------------------
// Optional initialization and termination methods
protected:
	/// <summary>To be implemented if you want initialize something after object to be constructed and before communication to be established.</summary>
	/// <returns>True if init is successful, false otherwise.</returns>
	virtual bool Init( void );
	
	/// <summary>To be implemented if you want finalize something after communication stops.</summary>
	virtual void Fini( void );

//------------------------------------------------------------------------------
// Message handling
protected:
	/// <summary>This is called when it receives a message (without having asked for it).
	/// <para>Implementation must exist in derived class.</para></summary>
	/// <param name="connection"> Connection reader.></param>
	/// <returns>True if the message was read and processed, false otherwise. See <see cref="CYarpCommBase::get_ErrorString"/> for error details.</returns>
	virtual bool read( yarp::os::ConnectionReader &connection );

public:
	/// <summary>Sends a specific command request over the port.</summary>
	/// <param name="command">Command code to send.</param>
	/// <param name="bReceiveReply">True: request destination to send a reply to this command. False: Inform destination that this request is not interest in a reply</param>
	/// <returns>True if send is successful, false otherwise. See <see cref="CYarpCommBase::get_ErrorString"/> for error details.</returns>
	bool Send( unsigned int command, bool bReceiveReply = true );
	
	/// <summary>Sends a request over the port.</summary>
	/// <param name="request">Message request to send.</param>
	/// <param name="bReceiveReply">True: request destination to send a reply to this request. False: Inform destination that this request is not interest in a reply</param>
	/// <returns>True if send is successful, false otherwise. See <see cref="CYarpCommBase::get_ErrorString"/> for error details.</returns>
	virtual bool Send( CMessage &request, bool bReceiveReply = true );

//------------------------------------------------------------------------------
// Utility functions to assess the state of network ports
public:
	/// <summary>Checks if <paramref name="portName"/> exists, i.e., if it is registered in yarpserver.</summary>
	/// <param name="port_name">Port name to check.</param>
	/// <returns>True if port is registered, false otherwise.</returns>
	static bool isPortRegistered( const std::string port_name );

	/// <summary>Checks if <paramref name="portName"/> is alive, i.e., if it is able to receiveconnections.</summary>
	/// <param name="port_name">Port name to check.</param>
	/// <returns>True if port is alive, false otherwise.</returns>
	static bool isPortAlive( const std::string port_name );
};

#endif //_YARP_COMM_BASE_H_
