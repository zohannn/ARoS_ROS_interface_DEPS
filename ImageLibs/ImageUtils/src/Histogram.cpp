#include "Histogram.h"

#include <opencv2/imgproc/imgproc.hpp>

CHistogram::CHistogram( int xrange, int yrange )
: nXRange( xrange )
, nYRange( yrange )
, strWindowName_Histogram( "Histogram" )
, vHistogramData( nXRange, 0 )
, img_hist( yrange, xrange, CV_8UC3 )
{
}

CHistogram::~CHistogram( void )
{
	if( !img_hist.empty() )
		img_hist.release();

	if( !img_src.empty() )
		img_src.release();

	if( !the_Histogram.empty() )
		the_Histogram.release();
	
	vHistogramData.clear();
}

void CHistogram::CopyData( std::vector<int> &vec_dst )
{
	vec_dst.assign( vHistogramData.begin(), vHistogramData.end() );
}

void CHistogram::ConvertHistogramData( bool bEqualize )
{
	if( bEqualize )
	{
		//Equalize the histogram
		cv::equalizeHist( (cv::Mat) the_Histogram, (cv::Mat) the_Histogram );
	}

	for( int x=0 ; x<the_Histogram.size[0] ; x++ )
	{
		vHistogramData.at( x ) = the_Histogram.at<int>( x );
	}
}

int CHistogram::get_XRange( void )
{
	return nXRange;
}

int CHistogram::get_YRange( void )
{
	return nYRange;
}

void CHistogram::set_XRange( int xrange )
{
	nXRange = xrange;
}

void CHistogram::set_YRange( int yrange )
{
	nYRange = yrange;
}
