#ifndef _GRASD_AMTEC_DEFINITIONS_H_
#define _GRASD_AMTEC_DEFINITIONS_H_

#include "common/m5apiw32.h"

namespace Amtec
{
	namespace Function_Error
	{
		enum _AMTEC_FUNCTION_ERROR_
		{
			ERROR_FUNCTIONNOTAVAILABLE	= ERRID_DEV_FUNCTIONNOTAVAILABLE,	//The function called is not available.
			ERROR_NOINITSTRING			= ERRID_DEV_NOINITSTRING,			//The InitString is missing during initialization.
			ERROR_NODEVICENAME			= ERRID_DEV_NODEVICENAME,			//The device name specified in InitString is wrong or invalid.
			ERROR_BADINITSTRING			= ERRID_DEV_BADINITSTRING,			//The InitString is incomplete or wrong.
			ERROR_INITERROR				= ERRID_DEV_INITERROR,				//Initialization of the interface failed. Check hardware and driver setup.
			ERROR_NOTINITIALIZED		= ERRID_DEV_NOTINITIALIZED,			//The function was called before initializing the device.
			ERROR_WRITEERROR			= ERRID_DEV_WRITEERROR,				//Error during an attempt to write data to the interface.
			ERROR_READERROR				= ERRID_DEV_READERROR,				//Error during an attempt to read data from the interface.
			ERROR_WRITETIMEOUT			= ERRID_DEV_WRITETIMEOUT,			//Timeout while sending data on the bus.
			ERROR_READTIMEOUT			= ERRID_DEV_READTIMEOUT,			//Timeout while reading data from a module.
			ERROR_WRONGMESSAGEID		= ERRID_DEV_WRONGMESSAGEID,			//The message received has an unexpected MessageID.
			ERROR_WRONGCOMMANDID		= ERRID_DEV_WRONGCOMMANDID,			//The message received has an unexpected CommandID.
			ERROR_WRONGPARAMETERID		= ERRID_DEV_WRONGPARAMETERID,		//The message received has an unexpected ParameterID.
			ERROR_EXITERROR				= ERRID_DEV_EXITERROR,				//Error occured while closing the interface.
			ERROR_NOMODULES				= ERRID_DEV_NOMODULES,				//No module found during initialization of the interface.
			ERROR_WRONGDEVICEID			= ERRID_DEV_WRONGDEVICEID,			//The given DeviceID is wrong.
			ERROR_NOLIBRARY				= ERRID_DEV_NOLIBRARY,				//A DLL file is missing to execute the function call.
			ERROR_ISINITIALIZED			= ERRID_DEV_ISINITIALIZED,			//The Interface has been already initialized.
			ERROR_WRONGEMSMODULEID		= ERRID_DEV_WRONGEMSMODULEID,		//The given EMS module ID does not exist.
			ERROR_EMSNOTINITIALIZED		= ERRID_DEV_EMSNOTINITIALIZED,		//The EMS module has not been initialized.
			ERROR_EMSMAXNUMBER			= ERRID_DEV_EMSMAXNUMBER,			//The maximum number of EMS modules has been reached.
			ERROR_EMSINITERROR			= ERRID_DEV_EMSINITERROR,			//Error initializing an EMS module.
			ERROR_WRONGEMSTYPE			= ERRID_DEV_WRONGEMSTYPE,			//This function is intended to use with a different EMS module type.
			ERROR_WRONGEMSCHANNELID		= ERRID_DEV_WRONGEMSCHANNELID,		//The given channel ID of the EMS module does not exist.
			ERROR_WRONGMP55MODULEID		= ERRID_DEV_WRONGMP55MODULEID,		//The given MP55 module ID does not exist.
			ERROR_WRONGSCHUNKMODULEID	= ERRID_DEV_WRONGSCHUNKMODULEID,	//The given SCHUNK module ID does not exist.
			ERROR_WRONGMODULEID			= ERRID_DEV_WRONGMODULEID,			//Wrong Module ID
			ERROR_MODULEERROR			= ERRID_DEV_MODULEERROR,			//Module Error
			ERROR_WAITTIMEOUT			= ERRID_DEV_WAITTIMEOUT,			//Wait Timeout
			ERROR_OPENINIFILE			= ERRID_DEV_OPENINIFILE				//Open Init File
		};
	}//end of namespace 'Function_Error'

	namespace Module
	{
		namespace State
		{
			enum _AMTEC_STATE_
			{
				STAT_ERROR				= STATEID_MOD_ERROR,			//An error occured in the module (Generic Error flag).
				STAT_HOME				= STATEID_MOD_HOME,				//The module was homed (Info).
				STAT_HALT				= STATEID_MOD_HALT,				//The module is in HALT state. Motion commands are not processed (Info).
				STAT_POWERFAULT			= STATEID_MOD_POWERFAULT,		//An error occured in the power bridge (Error).
				STAT_TOW_ERROR			= STATEID_MOD_TOW_ERROR,		//The drive was unable to follow the interpolated position. (Error).
				STAT_COMM_ERROR			= STATEID_MOD_COMM_ERROR,		//Problems in communication occured (Error).
				STAT_SWR				= STATEID_MOD_SWR,				//The Reference switch is active (Info).
				STAT_SW1				= STATEID_MOD_SW1,				//Limit switch 1 is active (Info).
				STAT_SW2				= STATEID_MOD_SW2,				//Limit switch 2 is active (Info).
				STAT_BRAKEACTIVE 		= STATEID_MOD_BRAKEACTIVE,		//The brake is active (Info).
				STAT_CURLIMIT			= STATEID_MOD_CURLIMIT,			//The PID loop output has reached the limit (Info).
				STAT_MOTION				= STATEID_MOD_MOTION,			//The module is in motion (Info).
				STAT_RAMP_ACC			= STATEID_MOD_RAMP_ACC,			//The module is in a phase of acceleration (Info). Only valid for ramp motion commands.
				STAT_RAMP_STEADY		= STATEID_MOD_RAMP_STEADY,		//The module is in a phase of steady speed (Info). Only valid for ramp motion commands.
				STAT_RAMP_DEC			= STATEID_MOD_RAMP_DEC,			//The module is in a phase of deceleration (Info). Only valid for ramp motion commands.
				STAT_RAMP_END			= STATEID_MOD_RAMP_END,			//The module has reached the target position (Info). Only valid for ramp motion commands.
				STAT_INPROGRESS			= STATEID_MOD_INPROGRESS,		//The module processes an actual Step motion command. (Info).
				STAT_FULLBUFFER			= STATEID_MOD_FULLBUFFER,		//A Step motion command is waiting in the message buffer (Info).
				STAT_POW_VOLT_ERR		= STATEID_MOD_POW_VOLT_ERR,		//A voltage drop occured (Error).
				STAT_POW_FET_TEMP		= STATEID_MOD_POW_FET_TEMP,		//A temperature problem occured in the power bridge (Error).
				STAT_POW_WDG_TEMP		= STATEID_MOD_POW_WDG_TEMP,		//A temperature problem occured in the motor (Error).
				STAT_POW_SHORTCUR		= STATEID_MOD_POW_SHORTCUR,		//A short current occured (Error).
				STAT_POW_HALLERR		= STATEID_MOD_POW_HALLERR,		//A communication error occured (Error).
				STAT_POW_INTEGRALERR	= STATEID_MOD_POW_INTEGRALERR,	//An error according to the integral criteria occured (current too high over time – Error).
				STAT_CPU_OVERLOAD		= STATEID_MOD_CPU_OVERLOAD,		//The CPU is too busy (Error).
				STAT_BEYOND_HARD		= STATEID_MOD_BEYOND_HARD,		//The module has reached the hard limit switch (Error).
				STAT_BEYOND_SOFT		= STATEID_MOD_BEYOND_SOFT,		//The module has reached the soft limit switch (Error).
				STAT_POW_SETUP_ERR		= 0x08000000L					//Error in initializing the current controller. Module settings disaccord with controller configuration (5A/10A types). Power must be switched off.
			};
		}//end of namespace 'State'

		namespace Configuration
		{
			enum _AMTEC_CONFIGURATION_
			{
				CONF_BRAKE_PRESENT			= CONFIGID_MOD_BRAKE_PRESENT,		//1 = Brake present
				CONF_BRAKE_AT_POWERON		= CONFIGID_MOD_BRAKE_AT_POWERON,	//0 = Brake will be released at Power On
				CONF_SWR_WITH_ENCODERZERO	= CONFIGID_MOD_SWR_WITH_ENCODERZERO,//1 = Encoder Index used for Homing
				CONF_SWR_AT_FALLING_EDGE	= CONFIGID_MOD_SWR_AT_FALLING_EDGE,	//1 = Homing finishes on falling edge of homing switch
				CONF_CHANGE_SWR_TO_LIMIT	= CONFIGID_MOD_CHANGE_SWR_TO_LIMIT,	//1 = Homing switch converts to limit switch after Homing is finished
				CONF_SWR_ENABLED			= CONFIGID_MOD_SWR_ENABLED,			//1 = Homing switch is enabled
				CONF_SWR_LOW_ACTIVE			= CONFIGID_MOD_SWR_LOW_ACTIVE,		//1 = Homing switch is low active
				CONF_SWR_USE_EXTERNAL		= CONFIGID_MOD_SWR_USE_EXTERNAL,	//1 = The external homing switch will be used
				CONF_SW1_ENABLED			= CONFIGID_MOD_SW1_ENABLED,			//1 = Limit switch //1 is enabled
				CONF_SW1_LOW_ACTIVE			= CONFIGID_MOD_SW1_LOW_ACTIVE,		//1 = Limit switch //1 is low active
				CONF_SW1_USE_EXTERNAL		= CONFIGID_MOD_SW1_USE_EXTERNAL,	//1 = The external limit switch //1 will be used
				CONF_SW2_ENABLED			= CONFIGID_MOD_SW2_ENABLED,			//1 = Limit switch 2 is enabled
				CONF_SW2_LOW_ACTIVE			= CONFIGID_MOD_SW2_LOW_ACTIVE,		//1 = Limit switch 2 is low active
				CONF_SW2_USE_EXTERNAL		= CONFIGID_MOD_SW2_USE_EXTERNAL,	//1 = The external limit switch 2 will be used
				CONF_LINEAR					= CONFIGID_MOD_LINEAR,				//1 = Module is of linear type
				CONF_ALLOW_FULL_CUR			= CONFIGID_MOD_ALLOW_FULL_CUR,		//0 = The max. cur commanded with PCube_moveCur will be limited to the nominal current.
				CONF_M3_COMPATIBLE			= CONFIGID_MOD_M3_COMPATIBLE,		//1 = Module is MoRSE3 compatible. This concerns CAN communication and behaviour of PCube_moveStep. The module does not accept motion commands unless Homing is finished successfully.
				CONF_LINEAR_SCREW			= CONFIGID_MOD_LINEAR_SCREW,		//1 = Module is linear module with ball screw actuator.
				CONF_DISABLE_ON_HALT		= CONFIGID_MOD_DISABLE_ON_HALT,		//1 = On error the motor is set to zero current.
				CONF_WATCHDOG_ENABLE		= CONFIGID_MOD_WATCHDOG_ENABLE,		//1 = Watchdog is enabled. The watchdog starts after reception of the first life sign from control (PCube_serveWatchdogAll).
				CONF_ZERO_MOVE_AFTER_HOK	= CONFIGID_MOD_ZERO_MOVE_AFTER_HOK,	//1 = After Homing is finished the module automatically moves to ist zero position
				CONF_DISABLE_ACK			= CONFIGID_MOD_DISABLE_ACK,			//1 = Messages are not acknowledged anymore. Get commands will still be answered. Valid only for CAN-Bus.
				CONF_SYNC_MOTION			= CONFIGID_MOD_SYNC_MOTION			//1 = Enables synchronized Motion commands. After sending the motion command the a special Start Motion broadcast is expected (PCube_startMotionAll). Valid only for CAN-Bus.
			};
		}//end of namespace 'Configuration'
		
		namespace Setup
		{
			enum _AMTEC_SETUP_
			{
				SETUP_ENCODER_FEEDBACK	= SETUPID_MOD_ENCODER_FEEDBACK,		//not used
				SETUP_RESOLVER_FEEDBACK	= SETUPID_MOD_RESOLVER_FEEDBACK,	//not used
				SETUP_ABSOLUTE_FEEDBACK	= SETUPID_MOD_ABSOLUTE_FEEDBACK,	//not used
				SETUP_4IN_4OUT			= SETUPID_MOD_4IN_4OUT,				//1 = The //15pole connector is configured for 4 I/O signals.
				SETUP_3IN_ENCODER_IN	= SETUPID_MOD_3IN_ENCODER_IN,		//1 = The //15pole connector is configured for encoder input.
				SETUP_3IN_ENCODER_OUT	= SETUPID_MOD_3IN_ENCODER_OUT,		//1 = The //15pole connector is configured for encoder output.
				SETUP_RS232				= SETUPID_MOD_RS232,				//1 = The module is configured for RS232-communication.
				SETUP_CAN				= SETUPID_MOD_CAN,					//1 = The module is configured for CAN-communication.
				SETUP_PROFIBUS			= SETUPID_MOD_PROFIBUS,				//1 = The module is configured for Profibus-communication
				SETUP_USE_M3ID			= SETUPID_MOD_USE_M3ID,				//1 = CAN identifiers for MoRSE3 modules are activated.
				SETUP_USE_M4ID			= SETUPID_MOD_USE_M4ID,				//1 = CAN identifiers for MoRSE4 modules are activated.
				SETUP_USE_CANOPEN		= SETUPID_MOD_USE_CANOPEN,			//1 = The module is configured for CANopen.
				SETUP_USE_SW2_AS_ENABLE	= SETUPID_MOD_USE_SW2_AS_ENABLE,	//1 = The input for limit switch 2 is used as enable signal for the drive.
				SETUP_USE_SW2_AS_BRAKE	= SETUPID_MOD_USE_SW2_AS_BRAKE,		//1 = The input for limit switch 2 is used to release the brake.
				SETUP_ERROR_TO_OUT0		= SETUPID_MOD_ERROR_TO_OUT0			//1 = An error is signalized on output 0.
			};
		}//end of namespace 'Setup'

		

		namespace Type
		{
			enum _AMTEC_TYPE_
			{
				TYPE_ROTARY = TYPEID_MOD_ROTARY,	//Rotary module
				TYPE_LINEAR = TYPEID_MOD_LINEAR		//Linear module
			};
		}//end of namespace 'Type'
	
	}//end of namespace 'Module'
	
	namespace Error
	{
		enum _AMTEC_ERROR_
		{
			ERROR_NONE = 0,			//no error
			ERROR_STATE = -1,		//state error in modules
			ERROR_OTHER = -2,		//when that is not a error in function neither in state.

		};
	}//end of namespace 'Error'

}//end of namespace 'Amtec'

#endif //_GRASD_AMTEC_DEFINITIONS_H_