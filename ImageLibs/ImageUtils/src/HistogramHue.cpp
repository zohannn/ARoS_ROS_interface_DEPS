#include "HistogramHue.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "MathOperations.h"

CHistogramHue::CHistogramHue( int xrange, int yrange )
: CHistogram( xrange, yrange )
, nHue_Spectrum_Height( 20 )
{
	strWindowName_Histogram = "Histogram Hue";

	// override the declaration of img_hist size to make room for spectrum
	if( !img_hist.empty() )
		img_hist.release();

	img_hist.create( yrange + nHue_Spectrum_Height, xrange, CV_8UC3 );
	img_hist.setTo( 0 );

	//-----------------------------------------------------------------------
	// Generate an image with the full Hue spectrum to show in histogram
	img_hue_spectrum.create( nHue_Spectrum_Height, xrange, CV_8UC3 );
	img_hue_spectrum.setTo( 0 );

	for( int h=0 ; h<xrange ; h++ )
	{
		cv::line( 
			img_hue_spectrum, 
			cv::Point( h, 0 ), 
			cv::Point( h, nHue_Spectrum_Height ),
			cv::Scalar( h/2, 255, 255 )	);
	}

	cv::cvtColor( img_hue_spectrum, img_hue_spectrum, 
	#if( ( CV_VERSION_MAJOR >= 3 ) && !defined( CV_VERSION_EPOCH ) )
		cv::COLOR_HSV2BGR
	#else
		CV_HSV2BGR
	#endif
		);

	img_hue_spectrum.copyTo( img_hist( cv::Rect( 0, nYRange, nXRange, nHue_Spectrum_Height ) ) );
}

CHistogramHue::~CHistogramHue( void )
{
	if( !img_hue_spectrum.empty() )
		img_hue_spectrum.release();
}

void CHistogramHue::CalcHistogram( cv::Mat * img )
{
	int 
		chan[] = {0},
		size[] = {180};
	
	// hue varies from 0 to 179, see cvtColor
	float hranges[] = { 0, 180 };
	
	const float* ranges[] = { hranges };

	// Calculate the histogram
	cv::calcHist( 
		img,			// Image to analyze
		1,				// Number of channels
		chan,			// What channels
		cv::Mat(),		// do not use mask
		the_Histogram,	// Where to store the histogram
		1,				// Dimension
		size,			// Size
		ranges,			// Ranges
		true,			// the histogram is uniform
		false			// Don't accumulate
		);
}

void CHistogramHue::ShowHistogram( void )
{
	ConvertHistogramData();

	int nVal = 0;

	// Clear the Histogram region on the image, and preserve the hue spectrum.
	img_hist( cv::Rect( 0, 0, nXRange, nYRange ) ).setTo( 0 );

	size_t hist_size = vHistogramData.size();

	for( size_t h=0 ; h<hist_size ; h++ )
	{
		nVal = vHistogramData.at( h );

		cv::rectangle(
			img_hist,
			cv::Point( static_cast<int>( h ), img_hist.rows - nHue_Spectrum_Height ),
			cv::Point( static_cast<int>( h ), img_hist.rows - nVal - nHue_Spectrum_Height ),
			cv::Scalar::all( 255 ) );
	}

	cv::imshow( strWindowName_Histogram, img_hist );
}

void CHistogramHue::ConvertHistogramData( bool bNormalize )
{
	double dMaxVal = 0.0;
	
	// Determine the max value on the histogram
	cv::minMaxLoc(
		the_Histogram,	// Source array
		nullptr,			// Pointer to return a min value
		&dMaxVal			// Pointer to return a max value
		);

	float 
		fVal = 0.0f,
		fScale = 1.0f;

	// Scale factor to normalize the histogram
	if( bNormalize )
	{
		fScale = ( static_cast<float>( nYRange ) ) / ( static_cast<float>( dMaxVal ) );
	}

	for( int x=0 ; x<the_Histogram.size[0] ; x++ )
	{
		fVal = the_Histogram.at<float>( x );
		vHistogramData.at( 2*x ) = round<float>( fVal*fScale );
		vHistogramData.at( 2*x+1 ) = round<float>( fVal*fScale );
	}
}

void CHistogramHue::CopyData( std::vector<int> &vec_dst )
{
	int size = nXRange/2;
	vec_dst.assign( size, 0 );

	for( int x=0 ; x<size ; x++ )
		vec_dst.at( x ) = vHistogramData.at( 2*x );
}
