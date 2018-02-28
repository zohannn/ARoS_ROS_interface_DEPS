#ifndef _HISTOGRAM_H_
#define _HISTOGRAM_H_

#pragma once

#include <opencv2/core/core.hpp>
#include <vector>

class CHistogram
{
public:
	CHistogram( int xrange = 0, int yrange = 0 );
	~CHistogram( void );

public:
	cv::Mat 
		img_src,
		img_hist;

	std::string strWindowName_Histogram;

protected:
	int 
		nXRange,
		nYRange;

	cv::MatND the_Histogram;

	std::vector<int> vHistogramData;

public:
	int get_XRange( void );
	int get_YRange( void );

	void set_XRange( int xrange );
	void set_YRange( int yrange );

	virtual void CalcHistogram( cv::Mat * img ){};
	virtual void ShowHistogram( void ){};

	virtual void CopyData( std::vector<int> &vec_dst );

	virtual void ConvertHistogramData( bool bEqualize = true );
};

#endif // _HISTOGRAM_H_
