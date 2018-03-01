#ifndef _VISION_DATA_TYPES_H_
#define _VISION_DATA_TYPES_H_

#pragma once

#include <opencv2/core/core.hpp>

enum _MORPH_OPERATIONS_{
	MORPH_OPERATION_OPEN = 1,
	MORPH_OPERATION_CLOSE,
};

namespace Seg
{
	enum POS
	{
		MIN = 0,
		MAX,
	};

	enum MORPH
	{
		OP = 0,
		ERODE,
		DILATE,
	};
};

// Define the type Pixel3 as a 3 coordinates point of unsigned char, to work with images more transparently
typedef cv::Point3_<unsigned char> Pixel3;
typedef std::pair< Pixel3, Pixel3 > Pixel_Pair;

#endif // _VISION_DATA_TYPES_H_
