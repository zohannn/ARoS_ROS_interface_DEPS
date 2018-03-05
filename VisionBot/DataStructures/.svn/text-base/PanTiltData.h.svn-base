#ifndef _PANTILT_DATA_H_
#define _PANTILT_DATA_H_

#pragma once

#include "AttachMutex.hpp"

class CPanTiltData
{
public:
	CPanTiltData( void );

public:
	CAttachMutex_n<int> nControlMode;

	CAttachMutex_n<float> 
		fPan_Angle,
		fTilt_Angle,
		fPan_Angle_temp,
		fTilt_Angle_temp,
		fPan_Velocity,
		fTilt_Velocity,
		fPan_Velocity_temp,
		fTilt_Velocity_temp;

//---------------------------------------------------------------------------
// Accessors
public:	
	void get_PanTiltAngles( float * pPan, float * pTilt );

	void set_PanTiltAngles( float pan, float tilt );

	void set_PanTiltAngles_temp( float pan, float tilt );

	void get_PanTiltVelocities( float * pPanVel, float * pTiltVel );

	void set_PanTiltVelocities( float pan_vel, float tilt_vel );

	void set_PanTiltVelocities_temp( float pan_vel, float tilt_vel );

//---------------------------------------------------------------------------
// Update information from temporary storage
public:
	void Update_Pan_Angle( void );

	void Update_Tilt_Angle( void );

	void Update_Pan_Velocity( void );

	void Update_Tilt_Velocity( void );

	void Update_All_Values( void );
};

#endif //_PANTILT_DATA_H_
