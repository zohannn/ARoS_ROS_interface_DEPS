#ifndef PANTILTWRAPPER_DEFS_H_
#define PANTILTWRAPPER_DEFS_H_

#pragma once

namespace PanTiltWrapper
{
	namespace Error
	{
		namespace Code{
			enum PanTilt_Wrapper_Error_Codes{
				SUCCESS = 0,
				CONFIG_FILE,
				DRIVER_OPEN,
				DRIVER_INVALID,
				COMMAND_EXEC,
				INSUFICIENT_FDATA,
				INVALID_COMMAND,
				//------------------------------------------
				NUMBER_OF_ERRORS
			};
		}

		static std::string Text[] =
		{
			"No Error",
			"Unable to load PanTilt module configuration file: ",
			"Error opening Device Driver!",
			"Invalid Device Driver!",
			"Error executing command: ",
			"Insuficient number of fdata values",
			"The command is invalid",
			//------------------------------------------
			"NULL"
		};
		
		static std::string Wrapper_Error_Prefix = "WRAPPER ERROR! - ";
	}
}

#endif
