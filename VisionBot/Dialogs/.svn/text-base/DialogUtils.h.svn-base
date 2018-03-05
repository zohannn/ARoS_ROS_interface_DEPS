#ifndef _DIALOG_UTILS_H_
#define _DIALOG_UTILS_H_

#pragma once

#include <afxwin.h>
#include "string_operations.h"

bool ReadTextFromEditBox( CEdit * pEdit, std::tstring &text );

template <typename T>
bool ReadValueFromEditBox( CEdit * pEdit, T * pVal )
{
	std::tstring text;

	if( !ReadTextFromEditBox( pEdit, text ) ) return false;

	return string_to_number( text, pVal );
}

void ShowFormatedTextInStatic( CStatic* static_text, std::tstring caption, float value, std::tstring unit );

#endif // _DIALOG_UTILS_H_
