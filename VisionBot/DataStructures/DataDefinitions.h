#ifndef _DATA_DEFINITIONS_H_
#define _DATA_DEFINITIONS_H_

#pragma once

#include "common.h"
#include "Preprocessor_Macros.h"

//------------------------------------------------------------------------------
// Define task numbers and names

namespace Task
{
	enum _THE_TASK_
	{
		TEST = 0,
		CONSTRUCTION_TOY_STEREO,
		DRINKING,
		TUM_INTEGRATION,
		CONSTRUCTION_TOY_FULL_SVS,
		CONSTRUCTION_TOY_FULL_AVT,
		COLOUR_LEARNING,
		FACE_TRACKING,
		MOBILE_ROBOT_TARGET_ACQUISITION,
		TOY_ROBOT_LEARNING,
		PLAY_PIANO,
		HR_TRANSPORT,
		DIFF_KINEMATICS,
		COMPUTER_ASSEMBLY,
		// add more tasks above this line...
		// Be warned:
		// Don't forget to add a string with the corresponding name below, or the program will crash!!!
		NUMBER_OF_TASKS,
	};

	static std::string Task_Name[] =
	{
		"Test stuff",
		"Construction_Toy_Stereo",
		"Drinking",
		"TUM_Integration",
		"Construction Toy Robot Full (Stereo)",
		"Construction Toy Robot Full (AVT)",
		"Colour Learning",
		"Face Tracking",
		"Mobile Robot Target 1",
		"Toy Robot Learning",
		"Play Piano",
		"HR Transport",
		"Differential Kinematics",
		"Computer Assembly"
		// add the name of the tasks above this line...
		"NO_TASK_NAME"
	};

#define isValid_Task( task ) ( ( task>=0 ) && ( task<NUMBER_OF_TASKS ) )
#define GET_VALID_Task( task ) isValid_Task( task ) ? task : NUMBER_OF_TASKS

#define isValid_TaskName( task ) isIndexInRange( Task_Name, task )
#define GET_VALID_TaskName( task ) GET_VALID_ArrayItem( Task_Name, task )
}

//------------------------------------------------------------------------------
// Define how many elements are part of the histogram

#ifndef HIST_LENGTH
#define HIST_LENGTH 256
#endif

//------------------------------------------------------------------------------

typedef struct _MobileRobotTarget {
	float angle;
	float distance;
	bool exist;
} structMobileRobotTarget;

//------------------------------------------------------------------------------
// Communication definitions: ID and port names
namespace Comm
{
	enum _COMM_DST_
	{
		ID_Vision_Server = 0,
		ID_Pan_Tilt,
		// add more connections here...
		TOTAL_COMM_DST,
	};

	static std::string PortName[] =
	{
		"/vision",
		"/vision/pantilt"
	};

	static std::string PeerName[] =
	{
		"",
		"/pantilt"
	};

	static std::string ID[] =
	{
		"Vision Server",
		"Pan Tilt"
	};
}

#endif // _DATA_DEFINITIONS_H_
