#ifndef _PROCESS_IMAGE_HR_TRANSPORT_H_
#define _PROCESS_IMAGE_HR_TRANSPORT_H_

#pragma once

#include "ProcessImage.h"

class CProcessImageHRTransport
	: public CProcessImage
{
public:
	CProcessImageHRTransport( int width = 640, int height = 480 );

public:	
	void ProcessTask( cv::Mat * mat_ptr );

	void ObjectPreProcessing( void );
	
	void Action_Interpretation( void );

	bool SearchAllObjects( void );

protected:
	std::vector< unsigned int > vColourTask;

};

#endif // _PROCESS_IMAGE_HR_TRANSPORT_H_
