#include "serial_asio.h"

#include <boost/asio/streambuf.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/bind.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost\bind\placeholders.hpp>

yarp::dev::serial_asio::serial_asio( void )
{
	io_work.reset( new boost::asio::io_service::work( io_service ) );
	thrd_io.reset( new boost::thread( boost::bind( &yarp::dev::serial_asio::io_service_runner, this ) ) );
	dead_line_timer.reset( new boost::asio::deadline_timer( io_service ) );
	Serial_Port.reset( new boost::asio::serial_port( io_service ) );
}

yarp::dev::serial_asio::~serial_asio()
{
	//io_service.stop();
	dead_line_timer->cancel();
	io_work->get_io_service().stop();
	thrd_io->join();
}

void yarp::dev::serial_asio::checkdeadline(const boost::system::error_code &ec)
{
	if (ec == boost::asio::error::operation_aborted)
	{	//this will happen each expires_from_now or expires_at is called. (and there are pending waits)
	}
	else if (ec)
	{
		//this should not happen, so we print the error and do not resubmit the async_wait
		std::cerr << "Serial port: " << ec.message() << std::endl;
		return;
	}
	else
	{
		if (dead_line_timer->expires_at() <= boost::asio::deadline_timer::traits_type::now())
		{
			io_service.post(
				[&]
			{
				try
				{
					Serial_Port->cancel();
				}
				catch (boost::system::system_error &ex)
				{
					std::cerr << "Serial port: " << ex.what() << std::endl;
				}
			});
			dead_line_timer->expires_at(boost::posix_time::pos_infin);
		}
	}
	
	dead_line_timer->async_wait( boost::bind( &yarp::dev::serial_asio::checkdeadline, this, _1 ) );
}

void yarp::dev::serial_asio::io_service_runner()
{
	try
	{
		io_service.run();
	}
	catch(boost::system::system_error &ex)
	{
		std::cerr << "Serial port runner: " << ex.what() << std::endl;
	}
}

bool yarp::dev::serial_asio::open( yarp::os::Searchable &config )
{
	std::string 
		sCom = config.check( "com", yarp::os::Value( "COM1" ), "Com Port: COM1, COM2, COM3, COM4" ).asString(),
		sParity = config.check( "parity", yarp::os::Value( "none" ), "Parity: none, even, odd" ).asString();

	int 
		nBaudRate = config.check( "baud", yarp::os::Value( 38400 ), "BaudRate: 600, 1200, 2400, 4800, 9600, 19200, 38400" ).asInt(),
		nDataBits = config.check( "databits", yarp::os::Value( 8 ), "Data bits: 5, 6, 7, 8" ).asInt();
		
	float fStopBits = static_cast<float>( config.check( "stopbits", yarp::os::Value( 1.0 ), "Stop bits: 1, 1.5, 2" ).asDouble() );
	
	return open( sCom, nBaudRate, nDataBits, fStopBits, sParity );
}

bool yarp::dev::serial_asio::open( const std::string strCom, int baud_rate, int data_bits, float stop_bits, const std::string parity)
{
	boost::system::error_code ec;

	Serial_Port->open( strCom, ec );

	if( ec )
	{
		std::cerr << ec.message() << std::endl;
		return false;
	}

	Serial_Port->set_option( boost::asio::serial_port_base::baud_rate( baud_rate ), ec );

	if( ec )
	{
		std::cerr << ec.message() << std::endl;
		Serial_Port->close();
		return false;
	}

	Serial_Port->set_option( boost::asio::serial_port_base::character_size( data_bits ) , ec );
	
	if( ec )
	{
		std::cerr << ec.message() << std::endl;
		Serial_Port->close();
		return false;
	}

	if( stop_bits == 2.0f )
	{
		Serial_Port->set_option( boost::asio::serial_port_base::stop_bits( boost::asio::serial_port_base::stop_bits::two ), ec );
	}
	else if( stop_bits == 1.5f )
	{
		Serial_Port->set_option( boost::asio::serial_port_base::stop_bits( boost::asio::serial_port_base::stop_bits::onepointfive ), ec );
	}
	else
	{
		Serial_Port->set_option( boost::asio::serial_port_base::stop_bits( boost::asio::serial_port_base::stop_bits::one ), ec );
	}

	if( ec )
	{
		std::cerr << ec.message() << std::endl;
		Serial_Port->close();
		return false;
	}

	if( parity.compare( "even" ) == 0 )
	{
		Serial_Port->set_option( boost::asio::serial_port_base::parity( boost::asio::serial_port_base::parity::even ), ec );
	}
	else if( parity.compare( "odd" ) == 0 )
	{
		Serial_Port->set_option( boost::asio::serial_port_base::parity( boost::asio::serial_port_base::parity::odd ), ec );
	}
	else
	{
		// none
		Serial_Port->set_option( boost::asio::serial_port_base::parity( boost::asio::serial_port_base::parity::none ), ec );
	}

	dead_line_timer->expires_at(boost::posix_time::pos_infin);
	dead_line_timer->async_wait( boost::bind( &yarp::dev::serial_asio::checkdeadline, this, _1 ) );

	if( ec )
	{
		std::cerr << ec.message() << std::endl;
		Serial_Port->close();
		return false;
	}

	return true;
}

bool yarp::dev::serial_asio::close()
{
	dead_line_timer->cancel();
	Serial_Port->close();

	return true;
}

bool yarp::dev::serial_asio::Write( const std::string strData )
{
	boost::asio::streambuf write_buffer;

	write_buffer.sputn( strData.c_str(), strData.size() );

	boost::system::error_code ec;

	boost::asio::write( *Serial_Port, write_buffer, ec );

	if( ec )
	{
		// Issue an error
		std::cerr << ec.message() << std::endl;
		return false;
	}

	return true;
}

int yarp::dev::serial_asio::Read_Wait_String( std::string &strData, const std::string waitString, int timeout_ms )
{	
	using boost::lambda::var;
	using boost::lambda::_1;
	using boost::lambda::_2;

	boost::asio::streambuf input_buffer;
	boost::system::error_code ec = boost::asio::error::would_block;
	std::size_t bytes_transfered = 0;
	//dead_line_timer->async_wait( boost::bind( &yarp::dev::serial_asio::checkdeadline, this ) );
	dead_line_timer->expires_from_now( boost::posix_time::milliseconds( timeout_ms ) );

	 // Start the asynchronous operation itself. The boost::lambda function
    // object is used as a callback and will update the ec variable when the
    // operation completes. The blocking_udp_client.cpp example shows how you
    // can use boost::bind rather than boost::lambda.

	io_service.post(
		[this, &ec, &bytes_transfered, &input_buffer,waitString]
	{
		boost::asio::async_read_until( *Serial_Port, input_buffer, waitString, (boost::lambda::var(ec) = _1, boost::lambda::var(bytes_transfered) = _2) );
	});
	// Block until the asynchronous operation has completed.
	do boost::this_thread::yield(); while (ec == boost::asio::error::would_block);

	 //// Block until the asynchronous operation has completed.
  //  do io_service.run_one(); while (ec == boost::asio::error::would_block);

	dead_line_timer->expires_at( boost::posix_time::pos_infin );
	
	if( ec == boost::asio::error::operation_aborted )
	{
		std::cerr << "Timeout occurred during serial port read operation." << std::endl;
		return 0;
	}
	else if(ec)
	{
		std::cerr << "Unspecified error ocurred during serial port read: " << ec.message() << std::endl;
		return 0;
	}
	

	// Read the stream this way was not working as expected,
	// so it was replaced by the code bellow
	/*std::istream serial_input_stream( &input_buffer );
	serial_input_stream >> strData;*/

	char * reply = new (std::nothrow) char[bytes_transfered+1];

	if( reply != nullptr )
	{
		std::streamsize strm_size = input_buffer.sgetn( reply, bytes_transfered );
		
		reply[bytes_transfered]='\0';
		
		strData = reply;

		delete[] reply;
		reply = nullptr;
	}
	else
		std::cerr << "Allocation of memory for serial port buffer failed!" << std::endl;

	return bytes_transfered+1;
}

int yarp::dev::serial_asio::Read_Wait_Character( std::string &strData, const char waitChar, int timeout_ms )
{
	using boost::lambda::var;
	using boost::lambda::_1;
	using boost::lambda::_2;

	boost::asio::streambuf input_buffer;
	boost::system::error_code ec = boost::asio::error::would_block;
	std::size_t bytes_transfered = 0;

//	dead_line_timer->async_wait( boost::bind( &yarp::dev::serial_asio::checkdeadline, this ) );
	dead_line_timer->expires_from_now( boost::posix_time::milliseconds( timeout_ms ) );

	boost::asio::async_read_until( *Serial_Port, input_buffer, waitChar, (var(ec) = _1, var(bytes_transfered) = _2) );

	 // Block until the asynchronous operation has completed.
    do io_service.run_one(); while (ec == boost::asio::error::would_block);

	dead_line_timer->expires_at( boost::posix_time::pos_infin );

	if( ec == boost::asio::error::operation_aborted )
	{
		std::cerr << "Timeout occurred during serial port read operation." << std::endl;
		return 0;
	}
	else if(ec)
	{
		std::cerr << "Unspecified error ocurred during serial port read: " << ec.message() << std::endl;
		return 0;
	}

	// Read the stream this way was not working as expected,
	// so it was replaced by the code bellow
	/*std::istream serial_input_stream( &input_buffer );
	serial_input_stream >> strData;*/

	char * reply = new (std::nothrow) char[bytes_transfered+1];

	if( reply != nullptr )
	{
		std::streamsize strm_size = input_buffer.sgetn( reply, bytes_transfered );
		
		reply[bytes_transfered]='\0';
		
		strData = reply;

		delete[] reply;
		reply = nullptr;
	}
	else
		std::cerr << "Allocation of memory for serial port buffer failed!" << std::endl;

	return bytes_transfered+1;
}

boost::shared_ptr< boost::asio::serial_port > yarp::dev::serial_asio::GetSerialPortDevice()
{
	return Serial_Port;
}