#ifdef USE_StereoVidere

#include "StereoProcess.h"

CStereoProcess::CStereoProcess( void )
: pDataSource( nullptr )
, objStereo( nullptr )
{
}

CStereoProcess::~CStereoProcess( void )
{
	if( objStereo != nullptr )
	{
		delete objStereo;
		objStereo = nullptr;
	}
	
	pDataSource = nullptr;
}

void CStereoProcess::set_DataSource( CData * ptrData )
{
	pDataSource = ptrData;
}

bool CStereoProcess::CalculateCoordinates( CObject_f * pObj )
{
	float 
		Coord_v[3] = { 0.0f, 0.0f, 0.0f },
		Coord_w[3] = { 0.0f, 0.0f, 0.0f };
	
	bool bValid = objStereo->CalculateCoordinates( pObj->Region, Coord_w, Coord_v );
	
	pObj->set_Coordinates_Vision( Coord_v[0], Coord_v[1], Coord_v[2] );
	pObj->set_Coordinates_World( Coord_w[0], Coord_w[1], Coord_w[2] );

	return bValid;
}

bool CStereoProcess::CalculateCoordinates_Mask( CObject_f * pObj )
{
	float 
		coord_v[3] = { 0.0f, 0.0f, 0.0f },
		coord_w[3] = { 0.0f, 0.0f, 0.0f };
	
	if( objStereo->CalculateCoordinates_Mask( pObj->RoI, pObj->imgMask, coord_w, coord_v ) )
	{
		pObj->set_Coordinates_Vision( coord_v[0], coord_v[1], coord_v[2] );
		pObj->set_Coordinates_World( coord_w[0], coord_w[1], coord_w[2] );

		return true;
	}

	else return false;
}

void CStereoProcess::Perform_Stereo_Process( void )
{
	objStereo->Process_Stereo();

	Process_Stereo_Data();
}

#endif // USE_StereoVidere
