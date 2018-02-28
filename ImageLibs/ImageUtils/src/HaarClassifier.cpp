#include "HaarClassifier.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <sstream>

#include "EnvironmentVars.h"

CHaarClassifier::CHaarClassifier( std::string cascade_path, bool isPathRelative )
: strCascadeName( "" )
{
	if( isPathRelative )
	{
		strCascadeName = get_Env_Var_ANSI( "OpenCV_ROOT" );
		
		strCascadeName += "/../data/haarcascades/";
	}

	strCascadeName += cascade_path;
	
	Haar_Cascade.load( strCascadeName );
}

CHaarClassifier::~CHaarClassifier( void )
{
	if( !matImageGray.empty() )
		matImageGray.release();
}

void CHaarClassifier::Remove_Duplicates( std::vector<cv::Rect> &vRect )
{
	std::vector<cv::Rect>::iterator 
		it_rect,
		it_rect_next;

	cv::Rect rect_diff( 0, 0, 0, 0 );

	float 
		fX1 = 0.0f,
		fX2 = 0.0f,
		fY1 = 0.0f,
		fY2 = 0.0f,
		fDist = 0.0f;

	bool bClear = false;

	while( !bClear )
	{
		bClear = true;

		if( vRect.size() < 2 ) break;

		for( it_rect=vRect.begin() ; it_rect<vRect.end() ; it_rect++ )
		{
			it_rect_next = it_rect + 1;
			if( it_rect_next >= vRect.end() ) continue;

			fX1 = static_cast<float>( it_rect->x + it_rect->height/2.0f );
			fX2 = static_cast<float>( it_rect_next->x + it_rect_next->height/2.0f );

			fY1 = static_cast<float>( it_rect->y + it_rect->width/2.0f );
			fY2 = static_cast<float>( it_rect_next->y + it_rect_next->width/2.0f );

			fDist = sqrt( pow( fX1-fX2, 2.0f ) + pow( fY1-fY2, 2.0f ) );

			if( fDist<30.0f )
			{
				bClear = false;

				if( it_rect->area() > it_rect_next->area() )
				{
					it_rect = vRect.erase( it_rect );
				}
				else
				{
					it_rect_next = vRect.erase( it_rect_next );
				}
			}
		}
	}
}

bool CHaarClassifier::Detect( cv::Mat * img, double scale )
{
	if( Haar_Cascade.empty() || ( img == nullptr ) ) return false;

	if( matImageGray.empty() )
	{
		matImageGray.create( img->size(), img->type() );
	}
	else
	{
		if( img->rows*img->cols != matImageGray.rows*matImageGray.cols )
		{
			matImageGray.release();
			matImageGray.create( img->size(), img->type() );
		}
	}

	matImageGray.setTo( 0 );

	vObjects.clear();

#ifdef USE_NVIDIA_CUDA

	cv::gpu::cvtColor( cv::gpu::GpuMat( *img ), matImageGray, CV_BGR2GRAY );
	
	cv::gpu::GpuMat objbuf;
	cv::Mat obj_storage;

	int detections_number = Haar_Cascade.detectMultiScale( 
		matImageGray,
		objbuf,
		1.0 / scale,
		2,
		cv::Size( 20, 20 ) );

	if( detections_number<1 ) return false;
	
	objbuf.colRange( 0, detections_number ).download( obj_storage );

	cv::Rect* faces_rect = obj_storage.ptr<cv::Rect>();

	vObjects.assign( detections_number, cv::Rect( 0, 0, 0, 0 ) );

	for( int i=0 ; i<detections_number ; i++ )
	{
		vObjects.at( i ) = faces_rect[i];
	}

	if( !obj_storage.empty() )
		obj_storage.release();

	if( !objbuf.empty() )
		objbuf.release();

#else

	cv::cvtColor( (*img), matImageGray, 
	#if( ( CV_VERSION_MAJOR >= 3 ) && !defined( CV_VERSION_EPOCH ) )
		cv::COLOR_BGR2GRAY
	#else
		CV_BGR2GRAY
	#endif
		);

	cv::equalizeHist( matImageGray, matImageGray );

	Haar_Cascade.detectMultiScale(
		matImageGray,
		vObjects,
		1.0 / scale,
		2,
		0,
		cv::Size( 20, 20 ) );
#endif

	Remove_Duplicates( vObjects );

	return ( vObjects.size() > 0 );
}

//Draw------------------------------------------------------------------------
void CHaarClassifier::Draw ( cv::Mat * img, bool bText )
{
	size_t vObj_size = vObjects.size();

	if( vObj_size <= 0 ) return;

	std::stringstream text;

	cv::Point pt_text;

	for( size_t i=0 ; i < vObj_size ; i++ )
    {
        // Draw the rectangle in the input image
		if( bText )
		{
			text.str( "# " );
			text << i+1;

			pt_text.x = vObjects.at( i ).x;
			pt_text.y = vObjects.at( i ).y + vObjects.at( i ).height + 25;

			cv::putText( *img, text.str().c_str(), pt_text, cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar( 0.0, 0.0, 255.0 ) );
		}

		cv::rectangle( *img, vObjects.at( i ), cv::Scalar( 0.0, 0.0, 255.0 ), 3 );
    }
}

size_t CHaarClassifier::get_NumberOfObjects( void )
{
	return vObjects.size();
}

void CHaarClassifier::get_RegionOfObjects( std::vector<cv::Rect> &vRect )
{
	vRect.assign( vObjects.begin(), vObjects.end() );
}

cv::Rect CHaarClassifier::get_RegionOfObject( int iObj )
{
	int nObjects = static_cast<int>( vObjects.size() );

	bool bValid = ( nObjects > 0 ) && ( iObj < nObjects );

	return bValid ? vObjects.at( iObj ) : cv::Rect( 0, 0, 0, 0 );
}

void CHaarClassifier::get_RegionOfObject( int iObj, cv::Rect * rect )
{
	*rect = get_RegionOfObject( iObj );
}

int CHaarClassifier::get_AreaOfObject( int iObj )
{
	int nObjects = static_cast<int>( vObjects.size() );

	bool bValid = ( nObjects > 0 ) && ( iObj < nObjects );

	return bValid ? vObjects.at( iObj ).area() : 0;
}
