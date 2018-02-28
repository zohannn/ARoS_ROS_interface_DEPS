#ifndef _GRABBER_DEFINITIONS_H_
#define _GRABBER_DEFINITIONS_H_

#pragma once

namespace GrabberType
{
	enum Type
	{
		SVS_Video = 0,
		SVS_Image_File,
		Common_Image_File,
		OpenCV_Camera,
		OpenCV_Video_File,
		YARP_Port,
		CMU1394,
		PS3Eye,
		NUM_OF_GRABBERS,
	};
}

#endif // _GRABBER_DEFINITIONS_H_
