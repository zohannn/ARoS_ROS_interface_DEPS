#include "YarpCommBase.h"

#include <yarp/conf/version.h>
#include <yarp/os/impl/NameClient.h>
#include <yarp/os/impl/Carriers.h>

#if ( YARP_VERSION_MAJOR >= 2 ) && ( YARP_VERSION_MINOR >= 3 ) && ( YARP_VERSION_PATCH >= 60 )
// Use in yarp versions 2.3.60+
	typedef yarp::os::Contact YARP_Address;
	typedef yarp::os::OutputProtocol YARP_OutputProtocol;
#else
	typedef yarp::os::impl::Address YARP_Address;
	typedef yarp::os::impl::OutputProtocol YARP_OutputProtocol;
#endif

//----------

CYarpCommBase::CYarpCommBase( const std::string port_name, const std::string port_name_server, Mode mode )
: yarp::os::PortReader()
, port()
, connectionMode( mode )
, strPortName( port_name )
, strPortNameServer( port_name_server )
, _bOpened( false )
{
}

CYarpCommBase::CYarpCommBase( const std::string port_name, const std::string port_name_server, bool isServer )
: yarp::os::PortReader()
, port()
, connectionMode( isServer ? CYarpCommBase::Server : CYarpCommBase::Client )
, strPortName( port_name )
, strPortNameServer( port_name_server )
, _bOpened( false )
{
}

CYarpCommBase::~CYarpCommBase( void )
{
	if( isOpened() )
		close();
}

CYarpCommBase::Mode CYarpCommBase::get_ConnectionMode( void )
{
	return connectionMode;
}

bool CYarpCommBase::open( void )
{
	//----------

	std::string strException = "";
	bool _bSuccess = false;

	//----------

	try {
		_bSuccess = Init();
		if( !_bSuccess ) {
			strException = "Initilization on the inherited class failed";
			throw strException;
		}

		_bSuccess = yarp::os::Network::checkNetwork( 0.5 );
		if( !_bSuccess )
		{
			yarp::os::Network::fini();
			yarp::os::Network::init();
			_bSuccess = yarp::os::Network::checkNetwork( 0.5 );
			if (!_bSuccess)
			{
				strException = "Yarp Server not running!";
				throw strException;
			}
		}
		
		if( strPortName.size() == 0 ) {
			strException = "Name port is empty";
			throw strException;
		}

		if( ( connectionMode == CYarpCommBase::Client ) && strPortNameServer.size() == 0 ) {
			strException = "Peername port is empty";
			throw strException;
		}

		_bSuccess = isPortAlive( strPortName.c_str() );
		//_bSuccess = yarp::os::Network::exists( strPortName.c_str() );
		if( _bSuccess ) {
			strException = "A port named \"";
			strException += strPortName;
			strException += "\" is being used";
			throw strException;
		}

		_bSuccess = isPortRegistered( strPortName.c_str() );
		if( _bSuccess ) {
			yarp::os::Network::unregisterName( strPortName.c_str() );
		}
	}
	catch( std::string &str ) {
		std::cout << "Exception: " << str << "." << std::endl;
		strError = str;
		_bOpened = false;
		return _bOpened;
	}

	//----------

	if( connectionMode == CYarpCommBase::Client )
	{
		try {
			_bSuccess = isPortAlive( strPortNameServer );
			if( !_bSuccess ) {
				strException = "The port named \"";
				strException += strPortNameServer;
				strException += "\" is not alive (peer)";
				throw strException;
			}

			_bSuccess = port.open( strPortName.c_str() );
			if( !_bSuccess ) {
				strException = "Could not open a port named \"";
				strException += strPortName;
				strException += "\"";
				throw strException;
			}

			_bSuccess = yarp::os::Network::connect( strPortName.c_str(), strPortNameServer.c_str() );
			if( !_bSuccess ) {
				port.close();
				yarp::os::Network::unregisterName( strPortName.c_str() );
				strException = "Could not connect the port \"";
				strException += strPortName;
				strException += "\" with the port \"";
				strException += strPortNameServer;
				strException += "\"";
				throw strException;
			}

			_bSuccess = yarp::os::Network::connect( strPortNameServer.c_str(), strPortName.c_str() );
			if( !_bSuccess ) {
				yarp::os::Network::disconnect( strPortName.c_str(), strPortNameServer.c_str() );
				port.close();
				yarp::os::Network::unregisterName( strPortName.c_str() );
				strException = "Could not connect the port \"";
				strException += strPortNameServer;
				strException += "\" with the port \"";
				strException += strPortName;
				strException += "\"";
				throw strException;
			}

			port.setReader( ( yarp::os::PortReader& )( *this ) );
		}
		catch( std::string &str ) {
			std::cout << "Exception: " << str << "." << std::endl;
			strError = str;
			_bOpened = false;
			return _bOpened;
		}
	}
	else
	{
		try {
			_bSuccess = port.open( strPortName.c_str() );
			if( !_bSuccess ) {
				strException = "Could not open a port named \"";
				strException += strPortName;
				strException += "\"";
				throw strException;
			}

			port.setReader( ( yarp::os::PortReader& )( *this ) );
			
		}
		catch( std::string &str ) {
		std::cout << "Exception: " << str << "." << std::endl;
			strError = str;
			_bOpened = false;
			return _bOpened;
		}
	}

	//----------

	_bOpened = true;
	return _bOpened;
}

bool CYarpCommBase::isOK( void )
{
	bool isok = false;
	std::string strException = "";

	try {
		isok = _bOpened;
		if( !isok )
		{
			strException = "The port was not open!";
			throw strException;
		}

		//as server, check only if we are ok
		isok = yarp::os::Network::checkNetwork( 0.3 );
		if( !isok )
		{
			strException = "Yarp Server not running!";
			throw strException;
		}

		isok = isPortRegistered( strPortName );
		if( !isok ) {
			strException = "This port named \"";
			strException += strPortName;
			strException += "\" was not found in nameserver";
			throw strException;
		}


		isok = isPortAlive( strPortName );
		if( !isok ) {
			strException = "This port named \"";
			strException += strPortName;
			strException += "\" was found in nameserver but could not reached (dead?)";
			throw strException;
		}

		//as client, check upon us and the server
		if( connectionMode == CYarpCommBase::Client )
		{
			isok = isPortRegistered( strPortNameServer );
			if( !isok ) {
				strException = "A port named \"";
				strException += strPortNameServer;
				strException += "\" was not found in nameserver";
				throw strException;
			}

			isok = isPortAlive( strPortNameServer );
			if( !isok ){
				strException = "The port named \"";
				strException += strPortNameServer;
				strException += "\" was found in nameserver but could not be reached (is peer dead?)";
				throw strException;
			}


			isok = yarp::os::Network::isConnected( strPortName.c_str(), strPortNameServer.c_str() );
			if( !isok ) {
				strException = "The ports are ok, but the connection \"";
				strException += strPortName;
				strException += "\" -> \"";
				strException += strPortNameServer;
				strException += "\" is invalid";
				throw strException;
			}

			isok = yarp::os::Network::isConnected( strPortNameServer.c_str(), strPortName.c_str() );
			if( !isok ) {
				strException = "The ports are ok, but the connection \"";
				strException += strPortNameServer;
				strException += "\" -> \"";
				strException += strPortName;
				strException += "\" is invalid";
				throw strException;
			}
		}

		//if we got here, everything is ok
		isok = true;
	}
	catch( std::string &str ) {
		std::cout << "Exception: " << str << "." << std::endl;
		strError = str;
	}

	return isok;
}

void CYarpCommBase::close( void )
{
	if( _bOpened )
	{
		if( connectionMode == CYarpCommBase::Client )
		{
			yarp::os::Network::disconnect( port.getName(), strPortNameServer.c_str() );
			yarp::os::Network::disconnect( strPortNameServer.c_str(), port.getName() );
		}
		port.interrupt();
		port.close();
		yarp::os::Network::unregisterName( port.getName() );
	
		_bOpened = false;
	}
	
	Fini();
}

bool CYarpCommBase::read( yarp::os::ConnectionReader &connection )
{
	return false;
}

bool CYarpCommBase::Send( unsigned int command, bool bReceiveReply )
{
	return Send( CMessage( command ), bReceiveReply );
}

bool CYarpCommBase::Send( CMessage &request, bool bReceiveReply )
{
	return false;
}

bool CYarpCommBase::isPortRegistered( const std::string port_name )
{
	yarp::os::impl::NameClient& name_client = yarp::os::impl::NameClient::getNameClient();

	YARP_Address address = name_client.queryName( port_name.c_str() );

	return address.isValid();
}

bool CYarpCommBase::isPortAlive( const std::string port_name )
{
	yarp::os::impl::NameClient& name_client = yarp::os::impl::NameClient::getNameClient();

	YARP_Address address = name_client.queryName( port_name.c_str() );

	YARP_Address address2(address);

	address2.setTimeout(2.0f);

	bool bRes = false;
	if( address.isValid() )
	{

		YARP_OutputProtocol *out = yarp::os::impl::Carriers::connect( address2 );

		if( !out )
			bRes = false;
		else
		{
			bRes = true;
			out->interrupt();
			out->close();
			delete out;
			out = nullptr;
		}
	}

	return bRes;
}



bool CYarpCommBase::Init( void )
{
	// All code "here" (in the inherited). This function is called into open() function in the base class.
	return true;
}

void CYarpCommBase::Fini( void )
{
	// All code "here" (in the inherited). This function is called into open() function in the base class.
}

bool CYarpCommBase::isOpened( void )
{
	return _bOpened;
}

bool CYarpCommBase::set_PortName( std::string port_name )
{
	if( isOpened() ) return false;

	strPortName = port_name;

	return true;
}

bool CYarpCommBase::set_PortNameServer( std::string port_name_server )
{
	if( isOpened() ) return false;

	strPortNameServer = port_name_server;

	return true;
}

std::string CYarpCommBase::get_PortName( void )
{
	return strPortName;
}

std::string CYarpCommBase::get_PortNameServer( void )
{
	return ( connectionMode == CYarpCommBase::Server ) ? strPortName : strPortNameServer;
}

std::string CYarpCommBase::get_ErrorString( void )
{
	return strError;
}
