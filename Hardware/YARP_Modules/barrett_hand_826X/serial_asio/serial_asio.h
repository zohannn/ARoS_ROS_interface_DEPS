#ifndef _SERIAL_ASIO_H
#define _SERIAL_ASIO_H

#include <yarp/dev/DeviceDriver.h>

#include <string>

#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>

namespace yarp
{
	namespace dev
	{
		class serial_asio;
	}
}

class yarp::dev::serial_asio 
	: public yarp::dev::DeviceDriver
{
public:
	serial_asio( void );
	~serial_asio( void );

	boost::asio::io_service io_service;
	boost::shared_ptr< boost::thread > thrd_io;
	boost::shared_ptr< boost::asio::io_service::work > io_work;
	boost::shared_ptr< boost::asio::deadline_timer > dead_line_timer;
	boost::shared_ptr< boost::asio::serial_port > Serial_Port;

	//TODO: change back to private
public:
	void checkdeadline(const boost::system::error_code &ec);

private:
	void io_service_runner();

	//----------------------------------------------------------------------
	// Device Driver implementation, specific functions
	// Virtual functions from DeviceDriver class
public:
	bool open( yarp::os::Searchable& config );

	bool close( void );

	//----------------------------------------------------------------------
	// Device Driver implementation, specific functions
	/*
	 *	@param Com: Com port name
	 *	@param baud_rate: baud rate for communications
	 *	@param data_bits: 5, 6, 7 or 8 bits 
	 *	@param stop_bits: 1, 1.5 or 2 stop bits
	 *	@param parity: "none", "even","odd"
	 */
	bool open( const std::string strCom, int baud_rate, int data_bits=8, float stop_bits=1, const std::string parity="none" );

	//----------------------------------------------------------------------

	/** Reads data from the serial port until get specified character or timeout occur
	 * @param pData - pointer to store data
	 * @param waitChar - character to wait for
	 * @param timeout - specifies the time to wait before returning from read. Negative value
	 *  means infinite timeout.
	 * @return number of bytes actualy read
	 */
	int Read_Wait_Character( std::string &strData, const char waitChar, int timeout_ms );

	/** Reads data from the serial port until get specified character or timeout occur
	 * when @param waitString as not been received, but lenght of stored data in 
	 * internal buffer is equal or bigger then @param nReadBytes, then that data is returned
	 * @param pData - pointer to store data
	 * @param waitString - string to wait for (no need to be null terminated!)
	 * @param timeout - specifies the time to wait before returning from read. Negative value
	 *  means infinite timeout.
	 * @return number of bytes actualy read
	 */
	int Read_Wait_String( std::string &strData, const std::string waitString, int timeout_ms );


	/** Writes data to the serial port
	 * @param pData - pointer to data to be written. It must be a null terminated string
	 * @return: true/false on success/failure
	 */
	bool Write( const std::string strData );

	boost::shared_ptr< boost::asio::serial_port > GetSerialPortDevice();
};

#endif // _SERIAL_ASIO_H
