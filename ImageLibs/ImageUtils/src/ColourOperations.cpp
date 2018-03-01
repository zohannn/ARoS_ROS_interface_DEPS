#include "ColourOperations.h"

#include <opencv2/imgproc/imgproc.hpp>

template <typename T>
cv::Point3_<T> cv::convert_Pixel( cv::Point3_<T> pix, int conversion_code )
{
	cv::Mat img_pixel( 1, 1, CV_8UC3 );
	
	img_pixel.at< cv::Point3_<T> >( 0, 0 ) = pix;
	
	cv::cvtColor( img_pixel, img_pixel, conversion_code );

	return img_pixel.at< cv::Point3_<T> >( 0, 0 );
}

//---------------------------------------------------------------------------
// Instantiate template for common types
template cv::Point3_<unsigned char> cv::convert_Pixel( cv::Point3_<unsigned char> pix, int conversion_code );
template cv::Point3_<unsigned int> cv::convert_Pixel( cv::Point3_<unsigned int> pix, int conversion_code );
