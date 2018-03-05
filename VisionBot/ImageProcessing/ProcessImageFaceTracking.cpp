#include "ProcessImageFaceTracking.h"

#include "vector_operations.hpp"

CProcessImageFaceTracking::CProcessImageFaceTracking( int width, int height )
: CProcessImage( width, height )
, Face( true, false )
#ifdef USE_AmariField2D
, DynField()
#endif
{
	sSegFile += "Videre/Segmentation_Parameters (Colour Learning).ini";

	SetupProcessing( width, height );

	nProcessImageID = ProcessImageID::PI_FaceTracking;
}

bool CProcessImageFaceTracking::SearchAllObjects( void )
{
	return SearchFaces();
}

void CProcessImageFaceTracking::ObjectPreProcessing( void )
{
}

void CProcessImageFaceTracking::ProcessTask( cv::Mat * mat_ptr )
{
	//--------------------------------------------------------------------------
	// Search for objects
	if( SearchAllObjects() ) 
		Face.Draw( mat_ptr, true );
}

bool CProcessImageFaceTracking::SearchFaces( void )
{
	pDataSource->vObjects.Obj.at( OBJECT_FACE ).clear();

	bool bFound = Face.Detect( &mat_ImageProcessed.Obj, 0.8 );

	if( bFound )
	{
		cv::Rect face_region;

		for( size_t i=0 ; i<Face.get_NumberOfFaces() ; i++ )
		{
			Face.get_RegionOfFace( i, &face_region );
			pDataSource->vObjects.Obj.at( OBJECT_FACE ).emplace_back( CObject_f( OBJECT_FACE, face_region ) );
		}
	}

	return bFound;
}

void CProcessImageFaceTracking::ObjectPostProcessing( void )
{
	ProcessAttention();
}

void CProcessImageFaceTracking::ProcessAttention( void )
{
#ifdef USE_AmariField2D
	size_t nFaces = pDataSource->vObjects.Obj.at( OBJECT_FACE ).size();

	if( nFaces<=0 ) return;
	
	std::vector< CObject_f > vFaces;

	vector_copy( pDataSource->vObjects.Obj.at( OBJECT_FACE ), vFaces );

	DynField.ClearInputs();

	float
		fPan = 0.0f,
		fTilt = 0.0f,
		distance = 0.0f;

	CPoint<float> pantilt;

	for( size_t i=0 ; i<nFaces ; i++ )
	{
		vFaces.at( i ).get_PanTiltAngles( &fPan, &fTilt );
		distance = vFaces.at( i ).get_DistanceFromRobot();
		
		pantilt.x = fPan;
		pantilt.y = fTilt;

		DynField.AddInput( pantilt, distance );
	}

	//DynField.Calculate_Kernel_Integral();
#endif
}
