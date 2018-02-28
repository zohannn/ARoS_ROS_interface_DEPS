#include "Filters.h"
#include "MathDefines.h"

template <typename T>
void apply_lowpass_filter_array( T * data_raw, T * data_filtered, int data_size, T cutoff_freq, T sample_rate )
{
	const T 
		One = static_cast<T>( 1 ),
		TwoPi = static_cast<T>( M_2PI_F ),
		RC = One / ( cutoff_freq * TwoPi ),
		dt = One / sample_rate,
		alpha =  dt/( RC + dt );

	data_filtered[0] = data_raw[0];

	for( int i=1 ; i<data_size ; i++ )
	{
		data_filtered[i] = data_filtered[i-1] + alpha * ( data_raw[i] - data_filtered[i-1] );
	}
}

template void apply_lowpass_filter_array( float * data_raw, float * data_filtered, int data_size, float cutoff_freq, float sample_rate );
template void apply_lowpass_filter_array( double * data_raw, double * data_filtered, int data_size, double cutoff_freq, double sample_rate );

template <typename T>
T apply_lowpass_filter_single_value( T data_raw, T previous_value, T cutoff_freq, T sample_rate )
{
	const T 
		One = static_cast<T>( 1 ),
		TwoPi = static_cast<T>( M_2PI_F ),
		RC = One / ( cutoff_freq * TwoPi ),
		dt = One / sample_rate,
		alpha =  dt/( RC + dt );

	return ( previous_value + alpha * ( data_raw - previous_value ) );
}

template float apply_lowpass_filter_single_value( float data_raw, float previous_value, float cutoff_freq, float sample_rate );
template double apply_lowpass_filter_single_value( double data_raw, double previous_value, double cutoff_freq, double sample_rate );
