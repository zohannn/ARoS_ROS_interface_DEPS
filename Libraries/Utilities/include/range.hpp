#ifndef _RANGE_HPP_
#define _RANGE_HPP_

#pragma once

#include <opencv2/core/core.hpp>

template <typename T>
bool in_range( const T &val, const std::pair< T, T > &range )
{
	return ( val >= range.first ) && ( val <= range.second );
}

template <typename T>
bool in_range( const cv::Point_<T> &pt, const std::pair< cv::Point_<T>, cv::Point_<T> > &range )
{
	return
		in_range( pt.x, std::pair<T, T>( range.first.x, range.second.x ) ) &&
		in_range( pt.y, std::pair<T, T>( range.first.y, range.second.y ) );
}

template <typename T>
bool in_range( const cv::Point3_<T> &pt, const std::pair< cv::Point3_<T>, cv::Point3_<T> > &range )
{
	return
		in_range( pt.x, std::pair<T, T>( range.first.x, range.second.x ) ) &&
		in_range( pt.y, std::pair<T, T>( range.first.y, range.second.y ) ) &&
		in_range( pt.z, std::pair<T, T>( range.first.z, range.second.z ) );
}

#endif // _RANGE_HPP_
