#include "DialogFileOpenSave_types.h"

CDialogFileOpenSave_ini::CDialogFileOpenSave_ini( void )
{
	strDefaultExtension = "ini";
	strFileFilters = "Configuration File (*.ini)|*.ini|All Files (*.*)|*.*|";
}

CDialogFileOpenSave_bmp::CDialogFileOpenSave_bmp( void )
{
	strDefaultExtension = "bmp";
	strFileFilters = "Bitmap Images (*.bmp)|*.bmp|";
}

CDialogFileOpenSave_avi::CDialogFileOpenSave_avi( void )
{
	strDefaultExtension = "avi";
	strFileFilters = "Video files (*.avi)|*.avi|";
}

CDialogFileOpenSave_images::CDialogFileOpenSave_images( void )
{
	strDefaultExtension = "bmp";
		
	strFileFilters = "Windows Bitmap Images (*.bmp; *.dib)|*.bmp;*.dib|";
	strFileFilters += "JPEG files (*.jpeg; *.jpg; *.jpe)|*.jpeg;*.jpg;*.jpe|";
	strFileFilters += "JPEG 2000 images (*.jp2)|*.jp2|";
	strFileFilters += "Portable Network Graphics (*.png)|*.png|";
	strFileFilters += "Portable image format (*.pbm; *.pgm; *.ppm)|*.pbm;*.pgm;*.ppm|";
	strFileFilters += "Sun rasters (*.sr; *.ras)|*.sr;*.ras|";
	strFileFilters += "TIFF files (*.tiff; *.tif)|*.tiff;*.tif|";
	//strFileFilters += "OpenEXR HDR images (*.exr)|*.exr|";
}
