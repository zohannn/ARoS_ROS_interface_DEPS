#ifndef _HISTOGRAM_HUE_H_
#define _HISTOGRAM_HUE_H_

#pragma once

#include "Histogram.h"

class CHistogramHue :
	public CHistogram
{
public:
	CHistogramHue( int xrange=360, int yrange=255 );
	~CHistogramHue( void );

protected:
	cv::Mat img_hue_spectrum;

	const int nHue_Spectrum_Height;

public:
	virtual void CalcHistogram( cv::Mat * img );
	virtual void ShowHistogram( void );

	virtual void CopyData( std::vector<int> &vec_dst );

	virtual void ConvertHistogramData( bool bNormalize = true );
};

#endif // _HISTOGRAM_HUE_H_
