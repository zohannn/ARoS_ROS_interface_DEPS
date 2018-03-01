#ifndef _COLOUR_OPERATIONS_H_
#define _COLOUR_OPERATIONS_H_

#pragma once

#include <opencv2/core/core.hpp>

namespace cv
{
	template <typename T>
	Point3_<T> convert_Pixel( Point3_<T> pix, int conversion_code );
}

#endif // _COLOUR_OPERATIONS_H_
