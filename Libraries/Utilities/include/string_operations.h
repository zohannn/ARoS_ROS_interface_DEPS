#ifndef _STRING_OPERATIONS_H_
#define _STRING_OPERATIONS_H_

#pragma once

#include <sstream>

#include "ansi_unicode_definitions.h"

/// <summary>Parse the contents of a string and convert it to a number.</summary>
///	<param name="str">string to be parsed.</param>
///	<param name="value">Pointer to numeric variable where the result will be stored.</param>
///	<returns>true if the operation was successful, false if it fails.</returns>
template <typename T>
bool string_to_number( std::tstring str, T * value )
{
	T val = static_cast<T>( 0 );

	std::tstringstream str_stream( str );
	str_stream >> val;
    
	if( str_stream.fail() )
	{
		return false;
	}
	else
	{
		*value = val;
		return true;
	}
}

/// <summary>Parse the contents of a character array and convert it to a number.</summary>
///	<param name="str">character array to be parsed.</param>
///	<param name="value">Pointer to numeric variable where the result will be stored.</param>
///	<returns>true if the operation was successful, false if it fails.</returns>
template <typename T>
bool string_to_number( t_char * str, T * value )
{
	std::tstring tstr( str );

	return string_to_number( tstr, value );
}

/// <summary>Replace all occurrences of a given string and replace them with other (ANSI).</summary>
///	<param name="str">String to be treated.</param>
///	<param name="from">String to search for to be replaced.</param>
///	<param name="to">String to replace.</param>
///	<returns>true if any replacement was performed, false if nothing was replaced.</returns>
bool string_replace_all( std::string& str, const std::string& from, const std::string& to );

/// <summary>Replace all occurrences of a given string and replace them with other (UNICODE).</summary>
///	<param name="str">String to be treated.</param>
///	<param name="from">String to search for to be replaced.</param>
///	<param name="to">String to replace.</param>
///	<returns>true if any replacement was performed, false if nothing was replaced.</returns>
bool string_replace_all( std::wstring& str, const std::wstring& from, const std::wstring& to );

#endif // _STRING_OPERATIONS_H_
