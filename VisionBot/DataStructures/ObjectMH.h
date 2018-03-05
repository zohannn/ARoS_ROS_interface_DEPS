#ifndef _OBJECT_MH_H_
#define _OBJECT_MH_H_

#pragma once

#include "MotionHistoryCartesian.h"
#include "Object_T.h"

class CObjectMH
	: public CObject_f
	, public CMotionHistoryCartesian
{
public:
	CObjectMH( void );
	CObjectMH( unsigned int type );
	CObjectMH( unsigned int type, int x, int y, int width, int height );
	CObjectMH( unsigned int type, cv::Rect region );
	CObjectMH( const CObjectMH& obj );

	bool operator==( CObjectMH obj );
	bool operator==( int id );
};

#endif // _OBJECT_MH_H_
