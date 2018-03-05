#include "cvEventHandlers.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "AttachMutex.hpp"
#include "VisionDefinitions.h"

void Update_Mouse_Position( int x, int y, void * usr_ptr )
{
	CAttachMutex_t< cv::Point3i > * mouse_pos = static_cast< CAttachMutex_t< cv::Point3i >* >( usr_ptr );

	if( mouse_pos == nullptr ) return;

	mouse_pos->lock();

	mouse_pos->x = x;
	mouse_pos->y = y;

	mouse_pos->unlock();

	mouse_pos = nullptr;
}

void Update_Text_Display_Mode( void * usr_ptr )
{
	CAttachMutex_t< cv::Point3i > * mouse_pos = static_cast< CAttachMutex_t< cv::Point3i >* >( usr_ptr );

	if( mouse_pos == nullptr ) return;

	mouse_pos->lock();

	if( mouse_pos->z == 0 )
	{
		mouse_pos->z = 1;
	}
	else
	{
		mouse_pos->z = 0;
		
		// Make sure the existing text is cleared and the default text appears on status bar
		cv::displayStatusBar( "Image from Grabber", "", 1 );
	}

	mouse_pos->unlock();

	mouse_pos = nullptr;
}

void onMouse( int nEvent, int x, int y, int, void * usr_ptr )
{
	switch( nEvent )
	{
#if( ( CV_VERSION_MAJOR >= 3 ) && !defined( CV_VERSION_EPOCH ) )
	case cv::EVENT_LBUTTONDOWN:
#else
	case CV_EVENT_LBUTTONDOWN:
#endif
		Update_Text_Display_Mode( usr_ptr );
		break;

#if( ( CV_VERSION_MAJOR >= 3 ) && !defined( CV_VERSION_EPOCH ) )
	case cv::EVENT_MOUSEMOVE:
#else
	case CV_EVENT_MOUSEMOVE:
#endif
		Update_Mouse_Position( x, y, usr_ptr );
		break;

	default:
		break;
	}
}
