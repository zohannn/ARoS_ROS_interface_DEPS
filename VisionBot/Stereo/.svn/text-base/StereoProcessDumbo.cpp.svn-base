#ifdef USE_StereoVidere

#include "StereoProcessDumbo.h"

CStereoProcessDumbo::CStereoProcessDumbo( void )
: CStereoProcess()
{
	objStereo = new
	#ifndef _DEBUG 
		(std::nothrow) 
	#endif
		CStereoVidereDumbo();
}

CStereoProcessDumbo::~CStereoProcessDumbo( void )
{
	if( objStereo != nullptr )
	{
		delete ( ( CStereoVidereDumbo* ) objStereo );
		objStereo = nullptr;
	}
}

void CStereoProcessDumbo::Process_Stereo_Data( void )
{
	//--------------------------------------------------------------------------
	// Get vision coordinates for all objects
	size_t
		iObjType = 0,
		iObj = 0,
		nObj = 0,
		iSubObj = 0,
		nSubObj = 0;
	
	// Calculate coordinates for all objects
	for( iObjType=0 ; iObjType<NUMBER_OF_OBJECT_TYPES ; iObjType++ )
	{
		nObj = pDataSource->vObjects.Obj.at( iObjType ).size();
		for( iObj=0 ; iObj<nObj ; iObj++ )
		{
			CalculateCoordinates( &pDataSource->vObjects.Obj.at( iObjType ).at( iObj ) );

			switch( iObjType )
			{
			case OBJECT_HANDLE:
				nSubObj = pDataSource->vObjects.Obj.at( iObjType ).at( iObj ).vSubObjects.size();
				for( iSubObj=0 ; iSubObj<nSubObj ; iSubObj++ )
				{
					CalculateCoordinates_Mask( &pDataSource->vObjects.Obj.at( iObjType ).at( iObj ).vSubObjects.at( iSubObj ) );
				}
				break;
			}
		}
	}
}

#endif // USE_StereoVidere
