#ifndef _ANSI_UNICODE_DEFINITIONS_H_
#define _ANSI_UNICODE_DEFINITIONS_H_

#pragma once

#include <tchar.h>
#include <atlbase.h>
#include <atlconv.h>
#include <sstream>

namespace std
{
#ifdef UNICODE
	#define tstring wstring
	#define tstringstream wstringstream
	#define tostringstream wostringstream
	#define tofstream wofstream
	#define tfstream wfstream
#else
	#define tstring string
	#define tstringstream stringstream
	#define tostringstream ostringstream
	#define tofstream ofstream
	#define tfstream fstream
#endif
};

#ifdef UNICODE
	#define t_char wchar_t
	#define tsscanf swscanf
	#define tsprintf swprintf
	#define tCString CStringW 

	#define CHAR_ZERO L'\0'
	#define STRING_EMPTY L""
#else
	#define t_char char
	#define tsscanf sscanf
	#define tsprintf sprintf
	#define tCString CString

	#define CHAR_ZERO '\0'
	#define STRING_EMPTY ""
#endif

#endif //_ANSI_UNICODE_DEFINITIONS_H_
