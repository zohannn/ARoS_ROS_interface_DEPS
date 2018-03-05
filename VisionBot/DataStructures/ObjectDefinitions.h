#ifndef _OBJECT_DEFINITIONS_H_
#define _OBJECT_DEFINITIONS_H_

#pragma once

#include <string>

#include "ColourDefs.h"
#include "Preprocessor_Macros.h"

#ifndef NUMBER_OF_OBJECT_NAMES
#define NUMBER_OF_OBJECT_NAMES SIZE_OF_ARRAY( Name_Of_Object )
#endif

#ifndef NUMBER_OF_PRIMARY_COLOURS
#define NUMBER_OF_PRIMARY_COLOURS SIZE_OF_ARRAY( Colour_Of_Object_Primary )
#endif

#ifndef NUMBER_OF_SECONDARY_COLOURS
#define NUMBER_OF_SECONDARY_COLOURS SIZE_OF_ARRAY( Colour_Of_Object_Secondary )
#endif

#ifndef isValid_ObjectName
#define isValid_ObjectName( type ) isIndexInRange( Name_Of_Object, type )
#endif

#ifndef GET_VALID_ObjectName
#define GET_VALID_ObjectName( type ) GET_VALID_ArrayItem( Name_Of_Object, type )
#endif

#ifndef GET_VALID_PrimaryColour
#define GET_VALID_PrimaryColour( type ) GET_VALID_ArrayItem( Colour_Of_Object_Primary, type )
#endif

#ifndef GET_VALID_SecondaryColour
#define GET_VALID_SecondaryColour( type ) GET_VALID_ArrayItem( Colour_Of_Object_Secondary, type )
#endif

static std::string Name_Of_Object[] =
{
	"Base",
	"Wheel",
	"Nut",
	"Column",
	"Column 1",
	"Column 2",
	"Column 3",
	"Column 4",
	"Hand",
	"Base Tip",
	"B_Hole 1",
	"B_Hole 2",
	"B_Hole 3",
	"B_Hole 4",
	"Top Floor",
	"Spot Green",
	"Spot Red",
	"Spot Magenta",
	"Spot Blue",
	"Glass Base",
	"Glass Top",
	"Liquid",
	"Cork",
	"Bottle",
	"Glass Full",
	"Object Red",
	"Cube Red",
	"Bolt Red",
	"Object Green",
	"Bolt Green",
	"Cube Green",
	"Cube Blue",
	"Cube Yellow",
	"Bolt Yellow",
	"Nut Orange",
	"Slat",
	"Slat Short",
	"Slat Medium",
	"Slat Long",
	"Face",
	"Object Red",
	"Object Green",
	"Object Blue",
	"Object Cyan",
	"Object Magenta",
	"Object Yellow",
	"Object Orange",
	"Grasp Point",
	"Handle",
	"Object Magenta Small",
	"Object Magenta Big",
	"Base Aerospatial",
	"Column Yellow",
	"Column Green",
	"Bar Blue",
	"Bar Red",
	"Custom object",
	"Empty Glass",
	"Glass Inverted",
	"Bottle Closed",
	"Bottle Open",
	"Box",
	"Glass",
	"Battery",
	"TeaBottleClosed",
	"TeaBottleOpen",
	"TeaGlassFull",
	"CoffeeBottleClosed",
	"CoffeeBottleOpen",
	"CoffeeGlassFull",
	"JuiceBottleClosed",
	"JuiceBottleOpen",
	"JuiceGlassFull",
	"TableDumbo",
	"Table",
	"Pose",		//identifier for an object which is actually only a pose (position + orientation).
	"Motherboard",
	"ScrewBox",
	"PowerSupply",
	"Cable",
	"Disk",
	"Second Hand",
	"PCI Card"
	// .............
	"INVALID"
};

static unsigned int Colour_Of_Object_Primary[] =
{
	COLOUR_R,	// Base
	COLOUR_G,	// Wheel
	COLOUR_M,	// Nut
	COLOUR_Y,	// Column
	COLOUR_Y,	// Column #1
	COLOUR_Y,	// Column #2
	COLOUR_Y,	// Column #3
	COLOUR_Y,	// Column #3
	COLOUR_G,	// Hand
	COLOUR_G,	// Base Tip
	COLOUR_Y,	// Base Hole #1
	COLOUR_R,	// Base Hole #2
	COLOUR_M,	// Base Hole #3
	COLOUR_B,	// Base Hole #4
	COLOUR_B,	// Top Floor
	COLOUR_G,	// Spot Green
	COLOUR_R,	// Spot Red
	COLOUR_M,	// Spot Magenta
	COLOUR_B,	// Spot Blue
	COLOUR_C,	// Glass Base
	COLOUR_G,	// Glass Top
	COLOUR_R,	// Liquid
	COLOUR_Y,	// Cork
	COLOUR_G,	// Bottle
	COLOUR_Y,	// Glass Full
	COLOUR_R,	// Object Red
	COLOUR_R,	// Cube Red
	COLOUR_R,	// Bolt Red
	COLOUR_G,	// Object Green
	COLOUR_G,	// Bolt Green
	COLOUR_G,	// Cube Green
	COLOUR_B,	// Cube Blue
	COLOUR_Y,	// Cube Yellow
	COLOUR_Y,	// Bolt Yellow
	COLOUR_W,	// Nut Orange
	COLOUR_W,	// Slat
	COLOUR_W,	// Slat Short
	COLOUR_W,	// Slat Medium
	COLOUR_W,	// Slat Long
	COLOUR_W,	// Face
	COLOUR_R,	// Object Red
	COLOUR_G,	// Object Green
	COLOUR_B,	// Object Blue
	COLOUR_C,	// Object Cyan
	COLOUR_M,	// Object Magenta
	COLOUR_Y,	// Object Yellow
	COLOUR_W,	// Object Orange
	COLOUR_G,	// Grasp point
	COLOUR_Y,	// Handle
	COLOUR_M,	// Object Magenta Small
	COLOUR_M,	// Object Magenta Big
	COLOUR_W,	// Base Aerospatial
	COLOUR_Y,	// Column Yellow
	COLOUR_G,	// Column Green
	COLOUR_B,	// Bar Blue
	COLOUR_R,	// Bar Red
	COLOUR_K,	// Custom object
	COLOUR_K,	// Empty Glass
	COLOUR_Y,	// Glass Inverted
	COLOUR_K,	// Bottle Closed
	COLOUR_K,	// Bottle Open
	COLOUR_W,	// Box
	COLOUR_Y,	// Glass
	COLOUR_K,	// Battery
	COLOUR_G,	// TeaBottleClosed
	COLOUR_G,	// TeaBottleOpen
	COLOUR_Y,	// TeaGlassFull
	COLOUR_Y,	 //CoffeeBottleClosed"
	COLOUR_Y,	//CoffeeBottleOpen"
	COLOUR_Y,	//CoffeeGlassFull"
	COLOUR_R,	//JuiceBottleClosed"
	COLOUR_R,	//JuiceBottleOpen"
	COLOUR_R,	//JuiceGlassFull"
	COLOUR_K,	//TableDumbo"
	COLOUR_K,	//Table"
	COLOUR_K,	//Pose",		//identifier for an object which is actually only a pose (position + orientation).
	COLOUR_K,	//Motherboard",
	COLOUR_M,	//ScrewBox",
	COLOUR_R,	//PowerSupply",
	COLOUR_Y,	//Cable",
	COLOUR_K,	//Disk",
	COLOUR_B,	//Second Hand",
	COLOUR_K,	//PCI Card",
	// ........................
	COLOUR_K	// No Object
};

static unsigned int Colour_Of_Object_Secondary[] =
{
	COLOUR_R,	// Base
	COLOUR_G,	// Wheel
	COLOUR_M,	// Nut
	COLOUR_Y,	// Column
	COLOUR_G,	// Column #1
	COLOUR_R,	// Column #2
	COLOUR_M,	// Column #3
	COLOUR_B,	// Column #4
	COLOUR_G,	// Hand
	COLOUR_G,	// Base Tip
	COLOUR_Y,	// Base Hole #1
	COLOUR_R,	// Base Hole #2
	COLOUR_M,	// Base Hole #3
	COLOUR_B,	// Base Hole #4
	COLOUR_B,	// Top Floor
	COLOUR_G,	// Spot Green
	COLOUR_R,	// Spot Red
	COLOUR_M,	// Spot Magenta
	COLOUR_B,	// Spot Blue
	COLOUR_C,	// Glass Base
	COLOUR_G,	// Glass Top
	COLOUR_R,	// Liquid
	COLOUR_Y,	// Cork
	COLOUR_G,	// Bottle
	COLOUR_Y,	// Glass Full
	COLOUR_R,	// Object Red
	COLOUR_R,	// Cube Red
	COLOUR_R,	// Bolt Red
	COLOUR_G,	// Object Green
	COLOUR_G,	// Bolt Green
	COLOUR_G,	// Cube Green
	COLOUR_B,	// Cube Blue
	COLOUR_Y,	// Cube Yellow
	COLOUR_Y,	// Bolt Yellow
	COLOUR_W,	// Nut Orange
	COLOUR_W,	// Slat
	COLOUR_W,	// Slat Short
	COLOUR_W,	// Slat Medium
	COLOUR_W,	// Slat Long
	COLOUR_K,	// Face
	COLOUR_R,	// Object Red
	COLOUR_G,	// bject Green
	COLOUR_B,	// Object Blue
	COLOUR_C,	// Object Cyan
	COLOUR_M,	// Object Magenta
	COLOUR_Y,	// Object Yellow
	COLOUR_W,	// Object Orange
	COLOUR_G,	// Grasp point
	COLOUR_G,	// Handle
	COLOUR_M,	// Object Magenta Small
	COLOUR_M,	// Object Magenta Big
	COLOUR_W,	// Base Aerospatial
	COLOUR_Y,	// Column Yellow
	COLOUR_G,	// Column Green
	COLOUR_B,	// Bar Blue
	COLOUR_R,	// Bar Red
	COLOUR_K,	// Custom object
	COLOUR_K,	// Empty Glass
	COLOUR_R,	// Glass Inverted
	COLOUR_K,	// Bottle Closed
	COLOUR_K,	// Bottle Open
	COLOUR_W,	// Box
	COLOUR_Y,	// Glass
	COLOUR_K,	// Battery
	COLOUR_G,	// TeaBottleClosed
	COLOUR_G,	// TeaBottleOpen
	COLOUR_Y,	// TeaGlassFull
	COLOUR_Y,	// CoffeeBottleClosed"
	COLOUR_Y,	// CoffeeBottleOpen"
	COLOUR_Y,	// CoffeeGlassFull"
	COLOUR_R,	// JuiceBottleClosed"
	COLOUR_R,	// JuiceBottleOpen"
	COLOUR_R,	// JuiceGlassFull"
	COLOUR_K,	// TableDumbo"
	COLOUR_K,	// Table"
	COLOUR_K,	// Pose",		//identifier for an object which is actually only a pose (position + orientation).
	COLOUR_K,	// Motherboard",
	COLOUR_M,	// ScrewBox",
	COLOUR_R,	// PowerSupply",
	COLOUR_Y,	// Cable",
	COLOUR_K,	// Disk",
	COLOUR_B,	// Second Hand",
	COLOUR_K,	// PCI Card",
	// ........................
	COLOUR_K	// No Object
};

#endif // _OBJECT_DEFINITIONS_H_
