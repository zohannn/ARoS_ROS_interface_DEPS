#include "ProcessImageDrinking.h"

CProcessImageDrinking::CProcessImageDrinking( int width, int height )
: CProcessImage( width, height )
, dTime_to_Retain_Information( 5.0 ) // seconds
, bHand_Near_Glass_Frame_Previous( false )
, bHand_Near_Glass_Frame_Current( false )
, bHand_Reach_Glass( false )
, bHand_Release_Glass( false )
, bHand_Near_Glass_Inverted_Frame_Previous( false )
, bHand_Near_Glass_Inverted_Frame_Current( false )
, bHand_Reach_Glass_Inverted( false )
, bHand_Release_Glass_Inverted( false )
, bHand_Near_Bottle_Frame_Previous( false )
, bHand_Near_Bottle_Frame_Current( false )
, bHand_Reach_Bottle( false )
, bHand_Release_Bottle( false )
{
	sSegFile += "Videre/Segmentation_Parameters_(Drinking_Task).ini";

	SetupProcessing( width, height );

	nProcessImageID = ProcessImageID::PI_Drinking;

	// Disable pointing detection for this task
	Gesture.bDetect_Pointing.set( false );
}

bool CProcessImageDrinking::SearchAllObjects( void )
{
	bool bFound = false;

	//--------------------------------------------------------------------------
	// Detect the objects

	bFound |= SearchObjects_Bottle();

	bFound |= SearchObjects_GlassInverted();

	bFound |= SearchObjects_Glass();

	bFound |= SearchObjects_Hand();

	//--------------------------------------------------------------------------

	return bFound;
}

bool CProcessImageDrinking::SearchObjects_Bottle( void )
{
	return Detect_Object_From_Primary_Colour( OBJECT_BOTTLE );
}

bool CProcessImageDrinking::SearchObjects_GlassInverted( void )
{
	//--------------------------------------------------------------------------
	// Column detection parameters
	const int
		OBJ_COLOUR_MAIN = GET_VALID_PrimaryColour( OBJECT_GLASS_INVERTED ),
		OBJ_COLOUR_SEC = GET_VALID_SecondaryColour( OBJECT_GLASS_INVERTED );

	//--------------------------------------------------------------------------
	// Auxiliary variables
	bool
		bFound = false,
		bIdentified = false,
		bOverlapped = false,
		bSpotUsed = false;

	cv::Rect rect_max;

	// Create one auxiliary object with no area.
	CObject_f
		obj( OBJECT_GLASS_INVERTED ),
		obj_spot;

	std::vector< cv::Rect >::iterator
		itRect,
		itRect2;

	//--------------------------------------------------------------------------
	// Clear objects from previous iteration
	pDataSource->vObjects.Obj.at( OBJECT_GLASS_INVERTED ).clear();

	//--------------------------------------------------------------------------
	itRect = vSegmentedImage.at( OBJ_COLOUR_MAIN ).vSpots.begin();

	while( itRect != vSegmentedImage.at( OBJ_COLOUR_MAIN ).vSpots.end() )
	{
		bSpotUsed = false;

		//Clear object properties
		obj.ClearObject();

		// Set the column area to the rectangle occupied by the yellow spot.
		obj.vSpot_Primary.emplace_back( *itRect );
		obj.set_Region( *itRect );

		// Go through the secondary colour.
		bIdentified = false;

		itRect2 = vSegmentedImage.at( OBJ_COLOUR_SEC ).vSpots.begin();

		// Go through all the spots of the secondary colour.
		while( itRect2 != vSegmentedImage.at( OBJ_COLOUR_SEC ).vSpots.end() )
		{
			bOverlapped = CheckOverlapSpot(
				&vSegmentedImage.at( OBJ_COLOUR_MAIN ).Image,	//Image 1
				&( *itRect ),									//Spot in image 1
				&vSegmentedImage.at( OBJ_COLOUR_SEC ).Image,	//Image 2
				&( *itRect2 )									//Spot in image 2
				);

			if( bOverlapped )
			{
				bIdentified = true;
				bSpotUsed = true;

				// The object area is the sum of all the spots that compose it.
				rect_max = MaxRect( obj.Region, *itRect2 );
				obj.set_Region( rect_max );
				obj.set_Type( OBJECT_GLASS_INVERTED );

				// Define the sub object
				obj.vSubObjects.emplace_back( CObject_f( OBJECT_RED, *itRect2 ) );

				itRect2 = vSegmentedImage.at( OBJ_COLOUR_SEC ).vSpots.erase( itRect2 );
			}
			else
			{
				itRect2++;
			}
		}

		// Test to see if the current Yellow spot was used.
		if( bSpotUsed )
		{
			// It was used, erase it, and go to the next one.
			itRect = vSegmentedImage.at( OBJ_COLOUR_MAIN ).vSpots.erase( itRect );

			//Add the identified object to the Data Source.
			pDataSource->vObjects.Obj.at( obj.get_Type() ).emplace_back( obj );

			bFound = true;
		}
		else
		{
			// It wasn't, go to the next one.
			itRect++;
		}
	}

	//--------------------------------------------------------------------------

	return bFound;
}

bool CProcessImageDrinking::SearchObjects_Glass( void )
{
	return Detect_Object_From_Primary_Colour( OBJECT_GLASS );
}

bool CProcessImageDrinking::SearchObjects_Hand( void )
{
	bool bResult = false;

	if( HandTracker.isReady() )
	{
		// Use hand tracker
		HandTracker.TrackObjects( &vSegmentedImage.at( COLOUR_C ).Image );

		HandTracker.get_Tracked_Objects_Vector( pDataSource->vObjects.Obj.at( OBJECT_HAND ) );

		bResult = ( pDataSource->vObjects.Obj.at( OBJECT_HAND ).size() > 0 );
	}
	
	else
	{
		// Use simple colour association
		bResult = Detect_Object_From_Primary_Colour( OBJECT_HAND );
	}

	Define_New_Region_of_Interest( OBJECT_HAND, cv::Rect( -40, 5, 80, 90 ) );

	return bResult;
}
void CProcessImageDrinking::ObjectPreProcessing( void )
{
}

void CProcessImageDrinking::ObjectPostProcessing( void )
{
	LookForNearObjects( OBJECT_HAND, OBJECT_GLASS );
	LookForNearObjects( OBJECT_HAND, OBJECT_GLASS_INVERTED );
	LookForNearObjects( OBJECT_HAND, OBJECT_BOTTLE );

	Detect_Gestures();
	
	Hand_Reach_Release_Glass();
	Hand_Reach_Release_GlassInverted();
	Hand_Reach_Release_Bottle();

	pDataSource->vDrinkTaskStatus.lock();

	pDataSource->vDrinkTaskStatus.Obj.at( LearningDrinkingTask::VisionInfo::Glass ) = isGlassPresent();
	
	pDataSource->vDrinkTaskStatus.Obj.at( LearningDrinkingTask::VisionInfo::GlassInverted ) = isGlassInvertedPresent();
	
	pDataSource->vDrinkTaskStatus.Obj.at( LearningDrinkingTask::VisionInfo::HandOverGlass ) = isGlass_in_HandOver_Position();

	pDataSource->vDrinkTaskStatus.Obj.at( LearningDrinkingTask::VisionInfo::BottleInPlace ) = isBottle_in_Place();

	pDataSource->vDrinkTaskStatus.Obj.at( LearningDrinkingTask::VisionInfo::BottleLifted ) = isBottle_Lifted();

	pDataSource->vDrinkTaskStatus.Obj.at( LearningDrinkingTask::VisionInfo::BottleTurned ) = isBottle_Turned();

	pDataSource->vDrinkTaskStatus.Obj.at( LearningDrinkingTask::VisionInfo::Hand_Reach_Glass ) = bHand_Reach_Glass ? 1 : 0;
	
	pDataSource->vDrinkTaskStatus.Obj.at( LearningDrinkingTask::VisionInfo::Hand_Release_Glass ) = bHand_Release_Glass ? 1 : 0;
	
	pDataSource->vDrinkTaskStatus.Obj.at( LearningDrinkingTask::VisionInfo::Hand_Reach_Inverted_Glass ) = bHand_Reach_Glass_Inverted ? 1 : 0;
	
	pDataSource->vDrinkTaskStatus.Obj.at( LearningDrinkingTask::VisionInfo::Hand_Release_Inverted_Glass ) = bHand_Release_Glass_Inverted ? 1 : 0;
	
	pDataSource->vDrinkTaskStatus.Obj.at( LearningDrinkingTask::VisionInfo::Hand_Reach_Bottle ) = bHand_Reach_Bottle ? 1 : 0;
	
	pDataSource->vDrinkTaskStatus.Obj.at( LearningDrinkingTask::VisionInfo::Hand_Release_Bottle ) = bHand_Release_Bottle ? 1 : 0;

	pDataSource->vDrinkTaskStatus.unlock();
}

// Process the image and retrieve data
void CProcessImageDrinking::ProcessTask(cv::Mat * mat_ptr )
{
	// Convert source image to HSV
	cv::cvtColor( *mat_ptr, mat_ImageHSV, CV_BGR2HSV );

	SegmentMainColours_cv();

	for( unsigned int nColour = 0; nColour<COLOUR_W; nColour++ )
	{
		vSegmentedImage.at( nColour ).doMorphologicalOperations();
	}

	DetectMainColourSpots();

	//--------------------------------------------------------------------------
	// Search for objects
	SearchAllObjects();
}

unsigned int CProcessImageDrinking::isGlassPresent( void )
{
	return ( pDataSource->vObjects.Obj.at( OBJECT_GLASS ).size() > 0 ) ? 1 : 0;
}

unsigned int CProcessImageDrinking::isGlassInvertedPresent( void )
{
	return ( pDataSource->vObjects.Obj.at( OBJECT_GLASS_INVERTED ).size() > 0 ) ? 1 : 0;
}

unsigned int CProcessImageDrinking::isGlass_in_HandOver_Position( void )
{
	const float HEIGHT_HAND_OVER_GLASS = 1300.0f; // mm

	size_t nGlass = pDataSource->vObjects.Obj.at( OBJECT_GLASS ).size();

	cv::Point3f pt( 0.0f, 0.0f, 0.0f );

	unsigned int uGlassInHandOverPos = 0;

	for( size_t iGlass = 0; iGlass < nGlass; iGlass++ )
	{
		pDataSource->vObjects.Obj.at( OBJECT_GLASS ).at( iGlass ).get_Coordinates_World( &pt );

		if( pt.x == 0.0f && pt.y == 0.0f && pt.z == 0.0f ) continue;

		if( pt.z > HEIGHT_HAND_OVER_GLASS )
			uGlassInHandOverPos = 1;
	}

	return uGlassInHandOverPos;
}

unsigned int CProcessImageDrinking::isBottle_in_Place( void )
{
	const cv::Point3f
		// This variable defines the desired position for the bottle (x,y,z) in mm
		ptBottlePosition(1100.0f, 50.0f, 1000.0f),
		// This variable defines the tolerance for the bottle to be considered in position (x,y,z) in mm
		// the tolerance is applied by considering the position +/- tolerance
		ptBottlePositionTolerance(100.0f, 100.0f, 100.0f);
	
	cv::Point3f
		ptCoord(0.0f, 0.0f, 0.0f),
		ptCoord_Min = ptBottlePosition,
		ptCoord_Max = ptBottlePosition;

	ptCoord_Min -= ptBottlePositionTolerance;
	ptCoord_Max += ptBottlePositionTolerance;

	unsigned int uBottle_in_Place = 0;

	size_t nBottles = pDataSource->vObjects.Obj.at( OBJECT_BOTTLE ).size();

	for( size_t i = 0; i < nBottles; i++ )
	{
		ptCoord = pDataSource->vObjects.Obj.at(OBJECT_BOTTLE).at(i).get_Coordinates_World();

		if( ptCoord.x >= ptCoord_Min.x && ptCoord.x <= ptCoord_Max.x &&
			ptCoord.y >= ptCoord_Min.y && ptCoord.y <= ptCoord_Max.y &&
			ptCoord.z >= ptCoord_Min.z && ptCoord.z <= ptCoord_Max.z )
			uBottle_in_Place = 1;
	}

	return uBottle_in_Place;
}

unsigned int CProcessImageDrinking::isBottle_Lifted( void )
{
	const float HEIGHT_BOTTLE_LIFTED = 950.0f; // mm

	size_t nBot = pDataSource->vObjects.Obj.at( OBJECT_BOTTLE ).size();

	cv::Point3f pt( 0.0f, 0.0f, 0.0f );

	unsigned int uBottleLifted = 0;

	for( size_t iBot = 0; iBot < nBot; iBot++ )
	{
		pDataSource->vObjects.Obj.at( OBJECT_BOTTLE ).at( iBot ).get_Coordinates_World( &pt );

		if( pt.x == 0.0f && pt.y == 0.0f && pt.z == 0.0f ) continue;

		if( pt.z > HEIGHT_BOTTLE_LIFTED && !isBottle_Turned())
			uBottleLifted = 1;
	}

	return uBottleLifted;
}

unsigned int CProcessImageDrinking::isBottle_Turned( void )
{
	size_t nBot = pDataSource->vObjects.Obj.at( OBJECT_BOTTLE ).size();

	cv::Rect rec( 0, 0, 0, 0 );

	unsigned int uBottleTurned = 0;

	for( size_t iBot = 0; iBot < nBot; iBot++ )
	{
		rec = pDataSource->vObjects.Obj.at( OBJECT_BOTTLE ).at( iBot ).Region;

		if( rec.area() < 1 ) continue;

		if( rec.width > rec.height && !isBottle_in_Place())
			uBottleTurned = 1;
	}

	return uBottleTurned;
}

void CProcessImageDrinking::Hand_Reach_Release_Object(unsigned int object, int gesture, CTimeCountBasic * timer_reach, CTimeCountBasic * timer_release, bool * bHand_Near_Frame_Previous, bool * bHand_Near_Frame_Current, bool * bHand_Reach, bool * bHand_Release)
{
	const int HAND_GESTURE = gesture;

	size_t nHands = pDataSource->vObjects.Obj.at(OBJECT_HAND).size();

	bool bHand_Gest = false;

	for (size_t iHand = 0; (iHand < nHands && !bHand_Gest); iHand++)
	{
		CObject_f * pHand = &pDataSource->vObjects.Obj.at(OBJECT_HAND).at(iHand);

		if (pHand->uGestureType == HAND_GESTURE)
			bHand_Gest = true;
	}

	if (*bHand_Reach)
	{
		if (timer_reach->HowLong_untilNow() > dTime_to_Retain_Information)
		{
			*bHand_Reach = false;
			timer_reach->set_Time_Finish();
		}
	}
	else if (*bHand_Release)
	{
		if (timer_release->HowLong_untilNow() > dTime_to_Retain_Information)
		{
			*bHand_Release = false;
			timer_release->set_Time_Finish();
		}
	}
	else
	{
		if (bHand_Gest)
		{
			*bHand_Near_Frame_Previous = *bHand_Near_Frame_Current;

			*bHand_Near_Frame_Current = isHand_with_Gesture_Near_Object(HAND_GESTURE, object);

			if (!*bHand_Near_Frame_Previous && *bHand_Near_Frame_Current)
			{
				*bHand_Reach = true;

				timer_reach->set_Time_Start();
			}

			if (*bHand_Near_Frame_Previous && !*bHand_Near_Frame_Current)
			{
				*bHand_Release = true;

				timer_release->set_Time_Start();
			}
		}
		else
		{
			*bHand_Near_Frame_Previous = false;
			*bHand_Near_Frame_Current = false;
		}
	}
}


void CProcessImageDrinking::Hand_Reach_Release_Glass( void )
{
	/*Hand_Reach_Release_Object(
		OBJECT_GLASS,
		Gesture::SIDE_GRIP,
		&Timer_Reach_Glass,
		&Timer_Release_Glass,
		&bHand_Near_Glass_Frame_Previous,
		&bHand_Near_Glass_Frame_Current,
		&bHand_Reach_Glass,
		&bHand_Release_Glass);*/

	Hand_Reach_Release_Object(
		OBJECT_GLASS,
		Gesture::NONE,
		&Timer_Reach_Glass,
		&Timer_Release_Glass,
		&bHand_Near_Glass_Frame_Previous,
		&bHand_Near_Glass_Frame_Current,
		&bHand_Reach_Glass,
		&bHand_Release_Glass);
}

void CProcessImageDrinking::Hand_Reach_Release_GlassInverted( void )
{
	/*Hand_Reach_Release_Object(
		OBJECT_GLASS_INVERTED,
		Gesture::SIDE_GRIP,
		&Timer_Reach_Glass_Inverted,
		&Timer_Release_Glass_Inverted,
		&bHand_Near_Glass_Inverted_Frame_Previous,
		&bHand_Near_Glass_Inverted_Frame_Current,
		&bHand_Reach_Glass_Inverted,
		&bHand_Release_Glass_Inverted);*/

	Hand_Reach_Release_Object(
		OBJECT_GLASS_INVERTED,
		Gesture::NONE,
		&Timer_Reach_Glass_Inverted,
		&Timer_Release_Glass_Inverted,
		&bHand_Near_Glass_Inverted_Frame_Previous,
		&bHand_Near_Glass_Inverted_Frame_Current,
		&bHand_Reach_Glass_Inverted,
		&bHand_Release_Glass_Inverted);
}

void CProcessImageDrinking::Hand_Reach_Release_Bottle( void )
{
	/*Hand_Reach_Release_Object(
		OBJECT_BOTTLE,
		Gesture::SIDE_GRIP,
		&Timer_Reach_Bottle,
		&Timer_Release_Bottle,
		&bHand_Near_Bottle_Frame_Previous,
		&bHand_Near_Bottle_Frame_Current,
		&bHand_Reach_Bottle,
		&bHand_Release_Bottle);*/

	Hand_Reach_Release_Object(
		OBJECT_BOTTLE,
		Gesture::NONE,
		&Timer_Reach_Bottle,
		&Timer_Release_Bottle,
		&bHand_Near_Bottle_Frame_Previous,
		&bHand_Near_Bottle_Frame_Current,
		&bHand_Reach_Bottle,
		&bHand_Release_Bottle);
}
