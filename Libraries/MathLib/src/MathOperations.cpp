#include "MathOperations.h"

//---------------------------------------------------------------------------

#define _USE_MATH_DEFINES

#include <cmath>
#include <stdlib.h>

//---------------------------------------------------------------------------

template<typename T>
T Deg2Rad<T>( T value )
{
	return value * static_cast<T>( M_PI/180.0 );
}
template float Deg2Rad<float>(float value);
template double Deg2Rad<double>( double value );

template<typename T>
T Rad2Deg<T>( T value )
{
	return value * static_cast<T>( 180.0/M_PI );
}
template float Rad2Deg<float>( float value );
template double Rad2Deg<double>( double value );

//---------------------------------------------------------------------------

template<typename T>
T modAny<T>( T value, T limit, bool unilateral )
{
	T zero = static_cast<T>( 0 );
	T aux = zero;
	T out = modf( value/limit, &aux ) * limit;

	if( unilateral && ( out < zero ) )
		out += limit;

	return out;
};
template float modAny<float>( float value, float upperLimit, bool unilateral );
template double modAny<double>( double value, double upperLimit, bool unilateral );

template<typename T>
T mod2pi<T>( T value )
{
	return modAny<T>( value, static_cast<T>(2.0*M_PI) );
};
template float mod2pi<float>( float value );
template double mod2pi<double>( double value );

template<typename T>
T CircularAngleInPi(T value)
{
	T tmp = mod2pi(value);
	if (tmp > static_cast<T>(M_PI))
		tmp -= static_cast<T>(2.0*M_PI);

	return tmp;
};
template float CircularAngleInPi<float>(float value);
template double CircularAngleInPi<double>(double value);

//---------------------------------------------------------------------------

template<typename T>
T randNum<T>( T min, T max )
{
	return ( ( max - min ) * ( rand() / static_cast<T>( RAND_MAX ) ) + min );
}
template int randNum<int>( int min, int max );
template float randNum<float>( float min, float max );
template double randNum<double>( double min, double max );

//---------------------------------------------------------------------------

template<typename T>
void FitValueToLimits( T * value, T limit_1, T limit_2 )
{
	T
		limit_low = limit_1,
		limit_high = limit_2;

	if( limit_1 > limit_2 )
	{
		limit_low = limit_2;
		limit_high = limit_1;
	}

	if( *value < limit_low ) *value = limit_low;
	if( *value > limit_high ) *value = limit_high;
}

template void FitValueToLimits<unsigned int>( unsigned int * value, unsigned int limit_1, unsigned int limit_2 );
template void FitValueToLimits<int>( int * value, int limit_1, int limit_2 );
template void FitValueToLimits<long>( long * value, long limit_1, long limit_2 );
template void FitValueToLimits<float>( float * value, float limit_1, float limit_2 );
template void FitValueToLimits<double>( double * value, double limit_1, double limit_2 );
template void FitValueToLimits<size_t>(size_t * value, size_t limit_1, size_t limit_2);

//---------------------------------------------------------------------------

template <typename T>
T norm2( T v1, T v2, T v3 )
{
	T power = static_cast<T>( 2 );
	return sqrt( pow( v1, power ) + pow( v2, power ) + pow( v3, power ) );
}

template float norm2<float>( float v1, float v2, float v3 );
template double norm2<double>( double v1, double v2, double v3 );
template long double norm2<long double>( long double v1, long double v2, long double v3 );

//---------------------------------------------------------------------------

template <typename T>
int round( T value )
{
	T offset = ( value > static_cast<T>( 0.0f ) ) ? static_cast<T>( 0.5f ) : static_cast<T>( -0.5f );

	return static_cast<int>( value + offset );
}

template int round<float>( float value );
template int round<double>( double value );

//---------------------------------------------------------------------------

template <typename T>
unsigned int u_round( T value )
{
	return static_cast<unsigned int>( round<T>( value ) );
}

template unsigned int u_round<float>( float value );
template unsigned int u_round<double>( double value );

//---------------------------------------------------------------------------

template <typename T>
int sign( T value )
{
	if( value < static_cast<T>( 0.0 ) )
	{
		return -1;
	}
	else if( value == static_cast<T>( 0.0 ) )
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
template int sign<int>( int value );
template int sign<long>( long value );
template int sign<long long>( long long value );
template int sign<float>( float value );
template int sign<double>( double value );
template int sign<long double>( long double value );

//---------------------------------------------------------------------------

template <typename T>
T LinearMap( T x, T min, T max, T outMin, T outMax ) 
{
    T m = ( outMax-outMin )/( max-min );
    T q = outMin - m*min;
    T ret = m*x+q;
    if (ret < outMin) return outMin;
    if (ret > outMax) return outMax;
    return ret;
}

template float LinearMap<float>( float x, float min, float max, float outMin, float outMax );
template double LinearMap<double>( double x, double min, double max, double outMin, double outMax );

//---------------------------------------------------------------------------

template <typename T>
T invLinearMap( T x, T min, T max, T outMin, T outMax ) 
{
    T m = - ( outMax-outMin )/( max-min );
    T q = outMax - m*min;
    T ret = m*x+q;
    if (ret < outMin) return outMin;
    if (ret > outMax) return outMax;
    return ret;
}

template float invLinearMap<float>( float x, float min, float max, float outMin, float outMax );
template double invLinearMap<double>( double x, double min, double max, double outMin, double outMax );

//---------------------------------------------------------------------------

int fmod_t( int _X, int _Y )
{
	return ( _X % _Y );
}

float fmod_t( float _X, float _Y )
{
	return fmodf( _X, _Y );
}

double fmod_t( double _X, double _Y )
{
	return fmod( _X, _Y );
}

template <typename T>
T round_rem( T val, T rmdr )
{
	const T 
		_one = static_cast<T>( -1.0 ),
		one = static_cast<T>( 1.0 ),
		two = static_cast<T>( 2.0 );

	T 
		rm = fmod_t( val, rmdr ),
		ret = val - rm,
		rmdr2 = val / two,
		mult = ( val<0 ) ? _one : one;

	if( fabs( (double)rm ) >= rmdr2 )
		ret += mult*rmdr;

	return ret;
}

template int round_rem<int>( int val, int rmdr );
template float round_rem<float>( float val, float rmdr );
template double round_rem<double>( double val, double rmdr );
