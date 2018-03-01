#include "BlobTracker.h"

CBlobTracker::CBlobTracker( void )
{
}

CBlobTracker::CBlobTracker( unsigned int width, unsigned int height )
{
	Setup( width, height );
}

CBlobTracker::~CBlobTracker( void )
{
	Clear();
}

void CBlobTracker::Setup( unsigned int width, unsigned int height )
{
	if( !mat_LabelImage.empty() )
		mat_LabelImage.release();

	mat_LabelImage.create( height, width, CV_32FC(1) );
	mat_LabelImage.setTo( 0 );

	if( !mat_ImageBlobs.empty() )
		mat_ImageBlobs.release();

	mat_ImageBlobs.create( height, width, CV_8UC1 );
	mat_ImageBlobs.setTo( 0 );
}

void CBlobTracker::Clear( void )
{
	if( !mat_LabelImage.empty() )
		mat_LabelImage.release();

	if( !mat_ImageBlobs.empty() )
		mat_ImageBlobs.release();

	cvb::cvReleaseTracks( TrackedObjects );
}

cv::Rect CBlobTracker::get_RegionFromTrack( cvb::CvTrack * ptrTrack )
{
	cv::Rect rec( 0, 0, 0, 0 );

	if( ptrTrack == nullptr ) return rec;

	rec.x = static_cast<int>( ptrTrack->minx );
	rec.y = static_cast<int>( ptrTrack->miny );
	rec.width = std::abs( static_cast<int>( ptrTrack->maxx - ptrTrack->minx - 1 ) );
	rec.height = std::abs( static_cast<int>( ptrTrack->maxy - ptrTrack->miny - 1 ) );

	return rec;
}

void CBlobTracker::Track( cv::Mat * pImage )
{
	if( pImage == nullptr ) return;
	
	if( pImage->empty() ) return;

	if( ( pImage->cols != mat_ImageBlobs.cols ) || ( pImage->rows != mat_ImageBlobs.rows ) )
		Setup( pImage->cols, pImage->rows );

	pImage->copyTo( mat_ImageBlobs );

	cvb::CvBlobs blobs;

	// Label the blobs
	cvb::cvLabel( &( (IplImage) mat_ImageBlobs ), &( (IplImage) mat_LabelImage ), blobs );

	// Update tracks according to detected blobs
	cvb::cvUpdateTracks( blobs, TrackedObjects, 30.0, 2 );

	// Manually update inactive flags, not required
	//UpdateInactiveFlags( pImage );

	cvb::cvReleaseBlobs( blobs );
}

void CBlobTracker::UpdateInactiveFlags( cv::Mat * mat_ptr )
{
	cv::Rect rec( 0, 0, 0, 0 );
	cvb::CvTrack * pTrack = nullptr;
	int nPixels = 0;

	for( auto t=TrackedObjects.begin() ; t!=TrackedObjects.end() ; t++ )
	{
		// Get the pointer of the track
		pTrack = t->second;

		// Get the RoI of the object in the image
		rec = get_RegionFromTrack( pTrack );

		nPixels = cv::countNonZero( (*mat_ptr)( rec ) );

		if( nPixels < 1 )
			pTrack->inactive++;
	}
}
