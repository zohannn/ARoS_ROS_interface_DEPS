#include "DialogUtils.h"
#include <exception>
#include <iomanip>

bool ReadTextFromEditBox( CEdit * pEdit, std::tstring &text )
{
	if( pEdit == nullptr ) return false;
	if( pEdit->m_hWnd == nullptr ) return false;

	int buf_size = pEdit->GetWindowTextLength() + 1;

	if( buf_size < 1 ) return false;

	try
	{
		t_char * buf = new t_char[buf_size];

		int num_char = pEdit->GetWindowText( buf, buf_size );

		text = buf;

		delete[] buf;
		buf = nullptr;

		return ( num_char > 0 );
	}
	catch( std::bad_alloc& ex )
	{
		std::tstring strError = _T( "Failed to allocate memory for buffer to read Edit box value! Exception caught: " );
		strError += CA2T( ex.what() );

		AfxMessageBox( strError.c_str(), MB_ICONERROR );

		return false;
	}
}

void ShowFormatedTextInStatic( CStatic* static_text, std::tstring caption, float value, std::tstring unit )
{
	std::tostringstream str_text;
	str_text << caption << std::setprecision( 4 ) << value << unit;
	static_text->SetWindowText( str_text.str().c_str() );
}
