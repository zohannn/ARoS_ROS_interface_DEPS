#include "ImageOperations.h"

#include <opencv2/imgproc/imgproc.hpp>
#include "VisionDefinitions.h"

cv::Mat rotate_scale_image( const cv::Mat& src, double angle, double scale, cv::Point2f rotation_point )
{
	cv::Point2f rot_pt( 0.0f, 0.0f );

	if( ( rotation_point.x >= 0.0f ) && ( rotation_point.y >= 0.0f ) )
	{
		rot_pt = rotation_point;
	}
	else
	{
		rot_pt = cv::Point2f( src.cols / 2.0f, src.rows / 2.0f );
	}

	cv::Mat
		dst( src.rows, src.cols, src.type(), cv::Scalar::all( 0.0 ) ),
		rotation_matrix = cv::getRotationMatrix2D( rot_pt, angle, scale );

	cv::warpAffine( src, dst, rotation_matrix, src.size() );

	return dst;
}

cv::Mat rotate_image( const cv::Mat& src, double angle, cv::Point2f rotation_point )
{
	return rotate_scale_image( src, angle, 1.0, rotation_point );
}

cv::Rect get_Region_All_Points( cv::Mat * mat_ptr )
{
	return get_Region_All_Points_Rotated( mat_ptr ).boundingRect();
}

cv::RotatedRect get_Region_All_Points_Rotated( cv::Mat * mat_ptr )
{
	if( mat_ptr->empty() )
		return cv::RotatedRect( cv::Point2f( 0.0f, 0.0f ), cv::Size2f( 0.0f, 0.0f ), 0.0f );

	// Find the region of the movement in the image
	// Create a copy of the gray image to be used in the findContours function
	cv::Mat mat_hull( mat_ptr->rows, mat_ptr->cols, CV_8UC1 );
	mat_ptr->copyTo( mat_hull );

	// Find the contours
	std::vector< std::vector< cv::Point > > vec_contours;

	cv::findContours( mat_hull, vec_contours, 
	#if( ( CV_VERSION_MAJOR >= 3 ) && !defined( CV_VERSION_EPOCH ) )
		cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE
	#else
		CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE
	#endif
		);

	// If there are no contours, exit
	if( vec_contours.size() < 1 ) 
		return cv::RotatedRect( cv::Point2f( 0.0f, 0.0f ), cv::Size2f( 0.0f, 0.0f ), 0.0f );

	// Merge the points of all contours into one vector
	std::vector< cv::Point > contour_pts;
	size_t contour_size = vec_contours.size();
	for( size_t i=0 ; i<contour_size ; i++ )
	{
		contour_pts.insert( contour_pts.end(), vec_contours.at( i ).begin(), vec_contours.at( i ).end() );
	}

	// Get the bounding box for all points
	std::vector< cv::Point > hull;
	cv::convexHull( cv::Mat( contour_pts ), hull );

	return minAreaRect( cv::Mat( hull ) );
}

void morphology_open( const cv::Mat& src, cv::Mat& dst, int it_erode, int it_dilate )
{
	cv::erode(
		src,				// Source image
		dst,				// Destiny image
		cv::Mat(),			// Kernel, by default 3x3
		cv::Point( -1, -1 ),// Anchor point, (-1,-1) indicates the center
		it_erode			// Number of iterations
		);

	cv::dilate(
		dst,				// Source image
		dst,				// Destiny image
		cv::Mat(),			// Kernel, by default 3x3
		cv::Point( -1, -1 ),// Anchor point, (-1,-1) indicates the center
		it_dilate			// Number of iterations
		);
}

void morphology_close( const cv::Mat& src, cv::Mat& dst, int it_erode, int it_dilate )
{
	cv::dilate(
		src,				// Source image
		dst,				// Destiny image
		cv::Mat(),			// Kernel, by default 3x3
		cv::Point( -1, -1 ),// Anchor point, (-1,-1) indicates the center
		it_dilate			// Number of iterations
		);

	cv::erode(
		dst,				// Source image
		dst,				// Destiny image
		cv::Mat(),			// Kernel, by default 3x3
		cv::Point( -1, -1 ),// Anchor point, (-1,-1) indicates the center
		it_erode			// Number of iterations
		);
}

void morphology( const cv::Mat& src, cv::Mat& dst, int it_erode, int it_dilate, int op )
{
	switch( op )
	{
	case MORPH_OPERATION_CLOSE:
		morphology_close( src, dst, it_erode, it_dilate );
		break;

	default:
	case MORPH_OPERATION_OPEN:
		morphology_open( src, dst, it_erode, it_dilate );
		break;
	}
}
