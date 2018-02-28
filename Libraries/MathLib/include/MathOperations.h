#ifndef _MATH_OPERATIONS_H_
#define _MATH_OPERATIONS_H_

//---------------------------------------------------------------------------

/**
* Convert an angle from degrees to radians
* @param value is the angle to convert
* @return converted angle
*/
template<typename T>
inline T Deg2Rad( T value );

/**
* Convert an angle from radians to degrees
* @param value is the angle to convert
* @return converted angle
*/
template<typename T>
inline T Rad2Deg( T value );

//---------------------------------------------------------------------------

/**
* Convert a value in a periodic value
* @param value is the linear measure
* @param limit is the max value in the circular measure
* @return converted value in a periodic value
*/
template<typename T>
inline T modAny( T value, T limit, bool unilateral = true );

/**
* Convert a linear value in a circular value between 0 to 2*pi
* @param value is the linear measure
* @return converted value from linear to circular
*/
template<typename T>
inline T mod2pi( T value ); 

/**
* Convert a linear value in a circular value between -pi to pi
* @param value is the linear measure
* @return converted value from linear to circular
*/
template<typename T>
inline T CircularAngleInPi(T value);

//---------------------------------------------------------------------------

/**
* Random number generator.
* Before use this function, you must have to use the following code line: 'srand( ( unsigned ) time( NULL ) );'
* @param min is the lower limit
* @param max is the upper limit
* @return random number in [min max]
*/
template<typename T>
inline T randNum( T min, T max );

//---------------------------------------------------------------------------

template <typename T>
inline void FitValueToLimits( T * value, T limit_1, T limit_2 );

//---------------------------------------------------------------------------

// Calculates Norm 2 - sqrt( v1^2 + v2^2 + v3^2 )
// The last param can be ignored if the calculation is only with 2 params
template <typename T>
inline T norm2( T v1, T v2, T v3=static_cast<T>( 0 ) );

//---------------------------------------------------------------------------

// Round a floating point number to an nearest integer number
// If decimal part is less than 0.5 the function returns the integer part,
// otherwise the function returns the integer part plus one 
// example: Round( 123.499 ) => function returns 123
//          Round( 123.500 ) => function returns 124
//          Round( 123.501 ) => function returns 124
template <typename T>
inline int round( T value );

template <typename T>
inline unsigned int u_round( T value );

//---------------------------------------------------------------------------

// Get the sign of a number
// If the value is positive the function returns the value +1,
// if the value is negative the function returns the value -1,
// if the value is zero the function returns the value 0;
template <typename T>
inline int sign( T value );

//---------------------------------------------------------------------------

//  Linear mapping from [min,max] to [outMin, outMax]
//           /--------  outMax
//          /
// --------/            outMin
//      min   max
template <typename T>
inline T LinearMap( T x, T min = -10, T max = 10, T outMin = -1, T outMax = 1 );

//---------------------------------------------------------------------------

// Inversed Linear mapping from [min,max] to [outMin, outMax]
// --------\             outMax
//          \
//           \--------   outMin
//      min   max
template <typename T>
inline T invLinearMap( T x, T min = -10, T max = 10, T outMin = -1, T outMax = 1 );

//---------------------------------------------------------------------------
// fmod_t returns the remainder of division of _X by _Y
int fmod_t( int _X, int _Y );
float fmod_t( float _X, float _Y );
double fmod_t( double _X, double _Y );

// Rounds input number 'val' to the nearest number whose division remainder is 'rmdr'
template <typename T>
inline T round_rem( T val, T rmdr );

#endif //_MATH_OPERATIONS_H_
