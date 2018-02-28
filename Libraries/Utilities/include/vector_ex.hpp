#ifndef _VECTOR_EX_HPP_
#define _VECTOR_EX_HPP_

#pragma once

#include <iostream>

#include "vector_operations.hpp"

///	<summary>Template container class based in std::vector that implements
/// a few extra features, like limiting the number of elements in the vector.</summary>
template <class T>
class vector_ex : public std::vector<T>
{
public:
	/// <summary>Default constructor. Sets the size limit to '0'.</summary>
	vector_ex( void ) 
	: vector_limit( 0 )
	{};

	vector_ex( vector_ex<T> &obj )
	: vector_limit( obj.vector_limit )
	{
		copy( obj );
	}

	~vector_ex( void )
	{
		clear();
	}

	//-----------------------------------------------------------------------
	// Limit handling
protected:
	/// <summary>Member variable that stores the size limit.</summary>
	size_t vector_limit;

public:
	/// <summary>Accessor to define the size limit of the vector.</summary>
	///	<param name="limit">Maximum number of elements in the vector.</param>
	void set_limit( size_t limit )
	{
		vector_limit = limit;
	};

	/// <summary>Get the size limit of the vector.</summary>
	///	<returns>The current size limit of the vector.</returns>
	size_t get_limit( void )
	{
		return vector_limit;
	};

	//-----------------------------------------------------------------------
	// Add elements
public:
	/// <summary>Add one element to the end of the vector, but respect the size limit.
	/// <para>If the limit is exceeded, discard elements at the begining to remain
	/// within the defined size limit.</para></summary>
	///	<param name="val">Object to be added.</param>
	void emplace_back_limited( T val )
	{
		emplace_back( val );
		vector_enforce_number_elements( *this, vector_limit );
	};

	//-----------------------------------------------------------------------
	// Accessors
public:
	/// <summary>Get element at position 'index', if the element does not exist, returns '0'.
	/// <para>This function performs a boundary check.</para></summary>
	///	<returns>The value of the element at position 'index' if it exists, '0' otherwise.</returns>
	T get( int index )
	{
		try
		{
			return at( index );
		}
		catch( const std::out_of_range& ex )
		{
			T ret;
			std::cerr << "Out of Range error: " << ex.what() << std::endl;
			// Set to zero
			memset( &ret, 0, sizeof( T ) );
			return ret;
		}
	};

	/// <summary>Get last element of the vector, if the element does not exist, returns '0'.</summary>
	///	<returns>The value of the last element if it exists, '0' otherwise.</returns>
	T last( void )
	{
		if( !empty() )
			return back();
		else
		{
			T ret;
			memset( &ret, 0, sizeof( T ) );
			return ret;
		}
	};
	
	/// <summary>Get first element of the vector, if the element does not exist, returns '0'.</summary>
	///	<returns>The value of the first element if it exists, '0' otherwise.</returns>
	T first( void )
	{
		if( !empty() )
			return front();
		else
		{
			T ret;
			memset( &ret, 0, sizeof( T ) );
			return ret;
		}
	};

	//-----------------------------------------------------------------------
public:
	/// <summary>Copy the contents of another vector.
	/// <para>Makes sure there is enough memory to perform the copy operation.</para></summary>
	///	<param name="vec">Vector to be copied.</param>
	void copy( std::vector<T>& vec )
	{
		this->assign( vec.begin(), vec.end() );
	};
};

#endif // _VECTOR_EX_HPP_
