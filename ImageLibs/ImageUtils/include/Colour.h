#ifndef _COLOUR_H_
#define _COLOUR_H_

#include <opencv2/core/core.hpp>

#include "ColourDefs.h"

class CColour
{
public:
	/// Avoid a c++ pitfall. Constructors with one argument serve double duty as type conversions. 
	/// Avoid constructors with a single integer argument! Use the explicit keyword if you can't avoid them.
	CColour( int colour = COLOUR_K );
	CColour( const CColour &col );

	//-----------------------------------------------------------------------
	// Data members
	std::string sName;
	
	int 
		r, 
		g, 
		b, 
		nColour_ID;

	//-----------------------------------------------------------------------
	// Functions
	void Copy_From( const CColour colour );

	void set_Colour( int colour );

	//-----------------------------------------------------------------------
	// Operators
	void operator=( int colour );

	void operator=( const CColour colour );

	void operator=( cv::Scalar colour );

	cv::Scalar get_cvScalar( void );
};

#endif // _COLOUR_H_
