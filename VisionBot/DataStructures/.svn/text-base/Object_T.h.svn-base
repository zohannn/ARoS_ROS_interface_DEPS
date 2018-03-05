#ifndef _OBJECT_MAIN_H_
#define _OBJECT_MAIN_H_

#pragma once

#include <opencv2/imgproc/imgproc.hpp>
#include <string>
#include <vector>

#include "Colour.h"
#include "common.h"
#include "ObjectDefinitions.h"
#include "Preprocessor_Macros.h"
#include "RectEx.hpp"
#include "vector_operations.hpp"
#include "VisionDefinitions.h"

#define isValid_ObjectType( type ) ( ( type>=0 ) && ( type<NUMBER_OF_OBJECT_TYPES ) )
#define GET_VALID_ObjectType( type ) isValid_ObjectType( type ) ? type : NUMBER_OF_OBJECT_TYPES

template <class T>
class CObject_
{
//---------------------------------------------------------------------
// Variables to store object properties
public:
	bool 
		/// <summary>Flag to signal if the object is inserted.</summary>	
		bInserted,
		/// <summary>Flag to signal if it's a valid object.</summary>	
		bValid,
		/// <summary>Flag to signal if the object should be visible to the vision system for debug purposes.</summary>	
		bVisible;
	
	/// <summary>Object identification code, attributed during runtime to differentiate objects of the same type.</summary>	
	int nObjectID;

	unsigned int 
		/// <summary>Minimum area required for the object.</summary>	
		uMinArea,
		/// <summary>Maximum area required for the object.</summary>	
		uMaxArea;

	cv::RectEx
		/// <summary>Entire region occupied by the object.</summary>	
		Region,
		/// <summary>Area of interest of the object.</summary>	
		RoI;

	CColour
		/// <summary>Object primary colour.</summary>	
		Colour_Primary,
		/// <summary>Object secondary colour.</summary>	
		Colour_Secondary;
	
	std::vector < CObject_<T> >
		/// <summary>Vector containing the objects that are near this one.</summary>	
		vNearObjects,
		/// <summary>Vector containing the objects that are part of this one. This is used in the Base, because of the holes.</summary>	
		vSubObjects,
		/// <summary>Vector containing the objects where this is inserted.</summary>	
		vInsertedAt;

	std::vector< cv::RectEx > 
		/// <summary>Vector of spots of the primary colour that define the object.</summary>	
		vSpot_Primary,
		/// <summary>Vector of spots of the secondary colour that define the object.</summary>	
		vSpot_Secondary;

	/// <summary>Binary mask of the object.</summary>	
	cv::Mat imgMask;

	/// <summary>String for debug purposes.</summary>	
	std::string strDebug;

	/// <summary>In case the object is a hand and it's making a gesture, this variable stores what gesture it is.</summary>	
	unsigned int uGestureType;

protected:
	cv::Point3_<T>
		/// <summary>(x,y,z) coordinates in the cameras reference frame.</summary>	
		Coord_Vision,
		/// <summary>(x,y,z) coordinates in the world reference frame.</summary>	
		Coord_World,
		/// <summary>(x,y,z) orientation in the world reference frame.</summary>	
		Orientation;
		
	/// <summary>Pan and Tilt angles necessary for to ARoS look at it.</summary>	
	cv::Point_<T> PanTilt;
	
protected:
	/// <summary>Object type code.</summary>	
	unsigned int uType;

	/// <summary>String with the object name.</summary>	
	std::string sName;

	/// <summary>Flag to signal if the object has 3D coordinates.</summary>	
	bool bHas3D;

public:
	//--------------------------------------------------------------------------
	// Construction and destruction
	CObject_( void )
	: bInserted( false ), bValid( true ), bHas3D( false ), bVisible( true )
	, uMinArea( 0 ), uMaxArea( 0 )
	, Region( 0, 0, 0, 0 ), RoI( 0, 0, 0, 0 )
	, nObjectID( OBJECT_NULL )
	, uGestureType( 0 )
	, strDebug( "" )
	{
		InitCoordZero();
		set_Type( NUMBER_OF_OBJECT_TYPES );
	};

	CObject_( unsigned int type )
	: bInserted( false ), bValid( true ), bHas3D( false ), bVisible( true )
	, Region( 0, 0, 0, 0 ), RoI( 0, 0, 0, 0 )
	, uMinArea( 0 ), uMaxArea( 0 )
	, nObjectID( OBJECT_NULL )
	, uGestureType( 0 )
	, strDebug( "" )
	{
		InitCoordZero();
		set_Type( type );
	};

	CObject_( unsigned int type, int x, int y, int width, int height )
	: bInserted( false ), bValid( true ), bHas3D( false ), bVisible( true )
	, Region( x, y, width, height ), RoI( x, y, width, height )
	, uMinArea( 0 ), uMaxArea( 0 )
	, nObjectID( OBJECT_NULL )
	, uGestureType( 0 )
	, strDebug( "" )
	{
		InitCoordZero();
		set_Type( type );
	};

	CObject_( unsigned int type, cv::Rect region )
	: bInserted( false ), bValid( true ), bHas3D( false ), bVisible( true )
	, Region( region ), RoI( region )
	, uMinArea( 0 ), uMaxArea( 0 )
	, nObjectID( OBJECT_NULL )
	, uGestureType( 0 )
	, strDebug( "" )
	{
		InitCoordZero();
		set_Type( type );
	};

	// Copy constructor: A must have when working with stl vectors
	CObject_( const CObject_& obj )
	{ 
		Copy( &obj, this ); 
	};

	~CObject_( void )
	{
		ClearObject();
	}

	// setup mask image
	void SetupMaskImage( cv::Mat mask )
	{
		CleanMaskImage();

		imgMask.create( mask.rows, mask.cols, mask.type() );
		mask.copyTo( imgMask );
	}

	void CleanMaskImage( void )
	{
		if( !imgMask.empty() )
			imgMask.release();
	}

	//--------------------------------------------------------------------------
	// Accessors
public:
	static std::string get_ObjectName( unsigned int type )
	{
		return GET_VALID_ObjectName( type );
	};

	void set_Region( int x, int y, int width, int height )
	{
		Region.copy_from( x, y, width, height );
		RoI.copy_from( x, y, width, height );
	};

	void set_Region( cv::RectEx region )
	{
		Region = region; RoI = region;
	};

	void set_Coordinates_Vision( T tX, T tY, T tZ )
	{
		Coord_Vision.x = tX;
		Coord_Vision.y = tY;
		Coord_Vision.z = tZ;
	};
	
	void set_Coordinates_Vision( cv::Point3_<T> pCoord )
	{
		set_Coordinates_Vision( pCoord.x, pCoord.y, pCoord.z );
	};

	void set_Coordinates_World( T tX, T tY, T tZ )
	{
		Coord_World.x = tX;
		Coord_World.y = tY;
		Coord_World.z = tZ;

		T zero = static_cast<T>( 0 );

		bHas3D = ( tX != zero ) && ( tY != zero ) && ( tZ != zero );
	};

	void set_Coordinates_World( cv::Point3_<T> pCoord )
	{
		set_Coordinates_World( pCoord.x, pCoord.y, pCoord.z );
	};

	void get_Coordinates_World( T * pCoord )
	{
		pCoord[0] = Coord_World.x;
		pCoord[1] = Coord_World.y;
		pCoord[2] = Coord_World.z;
	};

	void get_Coordinates_World( cv::Point3_<T> * pCoord )
	{
		*pCoord = Coord_World;
	};

	cv::Point3_<T> get_Coordinates_World( void )
	{
		// Copy contents to an auxiliary object
		cv::Point3_<T> pt = Coord_World;
		return pt;
	};

	void set_Orientation( T tXAngle, T tYAngle, T tZAngle )
	{
		Orientation.x = tXAngle;
		Orientation.y = tYAngle;
		Orientation.z = tZAngle;
	};

	void set_Orientation( cv::Point3_<T> pOri )
	{
		Orientation = pOri;
	};
	
	void get_Orientation( T * pXAngle, T * pYAngle, T * pZAngle )
	{
		*pXAngle = Orientation.x;
		*pYAngle = Orientation.y;
		*pZAngle = Orientation.z;
	};

	void get_Orientation( cv::Point3_<T> * pOri )
	{
		*pOri = Orientation;
	};

	void set_PanTiltAngles( cv::Point_<T> pPanTilt )
	{
		PanTilt = pPanTilt;
	};
	
	void set_PanTiltAngles( T tPan, T tTilt )
	{
		PanTilt.x = tPan; PanTilt.y = tTilt;
	};

	void get_PanTiltAngles( cv::Point_<T> *pPanTilt )
	{
		*pPanTilt = PanTilt;
	};
	
	void get_PanTiltAngles( T * pPan, T * pTilt )
	{
		*pPan = PanTilt.x; *pTilt = PanTilt.y;
	};

	// Distance in straight line on the ground
	T get_DistanceFromRobot( void )
	{
		return sqrt( pow( Coord_World.x, 2 ) + pow( Coord_World.y, 2 ) );
	}

	bool has3D( void )
	{
		return bHas3D;
	};

	std::string get_Name( void )
	{
		return sName;
	};
	
	unsigned int get_Type( void )
	{
		return uType;
	};

	void set_Type( unsigned int type )
	{
		uType = GET_VALID_ObjectType( type );

		// Set colours based on type
		Attrib_Colour( uType );

		// Set name based on type
		Attrib_Name( uType );

		// Define minimum area
		switch( type )
		{
		case OBJECT_BASE:
		case OBJECT_TOP_FLOOR:
			uMinArea = 5000u;
			break;
		}
	};

	void ClearObject( void )
	{
		bHas3D = false;
		bValid = false;
		bVisible = true;
		Region.copy_from( 0, 0, 0, 0 );
		RoI.copy_from( 0, 0, 0, 0 );
		uType = NUMBER_OF_OBJECT_TYPES;
		nObjectID = OBJECT_NULL;
		uGestureType = 0;
		sName.clear();
		strDebug.clear();
		InitCoordZero();

		vSpot_Primary.clear();
		vSpot_Secondary.clear();
		
		vNearObjects.clear();
		vSubObjects.clear();
		vInsertedAt.clear();

		CleanMaskImage();
	};

	//--------------------------------------------------------------------------
	// Setup functions to quickly set attributes depending on object type
private:
	void Attrib_Name( int type )
	{
		sName = GET_VALID_ObjectName( type );
	};

	void Attrib_Colour( int type )
	{
		Colour_Primary = GET_VALID_PrimaryColour( type );
		Colour_Secondary = GET_VALID_SecondaryColour( type );
	};

	//--------------------------------------------------------------------------
	// Initialization functions
protected:
	void InitCoordZero( void )
	{
		T tZero = static_cast<T>( 0 );

		Coord_Vision.x = tZero;
		Coord_Vision.y = tZero;
		Coord_Vision.z = tZero;

		Coord_World.x = tZero;
		Coord_World.y = tZero;
		Coord_World.z = tZero;

		Orientation.x = tZero;
		Orientation.y = tZero;
		Orientation.z = tZero;

		PanTilt.x = tZero;
		PanTilt.y = tZero;
	};

	virtual void set_Properties( void ){};

	//--------------------------------------------------------------------------
	// Operators
public:
	bool operator>( CObject_ &obj )
	{
		return ( Region > obj.Region );
	};

	bool const operator<( CObject_ &obj )
	{
		return ( Region < obj.Region );
	};

	bool operator>=( CObject_ &obj )
	{
		return ( Region >= obj.Region );
	};

	bool operator<=( CObject_ &obj )
	{
		return ( Region <= obj.Region );
	};

	bool operator==( unsigned int type )
	{
		return ( uType==type );
	};

	bool operator==( CObject_ &obj )
	{
		return ( uType == obj.uType );
	};

	//--------------------------------------------------------------------------
public:
	void Copy_From( const CObject_ * obj )
	{
		Copy( obj, this );
	};

protected:
	// Copy the contents of one object to another
	void Copy( const CObject_ * src, CObject_ * dst )
	{
		dst->uType = src->uType;
		dst->bInserted = src->bInserted;
		dst->bValid = src->bValid;
		dst->bVisible = src->bVisible;
		dst->sName = src->sName;
		dst->Coord_Vision = src->Coord_Vision;
		dst->Coord_World = src->Coord_World;
		dst->Orientation = src->Orientation;
		dst->Region = src->Region;
		dst->RoI = src->RoI;
		dst->Colour_Primary = src->Colour_Primary;
		dst->Colour_Secondary = src->Colour_Secondary;
		dst->bHas3D = src->bHas3D;
		dst->PanTilt = src->PanTilt;
		dst->nObjectID = src->nObjectID;
		dst->strDebug = src->strDebug;
		dst->uMinArea = src->uMinArea;
		dst->uMaxArea = src->uMaxArea;
		dst->uGestureType = src->uGestureType;

		//-------------------------------------------------------------------
		//  Copy vNearObjects
		vector_copy( src->vNearObjects, dst->vNearObjects );

		//-------------------------------------------------------------------
		//  Copy vSubObjects
		vector_copy( src->vSubObjects, dst->vSubObjects );

		//-------------------------------------------------------------------
		//  Copy vInsertedAt
		vector_copy( src->vInsertedAt, dst->vInsertedAt );

		//-------------------------------------------------------------------
		//  Copy vSpot_Primary
		vector_copy( src->vSpot_Primary, dst->vSpot_Primary );

		//-------------------------------------------------------------------
		//  Copy vSpot_Secondary
		vector_copy( src->vSpot_Secondary, dst->vSpot_Secondary );
		
		//-------------------------------------------------------------------
		//  Copy Mask image
		if( !src->imgMask.empty() )
		{
			dst->SetupMaskImage( src->imgMask );
		}
	};

};

typedef CObject_<int> CObject_i;
typedef CObject_<float> CObject_f;
typedef CObject_<double> CObject_d;

#endif // _OBJECT_MAIN_H_
