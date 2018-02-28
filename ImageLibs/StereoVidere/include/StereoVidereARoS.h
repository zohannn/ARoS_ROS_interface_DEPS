#ifndef _STEREO_VIDERE_AROS_H_
#define _STEREO_VIDERE_AROS_H_

#pragma once

#include "StereoVidere.h"

class CStereoVidereARoS 
	: public CStereoVidere
{
public:
	CStereoVidereARoS( void );
	~CStereoVidereARoS( void );

	//-----------------------------------------------------------------------
	// Coordinate transformation
public:
	void Compensate_Coordinates_Vision( float *tX, float *tY, float *tZ );

	void Compensate_Coordinates_World( float *tX, float *tY, float *tZ );

protected:
	void Load_Constant_Transformation_Parameters( void );

	//-----------------------------------------------------------------------
public:	
	// Calculate pan and tilt angles from ( x, y, z ) coordinates
	static void CalculatePanTiltFromCoordinates( float X, float Y, float Z, float * pPan, float * pTilt );
};

#endif // _STEREO_VIDERE_AROS_H_
