#ifndef _FILTERS_H_
#define _FILTERS_H_

///	<summary>Applies a low pass filter to a data array.</summary>
///	<param name="data_raw">Pointer to data array to be filtered.</param>
///	<param name="data_filtered">Pointer to array where the result will be stored.</param>
///	<param name="data_size">Size of the data array.</param>
///	<param name="cutoff_freq">Cutoff frequency (Hz).</param>
///	<param name="sample_rate">Sample rate (Hz).</param>
template <typename T>
void apply_lowpass_filter_array( T * data_raw, T * data_filtered, int data_size, T cutoff_freq, T sample_rate = static_cast<T>( 1000 ) );

///	<summary>Applies a low pass filter to a single value.</summary>
///	<param name="data_raw">Value to be filtered.</param>
///	<param name="previous_value">Previous value.</param>
///	<param name="cutoff_freq">Cutoff frequency (Hz).</param>
///	<param name="sample_rate">Sample rate (Hz).</param>
template <typename T>
T apply_lowpass_filter_single_value( T data_raw, T previous_value, T cutoff_freq, T sample_rate = static_cast<T>( 1000 ) );

#endif // _FILTERS_H_
