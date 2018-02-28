#ifndef _BLOB_TRACKER_H_
#define _BLOB_TRACKER_H_

#pragma once

#include <cvblob.h>

/// <summary>Blob tracker based in binary images implemented using cvBlob library.</summary>
class CBlobTracker
{
public:
	/// <summary>Default constructor.</summary>
	CBlobTracker( void );

	/// <summary>Constructor where the image size can be specified.
	/// <para>This invokes the function <see cref="CBlobTracker::Setup(int,int)"/>.</para></summary>
	/// <param name="width">Width of the image.</param>
	/// <param name="height">Height of the image.</param>
	CBlobTracker( unsigned int width, unsigned int height );
	
	/// <summary>Class destructor.</summary>
	~CBlobTracker( void );

	/// <summary>Setup the image that will be used (width and height).</summary>
	/// <param name="width">Width of the image.</param>
	/// <param name="height">Height of the image.</param>
	void Setup( unsigned int width, unsigned int height );

	/// <summary>Clear allocated memory.</summary>
	void Clear( void );

	/// <summary>Track objects in image.</summary>
	/// <param name="pImage">Pointer to image that will be used for tracking.</param>
	void Track( cv::Mat * pImage );

protected:
	/// <summary>cvBlob structure that holds the tracked objects.</summary>
	cvb::CvTracks TrackedObjects;

	cv::Mat 
		/// <summary>Image to be used by cvBlob for labelling.</summary>
		mat_LabelImage,
		/// <summary>Image to be used by cvBlob for blobs.</summary>
		mat_ImageBlobs;

	/// <summary>Extract a region from a CvTrack.</summary>
	/// <param name="ptrTrack">Pointer to the track.</param>
	/// <returns>A rectangle with the track region.</returns<
	cv::Rect get_RegionFromTrack( cvb::CvTrack * ptrTrack );

	/// <summary>This function goes through all the tracks and checks if there 
	/// are any pixels inside the region.
	// <para>If there are no pixels, increments the inactive flag.</para>
	// <para>Currently not being used!</para></summary
	/// <param name="ptr">Pointer to the image.</param>
	void UpdateInactiveFlags( cv::Mat * mat_ptr );
};

#endif // _BLOB_TRACKER_H_
