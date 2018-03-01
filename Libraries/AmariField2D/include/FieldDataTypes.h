#ifndef _UTILITY_FUNCTIONS_H_
#define _UTILITY_FUNCTIONS_H_

#include <math.h>

#ifndef USE_CMATRIX
#define USE_CMATRIX 1 // define as '0' if you don't want to use CMatrix<Type>
#endif

template<typename T> class CRange
{
public:
	//-------------------------------------------------------
	// Constructors
	CRange( T val_min = 0, T val_max = 0 )
		: min( val_min )
		, max (val_max )
		{};

	CRange( const CRange<T> &obj )
		: min( obj.min )
		, max( obj.max )
		{};

	virtual ~CRange( void ) {};

	//-------------------------------------------------------
	// Data variables
public:
	T
		min,
		max;

	//-------------------------------------------------------
	// Operators
public:
	CRange<T> operator=( CRange<T> op )
	{
		min=op.min;
		max=op.max;

		return *this;
	};

};

template<typename T> class CPoint
{
public:
	//-------------------------------------------------------
	// Constructors
	CPoint( T val_x = 0, T val_y = 0 )
		: x( val_x )
		, y( val_y ) 
		{};

	CPoint( const CPoint<T> &obj )
		: x( obj.x )
		, y( obj.y )
		{};

	virtual ~CPoint( void ) {};

	//-------------------------------------------------------
	// Data variables
public:
	T
		x,
		y;

	//-------------------------------------------------------
	// Operators
public:
	CPoint<T> operator=( CPoint<T> op )
	{
		x=op.x;
		y=op.y;

		return *this;
	};
};

template<typename T> class CFieldInput
{
public:
	//-------------------------------------------------------
	// Constructors
	CFieldInput( void )
		: Center( 0, 0 )
		, Sigma( 0, 0 )
		, A( 0 )
		, K( 0 )
		{};

	CFieldInput( CPoint<T> center, CPoint<T> sigma, T a = 0, T k = 0 )
		: Center( center )
		, Sigma( sigma )
		, A( a )
		, K( k )
		{};

	CFieldInput( const CFieldInput<T> &obj )
		: Center( obj.Center )
		, Sigma( obj.Sigma )
		, A( obj.A )
		, K( obj.K ) 
		{};

	virtual ~CFieldInput( void ) {};

	//-------------------------------------------------------
	// Data variables
public:
	CPoint<T>
		Center,	// Center of peak coordinates
		Sigma;	// Sigma for X and Y variable

	T
		A,	// Peak amplitude (from minimum to maximum values)
		K;	// Shift up or down gaussian function depeding on sign

	//-------------------------------------------------------
	// Operators
public:
	void operator=( CFieldInput<T> op )
	{
		Center = op.Center;
		Sigma = op.Sigma;
		A = op.A;
		K = op.K;
	};
};

//--------------------------------------------------------------------------
/*
SIGMOI2D
function g = sigmoid2d(X,K)

Generates a gaussian like function :

		dResult = 1/(1+exp( -K*dVal ));

	dVal - matrix over which gaussian function is computed

	A - Peak amplitude 
*/
template <typename T>
inline T sigmoid2d_func( T tVal, T K )
{
	return 1/( 1 + exp( -K * tVal ) );
}

template <typename T>
inline void sigmoid2d( T * tVal, int size, T K, T * pResult )
{
	if( tVal == NULL || pResult == NULL ) return;

	for( int i=0 ; i<size ; i++ )
	{
		tResult[i] = sigmoid2d_func<T>( tVal[i], K );
	}
}

#if USE_CMATRIX
// Function Overload to work with CMatrix
template <typename T>
inline void sigmoid2d( CMatrix<T> * pSrc, T K, CMatrix<T> * pDst )
{
	int 
		w = pSrc->get_Width(),
		h = pSrc->get_Height();

	for( int i=0 ; i<h ; i++ )
	{
		for( int j=0 ; j<w ; j++ )
		{
			(*pDst)( i, j ) = sigmoid2d_func<T>( (*pSrc)( i, j ), K );
		}
	}
}

#endif // USE_CMATRIX

//--------------------------------------------------------------------------
/*
GAUSS2D
function g = gauss2d(x,y ,Xc,sigma,A,K)

Generates a gaussian like function :

		g = A*exp[-(( x(1)-xo(1)).^2 + x(2)-xo(2)).^2)./(2*sigma^2) ] + K

	x - values over which gaussian function is computed

	y - values over which gaussian function is computed

	Xc - center of peak coordinates.

	A - Peak amplitude (from minimum to maximum values).

	K - shift up or down gaussian function depeding on sign.
*/  

template <typename T>
inline T gauss2d_func( T val_x, T val_y, T * center, T sigma, T A, T K )
{
	return A * exp( - ( pow( val_x - center[0], 2 ) + pow( val_y - center[1], 2 ) ) / ( 2 * pow( sigma, 2 ) ) ) + K;
}

template <typename T>
inline void gauss2d( T * var_x, int size_x, T * var_y, int size_y, T * center, T sigma, T A, T K, T ** result )
{
	for( register int x=0 ; x<size_x ; x++ )
	{
		for( register int y=0 ; y<size_y ; y++ )
		{
			result[x][y] = gauss2d_func<T>( var_x[x], var_y[y], center, sigma, A, K );
		}
	}
}

template <typename T>
inline void gauss2d( T * var_x, int size_x, T * var_y, int size_y, CPoint<T> * center, T sigma, T A, T K, T ** result )
{
	T c[2] = { center->x, center->y };

	gauss2d<T>( var_x, size_x, var_y, size_y, c, sigma, A, K, result );
}

#if USE_CMATRIX
template <typename T>
inline void gauss2d( CMatrix<T> * mat_x, CMatrix<T> * mat_y, T * center, T sigma, T A, T K, CMatrix<T> * m_result )
{
	int
		size_x = mat_x->get_NumberElements(),
		size_y = mat_y->get_NumberElements();

	for( register int x=0 ; x<size_x ; x++ )
	{
		for( register int y=0 ; y<size_y ; y++ )
		{
			(*m_result)( x, y ) = gauss2d_func<T>( mat_x->get_Element( x, 0 ), mat_y->get_Element( y, 0 ), center, sigma, A, K );
		}
	}
}

template <typename T>
inline void gauss2d( CMatrix<T> * mat_x, CMatrix<T> * mat_y, CPoint<T> * center, T sigma, T A, T K, CMatrix<T> * m_result )
{
	T c[2] = { center->x, center->y };

	gauss2d<T>( mat_x, mat_y, c, sigma, A, K, m_result );
}

#endif // USE_CMATRIX

//--------------------------------------------------------------------------
/*
GAUSS2D2
function g = gauss2d2(x,y, C,sigmax, sigmay, A,K)

Generates a gaussian like function :

	g = A * exp[-(( x-C(1)).^2)./(2*sigmax^2) ] * exp[-(( y-C(2)).^2)./(2*sigmay^2) ] + K

x - values along x on which function is evaluated

y - values along y on which function is evaluated

C - center of peak coordinates.

A - Peak amplitude (from minimum to maximum values).

K - shift up or down gaussian function depeding on sign.
	  
*/

template <typename T>
inline T gauss2d2_func( T val_x, T val_y, T * C, T * sigma, T A, T K )
{
	return A * exp( -( pow( val_x - C[0], 2 ) )/( 2*pow( sigma[0], 2 ) ) ) * exp( -( pow( val_y - C[1], 2 ) )/( 2*pow( sigma[1], 2 ) ) ) + K;
}

template <typename T>
inline void gauss2d2( T * var_x, int size_x, T * var_y, int size_y, T * C, T * sigma, T A, T K, T ** pResult )
{
	for( register int x=0 ; x<size_x ; x++ )
	{
		for( register int y=0 ; y<size_y ; y++ )
		{
			pResult[x][y] = gauss2d2_func<T>( var_x[x], var_y[y], C, sigma, A, K );
		}
	}
}

template <typename T>
inline void gauss2d2( T * var_x, int size_x, T * var_y, int size_y, CPoint<T> * C, T * sigma, T A, T K, T ** pResult )
{
	T c[2] = { C->x, C->y };

	gauss2d2<T>( var_x, size_x, var_y, size_y, c, sigma, A, K, pResult );
}

#if USE_CMATRIX
template <typename T>
inline void gauss2d2( CMatrix<T> * mat_x, CMatrix<T> * mat_y, T * C, T * sigma, T A, T K, CMatrix<T> * mat_res )
{
	int 
		size_x = mat_x->get_NumberElements(),
		size_y = mat_x->get_NumberElements();

	for( register int x=0 ; x<size_x ; x++ )
	{
		for( register int y=0 ; y<size_y ; y++ )
		{
			(*mat_res)( x, y ) = gauss2d2_func<T>( mat_x->get_Element( x, 0 ), mat_y->get_Element( y, 0 ), C, sigma, A, K );
		}
	}
}

template <typename T>
inline void gauss2d2( CMatrix<T> * mat_x, CMatrix<T> * mat_y, CPoint<T> * C, CPoint<T> * sigma, T A, T K, CMatrix<T> * mat_res )
{
	T 
		c[2] = { C->x, C->y },
		s[2] = { sigma->x, sigma->y };

	gauss2d2<T>( mat_x, mat_y, c, s, A, K, mat_res );
}

#endif // USE_CMATRIX

#endif //_UTILITY_FUNCTIONS_H_
