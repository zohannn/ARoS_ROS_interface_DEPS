#ifndef _TIMERS_H_
#define _TIMERS_H_

#pragma once

#ifndef DEFAULT_TIMEOUT
#define DEFAULT_TIMEOUT 500 // ms
#endif
namespace Timer
{
	namespace VisionBot
	{
		namespace ID
		{
			enum _TIMER_ID_
			{
				FRAME_RATE = 0,
				OBJECT_INFO,
				ACTIONS,
				COMMON_SUB_GOALS,
				PANTILT,
				NUMBER_OF_TIMERS
			};
		}

		namespace Timeout
		{
			enum _TIMER_TIMEOUT_	//milliseconds
			{
				FRAME_RATE = DEFAULT_TIMEOUT,
				OBJECT_INFO = DEFAULT_TIMEOUT,
				PANTILT = DEFAULT_TIMEOUT,
				ACTIONS = DEFAULT_TIMEOUT,
				COMMON_SUB_GOALS = DEFAULT_TIMEOUT
			};
		}
	}

	namespace InfoDlg
	{
		namespace ID
		{
			enum _TIMER_ID_
			{
				OBJECT_INFO = 0,
				ACTIONS,
				COMMON_SUB_GOALS,
				NUMBER_OF_TIMERS
			};
		}

		namespace Timeout
		{
			enum _TIMER_TIMEOUT_	//milliseconds
			{
				OBJECT_INFO = DEFAULT_TIMEOUT,
				ACTIONS = DEFAULT_TIMEOUT,
				COMMON_SUB_GOALS = DEFAULT_TIMEOUT
			};
		}
	}
}

#endif // _TIMERS_H_
