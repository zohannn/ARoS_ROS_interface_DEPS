#ifndef _PROCESS_IMAGE_TOY_ROBOT_FULL_SVS_H_
#define _PROCESS_IMAGE_TOY_ROBOT_FULL_SVS_H_

#pragma once

#include "ProcessImage.h"
#include "GestureClassifier.h"

class CProcessImageToyRobotFull_SVS
	: public CProcessImage
{
public:
	CProcessImageToyRobotFull_SVS( int width = 640, int height = 480 );
	virtual ~CProcessImageToyRobotFull_SVS( void ){};

	//--------------------------------------------------------------------------
	// Search for objects
public:
	virtual bool SearchAllObjects( void );

	bool SearchObjects_Base( void );

	bool SearchObjects_Column( void );

	bool SearchObjects_Wheel( void );

	bool SearchObjects_Nut( void );

	virtual bool SearchObjects_Hand( void );

	virtual bool SearchObjects_Top_Floor( void );

	bool SearchObjects_Auxiliary( void );

	//--------------------------------------------------------------------------
public:	
	virtual void ObjectPreProcessing( void );
	
	virtual void ObjectPostProcessing( void );

	// Process the image and retrieve data
	void ProcessTask( cv::Mat * img_ptr );

protected:
	void CalculateOrientation_Base( CObject_f * pBase );
	void CalculateOrientation_Column( CObject_f * pColumn );

	virtual void Action_Interpretation( void );

	void Check_Objects_Inserted_in_Base( void );
	void Check_Columns_Insertion( void );
};

#endif // _PROCESS_IMAGE_TOY_ROBOT_FULL_SVS_H_
