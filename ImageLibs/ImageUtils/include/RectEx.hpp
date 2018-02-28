#ifndef _RECT_EX_H_
#define _RECT_EX_H_

#include "RectOperations.hpp"

namespace cv
{
	//-----------------------------------------------------------------------
	// A reimplementation of cv::Rect to use comparison operators and other extending features
	template <class T>
	class RectEx_ : public Rect_<T>
	{
	//-----------------------------------------------------------------------
	// Constructors
	public:
		RectEx_( void ) 
			: Rect_<T>()
			{};

		RectEx_( T _x, T _y, T _width, T _height )
			: Rect_<T>( _x, _y, _width, _height )
			{};

		RectEx_( const RectEx_<T> &copy )
			: Rect_<T>( copy )
			{};

		RectEx_( const Rect_<T> &copy )
			: Rect_<T>( copy )
			{};

		RectEx_( const CvRect &copy )
			: Rect_<T>( copy )
			{};

		RectEx_( const Point_<T>& org, const Size_<T>& sz )
			: Rect_<T>( org, sz )
			{};

		void copy_from( T _x, T _y, T _width, T _height )
		{
			x = _x;
			y = _y;
			width = _width;
			height = _height;
		}

		virtual ~RectEx_( void ){};

		//-------------------------------------------------------------------
		// Operators

		void operator=( const Rect_<T> &op2 )
		{
			x = op2.x;
			y = op2.y;
			width = op2.width;
			height = op2.height;
		};

		void operator=( const RectEx_<T> &op2 )
		{
			x = op2.x;
			y = op2.y;
			width = op2.width;
			height = op2.height;
		};

		bool operator>( Rect_<T> &op2 )
		{
			return ( area() > op2.area() );
		};

		bool operator>( RectEx_<T> &op2 )
		{
			return ( area() > op2.area() );
		};

		bool operator<( Rect_<T> &op2 )
		{
			return ( area() < op2.area() );
		};

		bool operator<( RectEx_<T> &op2 )
		{
			return ( area() < op2.area() );
		};

		bool operator>=( Rect_<T> &op2 )
		{
			return ( area() >= op2.area() );
		};

		bool operator>=( RectEx_<T> &op2 )
		{
			return ( area() >= op2.area() );
		};

		bool operator<=( Rect_<T> &op2 )
		{
			return ( area() <= op2.area() );
		};

		bool operator<=( RectEx_<T> &op2 )
		{
			return ( area() <= op2.area() );
		};

		bool operator==( Rect_<T> &op2 )
		{
			return(
				x == op2.x &&
				y == op2.y &&
				width == op2.width &&
				height == op2.height
			);
		};

		bool operator==( RectEx_<T> &op2 )
		{
			return(
				x == op2.x &&
				y == op2.y &&
				width == op2.width &&
				height == op2.height 
			);
		};

		void apply_scale( float scale_x, float scale_y )
		{
			Rect temp( *this );
			scale_rectangle( scale_x, scale_y, &temp, this );
		};

		void apply_scale( float scale )
		{
			apply_scale( scale, scale );
		};

		T distance_to( RectEx_<T> rect )
		{
			// Provide the constant 2 in the correct type
			const T TWO = static_cast<T>( 2 );

			Point_<T>
				pt_c1( x + width/TWO, y + height/TWO ),
				pt_c2( rect.x + rect.width/TWO, rect.y + rect.height/TWO );
			
			float 
				fDiff_x = static_cast<float>( pt_c1.x - pt_c2.x ),
				fDiff_y = static_cast<float>( pt_c1.y - pt_c2.y );

			// Type conversion for float required to prevent error of ambiguous call to function pow
			float fResult = sqrt( pow( fDiff_x, 2 ) + pow( fDiff_y, 2 ) );

			return static_cast<T>( fResult );
		};
	};

	typedef RectEx_<int> RectEx;
	typedef RectEx_<int> RectEx_i;
	typedef RectEx_<float> RectEx_f;
	typedef RectEx_<double> RectEx_d;
}

#endif //_RECT_EX_H_
