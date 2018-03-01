#ifndef _MOTION_HISTORY_H_
#define _MOTION_HISTORY_H_

#pragma once

#include <opencv2/core/core.hpp>

#include "TimeCountOpenCV.h"
#include "vector_ex.hpp"

class CMotionHistory
{
//---------------------------------------------------------------------------
// Construction / Initialization and Destrution / Cleaning
public:
	CMotionHistory( unsigned int count = 5 );
	CMotionHistory( const CMotionHistory &obj );
	virtual ~CMotionHistory( void );

	void Copy_From( const CMotionHistory * obj );
	void ClearMotionHistory( void );

	void set_MotionHistorySize( unsigned int count );

protected:
	void Setup_vectors( void );

//---------------------------------------------------------------------------
// Data variables
protected:
	unsigned int uHistorySize;

	CTimeCountBasicOpenCV time_count;

	vector_ex< cv::Point3f > 
		vPos,
		vVel,
		vPos_Delta,
		vVel_Delta;

	vector_ex<float>
		vPos_scalar,
		vSpeed,
		v_dt,
		vSTC,
		vSTC_filt_1,
		vSTC_filt_2;

	bool bEnableRounding;

	float fRoundingValue;

//---------------------------------------------------------------------------
// Position and velocity adding
protected:
	void PushPosition( cv::Point3f pos );
	void PushPosition_ApplyLowPassFiltering( cv::Point3f pos, float cutoff_freq );
	void PushPosition_ApplyLowPassFiltering( cv::Point3f pos, cv::Point3f cutoff_freq );

	void PushPositionDelta( cv::Point3f pos );
	
	void PushVelocity( cv::Point3f vel );
	void PushVelocity_ApplyLowPassFiltering( cv::Point3f vel, float cutoff_freq );
	void PushVelocity_ApplyLowPassFiltering( cv::Point3f vel, cv::Point3f cutoff_freq );

public:
	void PushPositionCalculateVelocity( cv::Point3f pos );
	void PushPositionCalculateVelocity_ApplyLowPassFiltering( cv::Point3f pos, float cutoff_freq_pos, float cutoff_freq_vel );
	void PushPositionCalculateVelocity_ApplyLowPassFiltering( cv::Point3f pos, cv::Point3f cutoff_freq_pos, cv::Point3f cutoff_freq_vel );

//---------------------------------------------------------------------------
// Specific functions to be implemented in derived classes
public:
	virtual float MeanDistance( void );
	virtual float TotalDistance( void );

	float MeanSpeed( void );
	float MaxSpeed( void );
	float MinSpeed( void );

	virtual float Calculate_Distance( cv::Point3f dist3 ) = 0;
	virtual float Calculate_Speed( cv::Point3f vel ) = 0;

//---------------------------------------------------------------------------
// Accessors
public:
	cv::Point3f get_Position( int index );
	cv::Point3f get_Position_last( void );
	
	cv::Point3f get_Velocity( int index );
	cv::Point3f get_Velocity_last( void );
	
	float get_Speed( int index );
	float get_Speed_last( void );

	float get_dt( int index );
	float get_dt_last( void );
	
	float get_STC_raw( void );
	float get_STC_filtered( void );

	void set_Rounding( bool enable, float rounding_val = 0.0f );

//---------------------------------------------------------------------------
// Spatio Temporal Curvature related functions
protected:
	float Calculate_Spatio_Temporal_Curvature( void );

public:
	void Compute_Spatio_Temporal_Curvature( float cutoff_freq_1, float cutoff_freq_2 );
};

#endif // _MOTION_HISTORY_H_
