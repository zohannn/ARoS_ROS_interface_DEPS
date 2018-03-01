#ifndef BARRETTHAND_DEFINITIONS
#define BARRETTHAND_DEFINITIONS

#include <string>

//------------------------------------------------------------------------------------
// Define some conversion ratios to use in code, preventing unnecessary calculations

#ifndef BARRETTHAND_CONVERSION_RATIOS
#define BARRETTHAND_CONVERSION_RATIOS

#define BHAND_RATIO_3150_180	17.5 // 3150/180
#define BHAND_RATIO_17800_140	127.14285714285714285714285714286 // 17800/140
#define BHAND_RATIO_180_3150	0.05714285714285714285714285714286 // 180/3150
#define BHAND_RATIO_140_17800	0.00786516853932584269662921348315 // 140/17800

#endif //BARRETTHAND_CONVERSION_RATIOS

namespace BarrettHand
{
	namespace Finger
	{
		enum BH_DOF{
			Spread = 0,
			One,
			Two,
			Three,
			BARRETT_HAND_NUMBER_OF_DOF
		};

		static std::string Name[] =
		{
			"Spread",
			"Finger One",
			"Finger Two",
			"Finger Three",
			// .............
			"NULL"
		};
	}

	namespace Error
	{
		namespace Code{
			enum BarrettHand_Error_Codes{
				SUCCESS = 0,
				SERIAL_PORT_OPEN,
				SERIAL_PORT_CLOSE,
				SERIAL_PORT_READ,
				SERIAL_PORT_WRITE,
				PANTILT_REPLY,
				INVALID_FINGER_INDEX,
				STRING_PARSE,
				FINGER_ERROR_SPREAD,
				FINGER_ERROR_ONE,
				FINGER_ERROR_TWO,
				FINGER_ERROR_THREE,
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
			"!MODULE ERROR! - Barrett Hand reported an error in the reply: ",
			"!MODULE ERROR! - Invalid finger index: ",
			"!MODULE ERROR! - Received string from Barrett Hand incorrectly parsed.",
			"!MODULE ERROR! - Instruction to 'Spread' failed.",
			"!MODULE ERROR! - Instruction to 'Finger one' failed.",
			"!MODULE ERROR! - Instruction to 'Finger two' failed.",
			"!MODULE ERROR! - Instruction to 'Finger three' failed.",
			//------------------------------------------
			"NULL"
		};
	}
}


#endif // BARRETTHAND_DEFINITIONS
