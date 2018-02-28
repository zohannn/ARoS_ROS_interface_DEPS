#include "StereoVidere.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "MathDefines.h"
#include "range.hpp"

CStereoVidere::CStereoVidere( void )
: CConfigFile()
, pStereoImage( nullptr )
, stereoProcess( nullptr )
, Pan( 0.0f ), Tilt( 0.0f )
, uNum_Transf( 0 )
, mOriginal( 4, 1 ), mTransf( 4, 1 )
, T( 4, 4 ), T_1( 4, 4 ), T_2( 4, 4 )
, K1( 0.0f ), K2( 0.0f ), K3( 0.0f ), K4( 0.0f )
, strDisparityWindow( "Disparity image" )
, bUseMultiScale( false )
, bDisplayDisparity( false )
, bDisplayWindowCreated( false )
{
	memset( Offset, 0, sizeof( Offset ) );

	T.set_Identity();
	T_1.set_Identity();

	ptLimitMin.x = 0.0f;
	ptLimitMin.y = 0.0f;
	ptLimitMin.z = 0.0f;

	ptLimitMax.x = 0.0f;
	ptLimitMax.y = 0.0f;
	ptLimitMax.z = 0.0f;
}

CStereoVidere::~CStereoVidere( void )
{
	Destroy_SVS_Object();

	pStereoImage = nullptr;

	Clear_Constant_Transformation_Parameters();

	DestroyDisparityWindow();

	matDisparity.lock();
	matDisparity.Obj.release();
	matDisparity.unlock();

	strDisparityWindow.clear();
}

void CStereoVidere::Instantiate_SVS_Object( bool multiscale )
{
	bUseMultiScale = multiscale;
	
	if( bUseMultiScale )
		stereoProcess = new svsMultiProcess(); // for computing a disparity image in Multiscale mode
	else
		stereoProcess = new svsStereoProcess();
}

void CStereoVidere::Destroy_SVS_Object( void )
{
	if( stereoProcess != nullptr )
	{
		if( bUseMultiScale )
			delete ( ( svsMultiProcess * ) stereoProcess );
		else 
			delete stereoProcess;
		
		stereoProcess = nullptr;
	}
}

void CStereoVidere::set_StereoImage( svsStereoImage * img )
{
	pStereoImage = img;
}

void CStereoVidere::Resize_Transformation_Arrays( unsigned int num_transf )
{
	uNum_Transf = num_transf;

	// Resize transformation arrays and assign with '0'
	Alpha.assign( num_transf, 0.0f );
	A.assign( num_transf, 0.0f );
	D.assign( num_transf, 0.0f );
	Theta.assign( num_transf, 0.0f );
}

void CStereoVidere::Clear_Constant_Transformation_Parameters( void )
{
	Alpha.clear();
	A.clear();
	D.clear();
	Theta.clear();
}

void CStereoVidere::Setup_Transformation_Parameters( void )
{
	// Default parameters, if different, this function must be implemented in the derived class
	Theta.at( 1 ) = M_PI_2_F + Pan.get()*DEG_TO_RAD_F;
	Theta.at( 2 ) = Tilt.get()*DEG_TO_RAD_F;
}

void CStereoVidere::Process_Stereo( void )
{
	// Compute the stereo
	
	// Test pointers for stereo image and stereo process
	if( ( pStereoImage == nullptr ) || ( stereoProcess == nullptr ) ) return;

	// Try to perform stereo, if it fails exit.
	bool bStereoSuccess = false;

	if( bUseMultiScale )
		bStereoSuccess = ( (svsMultiProcess*) stereoProcess )->CalcStereo( pStereoImage );
	else
		bStereoSuccess = stereoProcess->CalcStereo( pStereoImage );

	if( !bStereoSuccess ) return;

	// Calculate transformation matrix
	CalculateTransfMatrix();

	if( bDisplayDisparity.get() )
		Generate_Disparity_Image();
}

bool CStereoVidere::Calculate_3D_Point( int nX, int nY, float * fX, float * fY, float * fZ )
{
	bool bSuccess = false;

	double
		dX = 0.0,
		dY = 0.0,
		dZ = 0.0;

	if( bUseMultiScale )
		bSuccess = ( ( svsMultiProcess * ) stereoProcess )->CalcPoint3D( nX, nY, pStereoImage, &dX, &dY, &dZ );
	else
		bSuccess = stereoProcess->CalcPoint3D( nX, nY, pStereoImage, &dX, &dY, &dZ );

	*fX = static_cast<float>( dX );
	*fY = static_cast<float>( dY );
	*fZ = static_cast<float>( dZ );

	return bSuccess;
}

bool CStereoVidere::Calculate_3D_Point( cv::Point2i pt_img, cv::Point3f * pt_vision )
{
	return Calculate_3D_Point( pt_img.x, pt_img.y, &pt_vision->x, &pt_vision->y, &pt_vision->z );
}

void CStereoVidere::Create_3D_Segmentation_Image_Coord_Limits( cv::Mat * mat_ptr )
{
	if( mat_ptr->empty() ) return;

	const unsigned char
		cOne = static_cast<unsigned char>( 255 ),
		cZero = static_cast<unsigned char>( 0 );

	mat_ptr->setTo( cZero );

	// Scale to convert to millimeters
	const float scale = 1000.0f;

	cv::Point2i iCoord( 0, 0 );

	cv::Point3f
		ptCoord_vision( 0.0f, 0.0f, 0.0f ),
		ptCoord_world( 0.0f, 0.0f, 0.0f );

	std::pair< cv::Point3f, cv::Point3f > limit;

	ptLimitMin.lock();
	limit.first = cv::Point3f( ptLimitMin.x, ptLimitMin.y, ptLimitMin.z );
	ptLimitMin.unlock();
	
	ptLimitMax.lock();
	limit.second = cv::Point3f( ptLimitMax.x, ptLimitMax.y, ptLimitMax.z );
	ptLimitMax.unlock();

	for( iCoord.x=0 ; iCoord.x<mat_ptr->cols ; iCoord.x++ )
	{
		for( iCoord.y=0 ; iCoord.y<mat_ptr->rows ; iCoord.y++ )
		{
			if( !Calculate_3D_Point( iCoord, &ptCoord_vision ) ) continue;

			// Scale application
			ptCoord_vision.x *= scale;
			ptCoord_vision.y *= scale;
			ptCoord_vision.z *= scale;

			Compensate_Coordinates_Vision( &ptCoord_vision.x, &ptCoord_vision.y, &ptCoord_vision.z );

			TransformCoordinates( ptCoord_vision, &ptCoord_world );

			mat_ptr->at<unsigned char>( iCoord ) = in_range( ptCoord_world, limit ) ? cOne : cZero;
		}
	}
}

void CStereoVidere::Create_3D_Segmentation_Image_Coord_Limits( cv::Mat * mat_ptr, cv::Point3f limit_min, cv::Point3f limit_max )
{
	ptLimitMin.lock();
	ptLimitMin.x = limit_min.x;
	ptLimitMin.y = limit_min.y;
	ptLimitMin.z = limit_min.z;
	ptLimitMin.unlock();

	ptLimitMax.lock();
	ptLimitMax.x = limit_max.x;
	ptLimitMax.y = limit_max.y;
	ptLimitMax.z = limit_max.z;
	ptLimitMax.unlock();

	Create_3D_Segmentation_Image_Coord_Limits( mat_ptr );
}

void CStereoVidere::Create_3D_Segmentation_Image_Distance_Limits( cv::Mat * mat_ptr, float limit_min, float limit_max )
{
	if( mat_ptr->empty() ) return;

	const unsigned char
		cOne = static_cast<unsigned char>( 255 ),
		cZero = static_cast<unsigned char>( 0 );

	std::pair<float, float> limit( limit_min, limit_max );

	mat_ptr->setTo( cZero );

	cv::Point2i iCoord( 0, 0 );

	cv::Point3f
		ptCoord_vision( 0.0f, 0.0f, 0.0f ),
		ptCoord_world( 0.0f, 0.0f, 0.0f );

	const float scale = 1000.0f;	// Scale to convert to millimeters
	
	float distance = 0.0f;

	const int
		cols = mat_ptr->cols,
		rows = mat_ptr->rows;

	for( iCoord.x=0 ; iCoord.x<cols ; iCoord.x++ )
	{
		for( iCoord.y=0 ; iCoord.y<rows ; iCoord.y++ )
		{
			if( !Calculate_3D_Point( iCoord, &ptCoord_vision ) ) continue;

			// Scale application
			ptCoord_vision.x *= scale;
			ptCoord_vision.y *= scale;
			ptCoord_vision.z *= scale;

			Compensate_Coordinates_Vision( &ptCoord_vision.x, &ptCoord_vision.y, &ptCoord_vision.z );

			TransformCoordinates( ptCoord_vision, &ptCoord_world );

			distance = sqrt( pow( ptCoord_world.x, 2 ) + pow( ptCoord_world.y, 2 ) );

			mat_ptr->at<unsigned char>( iCoord ) = in_range( distance, limit ) ? cOne : cZero;
		}
	}
}

void CStereoVidere::Generate_Disparity_Image( unsigned char gray_min, unsigned int gray_max )
{
	if( !pStereoImage->haveDisparity ) return;

	// A pair of vectors that will store the index and the value of valid data
	std::pair< 
		std::vector<int>,	// index
		std::vector<short>	// value
	> disp_data;

	for( int i=0 ; i<pStereoImage->imageBufsize ; i++ )
	{
		if( pStereoImage->disparity[i] > -1 )
		{
			disp_data.first.emplace_back( i );
			disp_data.second.emplace_back( pStereoImage->disparity[i] );
		}
	}

	// A pair of iterators to get the minimum and maximum value of the disparity
	std::pair< std::vector<short>::iterator, std::vector<short>::iterator > min_max;
	
	// Determine the minimum and maximum value of the disparity
	min_max = std::minmax_element( disp_data.second.begin(), disp_data.second.end() );

	const float 
		fDest_min = static_cast<float>( gray_min ),	// Minimum gray value in the generated image
		fDest_max = static_cast<float>( gray_max ),	// Maximum gray value in the generated image
		fSrc_min = static_cast<float>( *min_max.first ),
		fSrc_max = static_cast<float>( *min_max.second ),
		fDelta = ( fDest_max - fDest_min ) / ( fSrc_max - fSrc_min ),
		fB = fDest_min - fDelta*fSrc_min;

	matDisparity.lock();

	// if the image that will store the disparity image is empty, allocate memory to it
	if( matDisparity.Obj.empty() )
		matDisparity.Obj.create( pStereoImage->ip.height, pStereoImage->ip.width, CV_8UC1 );

	matDisparity.Obj.setTo( 0 );

	size_t data_size = disp_data.first.size();
	for( size_t i=0 ; i<data_size ; i++ )
	{
		matDisparity.Obj.data[ disp_data.first.at( i ) ] = static_cast<unsigned char>( fDelta * disp_data.second.at( i ) + fB );
	}

	if( bDisplayDisparity.get() && bDisplayWindowCreated.get() )
		cv::imshow( strDisparityWindow, matDisparity.Obj );

	matDisparity.unlock();
}

void CStereoVidere::Save_Disparity_Image_to_File( std::string strFile )
{
	matDisparity.lock();

	// If the image is empty generate it, otherwise use the one that we already have
	if( matDisparity.Obj.empty() )
		Generate_Disparity_Image();

	// If 'Generate_Disparity_Image' produced a valid image save it.
	if( !matDisparity.Obj.empty() )
		cv::imwrite( strFile, matDisparity.Obj );

	matDisparity.unlock();
}

bool CStereoVidere::get_DisplayDisparityWindow( void )
{
	return bDisplayDisparity.get();
}

void CStereoVidere::set_DisplayDisparityWindow( bool display )
{
	if( display == bDisplayDisparity.get() ) return;

	if( display )
		CreateDisparityWindow();
	else
		DestroyDisparityWindow();

	bDisplayDisparity.set( display );
}

void CStereoVidere::CreateDisparityWindow( void )
{
	if( !bDisplayWindowCreated.get() )
	{
		cv::namedWindow( strDisparityWindow );
		bDisplayWindowCreated.set( true );
	}
}

void CStereoVidere::DestroyDisparityWindow( void )
{
	if( bDisplayWindowCreated.get() )
	{
		cv::destroyWindow( strDisparityWindow );
		bDisplayWindowCreated.set( false );
	}
}

void CStereoVidere::CalculateTransfMatrix( void )
{
	Setup_Transformation_Parameters();

	T.set_Identity();

	size_t uTransf = Alpha.size();

	for( size_t i=0 ; i<uTransf ; i++ )
    {
        T_1.set_TransfMatrixData( Alpha.at( i ), A.at( i ), D.at( i ), Theta.at( i ) );
		T_2.set_Data( &T );
		T_2.Mul( &T_1, &T );
    }
}

void CStereoVidere::TransformCoordinates( float Original[3], float * pResult )
{
	unsigned int i=0;

	for( i=0 ; i<3 ; i++ ) 
	{
		mOriginal.set_Element( 0, i, Original[i] );
	}

	mOriginal.set_Element( 0, 3, 1.0f );

	T.Mul( &mOriginal, &mTransf );

	for( i=0 ; i<3 ; i++)
	{
		pResult[i] = mTransf( 0, i ) + Offset[i];
	}
}

void CStereoVidere::TransformCoordinates( cv::Point3f Original, cv::Point3f * pResult )
{
	float 
		res[3] = { 0.0f, 0.0f, 0.0f },
		orig[3] = { Original.x, Original.y, Original.z };

	TransformCoordinates( orig, res );

	pResult->x = res[0];
	pResult->y = res[1];
	pResult->z = res[2];
}

void CStereoVidere::TransformCoordinatesFull( float Original[3], float Pan_Angle, float Tilt_Angle, float * pResult )
{
	Pan.set( Pan_Angle );
	Tilt.set( Tilt_Angle );

	CalculateTransfMatrix();

	TransformCoordinates( Original, pResult );
}

void CStereoVidere::TransformCoordinatesFull( cv::Point3f Original, float Pan_Angle, float Tilt_Angle, cv::Point3f * pResult )
{
	float 
		res[3] = { 0.0f, 0.0f, 0.0f },
		orig[3] = { Original.x, Original.y, Original.z };

	TransformCoordinatesFull( orig, Pan_Angle, Tilt_Angle, res );

	pResult->x = res[0];
	pResult->y = res[1];
	pResult->z = res[2];
}

void CStereoVidere::Compensate_Coordinates_Vision( cv::Point3f * pt )
{
	Compensate_Coordinates_Vision( &pt->x, &pt->y, &pt->z );
}

void CStereoVidere::Compensate_Coordinates_World( cv::Point3f * pt )
{
	Compensate_Coordinates_World( &pt->x, &pt->y, &pt->z );
}

bool CStereoVidere::CalculateCoordinates( cv::Rect region, float * pResult_world, float * pResult_vision )
{
	return CalculateCoordinates( region.x, region.y, region.width, region.height, pResult_world, pResult_vision );
}

bool CStereoVidere::CalculateCoordinates( int nX, int nY, int nWidth, int nHeight, float * pResult_world, float * pResult_vision )
{
	cv::Point2i
		ptImg( 0, 0 ),
		ptMask( 0, 0 ),
		ptLimit( nX+nWidth, nY+nHeight );

	cv::Point3d ptCoord_d( 0.0, 0.0, 0.0 );

	cv::Point3f	
		ptVision( 0.0f, 0.0f, 0.0f ),
		ptWorld( 0.0f, 0.0f, 0.0f ),
		ptSumVision( 0.0f, 0.0f, 0.0f ),
		ptSumWorld( 0.0f, 0.0f, 0.0f );

	float 
		count = 0.0f,
		point_v[3] = { 0.0f, 0.0f, 0.0f },
		point_w[3] = { 0.0f, 0.0f, 0.0f };

	const float scale = 1000.0f;

	for( ptImg.x=nX ; ptImg.x<ptLimit.x ; ptImg.x++ )
	{
		for( ptImg.y=nY ; ptImg.y<ptLimit.y ; ptImg.y++ )
		{
			if( Calculate_3D_Point( ptImg, &ptVision ) )
			{
				Compensate_Coordinates_Vision( &ptVision );

				ptVision.x *= scale;
				ptVision.y *= scale;
				ptVision.z *= scale;

				ptSumVision.x += ptVision.x;
				ptSumVision.y += ptVision.y;
				ptSumVision.z += ptVision.z;

				TransformCoordinates( ptVision, &ptWorld );

				Compensate_Coordinates_World( &ptWorld );
			
				ptSumWorld.x += ptWorld.x;
				ptSumWorld.y += ptWorld.y;
				ptSumWorld.z += ptWorld.z;

				count ++;
			}
		}
	}
	
	bool bValid = ( count > 0.0f );

	if( bValid )
	{
		if( pResult_vision != nullptr )
		{
			pResult_vision[0] = ptSumVision.x / count;
			pResult_vision[1] = ptSumVision.y / count;
			pResult_vision[2] = ptSumVision.z / count;
		}

		if( pResult_world != nullptr )
		{
			pResult_world[0] = ptSumWorld.x / count;
			pResult_world[1] = ptSumWorld.y / count;
			pResult_world[2] = ptSumWorld.z / count;
		}
	}

	return bValid;
}

bool CStereoVidere::CalculateCoordinates_Mask( cv::Rect region, cv::Mat mask, float * pResult_world, float * pResult_vision )
{
	return CalculateCoordinates_Mask( region.x, region.y, region.width, region.height, mask, pResult_world, pResult_vision );
}

bool CStereoVidere::CalculateCoordinates_Mask( int nX, int nY, int nWidth, int nHeight, cv::Mat mask, float * pResult_world, float * pResult_vision )
{
	if( mask.empty() || cv::countNonZero( mask ) < 1 )
	{
		// If there is no mask or the mask is empty use regular function
		return CalculateCoordinates( nX, nY, nWidth, nHeight, pResult_world, pResult_vision );
	}

	cv::Point2i
		ptImg( 0, 0 ),
		ptMask( 0, 0 ),
		ptLimit( nX+nWidth, nY+nHeight );

	cv::Point3f	
		ptVision( 0.0f, 0.0f, 0.0f ),
		ptWorld( 0.0f, 0.0f, 0.0f ),
		ptSumVision( 0.0f, 0.0f, 0.0f ),
		ptSumWorld( 0.0f, 0.0f, 0.0f );

	float 
		count = 0.0f,
		point_v[3] = { 0.0f, 0.0f, 0.0f },
		point_w[3] = { 0.0f, 0.0f, 0.0f };

	const float scale = 1000.0f;

	for( ptImg.x=nX, ptMask.x=0 ; ptImg.x<ptLimit.x ; ptImg.x++, ptMask.x++ )
	{
		for( ptImg.y=nY, ptMask.y=0 ; ptImg.y<ptLimit.y ; ptImg.y++, ptMask.y++ )
		{
			if( mask.at<unsigned char>( ptMask.y, ptMask.x ) == 0 ) continue;

			if( !Calculate_3D_Point( ptImg, &ptVision ) ) continue;
				
			if( ( ptVision.x != 0.0f ) &&
				( ptVision.y != 0.0f ) &&
				( ptVision.z != 0.0f ) )
			{
				Compensate_Coordinates_Vision( &ptVision );

				ptVision.x *= scale;
				ptVision.y *= scale;
				ptVision.z *= scale;

				ptSumVision.x += ptVision.x;
				ptSumVision.y += ptVision.y;
				ptSumVision.z += ptVision.z;

				TransformCoordinates( ptVision, &ptWorld );
				
				Compensate_Coordinates_World( &ptWorld );
			
				ptSumWorld.x += ptWorld.x;
				ptSumWorld.y += ptWorld.y;
				ptSumWorld.z += ptWorld.z;

				count ++;
			}
		}
	}
	
	bool bValid = ( count > 0.0f );

	if( bValid )
	{
		if( pResult_vision != nullptr )
		{
			pResult_vision[0] = ptSumVision.x / count;
			pResult_vision[1] = ptSumVision.y / count;
			pResult_vision[2] = ptSumVision.z / count;
		}

		if( pResult_world != nullptr )
		{
			pResult_world[0] = ptSumWorld.x / count;
			pResult_world[1] = ptSumWorld.y / count;
			pResult_world[2] = ptSumWorld.z / count;
		}
	}

	return bValid;
}

bool CStereoVidere::Parse_Parameters_Load( void )
{
	bool bSuccess = true;

	cv::Point3f
		pt_limit_max( 0.0f, 0.0f, 0.0f ),
		pt_limit_min( 0.0f, 0.0f, 0.0f );

	//-----------------------------------------------------------------------
	// Stereo segmentation
	yarp::os::Bottle bot_main = prop_ConfigFile.findGroup( "Spatial_segmentation_limits" );

	yarp::os::Bottle bot = bot_main.findGroup( "X:" );
	if( bot.size() > 0 )
	{
		pt_limit_min.x = static_cast<float>( bot.get( 1 ).asInt() );
		pt_limit_max.x = static_cast<float>( bot.get( 2 ).asInt() );
	}
	else bSuccess = false;

	bot = bot_main.findGroup( "Y:" );
	if( bot.size() > 0 )
	{
		pt_limit_min.y = static_cast<float>( bot.get( 1 ).asInt() );
		pt_limit_max.y = static_cast<float>( bot.get( 2 ).asInt() );
	}
	else bSuccess = false;

	bot = bot_main.findGroup( "Z:" );
	if( bot.size() > 0 )
	{
		pt_limit_min.z = static_cast<float>( bot.get( 1 ).asInt() );
		pt_limit_max.z = static_cast<float>( bot.get( 2 ).asInt() );
	}
	else bSuccess = false;

	ptLimitMin.lock();
	ptLimitMin.x = pt_limit_min.x;
	ptLimitMin.y = pt_limit_min.y;
	ptLimitMin.z = pt_limit_min.z;
	ptLimitMin.unlock();

	ptLimitMax.lock();
	ptLimitMax.x = pt_limit_max.x;
	ptLimitMax.y = pt_limit_max.y;
	ptLimitMax.z = pt_limit_max.z;
	ptLimitMax.unlock();

	return bSuccess;
}
