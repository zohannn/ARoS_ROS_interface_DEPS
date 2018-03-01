#ifndef _ENVIRONMENT_VARIABLES_H_
#define _ENVIRONMENT_VARIABLES_H_

#pragma once

#include <string>
#include <iostream>

/// <summary>Template function to encapsulate the reading of environment variables
/// in ANSI or Unicode using the same function.</summary>
///	<param name="ReturnSize">Pointer to a variable that returns the size of the buffer.</param>
///	<param name="_DstBuf">Buffer to store the environment variable contents.</param>
///	<param name="_DstSizeInWords">Size of the provided buffer.</param>
///	<param name="_VarName">Environment variable name.</param>
///	<returns>Error code, 0 if there is no error.</returns>
template <typename char_type>
errno_t get_environment_variable( size_t * ReturnSize, char_type * _DstBuf, size_t _DstSizeInWords, const char_type * _VarName );

/// <summary>Read environment variable and return result as string (ANSI or Unicode).</summary>
///	<param name="varname">Name of environment variable (std::string or std::wstring).</param>
///	<returns>The content of the environment variable (std::string or std::wstring).</returns>
template <typename char_type>
std::basic_string < char_type, std::char_traits<char_type>, std::allocator<char_type> >
get_Env_Var_s( std::basic_string < char_type, std::char_traits<char_type>, std::allocator<char_type> > varname )
{
	size_t buf_size = 0;

	// Buffers to store data
	char_type * buf = nullptr;
	std::basic_string<char_type, std::char_traits<char_type>, std::allocator<char_type> > strBuffer;

	// Determine the required size
	auto error = get_environment_variable<char_type>( &buf_size, nullptr, 0, varname.c_str() );

	if( ( buf_size < 1 ) || ( error != 0 ) ) return strBuffer;

	try
	{
		// Allocate memory for buffer
		buf = new char_type[buf_size];

		// Get the environment variable
		error = get_environment_variable<char_type>( &buf_size, buf, buf_size, varname.c_str() );

		if( ( buf_size > 0 ) && ( error == 0 ) )
			strBuffer = buf;

		delete[] buf;
	}
	catch( std::bad_alloc& ex )
	{
		std::cerr << "Failed to allocate memory for buffer! Exception caught: " << ex.what() << std::endl;
	}

	buf = nullptr;

	return strBuffer;
}

#define get_Env_Var_ANSI get_Env_Var_s<char>
#define get_Env_Var_Unicode get_Env_Var_s<wchar_t>

#endif //_ENVIRONMENT_VARIABLES_H_
