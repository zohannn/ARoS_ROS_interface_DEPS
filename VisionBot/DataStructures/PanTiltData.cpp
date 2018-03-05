#include "pantilt_definitions.h"
#include "PanTiltData.h"

CPanTiltData::CPanTiltData( void )
: fPan_Angle( -10.0f ), fTilt_Angle( -45.0f )
, fPan_Velocity( 51.4f ), fTilt_Velocity( 87.0f )
, nControlMode( PanTilt::ControlMode::POSITION )
{
}

void CPanTiltData::get_PanTiltAngles( float * pPan, float * pTilt )
{
	*pPan = fPan_Angle.get();
	*pTilt = fTilt_Angle.get();
}

void CPanTiltData::get_PanTiltVelocities( float * pPan, float * pTilt )
{
	*pPan = fPan_Velocity.get();
	*pTilt = fTilt_Velocity.get();
}

void CPanTiltData::set_PanTiltAngles( float pan, float tilt )
{
	fPan_Angle.set( pan );
	fTilt_Angle.set( tilt );
}

void CPanTiltData::set_PanTiltAngles_temp( float pan, float tilt )
{
	fPan_Angle_temp.set( pan );
	fTilt_Angle_temp.set( tilt );
}

void CPanTiltData::set_PanTiltVelocities( float pan_vel, float tilt_vel )
{
	fPan_Velocity.set( pan_vel );
	fTilt_Velocity.set( tilt_vel );
}

void CPanTiltData::set_PanTiltVelocities_temp( float pan_vel, float tilt_vel )
{
	fPan_Velocity_temp.set( pan_vel );
	fTilt_Velocity_temp.set( tilt_vel );
}

void CPanTiltData::Update_Pan_Angle( void )
{
	fPan_Angle.set( fPan_Angle_temp.get() );
}

void CPanTiltData::Update_Tilt_Angle( void )
{
	fTilt_Angle.set( fTilt_Angle_temp.get() );
}

void CPanTiltData::Update_Pan_Velocity( void )
{
	fPan_Velocity.set( fPan_Velocity_temp.get() );
}

void CPanTiltData::Update_Tilt_Velocity( void )
{
	fTilt_Velocity.set( fTilt_Velocity_temp.get() );
}


void CPanTiltData::Update_All_Values( void )
{
	Update_Pan_Angle();
	Update_Tilt_Angle();
	Update_Pan_Velocity();
	Update_Tilt_Velocity();
}
