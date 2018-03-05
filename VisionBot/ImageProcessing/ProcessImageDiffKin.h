#ifndef _PROCESS_IMAGE_DIFF_KIN_H_
#define _PROCESS_IMAGE_DIFF_KIN_H_

#pragma once

#include "ProcessImageToyRobotFull_SVS.h"

class CProcessImageDiffKin
	: public CProcessImageToyRobotFull_SVS
{
public:
	CProcessImageDiffKin( int width = 640, int height = 480 );

public:	
	bool SearchAllObjects( void );

	bool SearchObjects_Red( void );

	bool SearchObjects_Yellow( void );

	bool SearchObjects_Green( void );

	bool SearchObjects_Magenta( void );

	bool SearchObjects_Handle( void );

	void ObjectPreProcessing( void );

	void CalculateOrientation_Handle( CObject_f * pHandle );

	void ObjectPostProcessing( void );
};

#endif // _PROCESS_IMAGE_DIFF_KIN_H_
