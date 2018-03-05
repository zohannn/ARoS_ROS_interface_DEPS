#ifndef _PROCESS_IMAGE_TOY_ROBOT_LEARNING_H_
#define _PROCESS_IMAGE_TOY_ROBOT_LEARNING_H_

#pragma once

#include "ProcessImageToyRobotFull_SVS.h"

class CProcessImageToyRobotLearning 
	: public CProcessImageToyRobotFull_SVS
{
public:
	CProcessImageToyRobotLearning( int width = 640, int height = 480 );

public:	
	void ObjectPreProcessing( void );
	
	void Action_Interpretation( void );

	bool SearchObjects_Hand( void );

	bool SearchObjects_Top_Floor( void );

	bool SearchAllObjects( void );
};

#endif // _PROCESS_IMAGE_TOY_ROBOT_LEARNING_H_
