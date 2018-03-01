#include "FaceDetection.h"

CFaceDetection::CFaceDetection( bool frontal_detection, bool profile_detection )
: Face_frontal( "haarcascade_frontalface_alt.xml" )
, Face_profile( "haarcascade_profileface.xml" )
, bFrontal( frontal_detection )
, bProfile( profile_detection )
{
}

size_t CFaceDetection::get_NumberOfFaces( void )
{
	return vFaces.size();
}

void CFaceDetection::get_RegionOfFaces( cv::Rect * rect )
{
	size_t nFaces = vFaces.size();

	if( nFaces <= 0 ) return;

	for( size_t i=0 ; i<nFaces ; i++ )
	{
		rect[i] = get_RegionOfFace( i );
	}
}

cv::Rect CFaceDetection::get_RegionOfFace( size_t iFace )
{
	size_t nFaces = vFaces.size();

	if( ( nFaces == 0 ) || ( iFace > nFaces ) ) return cv::Rect( 0, 0, 0, 0 );

	else return vFaces.at( iFace );
}

void CFaceDetection::get_RegionOfFace( size_t iFace, cv::Rect * rect )
{
	*rect = get_RegionOfFace( iFace );
}

int CFaceDetection::get_FaceArea( unsigned int iFace )
{
	size_t nFaces = vFaces.size();
	
	bool bValid = ( nFaces > 0 ) && ( iFace < nFaces );
	
	return bValid ? vFaces.at( iFace ).area() : 0;
}

bool CFaceDetection::Detect( cv::Mat * img, double scale )
{
	bool 
		bFr = false,
		bPr = false;

	vFaces.clear();

	size_t
		i = 0,
		n = 0;

	if( bFrontal )
	{
		bFr = Face_frontal.Detect( img, scale );

		if( bFr )
		{
			n = Face_frontal.get_NumberOfObjects();
			for( i=0 ; i<n ; i++ )
			{
				vFaces.emplace_back( Face_frontal.get_RegionOfObject( static_cast<int>( i ) ) );
			}
		}
	}

	if( bProfile )
	{
		bPr = Face_profile.Detect( img, scale );
		if( bPr )
		{
			n = Face_profile.get_NumberOfObjects();
			for( i=0 ; i<n ; i++ )
			{
				vFaces.emplace_back( Face_profile.get_RegionOfObject( static_cast<int>( i ) ) );
			}
		}
	}

	CHaarClassifier::Remove_Duplicates( vFaces );

	return( bFr || bPr );
}

void CFaceDetection::Draw( cv::Mat * img, bool bText )
{
	if( bFrontal ) Face_frontal.Draw( img, bText );
	if( bProfile ) Face_profile.Draw( img, bText );
}
