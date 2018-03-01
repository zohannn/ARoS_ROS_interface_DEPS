#ifndef PANTILT_DEFINITIONS
#define PANTILT_DEFINITIONS

#include <string>

namespace PanTilt
{
	namespace Axis
	{
		namespace ID{
			enum PT_AXES{
				PAN = 0,
				TILT,
				PANTILT_NUMBER_OF_AXES
			};
		}

		static std::string Name[] =
		{
			"Pan",
			"Tilt",
			// .............
			"NULL"
		};
	}

	namespace ControlMode
	{
		enum _CONTROL_MODE_
		{
			POSITION = 0,
			VELOCITY
		};
	}

	namespace Error
	{
		namespace Code{
			enum PanTilt_Error_Codes{
				SUCCESS = 0,
				SERIAL_PORT_OPEN,
				SERIAL_PORT_CLOSE,
				SERIAL_PORT_READ,
				SERIAL_PORT_WRITE,
				PANTILT_REPLY,
				INVALID_AXIS_INDEX,
				STRING_PARSE,
				AXIS_ERROR_PAN,
				AXIS_ERROR_TILT,
				AXES_ERROR_PANTILT,
				//------------------------------------------
				NUMBER_OF_ERRORS
			};
		}

		static std::string Text[] =
		{
			"No Error",
			"!MODULE ERROR! - Serial port failed to open.",
			"!MODULE ERROR! - Serial port failed to close.",
			"!MODULE ERROR! - Read from serial port failed.",
			"!MODULE ERROR! - Write in serial port failed.",
			"!MODULE ERROR! - Pan-Tilt reported an error in the reply: ",
			"!MODULE ERROR! - Invalid axis index: ",
			"!MODULE ERROR! - Received string from Pan-Tilt incorrectly parsed.",
			"!MODULE ERROR! - Instruction to axis 'Pan' failed.",
			"!MODULE ERROR! - Instruction to axis 'Tilt' failed.",
			"!MODULE ERROR! - Instruction to axes 'Pan' and 'Tilt' failed.",
			//------------------------------------------
			"NULL"
		};
	}
}

#ifndef PANTILT_CONTROL_STRINGS
	#define PANTILT_CONTROL_STRINGS
	#define PANTILT_WAIT_STRING "\r\n"
	#define PANTILT_WAIT_STRING_SIZE 2
#endif // PANTILT_CONTROL_STRINGS

#ifndef MAX_SIZE_REPLY_STRING
	#define MAX_SIZE_REPLY_STRING 128
#endif

#ifndef PT_LIM
#define PT_LIM
	enum _PT_LIM_
	{
		MIN = 0,
		MAX
	};
#endif
#endif // PANTILT_DEFINITIONS
