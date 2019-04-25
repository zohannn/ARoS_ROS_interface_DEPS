#include "ProcessImageComputerAssembly.h"

CProcessImageComputerAssembly::CProcessImageComputerAssembly( int width, int height )
: CProcessImage( width, height )
, dTime_to_Retain_Information( 5.0 ) // seconds
{
	sSegFile += "Videre/Segmentation_Parameters_(ComputerAssembly_Task).ini";

	SetupProcessing( width, height );

	nProcessImageID = ProcessImageID::PI_ComputerAssembly;

	// Disable pointing detection for this task
	Gesture.bDetect_Pointing.set( false );

	bScrewBoxPresent = false;
	bPowerSupplyPresent = false;
	bCablePresent = false;

	bmemorymode = false;
}

bool CProcessImageComputerAssembly::SearchAllObjects( void )
{
	bool bFound = false;

	//--------------------------------------------------------------------------
	// Detect the objects

	bFound |= SearchObjects_ScrewBox();

	bScrewBoxPresent = bFound;

	bFound |= SearchObjects_PowerSupply();

	bPowerSupplyPresent = bFound;

	bFound |= SearchObjects_Cable();

	bCablePresent = bFound;

	bFound |= SearchObjects_Hand();

	if (bmemorymode)
		bFound |= SearchObjects_SecondHand();

	//--------------------------------------------------------------------------

	return bFound;
}

bool CProcessImageComputerAssembly::SearchObjects_ScrewBox(void)
{
	return Detect_Object_From_Primary_Colour(OBJECT_SCREWBOX,1000);
}

bool CProcessImageComputerAssembly::SearchObjects_PowerSupply( void )
{
	return Detect_Object_From_Primary_Colour(OBJECT_POWERSUPPLY,1000);
}

bool CProcessImageComputerAssembly::SearchObjects_Cable( void )
{

	return Detect_Object_From_Primary_Colour(OBJECT_CABLE,400,2000);
}

bool CProcessImageComputerAssembly::SearchObjects_Hand( void )
{
	bool bResult = false;

	if( HandTracker.isReady() )
	{
		// Use hand tracker
		HandTracker.TrackObjects( &vSegmentedImage.at( COLOUR_B ).Image );

		HandTracker.get_Tracked_Objects_Vector( pDataSource->vObjects.Obj.at( OBJECT_HAND ) );

		bResult = ( pDataSource->vObjects.Obj.at( OBJECT_HAND ).size() > 0 );
	}
	
	else
	{
		// Use simple colour association
		bResult = Detect_Object_From_Primary_Colour( OBJECT_HAND,5,4000 );
	}

	Define_New_Region_of_Interest( OBJECT_HAND, cv::Rect( -40, 5, 80, 90 ) );

	return bResult;
}

bool CProcessImageComputerAssembly::SearchObjects_SecondHand(void)
{
	bool bResult = false;

	if (HandTracker.isReady())
	{
		// Use hand tracker
		HandTracker.TrackObjects(&vSegmentedImage.at(COLOUR_C).Image);

		HandTracker.get_Tracked_Objects_Vector(pDataSource->vObjects.Obj.at(OBJECT_SECONDHAND));

		bResult = (pDataSource->vObjects.Obj.at(OBJECT_SECONDHAND).size() > 0);
	}

	else
	{
		// Use simple colour association
		bResult = Detect_Object_From_Primary_Colour(OBJECT_SECONDHAND);
	}

	Define_New_Region_of_Interest(OBJECT_SECONDHAND, cv::Rect(-40, 5, 80, 90));

	return bResult;
}


void CProcessImageComputerAssembly::ObjectPreProcessing( void )
{
}

void CProcessImageComputerAssembly::ObjectPostProcessing( void )
{
	//LookForNearObjects( OBJECT_HAND, OBJECT_GLASS );		//Look OBJECT_HAND near OBJECT_GLASS

	//Detect_Gestures();

	unsigned int value=2;

	pDataSource->vComputerAssemblyTaskStatus.lock();

	pDataSource->vComputerAssemblyTaskStatus.Obj.at(LearningComputerAssemblyTask::VisionInfo::ScrewBox) = isScrewBoxPresent();

	pDataSource->vComputerAssemblyTaskStatus.Obj.at(LearningComputerAssemblyTask::VisionInfo::PowerSupply) = isPowerSupplyPresent();

	pDataSource->vComputerAssemblyTaskStatus.Obj.at(LearningComputerAssemblyTask::VisionInfo::Cable) = isCablePresent();

	value = isScrewBox_in_HandOverPosition();

	if (value==0 || value==1)
		pDataSource->vComputerAssemblyTaskStatus.Obj.at(LearningComputerAssemblyTask::VisionInfo::ScrewBox_HandOverPosition) = value;

	value = isPowerSupply_in_HandOverPosition();

	if (value == 0 || value == 1)
		pDataSource->vComputerAssemblyTaskStatus.Obj.at(LearningComputerAssemblyTask::VisionInfo::PowerSupply_HandOverPosition) = value;

	value = isCable_in_HandOverPosition();

	if (value == 0 || value == 1)
		pDataSource->vComputerAssemblyTaskStatus.Obj.at(LearningComputerAssemblyTask::VisionInfo::Cable_HandOverPosition) = value;

	value = isHumanHand_in_HandOverPosition();

	if (value == 0 || value == 1)
		pDataSource->vComputerAssemblyTaskStatus.Obj.at(LearningComputerAssemblyTask::VisionInfo::HumanHand_Lifted) = value;

	if (bmemorymode)
	{
		value = isSecondHumanHand_in_HandOverPosition();

		if (value == 0 || value == 1)
			pDataSource->vComputerAssemblyTaskStatus.Obj.at(LearningComputerAssemblyTask::VisionInfo::SecondHumanHand_Lifted) = value;
	}
	else
		pDataSource->vComputerAssemblyTaskStatus.Obj.at(LearningComputerAssemblyTask::VisionInfo::SecondHumanHand_Lifted) = false;
	
	pDataSource->vComputerAssemblyTaskStatus.unlock();
}

// Process the image and retrieve data
void CProcessImageComputerAssembly::ProcessTask(cv::Mat * mat_ptr )
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


unsigned int CProcessImageComputerAssembly::isScrewBoxPresent( void )
{
	return bScrewBoxPresent;
}

unsigned int CProcessImageComputerAssembly::isPowerSupplyPresent( void )
{
	return bPowerSupplyPresent;
}

unsigned int CProcessImageComputerAssembly::isCablePresent(void)
{
	return bCablePresent;
}


unsigned int CProcessImageComputerAssembly::isObject_Lifted(int iOBJECT, float fHeight)
{
	size_t nObj = pDataSource->vObjects.Obj.at(iOBJECT).size();

	cv::Point3f pt(0.0f, 0.0f, 0.0f);

	unsigned int uObjLifted = 0;

	for (size_t iObj = 0; iObj < nObj; iObj++)
	{
		pDataSource->vObjects.Obj.at(iOBJECT).at(iObj).get_Coordinates_World(&pt);

		if (pt.x == 0.0f && pt.y == 0.0f && pt.z == 0.0f) continue;

		if (pt.z > fHeight)
			uObjLifted = 1;
	}

	return uObjLifted;
}


unsigned int CProcessImageComputerAssembly::isObject_in_HandOverPosition(int iOBJECT, std::vector<float> fPosition,float fRadius)
{
	size_t nObj = pDataSource->vObjects.Obj.at(iOBJECT).size();

	cv::Point3f pt(0.0f, 0.0f, 0.0f);

	unsigned int uObjHandOverPosition = 0;

	for (size_t iObj = 0; iObj < nObj; iObj++)
	{
		pDataSource->vObjects.Obj.at(iOBJECT).at(iObj).get_Coordinates_World(&pt);

		if (pt.x == 0.0f && pt.y == 0.0f && pt.z == 0.0f) continue;

		float distance = sqrt(pow(pt.x - fPosition[0], 2) + pow(pt.y - fPosition[1], 2));

		if (distance < fRadius && fPosition[2]<pt.z)
			uObjHandOverPosition = 1;
	}

	return uObjHandOverPosition;
}


unsigned int CProcessImageComputerAssembly::isHumanHand_Lifted()
{
	return isObject_Lifted(OBJECT_HAND, 950.0f);
}

unsigned int CProcessImageComputerAssembly::isSecondHumanHand_Lifted()
{
	return isObject_Lifted(OBJECT_SECONDHAND, 950.0f);
}

unsigned int CProcessImageComputerAssembly::isHumanHand_in_HandOverPosition(void)
{
	std::vector<float> fPosition;

	size_t nObj = pDataSource->vObjects.Obj.at(OBJECT_HAND).size();

	cv::Point3f pt(0.0f, 0.0f, 0.0f);

	for (size_t iObj = 0; iObj < nObj; iObj++)
	{
		pDataSource->vObjects.Obj.at(OBJECT_HAND).at(iObj).get_Coordinates_World(&pt);

		if (pt.x == 0.0f && pt.y == 0.0f && pt.z == 0.0f) return 0;

		if (pt.z > 1000 && pt.y < 100 && pt.x < 1000.0)
			return 1;
		else
			return 0;
	}

	return 2;
}

unsigned int CProcessImageComputerAssembly::isSecondHumanHand_in_HandOverPosition(void)
{
	std::vector<float> fPosition;

	size_t nObj = pDataSource->vObjects.Obj.at(OBJECT_SECONDHAND).size();

	cv::Point3f pt(0.0f, 0.0f, 0.0f);

	for (size_t iObj = 0; iObj < nObj; iObj++)
	{
		pDataSource->vObjects.Obj.at(OBJECT_SECONDHAND).at(iObj).get_Coordinates_World(&pt);

		if (pt.x == 0.0f && pt.y == 0.0f && pt.z == 0.0f) return 0;

		if (pt.z > 1000.0 && pt.y < 100.0f && pt.y > -700.0f && pt.x < 1200.0 && pt.x>550.0)
			return 1;
		else
			return 0;
	}

	return 2;
}

unsigned int CProcessImageComputerAssembly::isScrewBox_in_HandOverPosition(void)
{
	std::vector<float> fPosition;

	if (!bmemorymode)
	{
		fPosition.assign(3, 0.0);

		fPosition[0] = 760.0f;
		fPosition[1] = -250.0f;
		fPosition[2] = 1050.0f;

		return isObject_in_HandOverPosition(OBJECT_SCREWBOX, fPosition, 200.0f);
	}
	else
	{
		size_t nObj = pDataSource->vObjects.Obj.at(OBJECT_SCREWBOX).size();

		cv::Point3f pt(0.0f, 0.0f, 0.0f);

		for (size_t iObj = 0; iObj < nObj; iObj++)
		{
			pDataSource->vObjects.Obj.at(OBJECT_SCREWBOX).at(iObj).get_Coordinates_World(&pt);

			if (pt.x == 0.0f && pt.y == 0.0f && pt.z == 0.0f) return 0;

			if (pt.z > 1000 && pt.y < 100.0f && pt.y > -700.0f && pt.x < 1200.0 && pt.x>650.0)
				return 1;
			else
				return 0;
		}
	}

	return 2;
}

unsigned int CProcessImageComputerAssembly::isPowerSupply_in_HandOverPosition(void)
{
	
	std::vector<float> fPosition;

	if (!bmemorymode)
	{
		fPosition.assign(3, 0.0);

		fPosition[0] = 860.0f;
		fPosition[1] = -280.0f;
		fPosition[2] = 1200.0f;

		return isObject_in_HandOverPosition(OBJECT_POWERSUPPLY, fPosition, 200.0f);
	}
	else
	{
		size_t nObj = pDataSource->vObjects.Obj.at(OBJECT_POWERSUPPLY).size();

		cv::Point3f pt(0.0f, 0.0f, 0.0f);

		for (size_t iObj = 0; iObj < nObj; iObj++)
		{
			pDataSource->vObjects.Obj.at(OBJECT_POWERSUPPLY).at(iObj).get_Coordinates_World(&pt);

			if (pt.x == 0.0f && pt.y == 0.0f && pt.z == 0.0f) return 0;

			if (pt.z > 1050.0f && pt.y < 100.0f && pt.y > -700.0f && pt.x < 1200.0f && pt.x>650.0f)
				return 1;
			else
				return 0;
		}
	}

	return 2;
}

unsigned int CProcessImageComputerAssembly::isCable_in_HandOverPosition(void)
{
	std::vector<float> fPosition;

	if (!bmemorymode)
	{
		fPosition.assign(3, 0.0);

		fPosition[0] = 700.0f;
		fPosition[1] = -280.0f;
		fPosition[2] = 1000.0f;

		return isObject_in_HandOverPosition(OBJECT_CABLE, fPosition, 200.0f);
	}
	else
	{
		size_t nObj = pDataSource->vObjects.Obj.at(OBJECT_CABLE).size();

		cv::Point3f pt(0.0f, 0.0f, 0.0f);

		for (size_t iObj = 0; iObj < nObj; iObj++)
		{
			pDataSource->vObjects.Obj.at(OBJECT_CABLE).at(iObj).get_Coordinates_World(&pt);

			if (pt.x == 0.0f && pt.y == 0.0f && pt.z == 0.0f) return 0;

			if (pt.z > 1000 && pt.y < 100.0f && pt.y > -700.0f && pt.x < 1200.0 && pt.x>650.0)
				return 1;
			else
				return 0;
		}
	}

	return 2;
}
