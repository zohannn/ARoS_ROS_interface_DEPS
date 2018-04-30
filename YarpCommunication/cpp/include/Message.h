#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <vector>
#include <string>

#include <yarp/os/Bottle.h>

/// <summary>CMessage serves as the core data structure to exchange information using CYarpCommunication</summary>
class CMessage
{
//------------------------------------------------------------------------------
public:
	/// <summary>Constructor: Creates an instance of CMessage.</summary>
	CMessage( void );

	/// <summary>Constructor from command: Creates an instance of CMessage, configured with the given command.</summary>
	CMessage( unsigned int command );

	/// <summary>Copy constructor: Creates an instance of CMessage, and copies the data from another CMessage.</summary>
	CMessage( const CMessage &msg );

	/// <summary>Destructor: destroys this CMessage instance.</summary>
	~CMessage( void );

//------------------------------------------------------------------------------
// Contents management
public:
	/// <summary>Clears all data in the object.</summary>
	void clear( void );

	/// <summary>Copies data from 'src' to 'dst'.</summary>
	/// <param name="src">Pointer to source object.</param>
	/// <param name="dst">Pointer to destination object.</param>
	void copy( CMessage *src, CMessage *dst );

	/// <summary>Copies data from 'src' to this object.</summary>
	/// <param name="src">Pointer to source object.</param>
	void copy_from( CMessage * src );

	/// <summary>Assigns data to this object from another.</summary>
	/// <param name="rightOp">Object to be copied.</param>
	/// <returns>This object after the copy.</returns>
	CMessage operator=( CMessage rightOp );

//------------------------------------------------------------------------------
// Member variables
public:
	/// <summary>Stores data in text to be human readable.</summary>
	std::string strText;

	/// <summary>Command ID.</summary>
	unsigned int uCommand;

	/// <summary>Vector of unsigned int parameters.</summary>
	std::vector<unsigned int> uParam;

	/// <summary>Vector of float parameters.</summary>
	std::vector<float> fData;


	/// <summary>Error code, if exists.</summary>
	int iErrorCode;

	/// <summary>Binary data.</summary>
	char *bin;

	/// <summary>Binary data size.</summary>
	size_t bin_size;

//------------------------------------------------------------------------------
// Type conversions
public:
	/// <summary>CMessage data in human readable format.</summary>
	/// <returns>Data in the object in string format.</returns>
	std::string asString( void );

	/// <summary>Get data from a Bottle.</summary>
	/// <param name="bottleMsg">Bottle object to be copied.</param>
	void fromBottle( yarp::os::Bottle &bottleMsg );

	/// <summary>Convert CMessage data to a Bottle.</summary>
	/// <param name="bottleMsg">Bottle object to put the data.</param>
	void toBottle( yarp::os::Bottle &bottleMsg );

	/// <summary>Retrieve CMessage data in Bottle format.</summary>
	/// <returns>Data in the object in Bootle format.</returns>
	yarp::os::Bottle toBottle( void );

//------------------------------------------------------------------------------
// Utility functions
public:
	/// <summary>Test if its a valid message or not, evaluates <see cref="CMessage::uCommand"/>.</summary>
	/// <returns>True if valid, false otherwise.</returns>
	bool isValid( void );
};

#endif //_MESSAGE_H_
