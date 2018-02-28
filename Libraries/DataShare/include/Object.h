#ifndef _OBJECT_H_
#define _OBJECT_H_

#pragma once
#include <array>

class Pose
{
public:
	std::array<float, 3> position;
	std::array<float, 3> orientation;

	Pose()
	{
		for( size_t i=0; i<position.size();i++ )
			position[i] = 0.0f;
		for( size_t i=0; i<orientation.size();i++ )
			orientation[i] = 0.0f;

		/*for (auto &p : position)
		 p = 0.0f;
		for (auto &p : orientation)
		 p = 0.0f;*/
	}

	Pose(std::array<float, 3> pos, std::array<float, 3> ori)
	{
		position = pos;
		orientation = ori;
	}
};

class Object
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
		dPhiDeg,
		/// <summary>Rotation Psi in degrees.</summary>
		dPsiDeg,
		/// <summary>Rotation Theta in degrees.</summary>
		dTetaDeg,
		/// <summary>Rotation Alpha in degrees.</summary>
		dAlfaDeg;

	/// <summary> Object size, x,y,z.</summary>
	float sZsize[3];

	//<summary> Object default pose. This will serve as a placeholder to store information of where e.g. the robot should place the object </summary>
	Pose default_pose;
	
	int 
		/// <summary> Flag that signals if and how the object is grasped
		/// <para>0 - is not grip.</para>
		/// <para>1 - grip by right hand.</para>
		///	<para>2 - grip by left hand.</para>
		/// </summary>
		isGrip,

		/// <summary> Flag that signals if the object is a target
		/// <para>0 - is not target.</para>
		/// <para>1 - target for right hand.</para>
		/// <para>2 - target for left hand.</para>
		/// <para>3 - target for the two hands.</para>
		/// </summary>
		isTarget,

		/// <summary> Flag that signals if the object is inserted
		/// <para>0 - is not inserted.</para>
		/// <para>1 - inserted in Robot Wksp.</para>
		/// <para>2 - inserted in Human Wksp.</para>
		/// </summary>
		isInserted,
		
		/// <summary>Object type.</summary>
		nType,
		
		/// <summary>Task ID.</summary>
		nTask;

	/// <summary> Flag that signals if the object is in the humans' hand.</summary>
	bool isInHumanHand;

	int aColor[3];

	int *pnListObject;

public:
	/// <summary>Constructor.</summary>
	Object( void );

	/// <summary>Copy constructor.</summary>
	/// <param name="obj">Object to be copied.</param>
	Object( const Object &obj );

	/// <summary>Comparison operator.</summary>
	/// <param name="o">Object to be compared.</param>
	/// <returns>true if the objects are equal, false otherwise.</returns>
	bool operator==( Object o );

	/// <summary>Compare object types.</summary>
	/// <param name="o1">Object 1.</param>
	/// <param name="o2">Object 2.</param>
	/// <returns>true if the types are equal, false otherwise.</returns>
	bool EqualType( Object o1, Object o2 );

	/// <summary>FindByType.</summary>
	/// <param name="o1">Object 1.</param>
	/// <param name="o2">Object 2.</param>
	/// <returns></returns>
	static bool FindByType( Object o1, Object o2 );

	/// <summary>FindByColor.</summary>
	/// <param name="o1">Object 1.</param>
	/// <param name="o2">Object 2.</param>
	/// <returns></returns>
	static bool FindByColor( Object o1, Object o2 );

	/// <summary>Compare object types.</summary>
	/// <param name="o">Object to be compared with 'this'.</param>
	/// <returns>true if the types are equal, false otherwise.</returns>
	bool GetByType( Object o );

	/// <summary>Compare object colors.</summary>
	/// <param name="o1">Object 1.</param>
	/// <param name="o2">Object 2.</param>
	/// <returns>true if the colors are equal, false otherwise.</returns>
	bool EqualColor( Object o1, Object o2 );

	/// <summary>Compare object color with given color.</summary>
	/// <param name="nColor">Color to compare.</param>
	/// <returns>true if the colors are equal, false otherwise.</returns>
	bool GetByColor( int nColor );
};

#endif //_OBJECT_H_
