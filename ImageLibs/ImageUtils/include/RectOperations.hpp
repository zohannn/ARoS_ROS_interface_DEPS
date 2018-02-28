#ifndef _RECT_OPERATIONS_H_
#define _RECT_OPERATIONS_H_

#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc_c.h>

#include "MathOperations.h"

//---------------------------------------------------------------------------
// Apply scale to a rectangle from it's center
template <class T>
void scale_rectangle( float scale_x, float scale_y, cv::Rect_<T> * src, cv::Rect_<T> * dst )
{
	// Create a local copy of the source rectangle
	cv::Rect_<T> src_cpy( *src );

	dst->width = static_cast<T>( src_cpy.width * scale_x );
	dst->height = static_cast<T>( src_cpy.height * scale_y );

	T
		x = src_cpy.x + static_cast<T>( src_cpy.width*( 1.0f - scale_x )/2.0f ),
		y = src_cpy.y + static_cast<T>( src_cpy.height*( 1.0f - scale_y )/2.0f );

	dst->x = ( x<0 ) ? 0 : x;
	dst->y = ( y<0 ) ? 0 : y;
}

template <class T>
void scale_rectangle( float scale, cv::Rect_<T> * src, cv::Rect_<T> * dst )
{
	scale_rectangle( scale, scale, src, dst );
};

template <class T>
cv::Rect_<T> scale_rectangle( float scale_x, float scale_y, cv::Rect_<T> * src )
{
	cv::Rect_<T> dst;
	scale_rectangle( scale_x, scale_y, src, &dst );
	return dst;
};

template <class T>
cv::Rect_<T> scale_rectangle( float scale, cv::Rect_<T> * src )
{
	cv::Rect_<T> dst;
	scale_rectangle( scale, src, &dst );
	return dst;
}

//---------------------------------------------------------------------------
// Reimplement cvMaxRect to match c++ style in OpenCV
template <class T>
cv::Rect_<T> MaxRect( cv::Rect_<T> rect1, cv::Rect_<T> rect2 )
{
	// Type conversion required for function cvMaxRect
	cv::Rect 
		r1( rect1 ),
		r2( rect2 );

	return cvMaxRect( &( (CvRect ) r1 ), &( (CvRect) r2 ) );
}
	
//---------------------------------------------------------------------------
// Simple region validation, eliminates negative values
template <class T>
void ValidateRegion( cv::Rect_<T> * region )
{
	const T val_zero = static_cast<T>( 0 );

	if( region->x < val_zero )
	{
		if( region->width < region->x )
			region->width = val_zero;
		else
			region->width -= region->x;

		region->x = val_zero;
	}

	if( region->y < val_zero )
	{
		if( region->height < region->y )
			region->height = val_zero;
		else
			region->height -= region->y;

		region->y = val_zero;
	}
}

//---------------------------------------------------------------------------
// Verifies if rectangles 'rect1' and 'rect2' overlap
template <class T>
bool CheckOverlapRegion( cv::Rect_<T> * rect1, cv::Rect_<T> * rect2 )
{
	cv::Rect_<T> rMax = MaxRect( *rect1, *rect2 );

	T 
		nTotalWidth = rect1->width + rect2->width,
		nTotalHeight = rect1->height + rect2->height;

	return ( rMax.width < nTotalWidth ) && ( rMax.height < nTotalHeight );
}

//---------------------------------------------------------------------------
// Fits 'region' to limits, if any point is out of bounds defined by 'region_limits', truncate to border
template <class T>
void FitRegionToLimits( const cv::Rect_<T> region_limits, cv::Rect_<T> * region )
{
	// Pre validation of 'roi_limits' and 'region'
	cv::Rect_<T> 
		roi_limit( region_limits ),
		roi_region( *region );

	ValidateRegion( &roi_limit );
	ValidateRegion( &roi_region );

	//-----------------------------------------------------------------------
	// Upper Left and Lower Right points for limits and roi regions
	cv::Point_<T> 
		pt_roi_ul( roi_region.x, roi_region.y ),
		pt_roi_lr( roi_region.x + roi_region.width, roi_region.y + roi_region.height ),
		pt_limit_ul( roi_limit.x, roi_limit.y ),
		pt_limit_lr( roi_limit.x + roi_limit.width, roi_limit.y + roi_limit.height );

	//-----------------------------------------------------------------------

	FitValueToLimits( &pt_roi_ul.x, pt_limit_ul.x, pt_limit_lr.x );
	FitValueToLimits( &pt_roi_ul.y, pt_limit_ul.y, pt_limit_lr.y );

	FitValueToLimits( &pt_roi_lr.x, pt_limit_ul.x, pt_limit_lr.x );
	FitValueToLimits( &pt_roi_lr.y, pt_limit_ul.y, pt_limit_lr.y );

	region->x = pt_roi_ul.x;
	region->y = pt_roi_ul.y;
	region->width = abs( pt_roi_ul.x - pt_roi_lr.x );	// abs needs to be the one outside cv namespace
	region->height = abs( pt_roi_ul.y - pt_roi_lr.y );// abs needs to be the one outside cv namespace
}

#endif // _RECT_OPERATIONS_H_
