#ifndef _OBJECT_BASIC_H_
#define _OBJECT_BASIC_H_

#pragma once 

#include <opencv2/core/core.hpp>

class CObjectBasic
{
public:
	CObjectBasic( void );

	CObjectBasic( unsigned int type );

	CObjectBasic( unsigned int type, cv::Point2i pt_i, cv::Point3f pt_w );

	CObjectBasic( unsigned int type, int i_X, int i_Y, float w_X, float w_Y, float w_Z );

	CObjectBasic( const CObjectBasic& obj );

	CObjectBasic& operator=( CObjectBasic obj );

	unsigned int uType;
	cv::Point2i Coord_Image;
	cv::Point3f Coord_World;
};

#endif // _OBJECT_BASIC_H_
