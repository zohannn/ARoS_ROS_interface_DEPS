#ifndef _ROBOT_BASE_H_
#define _ROBOT_BASE_H_

#pragma once

class RobotBase
{
public:
	float
		/// <summary>X coordinate.</summary>
		dX,
		/// <summary>Y coordinate.</summary>
		dY,
		/// <summary>Z coordinate.</summary>
		dZ,
		/// <summary>Rotation Phi in degrees.</summary>
		dPhiDeg;
		
	/// <summary>Robot type.</summary>
	int nType;

public:
	/// <summary>Constructor.</summary>
	RobotBase( void );

	/// <summary>Copy constructor.</summary>
	/// <param name="obj">Robot to be copied.</param>
	RobotBase( const RobotBase &obj );
	
	/// <summary>Comparison operator.</summary>
	/// <param name="r">Robot to be compared.</param>
	/// <returns>true if the robots are equal, false otherwise.</returns>
	bool operator==( RobotBase r );
};

#endif // _ROBOT_BASE_H_
