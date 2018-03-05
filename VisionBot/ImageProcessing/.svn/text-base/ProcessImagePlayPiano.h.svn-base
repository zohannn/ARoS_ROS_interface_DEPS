#ifndef _PROCESS_IMAGE_PLAY_PIANO_H_
#define _PROCESS_IMAGE_PLAY_PIANO_H_

#pragma once

#include "ProcessImage.h"
#include "HistogramHue.h"

#ifndef HIST_LENGTH_PIANO
#define HIST_LENGTH_PIANO 360
#endif

class CProcessImagePlayPiano
	: public CProcessImage
{
public:
	CProcessImagePlayPiano( int width = 640, int height = 480 );

public:
	void ProcessTask( cv::Mat * mat_ptr );

	void ObjectPreProcessing( void );

	void Action_Interpretation( void );

	bool SearchAllObjects( void );

protected:
	CHistogramHue histogram;

	std::vector< unsigned int > vColourTask;

	unsigned int uColoursTask;
};

#endif //_PROCESS_IMAGE_PLAY_PIANO_H_
