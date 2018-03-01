#ifndef _COLOUR_DEFS_H_
#define _COLOUR_DEFS_H_

#include <string>

#include "Preprocessor_Macros.h"

enum _THE_COLOURS_{
	COLOUR_R = 0,	// Red
	COLOUR_G,		// Green
	COLOUR_B,		// Blue
	COLOUR_C,		// Cyan
	COLOUR_M,		// Magenta
	COLOUR_Y,		// Yellow
	COLOUR_W,		// White
	COLOUR_K,		// Black
	// Add more colours here, before NUMBER_OF_COLOURS ...
	// Do not forget to add name, and colour components below
	NUMBER_OF_COLOURS,
};

#define isValid_Colour( colour ) ( ( colour>=0 ) && ( colour<NUMBER_OF_COLOURS ) )
#define GET_VALID_Colour( colour ) isValid_Colour( colour ) ? colour : NUMBER_OF_COLOURS

static std::string Name_Of_Colour[] =
{
	"Red",
	"Green",
	"Blue",
	"Cyan",
	"Magenta",
	"Yellow",
	"White",
	"Black",
	// .............
	"NO_COLOUR"
};

#define NUMBER_OF_COLOUR_NAMES SIZE_OF_ARRAY( Name_Of_Colour )
#define isValid_ColourName( colour ) ( ( colour>=0 ) && ( colour<SIZE_OF_ARRAY( Name_Of_Colour ) )
#define GET_VALID_ColourName( colour ) isValid_ColourName( colour ) ? Name_Of_Colour[colour] : Name_Of_Colour[NUMBER_OF_COLOUR_NAMES-1]

static int R_Component[] =
{
	255, 0,	0, 0, 255, 255, 255, 0, 0
};

static int G_Component[] =
{
	0, 255, 0, 255, 0, 255, 255, 0, 0
};

static int B_Component[] =
{
	0, 0, 255, 255, 255, 0, 255, 0, 0
};

#endif // _COLOUR_DEFS_H_
