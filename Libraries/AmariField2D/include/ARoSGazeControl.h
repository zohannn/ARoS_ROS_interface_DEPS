#ifndef _AROS_GAZE_CONTROL_H_
#define _AROS_GAZE_CONTROL_H_

#pragma once

#include "AmariField2D.hpp"

template <typename T>
class CARoSGazeControl : public CAmariField2D<T>
{
public:
	CARoSGazeControl( void );
	~CARoSGazeControl( void ) {};

	T InputAmplitude( T distance );

	void AddInput( CPoint<T> pantilt, T distance );

protected:
	CPoint<T> iSigma;

	T
		iK,			// Input parameter K
		iaA1,		//Input amplitude parameter A1
		iaBeta1;	//Input amplitude parameter Beta1

	CRange<T> Range_h;

};

#endif //_AROS_GAZE_CONTROL_H_
