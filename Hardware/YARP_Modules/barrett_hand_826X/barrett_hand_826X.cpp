#include "barrett_hand_826X.h"
#include <yarp/dev/PolyDriver.h>
#include <yarp/os/Property.h>
#include <yarp/os/Time.h>

#include <boost\array.hpp>
#include <boost\regex.hpp>
#include <boost\asio.hpp>
#include <boost\asio\write.hpp>
#include <boost\lambda\lambda.hpp>
#include <boost\lexical_cast.hpp>

#include <sstream>
#include <cmath>

#ifndef DEG_TO_RAD_F
#define DEG_TO_RAD_F 0.017453292519943295769236907684886f
#endif

#ifndef RAD_TO_DEG_F
#define RAD_TO_DEG_F 57.295779513082320876798154814105f
#endif


//------------------------------------------------------------------------------------

yarp::dev::BarrettHand826X::BarrettHand826X( void )
: strStop( "=> " )
{
	BHand_Finger_ID[ BarrettHand::Finger::Spread ] = 'S';
	BHand_Finger_ID[ BarrettHand::Finger::One ] = '1';
	BHand_Finger_ID[ BarrettHand::Finger::Two ] = '2';
	BHand_Finger_ID[ BarrettHand::Finger::Three ] = '3';

	BHand_Device_2_Module_Idx[0] = BarrettHand::Finger::One;
	BHand_Device_2_Module_Idx[1] = BarrettHand::Finger::Two;
	BHand_Device_2_Module_Idx[2] = BarrettHand::Finger::Three;
	BHand_Device_2_Module_Idx[3] = BarrettHand::Finger::Spread;

	BHand_SetParam_Ratio[ BarrettHand::Finger::Spread ] = BHAND_RATIO_3150_180;
	BHand_SetParam_Ratio[ BarrettHand::Finger::One ] = BHAND_RATIO_17800_140;
	BHand_SetParam_Ratio[ BarrettHand::Finger::Two ] = BHAND_RATIO_17800_140;
	BHand_SetParam_Ratio[ BarrettHand::Finger::Three ] = BHAND_RATIO_17800_140;

	BHand_GetParam_Ratio[ BarrettHand::Finger::Spread ] = BHAND_RATIO_180_3150;
	BHand_GetParam_Ratio[ BarrettHand::Finger::One ] = BHAND_RATIO_140_17800;
	BHand_GetParam_Ratio[ BarrettHand::Finger::Two ] = BHAND_RATIO_140_17800;
	BHand_GetParam_Ratio[ BarrettHand::Finger::Three ] = BHAND_RATIO_140_17800;

	rt_mode_on = false;
}

bool yarp::dev::BarrettHand826X::open( std::string cCom, long lBaudRate, double dCTfingers, double dCTspread, double dOTfingers, double dOTspread, double dSpeedfingers, double dSpeedspread, double dAccelfingers, double dAccelspread, long lMaxStrainfingers, long lMaxStrainspread, long iInitialize)
{	
	yarp::os::Property prop;
	
	prop.put( "device", "serial_asio" );
	prop.put( "com", cCom.c_str() );
	prop.put( "baud", lBaudRate );
	prop.put( "databits", 8 );
	prop.put( "stopbits", 1 );
	prop.put( "parity", "none" );
	
	if( !serial_port.open( prop ) ) return false;
	
	if( iInitialize==1 )
	{
		if( !Initialize() ) return false;
	}
	//-----------------------------------------------------------------------------
	// Open target
	double refs[4] = { dOTspread, dOTfingers, dOTfingers, dOTfingers };

	if( !setOpenTarget( refs ) ) return false;
		
	//-----------------------------------------------------------------------------
	// Close target
	refs[0] = dCTspread;
	refs[1] = dCTfingers;
	refs[2] = dCTfingers;
	refs[3] = dCTfingers;
	
	if( !setCloseTarget( refs ) ) return false;
		
	//-----------------------------------------------------------------------------
	// Finger velocity
	refs[0] = dSpeedspread;
	refs[1] = dSpeedfingers;
	refs[2] = dSpeedfingers;
	refs[3] = dSpeedfingers;
	
	if( !setRefSpeeds( refs ) ) return false;
		
	//-----------------------------------------------------------------------------
	// Finger acceleration
	refs[0] = dAccelspread;
	refs[1] = dAccelfingers;
	refs[2] = dAccelfingers;
	refs[3] = dAccelfingers;
	
	if( !setRefAccelerations( refs ) ) return false;

	//-----------------------------------------------------------------------------
	// Finger strain
	int lrefs[4] = { lMaxStrainspread, lMaxStrainfingers, lMaxStrainfingers, lMaxStrainfingers };
	
	if( !setMaxStrain( lrefs ) ) return false;
	
	//-----------------------------------------------------------------------------
	//double refs_pos[4];

	//refs_pos[0]=0;
	//refs_pos[1]=45;
	//refs_pos[2]=45;
	//refs_pos[3]=45;

	return positionMoveFingers(45.0);

	//return OpenHand();
}


bool yarp::dev::BarrettHand826X::open( yarp::os::Searchable& config )
{
	std::string sCom = config.check( "com", yarp::os::Value(1), "Com Port: COM1, COM2, COM3, COM4" ).asString();
	
	long 
		lBaud = static_cast<long>( config.check( "baud", yarp::os::Value(9600), "Baud Rate: 600, 1200, 2400, 4800, 9600, 19200, 38400").asInt() ),
		lMaxStrainfingers = static_cast<long>( config.check( "maxstrainfingers", yarp::os::Value(255), "Max Strain Fingers: 0-256").asInt() ),
		lMaxStrainspread = static_cast<long>( config.check( "maxstrainspread", yarp::os::Value(255), "Max Strain Spread: 0-256").asInt() ),
		iInitialize = static_cast<long>( config.check( "initialize", yarp::os::Value(255), "Initialize: 0-1").asInt() );

	double 
		dCTfingers = config.check( "ctfingers", yarp::os::Value(140.0), "Close Target: 0-140�").asDouble(),
		dCTspread = config.check( "ctspread", yarp::os::Value(180.0), "Close Target: 0-180�").asDouble(),
		dOTfingers = config.check( "otfingers", yarp::os::Value(0.0), "Open Target: 0-140�").asDouble(),
		dOTspread = config.check( "otspread", yarp::os::Value(0.0), "Open Target: 0-180�").asDouble(),
		dSpeedfingers = config.check( "speedfingers", yarp::os::Value(40.0), "Speed Fingers: 16-100").asDouble(),
		dSpeedspread = config.check( "speedspread", yarp::os::Value(30.0), "Speed Spread: 16-60").asDouble(),
		dAccelfingers = config.check( "accelfingers", yarp::os::Value(4.0), "Acceleration Fingers: 1-10").asDouble(),
		dAccelspread = config.check( "accelspread", yarp::os::Value(2.0), "Acceleration Spread: 1-10").asDouble();

	return open( sCom, lBaud, dCTfingers, dCTspread, dOTfingers, dOTspread, dSpeedfingers, dSpeedspread, dAccelfingers, dAccelspread, lMaxStrainfingers, lMaxStrainspread, iInitialize );
}

bool yarp::dev::BarrettHand826X::close( void )
{
	return serial_port.close();
}

bool yarp::dev::BarrettHand826X::getAxes( int *ax )
{
	*ax = getAxes();
	return true;
}

int yarp::dev::BarrettHand826X::getAxes( void )
{
	return BarrettHand::Finger::BARRETT_HAND_NUMBER_OF_DOF;
}

bool yarp::dev::BarrettHand826X::isValidAxis( int axis )
{
	return ( ( axis >= 0 ) && ( axis < getAxes() ) );
}

// Real Time (RT) mode functions

bool yarp::dev::BarrettHand826X::startRTmode()
{
	if(rt_mode_on)
		return true;
	try
	{
		boost::shared_ptr<boost::asio::serial_port> serial_dev;
		
		boost::asio::streambuf in_buf; std::string in_str; 
		std::string err("ERR");

		size_t bytes_transfered;

		//get the low level device.
		serial_dev = serial_port.GetSerialPortDevice();


		//Configure for RT mode:
		// Send: all motors velocity and control proportional gain (1 signed byte for each )
		// Receive: all motors absolute position  ( 2 unsigned bytes for each) and velocities (1 signed byte for each )

		std::string set_fingers;
		//F1
		set_fingers = "1FSET LCV 1 LCVC 1 LCPG 1 LFV 1 LFVC 1 LFS 0 LFAP 1 LFDP 0 LFDPC 1\r";
		boost::asio::write(*serial_dev,boost::asio::buffer(set_fingers.c_str(),set_fingers.size()));
		bytes_transfered = boost::asio::read_until(*serial_dev, in_buf, "=> ");	
		in_str.resize(bytes_transfered);
		in_buf.sgetn(&in_str.front(), bytes_transfered);
		if(in_str.find(err)!=std::string::npos)
			throw std::string("Error received from BarretHand. Controller in now running in Supervisory mode.");
		in_buf.consume(bytes_transfered);	

		//F2
		set_fingers = "2FSET LCV 1 LCVC 1 LCPG 1 LFV 1 LFVC 1 LFS 0 LFAP 1 LFDP 0 LFDPC 1\r";
		boost::asio::write(*serial_dev,boost::asio::buffer(set_fingers.c_str(),set_fingers.size()));
		bytes_transfered = boost::asio::read_until(*serial_dev, in_buf, "=> ");	
		in_str.resize(bytes_transfered);
		in_buf.sgetn(&in_str.front(), bytes_transfered);
		if(in_str.find(err)!=std::string::npos)
			throw std::string("Error received from BarretHand. Controller in now running in Supervisory mode.");
		in_buf.consume(bytes_transfered);

		//F3
		set_fingers = "3FSET LCV 1 LCVC 1 LCPG 1 LFV 1 LFVC 1 LFS 0 LFAP 1 LFDP 0 LFDPC 1\r";
		boost::asio::write(*serial_dev,boost::asio::buffer(set_fingers.c_str(),set_fingers.size()));
		bytes_transfered = boost::asio::read_until(*serial_dev, in_buf, "=> ");	
		in_str.resize(bytes_transfered);
		in_buf.sgetn(&in_str.front(), bytes_transfered);
		if(in_str.find(err)!=std::string::npos)
			throw std::string("Error received from BarretHand. Controller in now running in Supervisory mode.");
		in_buf.consume(bytes_transfered);

		//spread
		std::string set_spread("4FSET LCV 1 LCVC 1 LCPG 1 LFV 1 LFVC 1 LFS 0 LFAP 1 LFDP 0 LFDPC 1\r");
		boost::asio::write(*serial_dev,boost::asio::buffer(set_spread.c_str(),set_spread.size()));
		bytes_transfered = boost::asio::read_until(*serial_dev, in_buf, "=> ");
		in_str.resize(bytes_transfered);
		in_buf.sgetn(&in_str.front(), bytes_transfered);
		if(in_str.find(err)!=std::string::npos)
			throw std::string("Error received from BarretHand. Controller in now running in Supervisory mode.");
		in_buf.consume(bytes_transfered);

		//Send a Loop command
		std::string set_loop("1234LOOP\r");
		boost::asio::write(*serial_dev,boost::asio::buffer(set_loop.c_str(),set_loop.size()));
		bytes_transfered = boost::asio::read_until(*serial_dev, in_buf, "*");
		in_str.resize(bytes_transfered);
		in_buf.sgetn(&in_str.front(), bytes_transfered);
		if(in_str.find(err)!=std::string::npos)
			throw std::string("Error received from BarretHand. Controller in now running in Supervisory mode.");
		in_buf.consume(bytes_transfered);

	}
	catch(std::string const& ex)
	{
		std::cout <<  ex << std::endl;
		rt_mode_on = false;
		return true;
	}

	rt_mode_on = true;

	return rt_mode_on;
}

bool yarp::dev::BarrettHand826X::stopRTmode()
{
	if(!rt_mode_on)
		return false;
	try
	{
		boost::shared_ptr<boost::asio::serial_port> serial_dev;
		boost::asio::streambuf out_buf;
		boost::asio::streambuf in_buf; std::string in_str; 
		std::string err("ERR");

		//get the low level device.
		serial_dev = serial_port.GetSerialPortDevice();

		out_buf.sputn("\x03", 1);
		boost::asio::write(*serial_dev, out_buf);
		size_t bytes_transfered = boost::asio::read_until(*serial_dev, in_buf, "=> ");
		in_str.resize(bytes_transfered);
		in_buf.sgetn(&in_str.front(), bytes_transfered);
		//if(in_str.find(err)!=std::string::npos)
			//throw std::string("Error received from BarretHand. Controller in now running in Supervisory mode.");
		in_buf.consume(bytes_transfered);
	}
	catch(std::string const& ex)
	{
		std::cout <<  ex << std::endl;
		rt_mode_on = true;
		return false;
	}

	rt_mode_on = false;

	return !rt_mode_on;
}

bool yarp::dev::BarrettHand826X::getRTVelocities(double *vel)
{
	try
	{
		boost::shared_ptr<boost::asio::serial_port> serial_dev;
		boost::asio::streambuf out_buf;
		boost::asio::streambuf in_buf; std::string in_str;
		std::string err("ERR");
		size_t bytes_transfered = 0;

		//get the low level device.
		serial_dev = serial_port.GetSerialPortDevice();

		// send the command to receive a feedback
		//   F1    F2    F3    S
		// 1 + 2 1 + 2 1 + 2 1 + 2 = 12 bytes
		// v + p v + p v + p v + p
		//expression to wait for
		//either *xxxxxxxx or \n\rERR xxxx
		boost::regex rt_feedback_reg("\\*.{12}|\\n\\rERR (\\d+)\\n\\r=> ");
		out_buf.sputn("A\r", 1);
		boost::asio::write(*serial_dev, out_buf);
		bytes_transfered = boost::asio::read_until(*serial_dev, in_buf, rt_feedback_reg);	
		in_str.resize(bytes_transfered);
		in_buf.sgetn(&in_str.front(), bytes_transfered);
		//if(in_str.find(err)!=std::string::npos)
			//throw std::string("Error received from BarretHand. Controller in now running in Supervisory mode.");
	
		int curr_pos[4]; int curr_vel[4];
		// F1
		curr_vel[0] = static_cast<int>(in_str.at(1));
		vel[0] = static_cast<double>(curr_vel[0]*BHAND_RATIO_140_17800*BHAND_RATIO_N_sec);// to deg/sec
		curr_pos[0] = static_cast<int>(in_str.at(2) << 8) + static_cast<int>((unsigned char)in_str.at(3));
		// F2
		curr_vel[1] = static_cast<int>(in_str.at(4));
		curr_pos[1] = static_cast<int>(in_str.at(5) << 8) + static_cast<int>((unsigned char)in_str.at(6));
		vel[1] = static_cast<double>(curr_vel[1]*BHAND_RATIO_140_17800*BHAND_RATIO_N_sec); // to deg/sec
		// F3
		curr_vel[2] = static_cast<int>(in_str.at(7));
		curr_pos[2] = static_cast<int>(in_str.at(8) << 8) + static_cast<int>((unsigned char)in_str.at(9));
		vel[2] = static_cast<double>(curr_vel[2]*BHAND_RATIO_140_17800*BHAND_RATIO_N_sec); // to deg/sec
		// S
		curr_vel[3] = static_cast<int>(in_str.at(10));
		curr_pos[3] = static_cast<int>(in_str.at(11) << 8) + static_cast<int>((unsigned char)in_str.at(12));
		vel[3] = static_cast<double>(curr_vel[3]*BHAND_RATIO_180_3150*BHAND_RATIO_N_sec); // to deg/sec

		// from F1 F2 F3 S
		//swap to   S F1 F2 F3
		double temp;
		temp = vel[3];
		vel[3] = vel[2];
		vel[2] = vel[1];
		vel[1] = vel[0];
		vel[0] = temp;

		//std::cout << "vel: " << vel[1] << std::endl;


	}
	catch(std::string &ex){

		std::cout << "Hand error in RT mode: " << ex << std::endl;
		return false;
	}


	return true;
}

bool yarp::dev::BarrettHand826X::getRTPositions(double *pos)
{
	try
	{
		boost::shared_ptr<boost::asio::serial_port> serial_dev;
		boost::asio::streambuf out_buf;
		boost::asio::streambuf in_buf; std::string in_str;
		std::string err("ERR");
		size_t bytes_transfered = 0;

		//get the low level device.
		serial_dev = serial_port.GetSerialPortDevice();

		// send the command to receive a feedback
		//   F1    F2    F3    S
		// 1 + 2 1 + 2 1 + 2 1 + 2 = 12 bytes
		// v + p v + p v + p v + p
		//expression to wait for
		//either *xxxxxxxx or \n\rERR xxxx
		boost::regex rt_feedback_reg("\\*.{12}|\\n\\rERR (\\d+)\\n\\r=> ");
		out_buf.sputn("A\r", 1);
		boost::asio::write(*serial_dev, out_buf);
		bytes_transfered = boost::asio::read_until(*serial_dev, in_buf, rt_feedback_reg);	
		in_str.resize(bytes_transfered);
		in_buf.sgetn(&in_str.front(), bytes_transfered);
		//if(in_str.find(err)!=std::string::npos)
			//throw std::string("Error received from BarretHand. Controller in now running in Supervisory mode.");
	

		int curr_pos[4]; int curr_vel[4];

		// F1
		curr_vel[0] = static_cast<int>(in_str.at(1));
		curr_pos[0] = static_cast<int>(in_str.at(2) << 8) + static_cast<int>((unsigned char)in_str.at(3));
		// F2
		curr_vel[1] = static_cast<int>(in_str.at(4));
		curr_pos[1] = static_cast<int>(in_str.at(5) << 8) + static_cast<int>((unsigned char)in_str.at(6));
		// F3
		curr_vel[2] = static_cast<int>(in_str.at(7));
		curr_pos[2] = static_cast<int>(in_str.at(8) << 8) + static_cast<int>((unsigned char)in_str.at(9));
		// S
		curr_vel[3] = static_cast<int>(in_str.at(10));
		curr_pos[3] = static_cast<int>(in_str.at(11) << 8) + static_cast<int>((unsigned char)in_str.at(12));


		pos[0] = static_cast<double>(curr_pos[0])*BHAND_RATIO_140_17800; // to deg
		pos[1] = static_cast<double>(curr_pos[1])*BHAND_RATIO_140_17800; // to deg
		pos[2] = static_cast<double>(curr_pos[2])*BHAND_RATIO_140_17800; // to deg
		pos[3] = static_cast<double>(curr_pos[3])*BHAND_RATIO_180_3150; // to deg // S

		// from F1 F2 F3 S
		//swap to   S F1 F2 F3
		double temp;
		temp = pos[3];
		pos[3] = pos[2];
		pos[2] = pos[1];
		pos[1] = pos[0];
		pos[0] = temp;

		//std::cout << "pos: " << pos[1] << std::endl;

	}
	catch(std::string &ex){

		std::cout << "Hand error in RT mode: " << ex << std::endl;
		return false;
	}


	return true;
}

bool yarp::dev::BarrettHand826X::setRTVelocities(const std::vector<double>& vel,double* pos,double* vel_out)
{
	// vel is in rad/sec
	//std::cout << "vel: " << vel.at(1) << std::endl;
	try
	{
		boost::shared_ptr<boost::asio::serial_port> serial_dev;
		//boost::asio::streambuf out_buf;
		boost::asio::streambuf in_buf; 
		std::string in_str;
		std::string err("ERR");
		size_t bytes_transfered=0;
		//expression to wait for
		//either *xxxxxxxx or \n\rERR xxxx
		boost::regex rt_feedback_reg("\\*.{12}|\\n\\rERR (\\d+)\\n\\r=> ");

		//get the low level device.
		serial_dev = serial_port.GetSerialPortDevice();

		// send the command to set the velocities
		//   F1   F2  F3  S
		//   1+1 1+1 1+1 1+1  = 8 bytes
		boost::array<char,9> out_buffer;
		out_buffer[0]='C';

		// send the command to set the velocities
		//   F1   F2  F3  S
		//   1	1 1 1  = 4 bytes
		//double vel_max = 20; // deg/sec

		
		char ref_vel[8];
		// velocity value + proportional gain
		//if(vel.at(1)*RAD_TO_DEG_F < 0.0 && vel.at(1)*RAD_TO_DEG_F > -vel_max)
			//ref_vel[0] = static_cast<int>(std::ceil(-vel_max*BHAND_RATIO_17800_140));
		//else
		ref_vel[0] = static_cast<int>(std::ceil(vel.at(1)*RAD_TO_DEG_F*BHAND_RATIO_CVel_degs)); 		
		//ref_vel[1] = getGainVelocity(vel.at(1)*RAD_TO_DEG_F);
		ref_vel[1] = 80; 
		//if(vel.at(2)*RAD_TO_DEG_F < 0.0 && vel.at(2)*RAD_TO_DEG_F > -vel_max)
			//ref_vel[2] = static_cast<int>(std::ceil(-vel_max*BHAND_RATIO_17800_140)); 
		//else
		ref_vel[2] = static_cast<int>(std::ceil(vel.at(2)*RAD_TO_DEG_F*BHAND_RATIO_CVel_degs)); 
		//if (vel.at(2) <= 0)
		//	ref_vel[3] = getGainVelocity(vel.at(2)*RAD_TO_DEG_F)+7; 
		//else
		//ref_vel[3] = getGainVelocity(vel.at(2)*RAD_TO_DEG_F); 
		ref_vel[3] = 100;
		//if(vel.at(3)*RAD_TO_DEG_F < 0.0 && vel.at(3)*RAD_TO_DEG_F > -vel_max)
			//ref_vel[4] = static_cast<int>(std::ceil(-vel_max*BHAND_RATIO_17800_140));
		//else
		ref_vel[4] = static_cast<int>(std::ceil(vel.at(3)*RAD_TO_DEG_F*BHAND_RATIO_CVel_degs)); 
		//ref_vel[5] = getGainVelocity(vel.at(3)*RAD_TO_DEG_F); 
		ref_vel[5] = 80; 
		//ref_vel[6] = static_cast<int>(std::ceil(vel.at(0)*RAD_TO_DEG_F*BHAND_RATIO_17800_140/1000)); ref_vel[7] = 30; //S //TO DO Gain control
		ref_vel[6] = static_cast<int>(std::ceil(0.0*RAD_TO_DEG_F*BHAND_RATIO_CVel_degs)); 
		ref_vel[7] = 70; //S 
			
		//ref_vel[0] = static_cast<int>(std::ceil(vel.at(1)*RAD_TO_DEG_F)); //ref_vel[1] = 40; 
		//ref_vel[1] = getGainVelocity(vel.at(1)*RAD_TO_DEG_F); //F1
		//ref_vel[2] = static_cast<int>(std::ceil(vel.at(2)*RAD_TO_DEG_F)); //ref_vel[3] = 40; 
		//ref_vel[3] = getGainVelocity(vel.at(2)*RAD_TO_DEG_F)+5; //F2
		//ref_vel[4] = static_cast<int>(std::ceil(vel.at(3)*RAD_TO_DEG_F)); //ref_vel[5] = 40; 
		//ref_vel[5] = getGainVelocity(vel.at(3)*RAD_TO_DEG_F); //F3		
		//ref_vel[6] = static_cast<int>(std::ceil(vel.at(0)*RAD_TO_DEG_F)); ref_vel[7] = 30; //S //TO DO Gain control
		//ref_vel[6] = static_cast<int>(std::ceil(0.0*RAD_TO_DEG_F)); ref_vel[7] = 30; //S //TO DO Gain control		
		std::copy_n(ref_vel,8,out_buffer.begin()+1);
		
		// send
		//boost::asio::async_write(*serial_dev, boost::asio::buffer(out_buffer),
		//	boost::bind(&yarp::dev::BarrettHand826X::serial_write_handler,this,
		//				boost::asio::placeholders::error,
		//				boost::asio::placeholders::bytes_transferred));
		boost::asio::write(*serial_dev, boost::asio::buffer(out_buffer));
				
		// wait for feedback
		bytes_transfered = boost::asio::read_until(*serial_dev, in_buf, rt_feedback_reg);	
		in_str.resize(bytes_transfered);
		in_buf.sgetn(&in_str.front(), bytes_transfered);
		if(in_str.find(err)!=std::string::npos)
			throw std::string("Error received from BarretHand. Controller in now running in Supervisory mode.");

		int curr_pos[4]; int curr_vel[4];

		// F1
		curr_vel[0] = static_cast<int>(in_str.at(1));
		vel_out[0] = static_cast<double>(curr_vel[0]*BHAND_RATIO_140_17800*BHAND_RATIO_N_sec);// to deg/sec
		curr_pos[0] = static_cast<int>(in_str.at(2) << 8) + static_cast<int>((unsigned char)in_str.at(3));
		// F2
		curr_vel[1] = static_cast<int>(in_str.at(4));
		vel_out[1] = static_cast<double>(curr_vel[1]*BHAND_RATIO_140_17800*BHAND_RATIO_N_sec);// to deg/sec
		curr_pos[1] = static_cast<int>(in_str.at(5) << 8) + static_cast<int>((unsigned char)in_str.at(6));
		// F3
		curr_vel[2] = static_cast<int>(in_str.at(7));
		vel_out[2] = static_cast<double>(curr_vel[2]*BHAND_RATIO_140_17800*BHAND_RATIO_N_sec);// to deg/sec
		curr_pos[2] = static_cast<int>(in_str.at(8) << 8) + static_cast<int>((unsigned char)in_str.at(9));
		// S
		curr_vel[3] = static_cast<int>(in_str.at(10));
		vel_out[3] = static_cast<double>(curr_vel[3]*BHAND_RATIO_140_17800*BHAND_RATIO_N_sec);// to deg/sec
		curr_pos[3] = static_cast<int>(in_str.at(11) << 8) + static_cast<int>((unsigned char)in_str.at(12));


		pos[0] = static_cast<double>(curr_pos[0])*BHAND_RATIO_140_17800; // to deg
		pos[1] = static_cast<double>(curr_pos[1])*BHAND_RATIO_140_17800; // to deg
		pos[2] = static_cast<double>(curr_pos[2])*BHAND_RATIO_140_17800; // to deg
		pos[3] = static_cast<double>(curr_pos[3])*BHAND_RATIO_180_3150; // to deg // S

		// from F1 F2 F3 S
		//swap to   S F1 F2 F3
		double temp;
		temp = vel_out[3];
		vel_out[3] = vel_out[2];
		vel_out[2] = vel_out[1];
		vel_out[1] = vel_out[0];
		vel_out[0] = temp;

		temp = pos[3];
		pos[3] = pos[2];
		pos[2] = pos[1];
		pos[1] = pos[0];
		pos[0] = temp;

		//std::cout << "pos: " << pos[1] << std::endl;

	}catch(std::string &ex){

		std::cout << "Hand error in RT mode: " << ex << std::endl;
		return false;
	}

	return true;
}

void yarp::dev::BarrettHand826X::serial_write_handler(const boost::system::error_code& ec,
														std::size_t bytes_transferred)
{
	//boost::asio::streambuf in_buf;
	//boost::shared_ptr<boost::asio::serial_port> serial_dev;
	//serial_dev = serial_port.GetSerialPortDevice();
	//bytes_transferred = boost::asio::read_until(*serial_dev, in_buf, "*");
}

int yarp::dev::BarrettHand826X::getGainVelocity(double vel)
{
	//double v = std::abs(vel); // deg/sec
	return 70;
	/*
	if (vel>=0){
		if(vel < 8.0)
		{
			//return 46 + static_cast<int>(std::ceil(v));
		
			if(vel <= 1.1 )
				return 60;
			else if( vel <= 2.1)
				return 60;
			else if( vel<= 3.1)
				return 60;
			else if( vel <= 4.1)
				return 70;
			else if( vel <= 5.1)
				return 80;
			else if ( vel<= 6.1)
				return 60;
			else if ( vel<= 7.1)
				return 40;
			else if(vel<=8.1)
				return 40;
			
				//return static_cast<int>(std::ceil(16*exp(-0.063583*v)));
				//return static_cast<int>(std::ceil(200*exp(-0.670360*v)));
			//return static_cast<int>(200*exp(-0.670360*v)+0.5);
		}else{
			//return static_cast<int>(15.563*exp(-0.063583*v)+0.5);
			//return static_cast<int>(16*exp(-0.063583*v)+0.5);
			return 40;
		}
	}else{
		//return 35; 
		
		if(vel < -10.1)
		{
			return 250; 
		}else if(vel > -2.01){
			return 205;
			//return 35;
		}else if(vel > -5.01){
			return 200; 
		}else{
			return 2605;
		}
		
	}
	
	//if(v<=2.0)
	//{
	//	return static_cast<int>(std::ceil(200*exp(-0.670360*v)));
	//}else{
	//	return static_cast<int>(std::ceil(16*exp(-0.063583*v)));
	//}
	
	*/
	
}
bool yarp::dev::BarrettHand826X::Read_Reply( std::string& sReply, int timeout_ms )
{
	int nBytes = serial_port.Read_Wait_String( 
		sReply, 
		strStop, 
		timeout_ms );

	return ( nBytes > 0 );
}

bool yarp::dev::BarrettHand826X::Read_Reply_Discard( int timeout_ms )
{
	std::string reply = "";
	return Read_Reply( reply, timeout_ms );
}

bool yarp::dev::BarrettHand826X::Write_Command( std::string command )
{
	std::string cmd = command;
	cmd += "\r";
	return serial_port.Write( cmd );
}

bool yarp::dev::BarrettHand826X::Write_Command_Discard_Reply( std::string command, int timeout_ms )
{
	if( !Write_Command( command ) ) return false;

	return Read_Reply_Discard( timeout_ms );
}

bool yarp::dev::BarrettHand826X::Write_Command_Read_Reply( std::string command, std::string &sReply, int timeout_ms )
{
	if( !Write_Command( command ) ) return false;

	return Read_Reply( sReply, timeout_ms );
}

bool yarp::dev::BarrettHand826X::Write_Command_Read_Reply_Clean( std::string command, std::string &sReply, int timeout_ms )
{
	bool bSuccess = Write_Command_Read_Reply( command, sReply, timeout_ms );
	
	if( bSuccess )
	{
		size_t start_pos = 0;

		// Remove initial command from the reply
		if( ( start_pos = sReply.find( command, start_pos ) ) != std::string::npos )
		{
			sReply.erase( start_pos, command.length() );
		}

		start_pos = 0;

		// Remove everything after the stop string from the reply
		if( ( start_pos = sReply.find( strStop, start_pos ) ) != std::string::npos )
		{
			sReply.erase( start_pos, sReply.size() - start_pos );//strStop.length() );
		}

		start_pos = 0;

		// Remove all the "\n\r" from the reply
		while( ( start_pos = sReply.find( "\n\r", start_pos ) ) != std::string::npos )
		{
			sReply.erase( start_pos, 2 );
			start_pos = 0;
		}
	}

	return bSuccess;
}

bool yarp::dev::BarrettHand826X::Parse_int_from_command_reply( std::string reply, std::string command, int * val, int conv )
{
	int value = 0;

	std::stringstream strm_format;

	strm_format << command << "\n\r%d\n\r" << strStop;

	if( sscanf( reply.c_str(), strm_format.str().c_str(), &value ) > 0 )
	{
		*val = value * conv;
		return true;
	}
	else
	{
		return false;
	}
}

bool yarp::dev::BarrettHand826X::Parse_double_from_command_reply( std::string reply, std::string command, double * val, double conv )
{
	int value = 0;

	if( Parse_int_from_command_reply( reply, command, &value ) )
	{
		*val = static_cast<double>( value ) * conv;
		return true;
	}
	else
	{
		return false;
	}
}

bool yarp::dev::BarrettHand826X::getAxisName( int axis, yarp::os::ConstString& name )
{
	std::string sname = "";

	if( getAxisName( axis, sname ) )
	{
		name = yarp::os::ConstString( sname.c_str(), sname.size() );
		return true;
	}
	else
	{
		return false;
	}
}
	
bool yarp::dev::BarrettHand826X::getAxisName( int axis, std::string& name )
{
	if( !isValidAxis( axis ) ) return false;

	name = BarrettHand::Finger::Name[axis];

	return true;
}

bool yarp::dev::BarrettHand826X::Initialize( int *vec )
{
	std::stringstream command( "" );

	int i=0;

	do
	{
		if( isValidAxis( i ) )
			command << BHand_Finger_ID[vec[i]];

		i++;
	}while( vec[i]!=-1 );

	command << "HI";

	return Write_Command_Discard_Reply( command.str() );
}

bool yarp::dev::BarrettHand826X::Initialize( void )
{
	return Write_Command_Discard_Reply( "HI", 20000 );
}

bool yarp::dev::BarrettHand826X::OpenHand( void )
{
	return Write_Command_Discard_Reply( "GO" );
}

bool yarp::dev::BarrettHand826X::CloseHand( void )
{
	return Write_Command_Discard_Reply( "GC" );
}

bool yarp::dev::BarrettHand826X::OpenHand( int *vec )
{
	std::stringstream command( "" );

	int i=0;

	do
	{
		if( isValidAxis( i ) )
			command << BHand_Finger_ID[vec[i]];

		i++;
	}while( vec[i]!=-1 );

	command << "O";

	return Write_Command_Discard_Reply( command.str() );
}

bool yarp::dev::BarrettHand826X::CloseHand( int *vec )
{
	std::stringstream command( "" );

	int i=0;
	do
	{
		if( isValidAxis( i ) )
			command << BHand_Finger_ID[vec[i]];

		i++;
	}
	while( vec[i]!=-1 );
	
	command << "C";

	return Write_Command_Discard_Reply( command.str() );
}	

bool yarp::dev::BarrettHand826X::TorqueOpen( void )
{
	return Write_Command_Discard_Reply( "GTO" );
}

bool yarp::dev::BarrettHand826X::TorqueClose( void )
{
	return Write_Command_Discard_Reply( "GTC" );
}

bool yarp::dev::BarrettHand826X::TorqueOpen( int *vec )
{
	std::stringstream command( "" );

	int i=0;
	
	do
	{
		if( isValidAxis( i ) )
			command << BHand_Finger_ID[vec[i]];

		i++;
	}
	while( vec[i]!=-1 );
	
	command << "TO";

	return Write_Command_Discard_Reply( command.str() );
}

bool yarp::dev::BarrettHand826X::TorqueClose( int *vec )
{
	std::stringstream command( "" );

	int i=0;

	do
	{
		if( isValidAxis( i ) )
			command << BHand_Finger_ID[vec[i]];

		i++;
	}
	while( vec[i]!=-1 );
	
	command << "TC";

	return Write_Command_Discard_Reply( command.str() );
}

bool yarp::dev::BarrettHand826X::setOpenTarget( double *refs )
{
	for( int i=0 ; i<4 ; i++ )
	{
		if( !setOpenTarget( i, refs[i] ) )
			return false;
	}
	return true;
}

bool yarp::dev::BarrettHand826X::setOpenTarget( int j, double ref )
{
	if( !isValidAxis( j ) ) return false;

	int value = static_cast<int>( ref * BHand_SetParam_Ratio[j] );

	std::stringstream command( "" );
	command << BHand_Finger_ID[j] << "FSET OT " << value;

	return Write_Command_Discard_Reply( command.str() );
}

bool yarp::dev::BarrettHand826X::setCloseTarget( double *refs )
{
	for( int i=0 ; i<getAxes() ; i++ )
	{
		if( !setCloseTarget( i, refs[i] ) )
			return false;
	}
	return true;
}

bool yarp::dev::BarrettHand826X::setCloseTarget( int j, double ref )
{
	if( !isValidAxis( j ) ) return false;

	int value = static_cast<int>( ref * BHand_SetParam_Ratio[j] );

	std::stringstream command( "" );
	command << BHand_Finger_ID[j] << "FSET CT " << value;
		
	return Write_Command_Discard_Reply( command.str() );
}

bool yarp::dev::BarrettHand826X::getOpenTarget( double *refs )
{
	for( int i=0 ; i<getAxes() ; i++ )
	{
		if( !getOpenTarget( i, &refs[i] ) )
			return false;
	}
	return true;
}

bool yarp::dev::BarrettHand826X::getOpenTarget( int j, double *ref )
{
	 if( !isValidAxis( j ) ) return false;

	std::string msgout = "";
	
	std::stringstream command( "" );
	command << BHand_Finger_ID[j] << "FGET OT";
	
	if( !Write_Command_Read_Reply( command.str(), msgout ) ) return false;
	
	return Parse_double_from_command_reply( msgout, command.str(), ref, BHand_GetParam_Ratio[j] );
}

bool yarp::dev::BarrettHand826X::getCloseTarget( double *refs )
{
	for( int i=0 ; i<getAxes() ; i++ )
	{
		if( !getCloseTarget( i, &refs[i] ) )
			return false;
	}
	return true;
}

bool yarp::dev::BarrettHand826X::getCloseTarget( int j, double *ref )
{
	 if( !isValidAxis( j ) ) return false;

	std::string msgout = "";

	std::stringstream command( "" );
	command << BHand_Finger_ID[j] << "FGET CT";

	if( !Write_Command_Read_Reply( command.str(), msgout ) ) return false;
	
	return Parse_double_from_command_reply( msgout, command.str(), ref, BHand_GetParam_Ratio[j] );
}

bool yarp::dev::BarrettHand826X::positionMoveFingersRaw( double ref )
{
	std::stringstream command;
	command << "123M " << static_cast<int>( ref );

	return Write_Command_Discard_Reply( command.str() );
}

bool yarp::dev::BarrettHand826X::positionMoveFingers( double ref )
{
	return positionMoveFingersRaw( ref*BHAND_RATIO_17800_140 );
}

bool yarp::dev::BarrettHand826X::positionMoveFingers( const double *refs )
{	
	for( int i=1 ; i<getAxes() ; i++ )
	{
		if( !positionMove( i, refs[i] ) )
			return false;
	}
	return true;
}

bool yarp::dev::BarrettHand826X::positionMoveTwoFingersRaw( double *refs )
{
	std::stringstream command( "" );
	command << "12M " << static_cast<int>( refs[ BarrettHand::Finger::One ] );
	
	if( !Write_Command_Discard_Reply( command.str() ) ) return false;

	command.str( "" );
	command.clear();

	command << "3M " << static_cast<int>( refs[ BarrettHand::Finger::Three ] );

	return Write_Command_Discard_Reply( command.str() ); 
}

bool yarp::dev::BarrettHand826X::positionMoveTwoFingers( double *refs )
{
	refs[ BarrettHand::Finger::One ] *= BHAND_RATIO_17800_140;
	refs[ BarrettHand::Finger::Three ] *= BHAND_RATIO_17800_140;

	return positionMoveTwoFingersRaw( refs );
}

bool yarp::dev::BarrettHand826X::setRefSpeedOpen( int j, double sp )
{
	 if( !isValidAxis( j ) ) return false;

	const double 
		dFSpeed_Max[4] = { 60.0, 100.0, 100.0, 100.0 },
		dFSpeed_Min[4] = { 16.0, 16.0, 16.0, 16.0 };

	if( sp>dFSpeed_Max[j] )
		sp = dFSpeed_Max[j];

	if( sp<dFSpeed_Min[j] )
		sp = dFSpeed_Min[j];

	std::stringstream command( "" );
	command << BHand_Finger_ID[j] << "FSET MOV " << static_cast<int>( sp );
		
	return Write_Command_Discard_Reply( command.str() );
}

bool yarp::dev::BarrettHand826X::setRefSpeedsOpen( const double *spds )
{
	for( int i=0 ; i<getAxes() ; i++ )
	{
		if( !setRefSpeedOpen(i, spds[i]) )
			return false;
	}
	return true;
}

bool yarp::dev::BarrettHand826X::setRefSpeedClose( int j, double sp )
{
	 if( !isValidAxis( j ) ) return false;

	const double 
		dFSpeed_Max[4] = { 60.0, 100.0, 100.0, 100.0 },
		dFSpeed_Min[4] = { 16.0, 16.0, 16.0, 16.0 };

	if( sp>dFSpeed_Max[j] )
		sp = dFSpeed_Max[j];

	if( sp<dFSpeed_Min[j] )
		sp = dFSpeed_Min[j];

	std::stringstream command( "" );
	command << BHand_Finger_ID[j] << "FSET MCV " << static_cast<int>( sp );
	
	return Write_Command_Discard_Reply( command.str() );
}

bool yarp::dev::BarrettHand826X::setRefSpeedsClose( const double *spds )
{
	for( int i=0 ; i<getAxes() ; i++ )
	{
		if( !setRefSpeedClose( i, spds[i] ) )
			return false;
	}
	return true;
}

bool yarp::dev::BarrettHand826X::getPositionsRaw( double *pos )
{
	bool value = true;

	for( int i=0 ; i<getAxes() ; i++ )
	{
		if( !getPositionRaw( i, &pos[i] ) )
			value = false;
	}
		
	return value;
}

bool yarp::dev::BarrettHand826X::getPositionRaw( int j, double *ref )
{
	if( !isValidAxis( j ) ) return false;

	std::string msgout = "";

	std::stringstream command( "" );
	command << BHand_Finger_ID[j] << "FGET P";

	if( !Write_Command_Read_Reply( command.str(), msgout ) ) return false;

	return Parse_double_from_command_reply( msgout, command.str(), ref );
}

bool yarp::dev::BarrettHand826X::getPositions( double *pos )
{
	using boost::lambda::var;
	using boost::lambda::_1;
	using boost::lambda::_2;
	try
	{
		boost::shared_ptr<boost::asio::serial_port> serial_dev;
		boost::asio::streambuf buf;
		boost::asio::streambuf in_buf;

		size_t bytes_transfered = 0;
		boost::system::error_code ec = boost::asio::error::would_block;

		//get the low level device.
		serial_dev = serial_port.GetSerialPortDevice();

		//update current positions
		buf.sputn("FGET P\r", 7);
		boost::asio::write(*serial_dev, buf);
		boost::regex fgetpos_pattern(".*FGET P\\n\\r(\\d+) (\\d+) (\\d+) (\\d+)\\n\\r=> |.*ERR (\\d+)\\n\\r=> ");

	//	serial_port.dead_line_timer->async_wait( boost::bind( &yarp::dev::serial_asio::checkdeadline, boost::ref(serial_port )) );
		serial_port.dead_line_timer->expires_from_now( boost::posix_time::milliseconds( 1000 ) );
		
		 // Start the asynchronous operation itself. The boost::lambda function
		// object is used as a callback and will update the ec variable when the
		// operation completes. The blocking_udp_client.cpp example shows how you
		// can use boost::bind rather than boost::lambda.
//		boost::asio::async_read_until(*serial_dev, in_buf, fgetpos_pattern, (var(ec) = _1, var(bytes_transfered) = _2) );
		
		 // Block until the asynchronous operation has completed.
	//	do serial_dev->get_io_service().run_one(); while (ec == boost::asio::error::would_block);

//		if (ec)
//			throw boost::system::system_error(ec);

		serial_port.io_service.post(
		[this, &serial_dev, &ec, &bytes_transfered, &in_buf,&fgetpos_pattern]
		{
			boost::asio::async_read_until( *serial_dev, in_buf, fgetpos_pattern, (boost::lambda::var(ec) = _1, boost::lambda::var(bytes_transfered) = _2) );
			 // Block until the asynchronous operation has completed.
		});
		do ; while (ec == boost::asio::error::would_block);


	//	bytes_transfered = boost::asio::read_until(*serial_dev, in_buf, fgetpos_pattern, ec);

		if (ec)
			throw boost::system::system_error(ec);

		serial_port.dead_line_timer->expires_at( boost::posix_time::pos_infin );

		std::string str;
		str.resize(bytes_transfered);
		in_buf.sgetn(&str.front(), bytes_transfered);

		boost::smatch matches;

		if (boost::regex_match(str, matches, fgetpos_pattern))
		{
			if(matches.size() < 4)
				std::cout<<"barret error"<<std::endl;
			else
			for (size_t i = 1; i < matches.size() - 1; i++)
				pos[i - 1] = boost::lexical_cast<double>(std::string(matches[i].first, matches[i].second)) * BHand_GetParam_Ratio[BHand_Device_2_Module_Idx[i - 1]];
		}
		else
			return false;
	}
	catch(boost::system::system_error &ex)
	{
		if(ex.code() == boost::asio::error::operation_aborted)
			std::cerr << "Bhand: Timeout reading from serial port: " << ex.what() << std::endl;
		else
			std::cerr << "Bhand: " << ex.what() << std::endl;
		return false;
	}
	catch (boost::bad_lexical_cast const& ex)
	{
		std::cerr << "Could not convert hand current positions: " << ex.what() << std::endl;
		return false;
	}
	catch (boost::regex_error const& ex)
	{
		std::cerr << "Regex Error: " << ex.what() << std::endl;
		return false;
	}
	//swap to   S F1 F2 F3
		// from F1 F2 F3 S
	double temp;
	temp = pos[3];
	pos[3] = pos[2];
	pos[2] = pos[1];
	pos[1] = pos[0];
	pos[0] = temp;
	
	return true;
}

bool yarp::dev::BarrettHand826X::getPosition( int j, double *ref )
{
	double val = 0.0;

	if( getPositionRaw( j, &val ) )
	{
		*ref = val * BHand_GetParam_Ratio[j];
		return true;
	}
	else
	{
		return false;
	}
}


bool yarp::dev::BarrettHand826X::getStrainGauge( int j, double *strain )
{
	 if( !isValidAxis( j ) ) return false;

	std::string msgout = "";
	
	std::stringstream command( "" );
	command << BHand_Finger_ID[j] << "FGET SG";

	if( !Write_Command_Read_Reply( command.str(), msgout ) ) return false;
	
	return Parse_double_from_command_reply( msgout, command.str(), strain );
}

bool yarp::dev::BarrettHand826X::getStrainGauges( double *strain )
{
	for( int i=0; i<getAxes() ; i++ )
	{
		if( !getStrainGauge( i, &strain[i] ) )
			return false;
	}
	return true;
}

// ########################################################################

bool yarp::dev::BarrettHand826X::setPositionModeRaw( void )
{
	return false;
}

bool yarp::dev::BarrettHand826X::setPositionMode( void )
{
	// No conversion required
	return setPositionModeRaw();
}

bool yarp::dev::BarrettHand826X::positionMoveRaw( int j, double ref )
{
	 if( !isValidAxis( j ) ) return false;

	std::stringstream command( "" );
	command << BHand_Finger_ID[j] << "M " << static_cast<int>( ref );

	return Write_Command_Discard_Reply( command.str() );
}

bool yarp::dev::BarrettHand826X::positionMove( int j, double ref )
{
	return 
		isValidAxis( j ) ?
		positionMoveRaw( j, ref * BHand_SetParam_Ratio[j] ) :
		false;
}

bool yarp::dev::BarrettHand826X::positionMoveRaw( const double *refs )
{
	for( int i=0 ; i<getAxes() ; i++ )
	{
		if( !positionMoveRaw( i, refs[i] ) )
			return false;
	}
	return true;
}

bool yarp::dev::BarrettHand826X::positionMove( const double *refs )
{	
	for( int i=0 ; i<getAxes() ; i++ )
	{
		if( !positionMove( i, refs[i] ) )
			return false;
	}
	return true;
}

bool yarp::dev::BarrettHand826X::relativeMoveRaw( int j, double delta )
{
	 if( !isValidAxis( j ) ) return false;

	char cOption = ( delta>0.0 ) ? 'C' : 'O';
	
	std::stringstream command( "" );
	command << BHand_Finger_ID[j] << "I" << cOption << " " << abs( static_cast<int>( delta ) );

	return Write_Command_Discard_Reply( command.str() );
}

bool yarp::dev::BarrettHand826X::relativeMove( int j, double delta )
{
	return 
		isValidAxis( j ) ?
		relativeMoveRaw( j, delta * BHand_SetParam_Ratio[j] ) :
		false;
}

bool yarp::dev::BarrettHand826X::relativeMoveRaw( const double *deltas )
{
	for( int i=0 ; i<getAxes() ; i++ )
	{
		if( !relativeMoveRaw( i, deltas[i] ) )
			return false;
	}
	return true;
}

bool yarp::dev::BarrettHand826X::relativeMove( const double *deltas )
{
	for( int i=0 ; i<getAxes() ; i++ )
	{
		if( !relativeMove( i, deltas[i] ) )
			return false;
	}
		
	return true;
}

bool yarp::dev::BarrettHand826X::checkMotionDoneRaw( int j, bool *flag )
{
	int status = 0;
	bool bSuccess = getStatus( j, &status );
	*flag = ( status == 0 );
	return bSuccess;
}

bool yarp::dev::BarrettHand826X::checkMotionDone( int j, bool *flag )
{
	// No converion required
	 return checkMotionDoneRaw( j, flag );
}

bool yarp::dev::BarrettHand826X::checkMotionDoneRaw( bool *flag )
{
	for( int i=0 ; i<getAxes() ; i++ )
	{
		if( !checkMotionDoneRaw( i, flag ) )
		{
			*flag = false;
			return false;
		}
	}
	return true;
}

bool yarp::dev::BarrettHand826X::checkMotionDone( bool *flag )
{
	// No converion required
	return checkMotionDoneRaw( flag );
}

bool yarp::dev::BarrettHand826X::setRefSpeedRaw( int j, double sp )
{
	 if( !isValidAxis( j ) ) return false;

	const double 
		dFSpeed_Max[4] = { 60.0, 100.0, 100.0, 100.0 },
		dFSpeed_Min[4] = { 16.0, 16.0, 16.0, 16.0 };

	
	if( sp>dFSpeed_Max[j] )
		sp = dFSpeed_Max[j];

	if( sp<dFSpeed_Min[j] )
		sp = dFSpeed_Min[j];

	int ispeed = static_cast<int>( sp );
	
	std::stringstream command( "" );
	command << BHand_Finger_ID[j] << "FSET MCV " << ispeed;

	if( !Write_Command_Discard_Reply( command.str() ) ) return false;

	command.str( "" );
	command.clear();

	command << BHand_Finger_ID[j] << "FSET MOV " << ispeed;

	return Write_Command_Discard_Reply( command.str() );
}

bool yarp::dev::BarrettHand826X::setRefSpeed( int j, double sp )
{
	// No converion required
	return setRefSpeedRaw( j, sp );
}

bool yarp::dev::BarrettHand826X::setRefSpeedsRaw( const double *spds )
{
	for( int i=0 ; i<getAxes() ; i++ )
	{
		if( !setRefSpeedRaw( i, spds[i] ) )
			return false;
	}
	return true;
}

bool yarp::dev::BarrettHand826X::setRefSpeeds( const double *spds )
{
	// No converion required
	return setRefSpeedsRaw( spds );
}

bool yarp::dev::BarrettHand826X::setRefAccelerationRaw( int j, double acc )
{
	if( !isValidAxis( j ) ) return false;

	double dAcc = acc;
	
	if( dAcc>10.0 ) dAcc = 10.0;
	 
	if( dAcc<1.0 ) dAcc = 1.0;
	
	std::stringstream command( "" );
	command << BHand_Finger_ID[j] << "FSET ACCEL " << static_cast<int>( dAcc );

	return Write_Command_Discard_Reply( command.str() );
}

bool yarp::dev::BarrettHand826X::setRefAcceleration( int j, double acc )
{
	// No conversion required
	return setRefAccelerationRaw( j, acc );
}

bool yarp::dev::BarrettHand826X::setRefAccelerationsRaw( const double *accs )
{
	for( int i=0 ; i<getAxes() ; i++ )
	{
		if( !setRefAccelerationRaw( i, accs[i] ) )
			return false;
	}
	return true;}

bool yarp::dev::BarrettHand826X::setRefAccelerations( const double *accs )
{
	// No conversion required
	return setRefAccelerationsRaw( accs );
}


bool yarp::dev::BarrettHand826X::setMaxStrain( int * refs )
{
	for( int i=0 ; i<getAxes() ; i++ )
	{
		if( !setMaxStrain( i, refs[i] ) )
			return false;
	}
	return true;
}
	

bool yarp::dev::BarrettHand826X::setMaxStrain( int j, int ref )
{
	if( !isValidAxis( j ) ) return false;

	if( ref>256 ) ref = 256;
	 
	if( ref<0 ) ref = 0;
	
	std::stringstream command( "" );
	command << BHand_Finger_ID[j] << "FSET HSG " << ref;

	return Write_Command_Discard_Reply( command.str() );
}

bool yarp::dev::BarrettHand826X::getMaxStrain( int * refs )
{
	for( int i=0 ; i<getAxes() ; i++ )
	{
		if( !getMaxStrain( i, &refs[i] ) )
			return false;
	}
	return true;
}
	
bool yarp::dev::BarrettHand826X::getMaxStrain( int j, int * ref )
{
	 if( !isValidAxis( j ) ) return false;

	std::string msgout = "";
	
	std::stringstream command( "" );
	command << BHand_Finger_ID[j] << "FGET HSG";

	if( !Write_Command_Read_Reply( command.str(), msgout ) ) return false;
	
	return Parse_int_from_command_reply( msgout, command.str(), ref );
}

bool yarp::dev::BarrettHand826X::getRefSpeedRaw( int j, double *ref )
{
	 if( !isValidAxis( j ) ) return false;

	std::string msgout = "";
	
	std::stringstream command( "" );
	command << BHand_Finger_ID[j] << "FGET MOV";

	if( !Write_Command_Read_Reply( command.str(), msgout ) ) return false;
	
	return Parse_double_from_command_reply( msgout, command.str(), ref );
}

bool yarp::dev::BarrettHand826X::getRefSpeed( int j, double *ref )
{
	// No conversion required
	return getRefSpeedRaw( j, ref );
}

bool yarp::dev::BarrettHand826X::getRefSpeedsRaw( double *spds )
{
	for( int i=0 ; i<getAxes() ; i++ )
	{
		if( !getRefSpeedRaw( i, &spds[i] ) )
			return false;
	}
	return true;
}

bool yarp::dev::BarrettHand826X::getRefSpeeds( double *spds )
{
	// No conversion required
	return getRefSpeedsRaw( spds );
}

bool yarp::dev::BarrettHand826X::getRefAccelerationRaw( int j, double *acc )
{
	 if( !isValidAxis( j ) ) return false;

	std::string msgout = "";

	std::stringstream command( "" );
	command << BHand_Finger_ID[j] << "FGET ACCEL";

	if( !Write_Command_Read_Reply( command.str(), msgout ) ) return false;
	
	return Parse_double_from_command_reply( msgout, command.str(), acc );
}

bool yarp::dev::BarrettHand826X::getRefAcceleration( int j, double *acc )
{
	// No conversion required
	return getRefAccelerationRaw( j, acc );
}

bool yarp::dev::BarrettHand826X::getRefAccelerationsRaw( double *accs )
{
	for( int i=0 ; i<getAxes() ; i++ )
	{
		if( !getRefAccelerationRaw( i, &accs[i] ) )
			return false;
	}
	return true;
}

bool yarp::dev::BarrettHand826X::getRefAccelerations( double *accs )
{
	// No conversion required
	return getRefAccelerationsRaw( accs );
}

bool yarp::dev::BarrettHand826X::stopRaw( int j )
{
	return false;
}

bool yarp::dev::BarrettHand826X::stop( int j )
{
	return stopRaw( j );
}

bool yarp::dev::BarrettHand826X::stopRaw( void )
{
	return false;
}

bool yarp::dev::BarrettHand826X::stop( void )
{
	return stopRaw();
}

bool yarp::dev::BarrettHand826X::getStatus( int j, int * status )
{
	 if( !isValidAxis( j ) ) return false;

	std::string msgout = "";

	std::stringstream command( "" );
	command << BHand_Finger_ID[j] << "FGET S";

	if( !Write_Command_Read_Reply( command.str(), msgout ) ) return false;
	
	int value = 0;

	if( Parse_int_from_command_reply( msgout, command.str(), &value ) )
	{
		*status = value;
		return true;
	}
	else
	{
		return false;
	}
}

bool yarp::dev::BarrettHand826X::getStatus( int * status )
{
	for( int i=0 ; i<getAxes() ; i++ )
	{
		if( !getStatus( i, &status[i] ) )
			return false;
	}
	
	return true;
}

bool yarp::dev::BarrettHand826X::getStatus( int j, int * status, std::string &str_status )
{
	if( !getStatus( j, status ) ) return false;

	return getStatus_str( *status, str_status );
}

bool yarp::dev::BarrettHand826X::getStatus_str( int status, std::string &str_status )
{
	bool bSuccess = true;

	switch( status )
	{
	case 0:
		str_status = "No error";
		break;

	case 1:
		str_status = "No motor board found";
		break;

	case 2:
		str_status = "No motor found";
		break;

	case 4:
		str_status = "Motor not initialized";
		break;

	case 8:
	case 512:
		str_status = "(not used)";
		break;

	case 16:
		str_status = "Couldn't reach position";
		break;

	case 32:
		str_status = "Unknown command";
		break;

	case 64:
		str_status = "Unknown parameter name";
		break;

	case 128:
		str_status = "Invalid value";
		break;

	case 256:
		str_status = "Tried to write a read only parameter";
		break;

	case 1024:
		str_status = "Too many arguments for this command";
		break;

	case 2048:
		str_status = "Invalid RealTime control block header";
		break;

	case 4096:
		str_status = "Command can't have motor prefix";
		break;

	case 8192:
		str_status = "Overtemperature fault tripped";
		break;

	case 16384:
		str_status = "Cntl-C abort command received";
		break;

	default:
		str_status = "Invalid status code";
		bSuccess = false;
		break;
	}

	return bSuccess;
}