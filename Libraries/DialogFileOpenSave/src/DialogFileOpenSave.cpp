#include <afxdlgs.h>

#include "DialogFileOpenSave.h"

#include "ansi_unicode_definitions.h"

CDialogFileOpenSave::CDialogFileOpenSave( void )
: strDefaultExtension( "*" )
, strFileFilters( "All Files (*.*)|*.*|" )
, ulFlags( OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST )
{
}

CDialogFileOpenSave::~CDialogFileOpenSave( void )
{
	strDefaultExtension.clear();
	strFileFilters.clear();
}

std::string CDialogFileOpenSave::Prompt_User_Get_Filename( CFileDialog *pDlg )
{
	// TODO: Add your control notification handler code here
	std::string strFileName = "";

	if( pDlg == nullptr ) return strFileName;

	auto nResult = pDlg->DoModal();

	if( nResult == IDOK )
	{
		std::tstring file_name = _T( "" );
		
		file_name = pDlg->GetPathName();
		
	#ifdef UNICODE
		strFileName = CT2A( file_name.c_str() );
	#else
		strFileName = file_name;
	#endif
	}
		
	return strFileName;
}
