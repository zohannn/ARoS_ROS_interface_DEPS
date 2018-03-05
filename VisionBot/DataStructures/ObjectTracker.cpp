#include "ObjectTracker.h"

#include "common.h"
#include "vector_operations.hpp"

#ifndef ENABLE_OBJECT_TRACKER_DEBUG
#define ENABLE_OBJECT_TRACKER_DEBUG FALSE
#endif

CObjectTracker::CObjectTracker( unsigned int object_type )
: CBlobTracker()
, fMaxMovement( 0.0f )
, bReady( false )
#if ENABLE_FILE_LOGGING
, strLog( "coord_raw_filt_vel.log" )
, strLogHeader( "dt ; x_raw ; y_raw ; z_raw ; x_filt ; y_filt ; z_filt ; vel_x ; vel_y ; vel_z ; speed" )
#endif
{
	set_Type( object_type );
}

CObjectTracker::CObjectTracker( const CObjectTracker &obj )
: bReady( obj.bReady )
, fMaxMovement( obj.fMaxMovement )
, uObjectType( obj.uObjectType )
#if ENABLE_FILE_LOGGING
, strLog( obj.strLog )
, strLogHeader( obj.strLogHeader )
#endif
{
	vector_copy( obj.vObject, vObject );
}

CObjectTracker::~CObjectTracker( void )
{
	Clear();
}

void CObjectTracker::Setup( unsigned int width, unsigned int height )
{
	CBlobTracker::Setup( width, height );
	bReady = true;

	//--------------------------------------------------------------------------
	// Create log file
#if ENABLE_FILE_LOGGING
	stream_file_log.open( strLog, std::fstream::out );
	
	if( stream_file_log.is_open() )
	{
		stream_file_log << strLogHeader;
		stream_file_log.close();
	}
#endif
}

void CObjectTracker::Clear( void )
{
	bReady = false;
	CBlobTracker::Clear();
	vObject.clear();

#if ENABLE_FILE_LOGGING
	if( stream_file_log.is_open() )
		stream_file_log.close();
#endif
}

void CObjectTracker::set_Type( unsigned int type )
{
	uObjectType = GET_VALID_ObjectType( type );
}

void CObjectTracker::TrackObjects( cv::Mat * mat_ptr )
{
	// Apply blob tracking
	Track( mat_ptr );

	// Retrieve tracked objects from cvBlob
	RetrieveTrackedObjects();
}

float CObjectTracker::get_MaxMovement( void )
{
	return fMaxMovement;
}

unsigned int CObjectTracker::get_ObjectType( void )
{
	return uObjectType;
}

bool CObjectTracker::isReady( void )
{
	return bReady;
}

void CObjectTracker::RetrieveTrackedObjects( void )
{
	cv::Rect rec( 0, 0, 0, 0 );
	cvb::CvTrack * pTrack = nullptr;

	int id = 0;

#if ENABLE_OBJECT_TRACKER_DEBUG
	std::stringstream strm_text( "" );
#endif

	for( auto t=TrackedObjects.begin() ; t!=TrackedObjects.end() ; t++ )
	{
		// Get the pointer of the track
		pTrack = t->second;

		// Get the RoI of the object in the image
		rec = get_RegionFromTrack( pTrack );

		// ID of the object
		id = static_cast<int>( pTrack->id );
		
		// Find the object in the current list of detected objects
		auto it_obj = std::find( vObject.begin(), vObject.end(), id );

	#if ENABLE_OBJECT_TRACKER_DEBUG
		strm_text.str( "" );
		strm_text.clear();
		strm_text << "a = " << pTrack->active << " | i = " << pTrack->inactive << " | l = " << pTrack->lifetime;
	#endif

		// Test if object exists
		if( ( it_obj == vObject.end() ) || ( vObject.size() == 0 ) )
		{
			if( ( pTrack->active > 0 ) && ( pTrack->inactive < 1 ) )
			{
				// The object doesn't exist, add it
				vObject.emplace_back( CObjectMH( uObjectType, rec ) );
				vObject.back().nObjectID = id;
				vObject.back().set_Rounding( true, 10.0f );

			#if ENABLE_OBJECT_TRACKER_DEBUG
				vObject.back().strDebug = strm_text.str();
			#endif
			}
		}
		else
		{
			// The object exists
			if( pTrack->inactive > 1 )
			{
				// if inactive, remove it
				it_obj = vObject.erase( it_obj );
			}
			else
			{
				// If is active, update the region
				it_obj->set_Region( rec );

			#if ENABLE_OBJECT_TRACKER_DEBUG
				it_obj->strDebug = strm_text.str();
			#endif
			}
		}
	}

	pTrack = nullptr;

	bool bFound = false;

	auto it_obj = vObject.begin();

	while( it_obj < vObject.end() )
	{
		bFound = false;

		for( auto t=TrackedObjects.begin() ; t!=TrackedObjects.end() ; t++ )
		{
			pTrack = t->second;

			if( it_obj->nObjectID == pTrack->id )
			{
				bFound = true;
				break;
			}
		}

		if( !bFound )
			it_obj = vObject.erase( it_obj );
		else
			it_obj++;
	}
}

void CObjectTracker::get_Tracked_Objects_Vector( std::vector< CObject_f > &vec )
{
	size_t size_src = vObject.size();

	if( size_src != vec.size() )
		vec.resize( size_src );

	std::copy( vObject.begin(), vObject.end(), vec.begin() );
}

void CObjectTracker::Push_New_Coordinates( std::vector< CObject_f > &vecObject )
{
	cv::Point3f 
		pt_coord_raw( 0.0f, 0.0f, 0.0f ),
		pt_coord_filt( 0.0f, 0.0f, 0.0f ),
		CutOff_Freq_pos( 30.0f, 30.0f, 15.0f ),
		CutOff_Freq_vel( 15.0f, 15.0f, 15.0f );

	float 
		fMov = 0.0f,
		fSTC_filt = 0.0f;

	fMaxMovement = 0.0f;

#if ENABLE_FILE_LOGGING
	float
		dt = 0.0f,
		fSTC_raw = 0.0f,
		speed = 0.0f;

	cv::Point3f vel( 0.0f, 0.0f, 0.0f );

	if( !stream_file_log.is_open() )
		stream_file_log.open( strLog, std::ios::app );
#endif

	for( auto it_obj = vecObject.begin() ; it_obj != vecObject.end() ; it_obj++ )
	{
		auto it_obj_track = std::find( vObject.begin(), vObject.end(), it_obj->nObjectID );

		if( it_obj_track != vObject.end() )
		{
			it_obj->get_Coordinates_World( &pt_coord_raw );
			
			if( pt_coord_raw.x != 0.0f && pt_coord_raw.y != 0 && pt_coord_raw.z != 0.0f )
			{
				it_obj_track->PushPositionCalculateVelocity_ApplyLowPassFiltering( pt_coord_raw, CutOff_Freq_pos, CutOff_Freq_vel );

				// This is the coordinate filtered
				pt_coord_filt = it_obj_track->get_Position_last();

				it_obj->set_Coordinates_World( pt_coord_filt.x, pt_coord_filt.y, pt_coord_filt.z );

				//it_obj_track->Compute_Spatio_Temporal_Curvature( 200.0f, 200.0f );

				fMov = it_obj_track->get_Speed_last();

				if( fMov > fMaxMovement )
					fMaxMovement = fMov;

				#if ENABLE_FILE_LOGGING
				//fSTC_raw = it_obj_track->get_STC_raw();

				dt = it_obj_track->get_dt_last();

				vel = it_obj_track->get_Velocity_last();
				speed = it_obj_track->get_Speed_last();

				if( stream_file_log.is_open() )
				{
					//dt ; x_raw ; y_raw ; z_raw ; x_filt ; y_filt ; z_filt ; vel_x ; vel_y ; vel_z ; speed
					stream_file_log << dt << " ; " <<
						pt_coord_raw.x << " ; " << pt_coord_raw.y << " ; " << pt_coord_raw.z << " ; " <<
						pt_coord_filt.x << " ; " << pt_coord_filt.y << " ; " << pt_coord_filt.z << " ; " <<
						vel.x << " ; " << vel.y << " ; " << vel.z << " ; " <<
						speed << std::endl;
				}
				#endif
			}
		}
	}
}
