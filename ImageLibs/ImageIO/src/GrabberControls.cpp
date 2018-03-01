#include "GrabberControls.h"

#include <sstream>
#include <yarp/os/Property.h>

CGrabberControls::CGrabberControls( void )
: bWhiteBalanceAuto( true ), nBlue( 0 ), nGreen( 0 ), nRed( 0 )
, bAutoExposure( true ), nExposure( 0 )
, bAutoGain( true ), nGain( 0 )
, bAutoBrightness( true ), nBrightness( 0 )
, bFlipHorizontal( false ),	bFlipVertical( false )
, nGamma( 0 )
, nSaturation( 0 )
, bControlParameters( false )
, bControl_Exposure( false )
, bControl_Gain( false )
, bControl_Brightness( false )
, bControl_Gamma( false )
, bControl_Saturation( false )
, bControl_WhiteBalance( false )
, bControl_WB_Red( false )
, bControl_WB_Green( false )
, bControl_WB_Blue( false )
, bControl_Flip_Horizontal( false )
, bControl_Flip_Vertical( false )
{
}

CGrabberControls::CGrabberControls( const CGrabberControls &obj )
{
	CGrabberControls * pObj = const_cast<CGrabberControls*>( &obj );

	bWhiteBalanceAuto.set( pObj->bWhiteBalanceAuto.get() );
	nBlue.set( pObj->nBlue.get() );
	nGreen.set( pObj->nGreen.get() );
	nRed.set( pObj->nRed.get() );
	bAutoExposure.set( pObj->bAutoExposure.get() );
	nExposure.set( pObj->nExposure.get() );
	bAutoGain.set( pObj->bAutoGain.get() );
	nGain.set( pObj->nGain.get() );
	bAutoBrightness.set( pObj->bAutoBrightness.get() );
	nBrightness.set( pObj->nBrightness.get() );
	nGamma.set( pObj->nGamma.get() );
	nSaturation.set( pObj->nSaturation.get() );
	bControlParameters.set( pObj->bControlParameters.get() );
	bControl_Exposure.set( pObj->bControl_Exposure.get() );
	bControl_Gain.set( pObj->bControl_Gain.get() );
	bControl_Brightness.set( pObj->bControl_Brightness.get() );
	bControl_Gamma.set( pObj->bControl_Gamma.get() );
	bControl_Saturation.set( pObj->bControl_Saturation.get() );
	bControl_WhiteBalance.set( pObj->bControl_WhiteBalance.get() );
	bControl_WB_Red.set( pObj->bControl_WB_Red.get() );
	bControl_WB_Green.set( pObj->bControl_WB_Green.get() );
	bControl_WB_Blue.set( pObj->bControl_WB_Blue.get() );

	pObj = nullptr;
}

bool CGrabberControls::has_ParamaterControl( void )
{
	return bControlParameters.get();
}

bool CGrabberControls::has_Control_Exposure( void )
{
	return bControl_Exposure.get() && has_ParamaterControl();
}

bool CGrabberControls::has_Control_Gain( void )
{
	return bControl_Gain.get() && has_ParamaterControl();
}

bool CGrabberControls::has_Control_Brightness( void )
{
	return bControl_Brightness.get() && has_ParamaterControl();
}

bool CGrabberControls::has_Control_Gamma( void )
{
	return bControl_Gamma.get() && has_ParamaterControl();
}

bool CGrabberControls::has_Control_Saturation( void )
{
	return bControl_Saturation.get() && has_ParamaterControl();
}

bool CGrabberControls::has_Control_WhiteBalance( void )
{
	return bControl_WhiteBalance.get() && has_ParamaterControl();
}

bool CGrabberControls::has_Control_WB_Red( void )
{
	return bControl_WB_Red.get() && bControl_WhiteBalance.get() && has_ParamaterControl();
}

bool CGrabberControls::has_Control_WB_Green( void )
{
	return bControl_WB_Green.get() && bControl_WhiteBalance.get() && has_ParamaterControl();
}

bool CGrabberControls::has_Control_WB_Blue( void )
{
	return bControl_WB_Blue.get() && bControl_WhiteBalance.get() && has_ParamaterControl();
}

bool CGrabberControls::has_Control_FlipHorizontal( void )
{
	return bControl_Flip_Horizontal.get() && has_ParamaterControl();
}

bool CGrabberControls::has_Control_FlipVertical( void )
{
	return bControl_Flip_Vertical.get() && has_ParamaterControl();
}

void CGrabberControls::set_Exposure( int exp )
{
	set_Device_Exposure( exp );
	nExposure.set( exp );
}

void CGrabberControls::set_Exposure_Auto( bool autoexp )
{
	set_Device_Exposure_Auto( autoexp );
	bAutoExposure.set( autoexp );
}

void CGrabberControls::set_Gain(  int gain )
{
	set_Device_Gain( gain );
	nGain.set( gain );
}

void CGrabberControls::set_Gain_Auto( bool autogain )
{
	set_Device_Gain_Auto( autogain );
	bAutoGain.set( autogain );
}

void CGrabberControls::set_Brightness( int bright )
{
	set_Device_Brightness( bright );
	nBrightness.set( bright );
}

void CGrabberControls::set_Brightness_Auto( bool autobright )
{
	set_Device_Brightness_Auto( autobright );
	bAutoBrightness.set( autobright );
}

void CGrabberControls::set_WhiteBalance_Red( int red )
{
	set_Device_WhiteBalance_Red( red );
	nRed.set( red );
}

void CGrabberControls::set_WhiteBalance_Green( int green )
{
	set_Device_WhiteBalance_Green( green );
	nGreen.set( green );
}

void CGrabberControls::set_WhiteBalance_Blue( int blue )
{
	set_Device_WhiteBalance_Blue( blue );
	nBlue.set( blue );
}

void CGrabberControls::set_WhiteBalance_Auto( bool autobalance )
{
	set_Device_WhiteBalance_Auto( autobalance );
	bWhiteBalanceAuto.set( autobalance );
}

void CGrabberControls::set_Gamma( int gamma )
{
	set_Device_Gamma( gamma );
	nGamma.set( gamma );
}

void CGrabberControls::set_Saturation( int sat )
{
	set_Device_Saturation( sat );
	nSaturation.set( sat );
}

void CGrabberControls::set_FlipHorizontal( bool flip )
{
	set_Device_FlipHorizontal( flip );
	bFlipHorizontal.set( flip );
}

void CGrabberControls::set_FlipVertical( bool flip )
{
	set_Device_FlipVertical( flip );
	bFlipVertical.set( flip );
}

int CGrabberControls::get_Exposure( void )
{
	auto val = get_Device_Exposure();
	nExposure.set( val );
	return val;
}

bool CGrabberControls::get_Exposure_Auto( void )
{
	auto val = get_Device_Exposure_Auto();
	bAutoExposure.set( val );
	return val;
}

int CGrabberControls::get_Gain( void )
{
	auto val = get_Device_Gain();
	nGain.set( val );
	return val;
}

bool CGrabberControls::get_Gain_Auto( void )
{
	auto val = get_Device_Gain_Auto();
	bAutoGain.set( val );
	return val;
}

int CGrabberControls::get_Brightness( void )
{
	auto val = get_Device_Brightness();
	nBrightness.set( val );
	return val;
}

bool CGrabberControls::get_Brightness_Auto( void )
{
	auto val = get_Device_Brightness_Auto();
	bAutoBrightness.set( val );
	return val;
}


int CGrabberControls::get_WhiteBalance_Red( void )
{
	auto val = get_Device_WhiteBalance_Red();
	nRed.set( val );
	return val;
}

int CGrabberControls::get_WhiteBalance_Green( void )
{
	auto val = get_Device_WhiteBalance_Green();
	nGreen.set( val );
	return val;
}

int CGrabberControls::get_WhiteBalance_Blue( void )
{
	auto val = get_Device_WhiteBalance_Blue();
	nBlue.set( val );
	return val;
}

bool CGrabberControls::get_WhiteBalance_Auto( void )
{
	auto val = get_Device_WhiteBalance_Auto();
	bWhiteBalanceAuto.set( val );
	return val;
}

int CGrabberControls::get_Gamma( void )
{
	auto val = get_Device_Gamma();
	nGamma.set( val );
	return val;
}

int CGrabberControls::get_Saturation( void )
{
	auto val = get_Device_Saturation();
	nSaturation.set( val );
	return val;
}

bool CGrabberControls::get_FlipHorizontal( void )
{
	bool val = get_Device_FlipHorizontal();
	bFlipHorizontal.set( val );
	return val;
}

bool CGrabberControls::get_FlipVertical( void )
{
	bool val = get_Device_FlipVertical();
	bFlipVertical.set( val );
	return val;
}

bool CGrabberControls::Parse_Parameters_Load( void )
{
	yarp::os::Bottle bot;

	//-----------------------------------------------------------------------
	// White Balance values
	bot = prop_ConfigFile.findGroup( "White_Balance" );

	if( bot.check( "Red" ) )
		set_WhiteBalance_Red( bot.find( "Red" ).asInt() );

	if( bot.check( "Green" ) )
		set_WhiteBalance_Green( bot.find( "Green" ).asInt() );

	if( bot.check( "Blue" ) )
		set_WhiteBalance_Blue( bot.find( "Blue" ).asInt() );

	if( bot.check( "Auto" ) )
		set_WhiteBalance_Auto( bot.find( "Auto" ).asInt() == 1 );

	//-----------------------------------------------------------------------
	// Exposure
	bot = prop_ConfigFile.findGroup( "Exposure" );
	
	if( bot.check( "Value" ) )
		set_Exposure( bot.find( "Value" ).asInt() );

	if( bot.check( "Auto" ) )
		set_Exposure_Auto( bot.find( "Auto" ).asInt() == 1 );

	//-----------------------------------------------------------------------
	// Gain
	bot = prop_ConfigFile.findGroup( "Gain" );
	
	if( bot.check( "Value" ) )
		set_Gain( bot.find( "Value" ).asInt() );

	if( bot.check( "Auto" ) )
		set_Gain_Auto( bot.find( "Auto" ).asInt() == 1 );

	//-----------------------------------------------------------------------
	// Brightness
	bot = prop_ConfigFile.findGroup( "Brightness" );

	if( bot.check( "Value" ) )
		set_Brightness( bot.find( "Value" ).asInt() );

	if( bot.check( "Auto" ) )
		set_Brightness_Auto( bot.find( "Auto" ).asInt() == 1 );

	//-----------------------------------------------------------------------
	// Gamma
	bot = prop_ConfigFile.findGroup( "Gamma" );

	if( bot.check( "Value" ) )
		set_Gamma( bot.find( "Value" ).asInt() );

	//-----------------------------------------------------------------------
	// Saturation
	bot = prop_ConfigFile.findGroup( "Saturation" );

	if( bot.check( "Value" ) )
		set_Saturation( bot.find( "Value" ).asInt() );

	return true;
}

void CGrabberControls::Parse_Parameters_Save( void )
{
	int nAuto = 0;

	std::stringstream buf;
	
	strFileBuffer = "#Video parameters\n\n";

	nAuto = ( get_Exposure_Auto() ? 1 : 0 );
	
	buf.str( "" );
	buf.clear();
	buf << "[Exposure]" << std::endl << 
		"Auto " << nAuto << std::endl <<
		"Value " << get_Exposure() << std::endl << std::endl;

	strFileBuffer += buf.str();

	nAuto = ( get_Gain_Auto() ? 1 : 0 );

	buf.str( "" );
	buf.clear();
	buf << "[Gain]" << std::endl <<
		"Auto " << nAuto  << std::endl << 
		"Value " << get_Gain() << std::endl << std::endl;

	strFileBuffer += buf.str();

	nAuto = ( get_Brightness_Auto() ? 1 : 0 );
	
	buf.str( "" );
	buf.clear();
	buf << "[Brightness]" << std::endl <<
		"Auto " << nAuto  << std::endl << 
		"Value " << get_Brightness() << std::endl << std::endl;

	strFileBuffer += buf.str();

	buf.str( "" );
	buf.clear();
	buf << "[Gamma]" << std::endl <<
		"Value " << get_Gamma()  << std::endl << std::endl;

	strFileBuffer += buf.str();

	buf.str( "" );
	buf.clear();
	buf << "[Saturation]" << std::endl <<
		"Value " << get_Saturation()  << std::endl << std::endl;

	strFileBuffer += buf.str();

	nAuto = ( get_WhiteBalance_Auto() ? 1 : 0 );
	
	buf.str( "" );
	buf.clear();
	buf << "[White_Balance]" << std::endl <<
		"Auto " << nAuto << std::endl <<
		"Red " << get_WhiteBalance_Red()  << std::endl << 
		"Green " << get_WhiteBalance_Green()  << std::endl << 
		"Blue " << get_WhiteBalance_Blue()  << std::endl;

	strFileBuffer += buf.str();
}
