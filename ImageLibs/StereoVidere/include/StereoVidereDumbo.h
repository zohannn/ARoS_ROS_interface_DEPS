#ifndef _STEREO_VIDERE_DUMBO_H_
#define _STEREO_VIDERE_DUMBO_H_

#pragma once

#include "StereoVidere.h"

class CStereoVidereDumbo
	: public CStereoVidere
{
public:
	CStereoVidereDumbo( void );
	~CStereoVidereDumbo( void );

	//-----------------------------------------------------------------------
	// Coordinate transformation
public:
	void Compensate_Coordinates_Vision( float *tX, float *tY, float *tZ );

	// Not implemented
	static void CalculatePanTiltFromCoordinates( float X, float Y, float Z, float * pPan, float * pTilt ) {};

protected:
	void Load_Constant_Transformation_Parameters( void );

};

#endif // _STEREO_VIDERE_DUMBO_H_
