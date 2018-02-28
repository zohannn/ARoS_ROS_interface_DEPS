#include "EnvironmentVars.h"

template <>
errno_t get_environment_variable<char>( size_t * ReturnSize, char * _DstBuf, size_t _DstSizeInWords, const char * _VarName )
{
	return getenv_s( ReturnSize, _DstBuf, _DstSizeInWords, _VarName );
}

template <>
errno_t get_environment_variable<wchar_t>( size_t * ReturnSize, wchar_t * _DstBuf, size_t _DstSizeInWords, const wchar_t * _VarName )
{
	return _wgetenv_s( ReturnSize, _DstBuf, _DstSizeInWords, _VarName );
}
