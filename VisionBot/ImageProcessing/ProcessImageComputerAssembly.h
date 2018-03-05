#ifndef _PROCESS_IMAGE_COMPUTERASSEMBLY_H_
#define _PROCESS_IMAGE_COMPUTERASSEMBLY_H_

#pragma once

#include "ProcessImage.h"
#include "TimeCountOpenCV.h"

class CProcessImageComputerAssembly 
	: public CProcessImage
{
public:
	CProcessImageComputerAssembly( int width = 640, int height = 480 );

public:
	bool SearchAllObjects( void );

	bool SearchObjects_ScrewBox( void );

	bool SearchObjects_PowerSupply( void );

	bool SearchObjects_Cable( void );

	bool SearchObjects_Hand( void );

	bool SearchObjects_SecondHand(void);

	void ObjectPreProcessing( void );
	
	void ObjectPostProcessing( void );

	// Process the image and retrieve data
	void ProcessTask( cv::Mat * mat_ptr );

protected:
	unsigned int isScrewBoxPresent( void );
	unsigned int isPowerSupplyPresent( void );
	unsigned int isCablePresent(void);

	unsigned int isScrewBox_in_HandOverPosition(void);
	unsigned int isPowerSupply_in_HandOverPosition(void);
	unsigned int isCable_in_HandOverPosition(void);
	unsigned int isObject_Lifted(int iOBJECT, float fHeight);
	unsigned int isObject_in_HandOverPosition(int iOBJECT, std::vector<float> fPosition, float fRadius);
	unsigned int isHumanHand_in_HandOverPosition(void);
	unsigned int isSecondHumanHand_in_HandOverPosition(void);
	unsigned int isHumanHand_Lifted();
	unsigned int isSecondHumanHand_Lifted();

	bool bScrewBoxPresent;
	bool bPowerSupplyPresent;
	bool bCablePresent;

	bool bmemorymode;

	
	// How much time the information is going to be kept
	double dTime_to_Retain_Information;

	CTimeCountBasicOpenCV 
		Timer_Reach_Glass,
		Timer_Release_Glass,
		Timer_Reach_Glass_Inverted,
		Timer_Release_Glass_Inverted,
		Timer_Reach_Bottle,
		Timer_Release_Bottle;
};

#endif // _PROCESS_IMAGE_COMPUTERASSEMBLY_H_
