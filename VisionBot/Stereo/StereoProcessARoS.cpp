#ifdef USE_StereoVidere

#include "StereoProcessARoS.h"

CStereoProcessARoS::CStereoProcessARoS( void )
: CStereoProcess()
{
	objStereo = new 
	#ifndef _DEBUG 
		(std::nothrow) 
	#endif
		CStereoVidereARoS();
}

CStereoProcessARoS::~CStereoProcessARoS( void )
{
	if( objStereo != nullptr )
	{
		delete ( ( CStereoVidereARoS* ) objStereo );
		objStereo = nullptr;
	}
}

void CStereoProcessARoS::CalculatePanTiltFromCoordinates( CObject_f * pObj )
{
	cv::Point3f coord;
	
	float
		pan = 0.0f,
		tilt = 0.0f;

	pObj->get_Coordinates_World( &coord );
	
	CStereoVidereARoS::CalculatePanTiltFromCoordinates( coord.x, coord.y, coord.z, &pan, &tilt );

	pObj->set_PanTiltAngles( pan, tilt );
}

void CStereoProcessARoS::Process_Stereo_Data( void )
{
	//--------------------------------------------------------------------------
	// Get vision coordinates for all objects

	size_t
		i = 0, 
		j = 0,
		k = 0,
		n = 0,
		nSubObj = 0,
		nNearObj = 0;

	CObject_f * pObj = nullptr;

	// Calculate coordinates for all objects
	for( j=0 ; j<NUMBER_OF_OBJECT_TYPES ; j++ )
	{
		n = pDataSource->vObjects.Obj.at( j ).size();
		for( i=0 ; i<n ; i++ )
		{
			pObj = &pDataSource->vObjects.Obj.at( j ).at( i );

			unsigned int obj_type = pObj->get_Type();

			// Main coordinates
			switch( obj_type )
			{
			case OBJECT_BASE:
			case OBJECT_TOP_FLOOR:
				CalculateCoordinates( pObj );
				break;

			default:
				CalculateCoordinates_Mask( pObj );
				break;
			}

			CalculatePanTiltFromCoordinates( pObj );

			// Special operations required by certain types of objects
			switch( obj_type )
			{
			// If the object is a base, a column or a wheel, calculate coordinates for sub-objects
			
			case OBJECT_BASE:// to allow orientation calculation
			case OBJECT_WHEEL:// to allow post processing in Toy Robot Learning Task
				nNearObj = pObj->vNearObjects.size();
				for( k=0 ; k<nNearObj ; k++ )
				{
					CalculateCoordinates_Mask( &pObj->vNearObjects.at( k ) );
				}

			// to allow orientation calculation
			case OBJECT_COLUMN_1:
			case OBJECT_COLUMN_2:
			case OBJECT_COLUMN_3:
			case OBJECT_COLUMN_4:
			case OBJECT_HANDLE:
				nSubObj = pObj->vSubObjects.size();
				for( k=0 ; k<nSubObj ; k++ )
				{
					// The coordinates for the holes need to be calculated without the mask
					if( obj_type == OBJECT_BASE )
						CalculateCoordinates( &pObj->vSubObjects.at( k ) );
					else
						CalculateCoordinates_Mask( &pObj->vSubObjects.at( k ) );
				}
				break;
 
			default:
				break;
			}
		}
	}

	pObj = nullptr;
}

#endif // USE_StereoVidere
