/*------------------------------------------------------------------------------
This file offers template implementations to guard data types with a Mutex
See the description in "MutexBase.hpp" for details on the base classes
------------------------------------------------------------------------------*/

#ifndef _ATTACH_MUTEX_HPP_
#define _ATTACH_MUTEX_HPP_

#pragma once

#include "MutexSTL.h"
#include "MutexYarp.h"

#ifdef USE_YARP
	#define DEFAULT_MUTEX_TYPE CMutexYarp
	#define DEFAULT_MUTEX_TYPE_PTR CMutexYarpPtr
#elif defined( USE_STL )
	#define DEFAULT_MUTEX_TYPE CMutexSTL
	#define DEFAULT_MUTEX_TYPE_PTR CMutexSTLPtr
#else
	#error No valid base class for CAttachMutex_...<DataType,MutexType> was found. Either YARP, STL or both must be used to compile MutexWrapper library.
#endif

//------------------------------------------------------------------------------
/// <summary>CAttachMutex_t<Type> offers a Mutex wrapper for 
/// complex types like: struct, class, ...
/// <para>Derived from CMutexBase.</para>
/// <para>No constructor, destructor or any other functions are required
/// in this class, all the methods and members are inherited.</para></summary>
template <typename DataType, typename MutexType=DEFAULT_MUTEX_TYPE>
class CAttachMutex_t
	: public MutexType
	, public DataType
{};

//------------------------------------------------------------------------------
/// <summary>CAttachMutex_n<Type> offers a Mutex wrapper for 
/// native types like: int, float, double, ...
/// <para>Derived from CMutexBase.</para></summary>
template <typename DataType, typename MutexType=DEFAULT_MUTEX_TYPE>
class CAttachMutex_n
	: public MutexType
{
protected:
	/// <summary>Member variable that stores the data.</summary>
	DataType value;

public:
	/// <summary>Default constructor.
	/// <para>Initializes data variable with zero.</para></summary>
	/// <param name="val">Value to assign to data variable.</param>
	CAttachMutex_n( DataType val = 0 )
		: value( val )
		{};

	/// <summary>Accessor to assign to protected data.</summary>
	/// <param name="val">Value to assign to data variable.</param>
	void set( DataType val )
	{
		MutexType::lock();
		value = val;
		MutexType::unlock();
	};

	/// <summary>Accessor to read data.</summary>
	/// <returns>Value of the protected data.</param>
	DataType get( void )
	{
		MutexType::lock();
		DataType val = value;
		MutexType::unlock();
		return val;
	};
};

//---------------------------------------------------------------------------
/// <summary>CAttachMutex_n<Type> offers a Mutex wrapper to be  
/// attached to a public object of any type 'Type' accessible, named 'Obj'</summary>
template <typename DataType, typename MutexType=DEFAULT_MUTEX_TYPE>
class CAttachMutex_o
	: public MutexType
{
public:
	/// <summary>Member variable that stores the data.</summary>
	DataType Obj;
};

//---------------------------------------------------------------------------
/// <summary>CAttachMutex_t<Type> offers a Mutex pointer wrapper for 
/// complex types like: struct, class, ...</summary>
template <typename DataType, typename MutexType=DEFAULT_MUTEX_TYPE_PTR>
class CAttachMutexPtr_t
	: public MutexType
	, public DataType
{};

//---------------------------------------------------------------------------
/// <summary>CAttachMutex_n<Type> offers a Mutex pointer wrapper for 
/// native types like: int, float, double, ...</summary>
template <typename DataType, typename MutexType=DEFAULT_MUTEX_TYPE_PTR>
class CAttachMutexPtr_n
	: public MutexType
{
protected:
	/// <summary>Member variable that stores the data.</summary>
	DataType value;

public:
	/// <summary>Default constructor.
	/// <para>Initializes data variable with zero.</para></summary>
	/// <param name="val">Value to assign to data variable.</param>
	CAttachMutexPtr_n( DataType val = 0 )
		: value( val )
		{};

	/// <summary>Accessor to assign to protected data.</summary>
	/// <param name="val">Value to assign to data variable.</param>
	void set( DataType val )
	{
		if( !valid() ) return;

		MutexType::lock();
		value = val;
		MutexType::unlock();
	};

	/// <summary>Accessor to read data.</summary>
	/// <returns>Value of the protected data.</param>
	DataType get( void )
	{
		if( !valid() ) return static_cast<DataType>( 0 );

		MutexType::lock();
		DataType val = value;
		MutexType::unlock();
		return val;
	};
};

//---------------------------------------------------------------------------
/// <summary>CAttachMutex_n<Type> offers a Mutex pointer wrapper to be  
/// attached to a public object of any type.</summary>
template <typename DataType, typename MutexType=DEFAULT_MUTEX_TYPE_PTR>
class CAttachMutexPtr_o
	: public MutexType
{
public:
	/// <summary>Member variable that stores the data.</summary>
	DataType Obj;
};

#endif // _ATTACH_MUTEX_HPP_
