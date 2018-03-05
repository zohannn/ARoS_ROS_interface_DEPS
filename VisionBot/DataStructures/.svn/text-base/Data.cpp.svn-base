#include "VisionBotDlg.h"
#include "Data.h"
#include "MutexUtils.hpp"
#include "vector_operations.hpp"

CData::CData( void )
: sRobotName( "ARoS" )
, image_procObject( nullptr )
, uTask( Task::CONSTRUCTION_TOY_STEREO )
, HumanInfo()
, bUseHandTracker( false )
, bUseBodyMovement( false )
, objBase( OBJECT_BASE )
{
	for( int i = 0; i < 3; i++ )
	{
		mobileRobotTarget.Obj[i].angle = 0.0f;
		mobileRobotTarget.Obj[i].distance = 0.0f;
	}
	
	//--------------------------------------------------------------------------
	// Setup the base

	// Flag the initial base object as invalid
	objBase.bValid = false;

	objBase.vSubObjects.emplace_back( OBJECT_BASE_HOLE_1 );
	objBase.vSubObjects.emplace_back( OBJECT_BASE_HOLE_2 );
	objBase.vSubObjects.emplace_back( OBJECT_BASE_HOLE_3 );
	objBase.vSubObjects.emplace_back( OBJECT_BASE_HOLE_4 );

	//--------------------------------------------------------------------------
	
	vHistogram.Obj.assign( HIST_LENGTH, 0 );

	vInsertedObjects.Obj.assign( 2, std::vector<int>() );
	vInsertedObjects_Shared.Obj.assign( 2, std::vector<int>() );

	vObjects.Obj.assign( NUMBER_OF_OBJECT_TYPES, std::vector< CObject_f >() );
	vObjects_Shared.Obj.assign( NUMBER_OF_OBJECT_TYPES, std::vector< CObject_f >() );

	//--------------------------------------------------------------------------
	// Nothing more to do here!
}

CData::~CData( void )
{
	sRobotName.clear();

	vObjects_Shared.lock();
	vObjects.lock();
	for( int i = 0; i < NUMBER_OF_OBJECT_TYPES; i++ )
	{
		vObjects.Obj.at( i ).clear();
		vObjects_Shared.Obj.at( i ).clear();
	}
	vObjects.unlock();
	vObjects_Shared.unlock();

	vInsertedObjects_Shared.lock();
	vInsertedObjects_Shared.Obj.at( 0 ).clear();
	vInsertedObjects_Shared.Obj.at( 1 ).clear();
	vInsertedObjects_Shared.unlock();

	vInsertedObjects.lock();
	vInsertedObjects.Obj.at( 0 ).clear();
	vInsertedObjects.Obj.at( 1 ).clear();
	vInsertedObjects.unlock();

	Column_InsertedAt_Shared.lock();
	Column_InsertedAt_Shared.Obj.clear();
	Column_InsertedAt_Shared.unlock();
	
	Column_InsertedAt.lock();
	Column_InsertedAt.Obj.clear();
	Column_InsertedAt.unlock();

	vObjPointed.lock();
	vObjPointed.Obj.clear();
	vObjPointed.unlock();

	vTaskObjects.lock();
	vTaskObjects.clear();
	vTaskObjects.unlock();
}

void CData::set_Task( unsigned int task )
{
	uTask.set( task );
	SetupTaskObjects();
}

unsigned int CData::get_Task( void )
{
	return uTask.get();
}

std::string CData::get_RobotName( void )
{
	return sRobotName;
}

void CData::Update_LastKnownGoodBase( void )
{
	size_t 
		num_bases = vObjects.Obj.at( OBJECT_BASE ).size(),
		num_holes = 0;

	unsigned int uType = OBJECT_NULL;

	bool bUpdated = false;

	// As soon as a new base with at least one hole is detected, the others bases are discarded
	for( size_t iBase = 0; ( iBase<num_bases ) && !bUpdated ; iBase++ )
	{
		num_holes = vObjects.Obj.at( OBJECT_BASE ).at( iBase ).vSubObjects.size();

		if( num_holes > 0 )
		{
			for( size_t iHole = 0; iHole < num_holes; iHole++ )
			{
				uType = vObjects.Obj.at( OBJECT_BASE ).at( iBase ).vSubObjects.at( iHole ).get_Type();

				int index = -1;

				switch( uType )
				{
				case OBJECT_BASE_HOLE_1:
				case OBJECT_BASE_HOLE_2:
				case OBJECT_BASE_HOLE_3:
				case OBJECT_BASE_HOLE_4:
					index = uType - OBJECT_BASE_HOLE_1;
					break;

				default:
					break;
				}

				if( index >= 0 && index < 4 )
				{
					if( vObjects.Obj.at( OBJECT_BASE ).at( iBase ).vSubObjects.at( iHole ).has3D() )
					{
						objBase.vSubObjects.at( index ).Copy_From( &vObjects.Obj.at( OBJECT_BASE ).at( iBase ).vSubObjects.at( iHole ) );
					}

					objBase.bValid = true;
					bUpdated = true;
				}
			}
		}

		if( bUpdated && vObjects.Obj.at( OBJECT_BASE ).at( iBase ).has3D() )
		{
			objBase.set_Coordinates_World( vObjects.Obj.at( OBJECT_BASE ).at( iBase ).get_Coordinates_World() );
		}
	}
}

bool CData::Parse_Parameters_Load( void )
{
	// Eventual configurations saved in a file.
	std::string 
		strAppConfig = "App_Config",
		strAppConfigRobot = "Robot",
		strAppConfigTask = "Task",
		strAppOptional = "Optional_Features",
		strBody = "Use_Body_Movement",
		strHand = "Use_Hand_Tracker";

	bool bSuccess = true;

	yarp::os::Bottle bot = prop_ConfigFile.findGroup( strAppConfig.c_str() );

	if( bot.check( strAppConfigRobot.c_str() ) )
	{
		sRobotName = bot.find( strAppConfigRobot.c_str() ).asString();
		uTask.set( static_cast<unsigned int>( bot.find( strAppConfigTask.c_str() ).asInt() ) );
	}
	else bSuccess = false;

	std::string strPar = "";

	bot = prop_ConfigFile.findGroup( strAppOptional.c_str() );

	if( bot.check( strBody.c_str() ) )
	{
		strPar = bot.find( strBody.c_str() ).asString();

		bUseBodyMovement = ( strPar.compare( "yes" ) == 0 );
	}
	else
	{
		bUseBodyMovement = false;
	}

	if( bot.check( strHand.c_str() ) )
	{
		strPar = bot.find( strHand.c_str() ).asString();

		bUseHandTracker = ( strPar.compare( "yes" ) == 0 );
	}
	else
	{
		bUseHandTracker = false;
	}

	return bSuccess;
}

void CData::Refresh_Information( void )
{
	//--------------------------------------------------------------------------
	// Copy object information from the area memory used in runtime to a shared
	// area available for communication
	vObjects.lock();
	vObjects_Shared.lock();

	for( int i=0 ; i<NUMBER_OF_OBJECT_TYPES ; i++ )
	{
		vector_copy( vObjects.Obj.at( i ), vObjects_Shared.Obj.at( i ) );		
	}

	vObjects_Shared.unlock();
	vObjects.unlock();

	//--------------------------------------------------------------------------
	// Inserted objects vector
	vInsertedObjects_Shared.lock();
	vInsertedObjects.lock(); 

	vector_copy( vInsertedObjects.Obj.at( 0 ), vInsertedObjects_Shared.Obj.at( 0 ) );
	vector_copy( vInsertedObjects.Obj.at( 1 ), vInsertedObjects_Shared.Obj.at( 1 ) );

	vInsertedObjects.unlock(); 
	vInsertedObjects_Shared.unlock();

	//--------------------------------------------------------------------------
	// Copy column insertion information from the area memory used in runtime 
	// to a shared area available for communication
	mutex_vector_copy( Column_InsertedAt, Column_InsertedAt_Shared );
}

bool CData::get_UseHandTracker( void )
{
	return bUseHandTracker;
}

bool CData::get_UseBodyMovement( void )
{
	return bUseBodyMovement;
}

unsigned int CData::get_ObjectInsertedWhere( unsigned int nObjectType )
{
	std::vector<bool> vInserted( 2, false );

	unsigned int uInsertedAt = Side::NOT_INSERTED;

	size_t 
		iObj = 0,
		nObjs = 0;

	switch( nObjectType )
	{
	case OBJECT_WHEEL:
	case OBJECT_NUT:
		vInsertedObjects_Shared.lock();
		for( size_t iSide=0 ; iSide<2 ; iSide++ )
		{
			nObjs = vInsertedObjects_Shared.Obj.at( iSide ).size();

			for( iObj=0 ; iObj<nObjs ; iObj++ )
			{
				if( vInsertedObjects_Shared.Obj.at( iSide ).at( iObj ) == nObjectType )
					vInserted.at( iSide ) = true;
			}
		}	
		vInsertedObjects_Shared.unlock();
		
		if( vInserted.at( Side::ROBOT - 1 ) && vInserted.at( Side::HUMAN - 1 ) ) uInsertedAt = Side::BOTH;
		else if( vInserted.at( Side::ROBOT - 1 ) ) uInsertedAt = Side::ROBOT;
		else if( vInserted.at( Side::HUMAN - 1 ) ) uInsertedAt = Side::HUMAN;
		else uInsertedAt = Side::NONE;
		break;

	case OBJECT_COLUMN_1:
	case OBJECT_COLUMN_2:
	case OBJECT_COLUMN_3:
	case OBJECT_COLUMN_4:
		Column_InsertedAt_Shared.lock();
		nObjs = Column_InsertedAt_Shared.Obj.size();
		for( iObj = 0; iObj < nObjs; iObj++ )
		{
			if( Column_InsertedAt_Shared.Obj.at( iObj ).first == nObjectType )
				uInsertedAt = Column_InsertedAt_Shared.Obj.at( iObj ).second;
		}
		Column_InsertedAt_Shared.unlock();
		break;

	case OBJECT_TOP_FLOOR:
		vObjects_Shared.lock();
		nObjs = vObjects_Shared.Obj.at( OBJECT_TOP_FLOOR ).size();
		for( iObj = 0; iObj < nObjs ; iObj++ )
		{
			if( vObjects_Shared.Obj.at( OBJECT_TOP_FLOOR ).at( iObj ).bInserted )
				uInsertedAt = Side::INSERTED;
		}
		vObjects_Shared.unlock();
		break;

	default:
		uInsertedAt = Side::NOT_INSERTED;
		break;
	}

	return uInsertedAt;
}

unsigned int CData::get_HandNearObject( unsigned int nObjectType )
{
	vObjects_Shared.lock();
	size_t nHands = vObjects_Shared.Obj.at( OBJECT_HAND ).size();

	unsigned int isNear = 0;

	if( nHands < 1 )
	{
		vObjects_Shared.unlock();
		return isNear;
	}

	for( size_t iHand=0 ; iHand<nHands ; iHand++ )
	{
		if( ( vObjects_Shared.Obj.at( OBJECT_HAND ).at( iHand ).uGestureType == Gesture::ABOVE_GRIP ) ||
			( vObjects_Shared.Obj.at( OBJECT_HAND ).at( iHand ).uGestureType == Gesture::SIDE_GRIP ) )
		{
			size_t nObjs = vObjects_Shared.Obj.at( OBJECT_HAND ).at( iHand ).vNearObjects.size();

			if( nObjs < 1 ) continue;

			for( size_t iObj = 0; iObj < nObjs; iObj++ )
			{
				if( vObjects_Shared.Obj.at( OBJECT_HAND ).at( iHand ).vNearObjects.at( iObj ).get_Type() == nObjectType )
					isNear = 1;
			}
		}
	}

	vObjects_Shared.unlock();

	return isNear;
}

void CData::set_HistogramData( std::vector<int> v_data )
{
	vHistogram.lock();

	vHistogram.Obj = v_data;

	vHistogram.unlock();
}

void CData::get_HistogramData( std::vector<int> &v_data )
{
	vHistogram.lock();

	v_data = vHistogram.Obj;

	vHistogram.unlock();
}

void CData::SetupTaskObjects()
{
	vTaskObjects.lock();

	vTaskObjects.clear();

	switch( uTask.get() )
	{
	case Task::CONSTRUCTION_TOY_STEREO:
		vTaskObjects.emplace_back( OBJECT_BASE );
		vTaskObjects.emplace_back( OBJECT_WHEEL );
		vTaskObjects.emplace_back( OBJECT_NUT );
		vTaskObjects.emplace_back( OBJECT_COLUMN );
		vTaskObjects.emplace_back( OBJECT_HAND );
		break;

	default:
	case Task::CONSTRUCTION_TOY_FULL_SVS:
		vTaskObjects.emplace_back( OBJECT_BASE );
		vTaskObjects.emplace_back( OBJECT_WHEEL );
		vTaskObjects.emplace_back( OBJECT_NUT );
		vTaskObjects.emplace_back( OBJECT_COLUMN_1 );
		vTaskObjects.emplace_back( OBJECT_COLUMN_2 );
		vTaskObjects.emplace_back( OBJECT_COLUMN_3 );
		vTaskObjects.emplace_back( OBJECT_COLUMN_4 );
		vTaskObjects.emplace_back( OBJECT_TOP_FLOOR );
		vTaskObjects.emplace_back( OBJECT_HAND );
		vTaskObjects.emplace_back( OBJECT_RED );
		vTaskObjects.emplace_back( OBJECT_BLUE );
		break;

	case Task::TOY_ROBOT_LEARNING:
		vTaskStatus.assign( LearnTaskObj::NUMBER_OBJECTS_LEARN_TASK, Side::NOT_INSERTED );

		vTaskObjects.emplace_back( OBJECT_BASE );
		vTaskObjects.emplace_back( OBJECT_WHEEL );
		vTaskObjects.emplace_back( OBJECT_NUT );
		vTaskObjects.emplace_back( OBJECT_COLUMN_1 );
		vTaskObjects.emplace_back( OBJECT_COLUMN_2 );
		vTaskObjects.emplace_back( OBJECT_COLUMN_3 );
		vTaskObjects.emplace_back( OBJECT_COLUMN_4 );
		vTaskObjects.emplace_back( OBJECT_TOP_FLOOR );
		vTaskObjects.emplace_back( OBJECT_HAND );
		vTaskObjects.emplace_back( OBJECT_RED );
		vTaskObjects.emplace_back( OBJECT_BLUE );
		break;

	case Task::DRINKING:
		/*vTaskObjects.emplace_back( OBJECT_BOTTLE );
		vTaskObjects.emplace_back( OBJECT_CORK );
		vTaskObjects.emplace_back( OBJECT_GLASS_BASE );
		vTaskObjects.emplace_back( OBJECT_GLASS_FULL );
		vTaskObjects.emplace_back( OBJECT_GLASS_TOP );
		vTaskObjects.emplace_back( OBJECT_LIQUID );
		vTaskObjects.emplace_back( OBJECT_HAND );*/
		vDrinkTaskStatus.Obj.assign( LearningDrinkingTask::VisionInfo::LearningDrinkTask_VisionInfo, 0 );
		break;

	case Task::TUM_INTEGRATION:
		vTaskObjects.emplace_back( OBJECT_OBJ_RED );
		vTaskObjects.emplace_back( OBJECT_CUBE_RED );
		vTaskObjects.emplace_back( OBJECT_BOLT_RED );
		vTaskObjects.emplace_back( OBJECT_OBJ_GREEN );
		vTaskObjects.emplace_back( OBJECT_BOLT_GREEN );
		vTaskObjects.emplace_back( OBJECT_CUBE_GREEN );
		vTaskObjects.emplace_back( OBJECT_CUBE_BLUE );
		vTaskObjects.emplace_back( OBJECT_CUBE_YELLOW );
		vTaskObjects.emplace_back( OBJECT_BOLT_YELLOW );
		vTaskObjects.emplace_back( OBJECT_NUT_ORANGE );
		vTaskObjects.emplace_back( OBJECT_SLAT );
		vTaskObjects.emplace_back( OBJECT_SLAT_SHORT );
		vTaskObjects.emplace_back( OBJECT_SLAT_MEDIUM );
		vTaskObjects.emplace_back( OBJECT_SLAT_LONG );
		vTaskObjects.emplace_back( OBJECT_HAND );
		break;

	case Task::COLOUR_LEARNING:
		vTaskObjects.emplace_back( OBJECT_HAND );
		vTaskObjects.emplace_back( OBJECT_RED );
		vTaskObjects.emplace_back( OBJECT_GREEN );
		vTaskObjects.emplace_back( OBJECT_BLUE );
		vTaskObjects.emplace_back( OBJECT_MAGENTA );
		vTaskObjects.emplace_back( OBJECT_YELLOW );
		break;

	case Task::COMPUTER_ASSEMBLY:
		vTaskObjects.emplace_back(OBJECT_HAND);
		vTaskObjects.emplace_back(OBJECT_SECONDHAND);
		vTaskObjects.emplace_back(OBJECT_SCREWBOX);
		vTaskObjects.emplace_back(OBJECT_POWERSUPPLY);
		vTaskObjects.emplace_back(OBJECT_CABLE);
		vComputerAssemblyTaskStatus.Obj.assign(LearningComputerAssemblyTask::VisionInfo::LearningComputerAssemblyTask_VisionInfo, 0);
		break;

	}

	vTaskObjects.unlock();
}

int CData::get_TaskNumObjects( void )
{
	vTaskObjects.lock();

	int temp = static_cast<int>( vTaskObjects.size() );

	vTaskObjects.unlock();

	return temp;
}

int CData::get_TaskObject( int index )
{
	vTaskObjects.lock();

	int size = static_cast<int>( vTaskObjects.size() );

	int temp = ( index >= 0 && index < size ) ? vTaskObjects.at( index ) : -1;

	vTaskObjects.unlock();

	return temp;
}

std::vector< int > CData::get_TaskObjects( void )
{
	std::vector< int > temp;

	mutex_vector_copy( vTaskObjects, temp );

	return temp;
}

void CData::set_MobileRobotTarget( unsigned int type, float angle, float distance, bool exist )
{
	int colour( 0 );

	switch( type )
	{
		default:
		case OBJECT_RED:
			colour = 0;
		break;

		case OBJECT_YELLOW:
			colour = 1;
		break;

		case OBJECT_GREEN:
			colour = 2;
		break;
		
		case OBJECT_CYAN:
			colour = 3;
		break;

		case OBJECT_BLUE:
			colour = 4;
		break;

		case OBJECT_MAGENTA:
			colour = 5;
		break;
	}

	mobileRobotTarget.lock();

	if( exist )
	{
		mobileRobotTarget.Obj[colour].angle = angle;
		mobileRobotTarget.Obj[colour].distance = distance;
	}
	mobileRobotTarget.Obj[colour].exist = exist;

	mobileRobotTarget.unlock();
}

int CData::get_MobileRobotTarget(unsigned int type, float *angle, float *distance, bool *exist)
{
	int error( 0 );

	if( type >= 0 && type < 6 )
	{
		mobileRobotTarget.lock();

		*angle = mobileRobotTarget.Obj[type].angle;
		*distance = mobileRobotTarget.Obj[type].distance;
		*exist = mobileRobotTarget.Obj[type].exist;
		
		if (*exist == false)
			error = -1;

		mobileRobotTarget.unlock();
	}
	else
	{
		error = -2;
	}
	
	return error;
}
