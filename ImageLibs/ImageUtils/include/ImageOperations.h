#ifndef _IMAGE_OPERATIONS_H_
#define _IMAGE_OPERATIONS_H_

#pragma once

#include <opencv2/core/core.hpp>

// Generic function to rotate and scale a cv::Mat image
// src -> Image to be rotated
// angle -> Angle of rotation in deg
// scale -> Scale factor to be applied to image. Smaller than 1, the image is reduced. Larger than 1, the image is increased
// rotation_point -> The point in image where the rotation will be applied. If (-1,-1) then, the center of the image is used
// returns -> The rotated image
cv::Mat rotate_scale_image( const cv::Mat& src, double angle, double scale = 1.0, cv::Point2f rotation_point = cv::Point2f( -1.0f, -1.0f ) );

// Rotate an image by 'angle' degrees in point 'rotation_point'
cv::Mat rotate_image( const cv::Mat& src, double angle, cv::Point2f rotation_point = cv::Point2f( -1.0f, -1.0f ) );

cv::Rect get_Region_All_Points( cv::Mat * mat_ptr );

cv::RotatedRect get_Region_All_Points_Rotated( cv::Mat * mat_ptr );
	
void morphology_open( const cv::Mat& src, cv::Mat& dst, int it_erode, int it_dilate );
	
void morphology_close( const cv::Mat& src, cv::Mat& dst, int it_erode, int it_dilate );

void morphology( const cv::Mat& src, cv::Mat& dst, int it_erode, int it_dilate, int op );

#endif //_IMAGE_OPERATIONS_H_
