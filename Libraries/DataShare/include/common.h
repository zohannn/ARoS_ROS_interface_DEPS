#ifndef _COMMON_H_
#define _COMMON_H_

#pragma once

#include <array>
#include <string>

#define _R_ 0
#define _G_ 1
#define _B_ 2
#define _C_ 3
#define _M_ 4
#define _Y_ 5
#define _W_ 6
#define _K_ 7
#define _GR_ 8
#define _O_ 9
#define _BE_ 10
#define _BR_ 11

#define RIGHT_ARM 0
#define LEFT_ARM 1

//------------------------------------------------------------------------------

namespace ActionExecutionLayer_ToyVehicle
{
	enum ActionExecutionLayerEnum
	{
		RW_AG_I_AEL=0,		//Reach wheel in above grip and insert
		RN_SG_I_AEL,		//Reach OBJECT_NUT in side grip and insert
		RCol1_AG_I_AEL,		//Reach column 1 in above grip and insert
		RCol2_AG_I_AEL,		//Reach column 2 in above grip and insert
		RCol3_AG_I_AEL,		//Reach column 3 in above grip and insert
		RCol4_AG_I_AEL,		//Reach column 4 in above grip and insert
		RTopF_SG_I_AEL,		//Reach top floor in above grip and insert
		Rother_I_AEL,		//Reach other object and insert
		RWhOR_AG_I_AEL,		//Reach wheel in above grip from the hand of the other robot and insert
		RNhOR_BG_I_AEL,		//Reach OBJECT_NUT in bottom grip from the hand of the other robot and insert
		RCol1hOR_AG_I_AEL,	//Reach column 1 from the hand of the other robot and insert
		RCol2hOR_AG_I_AEL,	//Reach column 2 from the hand of the other robot and insert
		RCol3hOR_AG_I_AEL,	//Reach column 3 from the hand of the other robot and insert
		RCol4hOR_AG_I_AEL,	//Reach column 4 from the hand of the other robot and insert
		RTopFhOR_SG_I_AEL,	//Reach top floor from the hand of the other robot and insert
		RotherhOR_I_AEL,	//Reach other object from the hand of the other robot and insert
		RW_SG_H_AEL,		//Reach wheel in side grip and handover
		RN_AG_H_AEL,		//Reach OBJECT_NUT in above grip and handover
		RCol1_BG_H_AEL,		//Reach column 1 in bottom grip and handover
		RCol2_BG_H_AEL,		//Reach column 2 in bottom grip and handover
		RCol3_BG_H_AEL,		//Reach column 3 in bottom grip and handover
		RCol4_BG_H_AEL,		//Reach column 4 in bottom grip and handover
		RTopF_SG_H_AEL,		//Reach top floor in side grip and handover
		Rother_H_AEL,		//Reach other object and handover
		P_W_AEL,			//point towards wheel
		P_N_AEL,			//point towards OBJECT_NUT
		P_Col1_AEL,			//point towards column 1
		P_Col2_AEL,			//point towards column 2
		P_Col3_AEL,			//point towards column 3
		P_Col4_AEL,			//point towards column 4
		P_TopF_AEL,			//point towards top floor
		P_other_AEL,		//point towards other
		HO_AEL,				//Ask with hand
		RBase_AEL,			//Reach OBJECT_BASE
		RCol1_BG_REGRASP_I,	//Reach column 1 in BG and regrasp
		RCol2_BG_REGRASP_I,	//Reach column 2 in BG and regrasp
		RCol3_BG_REGRASP_I,	//Reach column 3 in BG and regrasp
		RCol4_BG_REGRASP_I,	//Reach column 4 in BG and regrasp
		RCol1_BG_REGRASP_H,	//Reach column 1 in BG and regrasp
		RCol2_BG_REGRASP_H,	//Reach column 2 in BG and regrasp
		RCol3_BG_REGRASP_H,	//Reach column 3 in BG and regrasp
		RCol4_BG_REGRASP_H,	//Reach column 4 in BG and regrasp
		RCol2_Col4_AG_I,	//Reach column 2 and 4 in AG and insert
		RCol2_Col4_SWAP_POSITIONS,	//Reach column 2 and 4 and swap positions
		RBase_two_arms,	//Reach the base, grasp it and transport it to the correct place by using both arms synchronously 
		ComError,			//Comunicate the error
		LastActionExecutionLayer //List end
	};
}

//------------------------------------------------------------------------------

namespace ActionExecutionLayer_Aerospacial
{
	enum ActionExecutionLayerEnum
	{
		RBarBlue=0,
		RBarRed,
		RColumnGreen,
		RColumnYellow,
		OK,
		LastActionExecutionLayer //List end
	};
}

//-----------------------------------------------------------------------------
/*namespace ActionExecutionLayer_DrinkingTask
{
	enum ActionExecutionLayerEnum
	{
		//Reach closed bottle with side grip and hold out until human removes stopper
		R_cB_SG_HoldUntil_HremStopper,
		//Reach closed bottle from Human hand with side grip and hold out until human removes stopper
		R_cB_Hhand_SG_HoldUntil_HremStopper,
		//Request closed bottle
		Req_cB,
		//Reach open bottle with above grip to hold out
		R_oB_AG_HOut,
		//Reach open bottle with side grip and Fill Glass
		R_oB_SG_FilGl,
		//Reach open bottle from human hand with side grip and fill glass
		R_oB_Hhand_SG_FillGl,
		//Request Open bottle
		Req_oB,
		//Reach empty glass with above grip and hold out
		R_eG_AG_HOut,
		//Reach empty glass from human hand with side grip and place in robot workspace
		R_eG_Hhand_SG_P_RWksp,
		//Request empty glass
		Req_eG,
		//Reach inverted glass with above grip and hold out
		R_iG_AG_HOut,
		//Reach inverted glass with inverted side grip and turn it
		R_iG_ISG_Turn,
		//Reach full glass with above grip and hold out
		R_fG_AG_HOut,
		LastActionExecutionLayer //List end
	};
}*/

namespace MovementTypes
{
	enum Types
	{
		DEFAULT = 0,
		ONLY_ARM,
		ONLY_HAND,
		PICK_RIGHT,
		PLACE_RIGHT,
		RETURN_RIGHT,
		POINTING_RIGHT,
		ROTATE_RIGHT,
		PICK_LEFT,
		PLACE_LEFT,
		RETURN_LEFT,
		POINTING_LEFT,
		ROTATE_LEFT,
		PICK_TWOARMS,
		PLACE_TWOARMS,
		RETURN_TWOARMS,
		PLACEFILLGLASS_RIGHT,
		RETURNFILLGLASS_RIGHT,
		PLACEOPENBOTTLE_RIGHT,
		PLACEFILLGLASS_LEFT,
		RETURNFILLGLASS_LEFT,
		PLACEOPENBOTTLE_LEFT,
		lastMovementType
	};
}

namespace LearningDrinkingTask
{
	enum LearningDrinkingTaskActions
	{
		GRASP_INV_GLASS = 0,
		INVERT_GLASS,
		RETURN_FROM_GLASS,
		GRASP_BOTTLE,
		TRANSP_BOTTLE,
		FILL_GLASS,
		PLACE_BOTTLE,
		RETURN_BOTTLE,
		GRASP_GLASS_HANDOVER,
		RETURN_HANDOVER,
		GOTO_HOLDPOSITION,
		RETURN_FROM_HOLDPOSITION,
		PUT_BOTTLE_IN_TABLE,
		RETURN_FROM_PUTTING_BOTTLE_IN_TABLE,
		LastLearningDrinkingTaskAction
	};
	
	namespace VisionInfo
	{
		enum LearningDrinkTask_VisionInfo
		{
			Glass = 0,
			GlassInverted,
			HandOverGlass,
			BottleInPlace,
			BottleLifted,
			BottleTurned,
			Hand_Reach_Glass,
			Hand_Release_Glass,
			Hand_Reach_Inverted_Glass,
			Hand_Release_Inverted_Glass,
			Hand_Reach_Bottle,
			Hand_Release_Bottle,
			LearningDrinkTask_VisionInfo
		};
	}
}


namespace LearningComputerAssemblyTask
{
	enum LearningComputerAssemblyTaskActions
	{
		HANDOVER_MOTHERBOARD = 0,
		RETURN_FROM_HANDOVER_MOTHERBOARD,
		HANDOVER_SCREWBOX,
		RETURN_FROM_HANDOVER_SCREWBOX,
		HANDOVER_POWERSUPPLY,
		RETURN_FROM_HANDOVER_POWERSUPPLY,
		HANDOVER_CABLE,
		RETURN_FROM_HANDOVER_CABLE,
		HANDOVER_DISK,
		RETURN_FROM_HANDOVER_DISK,
		HANDOVER_PCICARD,
		RETURN_FROM_HANDOVER_PCICARD,
		LastLearningComputerAssemblyTaskAction
	};

	namespace VisionInfo
	{
		enum LearningComputerAssemblyTask_VisionInfo
		{
			ScrewBox = 0,
			PowerSupply,
			Cable,
			ScrewBox_HandOverPosition,
			PowerSupply_HandOverPosition,
			Cable_HandOverPosition,
			HumanHand_Lifted,
			SecondHumanHand_Lifted,
			HumanGrasp,
			LearningComputerAssemblyTask_VisionInfo
		};
	}
}

namespace LearningToyVehicleTask
{
	enum LearningToyVehicleTaskActions
	{
		GRASP_WHEEL = 0,
		INSERT_WHEEL,
		RETURN_FROM_WHEEL,
		ALL_MOV_WHEEL,
		GRASP_NUT,
		INSERT_NUT,
		RETURN_FROM_NUT,
		ALL_MOV_NUT,
		GRASP_COLUMN1,
		INSERT_COLUMN1,
		RETURN_FROM_COLUMN1,
		ALL_MOV_COLUMN1 ,
		LastLearningToyVehicleTaskAction
	};

	/*namespace VisionInfo
	{
		enum LearningToyVehicleTask_VisionInfo
		{
			Glass = 0,
			GlassInverted,
			HandOverGlass,
			BottleInPlace,
			BottleLifted,
			BottleTurned,
			Hand_Reach_Glass,
			Hand_Release_Glass,
			Hand_Reach_Inverted_Glass,
			Hand_Release_Inverted_Glass,
			Hand_Reach_Bottle,
			Hand_Release_Bottle,
			LearningToyVehicleTask_VisionInfo
		};
	}*/
}




//------------------------------------------------------------------------------
enum RobotName
{
	aros=0,
	aros_two_arms,
	dumbo,
	jumbo,
	lastrobot
};

//------------------------------------------------------------------------------

#ifdef OBJECT_NULL
#undef OBJECT_NULL
#endif

#define OBJECT_NULL -1

// Object types definition
enum _OBJECTS_
{
	OBJECT_BASE = 0,
	OBJECT_WHEEL,
	OBJECT_NUT,
	OBJECT_COLUMN,
	OBJECT_COLUMN_1,
	OBJECT_COLUMN_2,
	OBJECT_COLUMN_3,
	OBJECT_COLUMN_4,
	OBJECT_HAND,
	OBJECT_BASE_TIP,
	OBJECT_BASE_HOLE_1,
	OBJECT_BASE_HOLE_2,
	OBJECT_BASE_HOLE_3,
	OBJECT_BASE_HOLE_4,
	OBJECT_TOP_FLOOR,
	OBJECT_SPOT_GREEN,
	OBJECT_SPOT_RED,
	OBJECT_SPOT_MAGENTA,
	OBJECT_SPOT_BLUE,
	OBJECT_GLASS_BASE,
	OBJECT_GLASS_TOP,
	OBJECT_LIQUID,
	OBJECT_CORK,
	OBJECT_BOTTLE,
	OBJECT_GLASS_FULL,
	OBJECT_OBJ_RED,
	OBJECT_CUBE_RED,
	OBJECT_BOLT_RED,
	OBJECT_OBJ_GREEN,
	OBJECT_BOLT_GREEN,
	OBJECT_CUBE_GREEN,
	OBJECT_CUBE_BLUE,
	OBJECT_CUBE_YELLOW,
	OBJECT_BOLT_YELLOW,
	OBJECT_NUT_ORANGE,
	OBJECT_SLAT,
	OBJECT_SLAT_SHORT,
	OBJECT_SLAT_MEDIUM,
	OBJECT_SLAT_LONG,
	OBJECT_FACE,
	OBJECT_RED,
	OBJECT_GREEN,
	OBJECT_BLUE,
	OBJECT_CYAN,
	OBJECT_MAGENTA,
	OBJECT_YELLOW,
	OBJECT_ORANGE,
	OBJECT_GRASP_POINT,
	OBJECT_HANDLE,
	OBJECT_MAGENTA_SMALL,
	OBJECT_MAGENTA_BIG,
	OBJECT_BASE_AEROSPACIAL,
	OBJECT_COLUMN_YELLOW,
	OBJECT_COLUMN_GREEN,
	OBJECT_BAR_BLUE,
	OBJECT_BAR_RED,
	OBJECT_CUSTOM,
	OBJECT_GLASS_EMPTY,
	OBJECT_GLASS_INVERTED,
	OBJECT_BOTTLE_CLOSED,
	OBJECT_BOTTLE_OPEN,
	OBJECT_BOX,
	OBJECT_GLASS,
	OBJECT_BATTERY,
	OBJECT_BOTTLE_CLOSED_TEA,
	OBJECT_BOTTLE_OPEN_TEA,
	OBJECT_GLASS_FULL_TEA,
	OBJECT_BOTTLE_CLOSED_COFFEE,
	OBJECT_BOTTLE_OPEN_COFFEE,
	OBJECT_GLASS_FULL_COFFEE,
	OBJECT_BOTTLE_CLOSED_JUICE,
	OBJECT_BOTTLE_OPEN_JUICE,
	OBJECT_GLASS_FULL_JUICE,
	OBJECT_TABLEDUMBO,
	OBJECT_TABLE,		//World working table.
	OBJECT_POSE,		//identifier for an object which is actually only a pose (position + orientation).
	OBJECT_MOTHERBOARD,		
	OBJECT_SCREWBOX,
	OBJECT_POWERSUPPLY,
	OBJECT_CABLE,
	OBJECT_DISK,
	OBJECT_SECONDHAND,
	OBJECT_PCICARD,
	//Add more objects here, before NUMBER_OF_OBJECT_TYPES ...
	NUMBER_OF_OBJECT_TYPES,
};

const std::array<std::string, NUMBER_OF_OBJECT_TYPES> objects_names =
{
	"Base",
	"Wheel",
	"Nut",
	"Column",
	"Column 1",
	"Column 2",
	"Column 3",
	"Column 4",
	"Hand",
	"Base Tip",
	"Hole 1",
	"Hole 2",
	"Hole 3",
	"Hole 4",
	"Top Floor",
	"Sopt Green",
	"Spot Red",
	"Stop Magenta",
	"Stop Blue",
	"Glass Base",
	"Glass Top",
	"Liquid",
	"Cork",
	"Bottle",
	"Full Glass",
	"Red Object",
	"Red Cube",
	"Red Bolt",
	"Green Object",
	"Green Bolt",
	"Green Cube",
	"Blue Cube",
	"Yellow Cube",
	"Yellow Bolt",
	"Orange Nut",
	"Slat",
	"Short Slat",
	"Medium slat",
	"Long Slat",
	"Face",
	"Red",
	"Green",
	"Blue",
	"Cyean",
	"Magenta",
	"Yellow",
	"Orange",
	"Grasp point",
	"Handle",
	"Small magenta",
	"Big Magenta",
	"Aerospacial base",
	"Yellow Column",
	"Green Column",
	"Blue Bar",
	"Red Bar",
	"Custom",
	"Empty glass",
	"Inverted glass",
	"Closed Bottle",
	"Open Bottle",
	"Box",
	"Glass",
	"Battery",
	"Tea's Closed Bottle",
	"Tea's Open Bottle",
	"Tea's Full Glass",
	"Coffee's Closed Bottle",
	"Coffee's Open Bottle",
	"Coffee's Full Glass",
	"Orange Juice's Closed Bottle",
	"Orange Juice's Open Bottle",
	"Orange Juice's Full Glass",
	"Dumbo's Table",
	"Table",		//World working table.
	"Pose",		//identifier for an object which is actually only a pose (position + orientation).
	"Motherboard",
	"PCI Card",
	"Power Supply",
	"Cable",
	"Disk",
	"Second Hand",
	"PCI Card"
};

//------------------------------------------------------------------------------
// Common definitions to identify information provided by VisionBot to others

namespace Side
{
	enum _PLACE_INSERTED_
	{
		NONE = 0,
		ROBOT,
		HUMAN,
		BOTH,
	};

	enum _INSERTED_
	{
		NOT_INSERTED = 0,
		INSERTED,
	};

	enum _HAND_SIDE_
	{
		LEFT = 0,
		RIGHT,
	};
}

namespace Grip
{
	enum _GRIP_TYPE_
	{
		NONE = 0,
		ABOVE,
		SIDE,
	};
}

namespace Gesture
{
	enum _GESTURE_TYPE_
	{
		NONE = Grip::NONE,
		ABOVE_GRIP = Grip::ABOVE,
		SIDE_GRIP = Grip::SIDE,
		POINTING,
		HAND_OUT,
	};
}

//------------------------------------------------------------------------------
// Labels for objects that are part of the Learning task

namespace LearnTaskObj
{
	enum _LEARN_TASK_OBJ_
	{
		BA = 0,
		LW,
		LN,
		RW,
		RN,
		C1,
		C2,
		C3,
		C4,
		TF,
		//------------------------
		// Insert extra objects above this line
		NUMBER_OBJECTS_LEARN_TASK,
	};
}

//------------------------------------------------------------------------------

static unsigned int CSG_Objects[] =
{
	OBJECT_WHEEL,
	OBJECT_NUT,
	OBJECT_COLUMN_1,
	OBJECT_COLUMN_2,
	OBJECT_COLUMN_3,
	OBJECT_COLUMN_4,
	OBJECT_TOP_FLOOR
};

//------------------------------------------------------------------------------
#ifndef HEIGHT_SECOND_FLOOR
#define HEIGHT_SECOND_FLOOR 1150.0f	// mm
#endif

//------------------------------------------------------------------------------

#ifndef ROBOT_WORKSPACE_LIMIT

#define ROBOT_WORKSPACE_LIMIT

#define ROBOT_WORKSPACE_LIMIT_MM 850
#define ROBOT_WORKSPACE_LIMIT_MM_F 850.0f
#define ROBOT_WORKSPACE_LIMIT_MM_D 850.0

#define ROBOT_WORKSPACE_LIMIT_CM 85
#define ROBOT_WORKSPACE_LIMIT_CM_F 85.0f
#define ROBOT_WORKSPACE_LIMIT_CM_D 85.0

#endif

template< typename T >
unsigned int get_Workspace_Position( T x_mm, T y_mm = 0, T z_mm = 0 )
{
	return 
		// If X coordinate is close to 'zero', the object is most likely invalid
		( x_mm < static_cast<T>( 0.1f ) ) ? Side::NONE :
		// If X coordinate is under the specified limit its in the robots' workspace, otherwise its in the human workspace
		( x_mm < static_cast<T>( ROBOT_WORKSPACE_LIMIT_MM_F ) ) ? Side::ROBOT : Side::HUMAN;
}

#endif //_COMMON_H_
