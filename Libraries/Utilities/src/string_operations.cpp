#include "string_operations.h"

bool string_replace_all( std::string& str, const std::string& from, const std::string& to )
{
	if( from.empty() || str.empty() ) return false;

	size_t start_pos = 0;
	
	bool bReplace = false;

	while( ( start_pos = str.find( from, start_pos ) ) != std::string::npos )
	{
		str.replace( start_pos, from.length(), to );
		start_pos += to.length();

		bReplace = true;
	}

	return bReplace;
}

bool string_replace_all( std::wstring& str, const std::wstring& from, const std::wstring& to )
{
	if( from.empty() || str.empty() ) return false;

	size_t start_pos = 0;

	bool bReplace = false;

	while( ( start_pos = str.find( from, start_pos ) ) != std::wstring::npos )
	{
		str.replace( start_pos, from.length(), to );
		start_pos += to.length();

		bReplace = true;
	}

	return bReplace;
}
