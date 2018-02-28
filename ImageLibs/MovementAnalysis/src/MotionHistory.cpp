#include "MotionHistory.h"

#include "Filters.h"
#include "MathOperations.h"
#include "vector_operations.hpp"

CMotionHistory::CMotionHistory( unsigned int count )
: uHistorySize( count )
, bEnableRounding( false )
, fRoundingValue( 0.0f )
{
	time_count.set_Time_Start();
	
	Setup_vectors();
}

void CMotionHistory::Setup_vectors( void )
{
	vPos.set_limit( uHistorySize );
	vVel.set_limit( uHistorySize );
	vPos_Delta.set_limit( uHistorySize );
	vVel_Delta.set_limit( uHistorySize );
	vPos_scalar.set_limit( uHistorySize );
	vSpeed.set_limit( uHistorySize );
	v_dt.set_limit( uHistorySize );
	vSTC.set_limit( uHistorySize );
	vSTC_filt_1.set_limit( uHistorySize );
	vSTC_filt_2.set_limit( uHistorySize );
}

CMotionHistory::CMotionHistory( const CMotionHistory &obj )
{
	Copy_From( &obj );
}

CMotionHistory::~CMotionHistory( void )
{
	ClearMotionHistory();
}

void CMotionHistory::ClearMotionHistory( void )
{
	uHistorySize = 0;
	
	Setup_vectors();
	
	vPos.clear();
	vVel.clear();
	vPos_Delta.clear();
	vVel_Delta.clear();
	vPos_scalar.clear();
	vSpeed.clear();
	v_dt.clear();
	vSTC.clear();
	vSTC_filt_1.clear();
	vSTC_filt_2.clear();
}

void CMotionHistory::Copy_From( const CMotionHistory * obj )
{
	uHistorySize = obj->uHistorySize;
	bEnableRounding = obj->bEnableRounding;
	fRoundingValue = obj->fRoundingValue;

	vector_copy( obj->vPos, vPos );
	vector_copy( obj->vVel, vVel );
	vector_copy( obj->vPos_Delta, vPos_Delta );
	vector_copy( obj->vVel_Delta, vVel_Delta );
	vector_copy( obj->vPos_scalar, vPos_scalar );
	vector_copy( obj->vSpeed, vSpeed );
	vector_copy( obj->v_dt, v_dt );
	vector_copy( obj->vSTC, vSTC );
	vector_copy( obj->vSTC_filt_1, vSTC_filt_1 );
	vector_copy( obj->vSTC_filt_2, vSTC_filt_2 );

	Setup_vectors();
}

void CMotionHistory::set_MotionHistorySize( unsigned int count )
{
	uHistorySize = count;
}

void CMotionHistory::set_Rounding( bool enable, float rounding_val )
{
	bEnableRounding = enable;
	fRoundingValue = rounding_val;
}

void CMotionHistory::PushPosition( cv::Point3f pos )
{
	cv::Point3f pt_round( pos );

	if( bEnableRounding )
	{
		pt_round.x = round_rem( pos.x, fRoundingValue );
		pt_round.y = round_rem( pos.y, fRoundingValue );
		pt_round.z = round_rem( pos.z, fRoundingValue );
	}

	vPos.emplace_back_limited( pt_round );
}

void CMotionHistory::PushPositionDelta( cv::Point3f pos )
{
	vPos_Delta.emplace_back_limited( pos );
}

void CMotionHistory::PushVelocity( cv::Point3f vel )
{
	vVel.emplace_back_limited( vel );

	vSpeed.emplace_back_limited( Calculate_Speed( vel ) );
}

void CMotionHistory::PushPositionCalculateVelocity( cv::Point3f pos )
{
	//-----------------------------------------------------------------------
	// Calculate dt
	time_count.set_Time_Finish();
	float dt = static_cast<float>( time_count.HowLong() );
	time_count.set_Time_Start();

	v_dt.emplace_back_limited( dt );

	//-----------------------------------------------------------------------
	// Add current position
	PushPosition( pos );

	int size = static_cast<int>( vPos.size() );

	//-----------------------------------------------------------------------
	if( size > 1 )
	{
		cv::Point3f delta_p( 0.0f, 0.0f, 0.0f );

		// Calculate position variation
		delta_p.x = vPos.get( size - 1 ).x - vPos.get( size - 2 ).x;
		delta_p.y = vPos.get( size - 1 ).y - vPos.get( size - 2 ).y;
		delta_p.z = vPos.get( size - 1 ).z - vPos.get( size - 2 ).z;
		
		PushPositionDelta( delta_p );
		
		// Calculate velocity
		delta_p.x /= dt;
		delta_p.y /= dt;
		delta_p.z /= dt;

		PushVelocity( delta_p );
	}
}

void CMotionHistory::PushPosition_ApplyLowPassFiltering( cv::Point3f pos, float cutoff_freq )
{
	PushPosition_ApplyLowPassFiltering( pos, cv::Point3f( cutoff_freq, cutoff_freq, cutoff_freq ) );
}

void CMotionHistory::PushPosition_ApplyLowPassFiltering( cv::Point3f pos, cv::Point3f cutoff_freq )
{
	const float	fSampleRate = 1000.0f;

	if( vPos.empty() )
	{
		PushPosition( pos );
	}
	else
	{
		cv::Point3f pos_filtered( 0.0f, 0.0f, 0.0f );

		pos_filtered.x = apply_lowpass_filter_single_value( pos.x, vPos.back().x, cutoff_freq.x, fSampleRate );
		pos_filtered.y = apply_lowpass_filter_single_value( pos.y, vPos.back().y, cutoff_freq.y, fSampleRate );
		pos_filtered.z = apply_lowpass_filter_single_value( pos.z, vPos.back().z, cutoff_freq.z, fSampleRate );

		PushPosition( pos_filtered );
	}
}

void CMotionHistory::PushVelocity_ApplyLowPassFiltering( cv::Point3f vel, float cutoff_freq )
{
	PushVelocity_ApplyLowPassFiltering( vel, cv::Point3f( cutoff_freq, cutoff_freq, cutoff_freq ) );
}

void CMotionHistory::PushVelocity_ApplyLowPassFiltering( cv::Point3f vel, cv::Point3f cutoff_freq )
{
	const float	fSampleRate = 1000.0f;

	if( vVel.empty() )
	{
		PushVelocity( vel );
	}
	else
	{
		cv::Point3f vel_filtered( 0.0f, 0.0f, 0.0f );

		vel_filtered.x = apply_lowpass_filter_single_value( vel.x, vVel.back().x, cutoff_freq.x, fSampleRate );
		vel_filtered.y = apply_lowpass_filter_single_value( vel.y, vVel.back().y, cutoff_freq.y, fSampleRate );
		vel_filtered.z = apply_lowpass_filter_single_value( vel.z, vVel.back().z, cutoff_freq.z, fSampleRate );

		PushVelocity( vel_filtered );
	}
}

void CMotionHistory::PushPositionCalculateVelocity_ApplyLowPassFiltering( cv::Point3f pos, float cutoff_freq_pos, float cutoff_freq_vel )
{
	PushPositionCalculateVelocity_ApplyLowPassFiltering( 
		pos,
		cv::Point3f( cutoff_freq_pos, cutoff_freq_pos, cutoff_freq_pos ),
		cv::Point3f( cutoff_freq_vel, cutoff_freq_vel, cutoff_freq_vel )
		);
}

void CMotionHistory::PushPositionCalculateVelocity_ApplyLowPassFiltering( cv::Point3f pos, cv::Point3f cutoff_freq_pos, cv::Point3f cutoff_freq_vel )
{
	//-----------------------------------------------------------------------
	// Calculate dt
	time_count.set_Time_Finish();
	float dt = static_cast<float>( time_count.HowLong() );
	time_count.set_Time_Start();

	v_dt.emplace_back_limited( dt );

	//-----------------------------------------------------------------------
	// Add current position

	PushPosition_ApplyLowPassFiltering( pos, cutoff_freq_pos );

	cv::Point3f delta_p( 0.0f, 0.0f, 0.0f );

	size_t size = vPos.size();

	//-----------------------------------------------------------------------
	if( size > 1 )
	{
		// Calculate position variation
		delta_p.x = vPos.at( size - 1 ).x - vPos.at( size - 2 ).x;
		delta_p.y = vPos.at( size - 1 ).y - vPos.at( size - 2 ).y;
		delta_p.z = vPos.at( size - 1 ).z - vPos.at( size - 2 ).z;
		
		PushPositionDelta( delta_p );
		
		// Calculate velocity and convert to cm/s
		delta_p.x /= dt;
		delta_p.y /= dt;
		delta_p.z /= dt;

		PushVelocity_ApplyLowPassFiltering( delta_p, cutoff_freq_vel );
		//PushVelocity( delta_p );
	}
}

cv::Point3f CMotionHistory::get_Position( int index )
{
	return vPos.get( index );
}

cv::Point3f CMotionHistory::get_Position_last( void )
{
	return vPos.last();
}

cv::Point3f CMotionHistory::get_Velocity( int index )
{
	return vVel.get( index );
}

cv::Point3f CMotionHistory::get_Velocity_last( void )
{
	return vVel.last();
}

float CMotionHistory::get_Speed( int index )
{
	return vSpeed.get( index );
}

float CMotionHistory::get_Speed_last( void )
{
	return vSpeed.last();
}

float CMotionHistory::get_dt( int index )
{
	if( !v_dt.empty() )
		return v_dt.at( index );
	else
		return 1.0f;
}

float CMotionHistory::get_dt_last( void )
{
	if( !v_dt.empty() )
		return v_dt.back();
	else
		return 1.0f;
}

float CMotionHistory::get_STC_raw( void )
{
	return vSTC.last();
}

float CMotionHistory::get_STC_filtered( void )
{
	return vSTC_filt_1.last();
}

float CMotionHistory::Calculate_Spatio_Temporal_Curvature( void )
{
	size_t 
		size_delta = vPos_Delta.size(),
		size_dt = v_dt.size();
	
	if( ( size_delta < 2 ) || ( size_dt < 2 ) )
		return 0.0f;
	
	cv::Point3f 
		pos_diff = vPos_Delta.back(),
		pos_diff_prev = vPos_Delta[ size_delta - 2 ];

	float
		dt = v_dt.back(),
		dt2 = pow( dt, 2.0f ),
		xd = pos_diff.x / dt,
		yd = pos_diff.y / dt,
		zd = pos_diff.z / dt,
		xd2 = ( pos_diff.x - pos_diff_prev.x ) / dt2,
		yd2 = ( pos_diff.y - pos_diff_prev.y ) / dt2,
		zd2 = ( pos_diff.z - pos_diff_prev.z ) / dt2,
		A = yd * dt2 - dt * yd2,
		B = dt * xd2 - xd * dt2,
		C = xd * yd2 - yd * xd2,
		D = zd * dt2 - dt * zd2,
		E = zd * xd2 - xd * zd2,
		F = zd * yd2 - yd * zd2;
	
	float num = 
			pow( A, 2.0f ) + 
			pow( B, 2.0f ) + 
			pow( C, 2.0f ) + 
			pow( D, 2.0f ) + 
			pow( E, 2.0f ) + 
			pow( F, 2.0f );

	float den = 
			pow( xd, 2.0f ) + 
			pow( yd, 2.0f ) + 
			pow( zd, 2.0f ) + 
			pow( dt, 2.0f );

	// sqrt( num ) / sqrt( den^3 ) )
	return sqrt( num ) / pow( den, 1.5f );
}

void CMotionHistory::Compute_Spatio_Temporal_Curvature( float cutoff_freq_1, float cutoff_freq_2 )
{
	// Define max value for STC
	const float fSTC_limit = 2000.0f;

	// Compute current value of STC
	float 
		fSTC = Calculate_Spatio_Temporal_Curvature(),
		fSTC_previous = 0.0f;

	// Enforce maximum limit for instant STC value
	float fSTC_current = ( fSTC < fSTC_limit ) ? fSTC : fSTC_limit;

	vSTC.emplace_back_limited( fSTC_current );

	//-----------------------------------------------------------------------
	// Apply first stage of filtering
	if( vSTC_filt_1.size() > 0 )
		fSTC_previous = vSTC_filt_1.back();

	float fSTC_filt = apply_lowpass_filter_single_value( fSTC_current, fSTC_previous, cutoff_freq_1 );

	vSTC_filt_1.emplace_back_limited( fSTC_filt );

	//-----------------------------------------------------------------------
	// Apply second stage of filtering
	/*if( vSTC_filt_2.size() > 0 )
		fSTC_previous = vSTC_filt_2.back();

	fSTC_current = fSTC_filt;

	fSTC_filt = apply_lowpass_filter_single_value( fSTC_current, fSTC_previous, cutoff_freq_2 );
	
	vSTC_filt_2.emplace_back_limited( fSTC_filt );*/
}

float CMotionHistory::MeanSpeed( void )
{
	size_t size = vSpeed.size();

	if( size < 1 ) return 0.0f;

	float fRes = 0.0f;

	for( size_t i=0 ; i<size ; i++ )
	{
		fRes += vSpeed[i];
	}
	
	return fRes/static_cast<float>( size );
}

float CMotionHistory::MaxSpeed( void )
{
	float fMax = 0.0f;

	if( vSpeed.size() > 0 )
	{
		fMax = *std::max_element( vSpeed.begin(), vSpeed.end() );
	}
	
	return fMax;
}

float CMotionHistory::MinSpeed( void )
{
	float fMin = 0.0f;

	if( vSpeed.size() > 0 )
	{
		fMin = *std::min_element( vSpeed.begin(), vSpeed.end() );
	}
	
	return fMin;
}

float CMotionHistory::TotalDistance( void )
{
	float fRes = 0.0f;

	size_t vPosDelta_size = vPos_Delta.size();

	for( size_t i=0 ; i<vPosDelta_size ; i++ )
	{
		fRes += Calculate_Distance( vPos_Delta[i] );
	}

	return fRes;
}

float CMotionHistory::MeanDistance( void )
{
	return TotalDistance()/static_cast<float>( vPos_Delta.size() );
}
