#include "GestureClassifier.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>

#include "commands.h"
#include "common.h"
#include "Files_Directories.h"
#include "ImageOperations.h"
#include "VisionDefinitions.h"

CGestureClassifier::CGestureClassifier( bool detect_ag, bool detect_sg, bool detect_pt )
: bActive( true )
, nWidthGesture( 31 )
, bDetect_AboveGrip( detect_ag )
, bDetect_SideGrip( detect_sg )
, bDetect_Pointing( detect_pt )
{
	// Default segmentation values for hand skin colour
	Hand_hue[Seg::MIN].set( 40 );
	Hand_hue[Seg::MAX].set( 95 );

	Hand_sat[Seg::MIN].set( 25 );
	Hand_sat[Seg::MAX].set( 60 );

	Hand_val[Seg::MIN].set( 25 );
	Hand_val[Seg::MAX].set( 57 );

	Hand_morph[Seg::OP].set( MORPH_OPERATION_OPEN );

	std::vector< std::string >
		vDir,
		vVar;

	vVar.emplace_back( "/Config/Parameters/" );

	std::string file_path = "";

	if( Find_File( "Gesture_Parameters.ini", file_path, vDir, vVar ) )
	{
		Load_parameters_from_file( file_path );
	}
}


CGestureClassifier::~CGestureClassifier( void )
{
	Reset();
}

void CGestureClassifier::Reset( void )
{
	vGestures.lock();
	vGestures.Obj.clear();
	vGestures.unlock();

	PointedObject.lock();
	PointedObject.Obj.clear();
	PointedObject.unlock();
}

bool CGestureClassifier::Parse_Parameters_Load( void )
{
	bool bSuccess = true;

	unsigned int uVal = 0;
	
	int 
		i = 0,
		nSize = 0;
	
	std::string str = "";

	yarp::os::Bottle 
		bot_group,
		bot;

	//--------------------------------------------------------------------------
	// General parameters
	bot_group = prop_ConfigFile.findGroup( "General" );

	str = bot_group.findGroup( "Active" ).get( 1 ).asString();
	bActive.set( str == "yes" );

	nWidthGesture.set( bot_group.findGroup( "Point_or_side_diff" ).get( 1 ).asInt() );
	
	//--------------------------------------------------------------------------
	// Hu moments parameters
	bot_group = prop_ConfigFile.findGroup( "Hu_Moments" );

	// ----- power -----
	bot = bot_group.findGroup( "power" );
	nSize = bot.size() - 1;
	
	bSuccess &= ( nSize > 0 );

	dPower.lock();
	dPower.Obj.assign( nSize, 0.0 );
	
	for( i = 0; i < nSize; i++ )
		dPower.Obj.at( i ) = bot.get( i+1 ).asDouble();
	
	dPower.unlock();

	// ----- Above grip	-----
	bot = bot_group.findGroup( "Grip_Above" );

	nSize = bot.size() - 1;
	
	bSuccess &= ( nSize > 0 );

	vMoments_Above.lock();
	
	vMoments_Above.Obj.assign( nSize, 0.0 );
	
	for( i = 0; i < nSize; i++ )
		vMoments_Above.Obj.at( i ) = bot.get( i + 1 ).asDouble();

	vMoments_Above.unlock();

	// ----- Side grip	-----
	bot = bot_group.findGroup( "Grip_Side" );

	nSize = bot.size() - 1;

	bSuccess &= ( nSize > 0 );

	vMoments_Side.lock();

	vMoments_Side.Obj.assign( nSize, 0.0 );

	for( i = 0; i < nSize; i++ )
		vMoments_Side.Obj.at( i ) = bot.get( i + 1 ).asDouble();

	vMoments_Side.unlock();

	// ----- Pointing	-----
	bot = bot_group.findGroup( "Pointing" );

	nSize = bot.size() - 1;

	bSuccess &= ( nSize > 0 );

	vMoments_Point.lock();

	vMoments_Point.Obj.assign( nSize, 0.0 );

	for( i = 0; i < nSize; i++ )
		vMoments_Point.Obj.at( i ) = bot.get( i + 1 ).asDouble();

	vMoments_Point.unlock();

	//--------------------------------------------------------------------------
	// Colour parameters
	bot_group = prop_ConfigFile.findGroup( "Hand_colour" );

	bot = bot_group.findGroup( "Hue" );
	uVal = static_cast<unsigned int>( bot.get( 1 ).asInt() );
	Hand_hue[Seg::MIN].set( uVal );
	uVal = static_cast<unsigned int>( bot.get( 2 ).asInt() );
	Hand_hue[Seg::MAX].set( uVal );

	bot = bot_group.findGroup( "Saturation" );
	uVal = static_cast<unsigned int>( bot.get( 1 ).asInt() );
	Hand_sat[Seg::MIN].set( uVal );
	uVal = static_cast<unsigned int>( bot.get( 2 ).asInt() );
	Hand_sat[Seg::MAX].set( uVal );

	bot = bot_group.findGroup( "Value" );
	uVal = static_cast<unsigned int>( bot.get( 1 ).asInt() );
	Hand_val[Seg::MIN].set( uVal );
	uVal = static_cast<unsigned int>( bot.get( 2 ).asInt() );
	Hand_val[Seg::MAX].set( uVal );

	bot = bot_group.findGroup( "Morph" );

	str = bot.get( 1 ).asString();

	if( str == "open" ) Hand_morph[Seg::OP].set( MORPH_OPERATION_OPEN );
	else if( str == "close" ) Hand_morph[Seg::OP].set( MORPH_OPERATION_CLOSE );
	else Hand_morph[Seg::OP].set( 0 );

	Hand_morph[Seg::ERODE].set( bot.get( 2 ).asInt() );
	Hand_morph[Seg::DILATE].set( bot.get( 3 ).asInt() );

	return bSuccess;
}

void CGestureClassifier::Parse_Parameters_Save( void )
{
	std::stringstream stream_params;

	// File start
	stream_params << "# Gesture classification parameters" << std::endl << std::endl;

	//--------------------------------------------------------------------------
	// General parameters
	stream_params <<
		"# General parameters" << std::endl <<
		"# Active: whether to use or not the gesture detection." << std::endl <<
		"# Point_or_side_diff: Width difference to distinguish the gestures pointing and side grip, bellow this value the gesture is considered pointing." << std::endl <<
		"[General]" << std::endl <<
		"Active " << ( bActive.get() ? "yes" : "no" ) << std::endl <<
		"Point_or_side_diff " << nWidthGesture.get() << std::endl <<
		std::endl;
	
	//--------------------------------------------------------------------------
	// Hu moments parameters
	stream_params <<
		"# Hu moments parameters" << std::endl <<
		"# power: power of multiplier for Hu parameters for each moment," << std::endl <<
		"# each parameter will be multiplied by 10^power" << std::endl <<
		"# (first pair)^(first power), (second pair)^(second power), ..." << std::endl <<
		"# each gesture has a sequence of (min,max) values for each moment used" << std::endl <<
		"# ex: m1_min m1_max m2_min m2_max ..." << std::endl <<
		"[Hu_Moments]" << std::endl;

	size_t 
		i = 0, 
		vSize = 0;

	stream_params << "power ";

	dPower.lock();
	vSize = dPower.Obj.size();

	for( i = 0; i < vSize; i++ )
		stream_params << dPower.Obj.at( i ) << " ";

	dPower.unlock();

	stream_params << std::endl << "Grip_Above ";

	vMoments_Above.lock();
	vSize = vMoments_Above.Obj.size();

	for( i = 0; i < vSize; i++ )
		stream_params << vMoments_Above.Obj.at( i ) << " ";

	vMoments_Above.unlock();

	stream_params << std::endl << "Grip_Side ";
	vMoments_Side.lock();
	vSize = vMoments_Side.Obj.size();

	for( i = 0; i < vSize; i++ )
		stream_params << vMoments_Side.Obj.at( i ) << " ";

	vMoments_Side.unlock();

	stream_params << std::endl << "Pointing ";
	vMoments_Point.lock();
	vSize = vMoments_Point.Obj.size();

	for( i = 0; i < vSize; i++ )
		stream_params << vMoments_Point.Obj.at( i ) << " ";

	vMoments_Point.unlock();

	stream_params << std::endl << std::endl;

	//--------------------------------------------------------------------------
	// Skin colour parameters
	stream_params << 
		"# Parameters that control hand detection using skin colour" << std::endl <<
		"[Hand_colour]" << std::endl <<
		"Hue " <<
		Hand_hue[Seg::MIN].get() << " " <<
		Hand_hue[Seg::MAX].get() << std::endl <<
		"Saturation " <<
		Hand_sat[Seg::MIN].get() << " " <<
		Hand_sat[Seg::MAX].get() << std::endl <<
		"Value " <<
		Hand_val[Seg::MIN].get() << " " <<
		Hand_val[Seg::MAX].get() << std::endl <<
		"Morph " <<
		( ( Hand_morph[Seg::OP].get() == MORPH_OPERATION_CLOSE ) ? "close " : "open " ) <<
		Hand_morph[Seg::ERODE].get() << " " <<
		Hand_morph[Seg::DILATE].get() << std::endl;

	//--------------------------------------------------------------------------
	// Store in buffer to write to file
	strFileBuffer += stream_params.str();
}

void CGestureClassifier::mark_detected_gesture( unsigned int gesture, cv::Rect RoI_hand, cv::Mat &img_Original )
{
	const int OPENCV_FONT =
		#if( ( CV_VERSION_MAJOR >= 3 ) && !defined( CV_VERSION_EPOCH ) )
			cv::FONT_HERSHEY_SIMPLEX
		#else
			CV_FONT_HERSHEY_SIMPLEX
		#endif
		;

	cv::Point ptText( RoI_hand.x, RoI_hand.y - 15 );

	if( ptText.x < 0 ) ptText.x = 0;
	if( ptText.y < 0 ) ptText.y = 0;

	std::string strGesture = "";

	switch( gesture )
	{
	//case NONE:break;
		
	case Gesture::ABOVE_GRIP:
		strGesture = "Above grip";
		//cv::putText( img_Original, "Above grip", ptText, OPENCV_FONT, 0.6, cv::Scalar::all( 255.0 ) );
		break;
	
	case Gesture::SIDE_GRIP:
		strGesture = "Side grip";
		//cv::putText( img_Original, "Side grip", ptText, OPENCV_FONT, 0.6, cv::Scalar::all( 255.0 ) );
		break;

	case Gesture::POINTING:
		strGesture = "Pointing";
		//cv::putText( img_Original, "Pointing", ptText, OPENCV_FONT, 0.6, cv::Scalar::all( 255.0 ) );
		break;

	case Gesture::HAND_OUT:
		strGesture = "Hand out";
		//cv::putText( img_Original, "Hand Out", ptText, OPENCV_FONT, 0.6, cv::Scalar::all( 255.0 ) );
		break;

	default:
		break;
	}

	if( strGesture.size() > 0 )
		cv::putText( img_Original, strGesture.c_str(), ptText, OPENCV_FONT, 0.6, cv::Scalar::all( 255.0 ) );
}

unsigned int CGestureClassifier::detect( cv::Rect RoI_hand, cv::Mat &mask, cv::Mat &img_Original, cv::Mat &img_HSV_hand )
{
	unsigned int uDetected_Gesture = Gesture::NONE;

	if( !bActive.get() ) return uDetected_Gesture;

	//--------------------------------------------------------------------------
	// Hand detection using skin colour segmentation

	if( img_HSV_hand.empty() )
		cv::cvtColor( img_Original( RoI_hand ), img_HSV_hand, 
		#if( ( CV_VERSION_MAJOR >= 3 ) && !defined( CV_VERSION_EPOCH ) )
			cv::COLOR_BGR2HSV 
		#else
			CV_BGR2HSV 
		#endif
		);

	const double
		dConv_Hue = 0.5,		// 180/360
		dConv_Sat_Val = 2.55;	// 255/100;

	// Define lower and upper bounds for segmentation
	cv::Scalar
		hsv_min( 
			static_cast<double>( Hand_hue[Seg::MIN].get() ) * dConv_Hue,
			static_cast<double>( Hand_sat[Seg::MIN].get() ) * dConv_Sat_Val,
			static_cast<double>( Hand_val[Seg::MIN].get() ) * dConv_Sat_Val ),
		hsv_max(
			static_cast<double>( Hand_hue[Seg::MAX].get() ) * dConv_Hue,
			static_cast<double>( Hand_sat[Seg::MAX].get() ) * dConv_Sat_Val,
			static_cast<double>( Hand_val[Seg::MAX].get() ) * dConv_Sat_Val );

	cv::Mat	hsv_mask( img_HSV_hand.size(), CV_8UC1, cv::Scalar::all( 0.0 ) );

	cv::inRange( img_HSV_hand, hsv_min, hsv_max, hsv_mask );

	//--------------------------------------------------------------------------
	// Perform morphological operations in the segmented hand

	morphology( hsv_mask, hsv_mask, Hand_morph[Seg::ERODE].get(), Hand_morph[Seg::DILATE].get(), Hand_morph[Seg::OP].get() );

	//--------------------------------------------------------------------------

	std::vector< std::vector< cv::Point > > contours_hsv_mask;
	
	cv::findContours( hsv_mask, contours_hsv_mask, cv::noArray(), 
	#if( ( CV_VERSION_MAJOR >= 3 ) && !defined( CV_VERSION_EPOCH ) )
		cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE
	#else		
		CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE
	#endif
	);

	hsv_mask.release();

	int 
		max_c2 = 0,
		max_c3 = 0;

	if( contours_hsv_mask.size()<1 ) return uDetected_Gesture;

	double 
		area = 0.0,
		areamax = 0.0;

	size_t vSize = contours_hsv_mask.size();
	for( size_t i = 0; i<vSize; i++ )
	{
		area = cv::contourArea( contours_hsv_mask.at( i ) );

		if( area>areamax )
		{
			areamax = area;
			max_c2 = i;
		}
	}

	const cv::Scalar Hand_Colour_Mask( 0.0, 0.0, 250.0 );

	//"pinta" de vermelho a mão
	cv::drawContours( img_Original( RoI_hand ), contours_hsv_mask, max_c2, Hand_Colour_Mask, 
	#if( ( CV_VERSION_MAJOR >= 3 ) && !defined( CV_VERSION_EPOCH ) )
		cv::FILLED	
	#else
		CV_FILLED
	#endif
	);
	
	cv::Mat image_roi_red( RoI_hand.size(), CV_8UC1 );
		
	//cria máscara correspondente ao vermelho atribuido à mão
	cv::inRange( img_Original( RoI_hand ), Hand_Colour_Mask, Hand_Colour_Mask, image_roi_red );

	cv::Mat bin_red( img_Original.size(), CV_8UC1 );
	bin_red.setTo( 0 );

	//cria máscara correspondente a cor atribuida para a região da mão (desta vez na imagem original)
	cv::inRange( img_Original( RoI_hand ), Hand_Colour_Mask, Hand_Colour_Mask, bin_red( RoI_hand ) );

	cv::dilate( bin_red, bin_red, cv::Mat(), cv::Point( -1, -1 ), 1 );

	std::vector< std::vector< cv::Point > > contoursConvex;

	cv::findContours( bin_red( RoI_hand ), contoursConvex, cv::noArray(),
	#if( ( CV_VERSION_MAJOR >= 3 ) && !defined( CV_VERSION_EPOCH ) )
		cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE
	#else		
		CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE
	#endif
		);

	bin_red.release();

	//////////////////////HULL CONVEXITY DEFECTS////////////////////////////
	std::vector< std::vector< cv::Point > > hull( contoursConvex.size() );

	areamax = 0;
	vSize = contoursConvex.size();
	for( size_t i = 0; i < vSize; i++ )
	{
		cv::convexHull( cv::Mat( contoursConvex.at( i ) ), hull.at( i ), false );

		area = cv::contourArea( contoursConvex.at( i ) );

		if( area>areamax && area<=RoI_hand.area() )
		{
			areamax = area;
			max_c3 = i;
		}
	}

	////////////////////////////////////////////////////////////////////////
	//complemento aos momentos: agarrar de lado "confunde-se" com apontar //
	//sendo assim usa-se a largura da convex hull para distinguir entre   // 
	//apontar e os outros 2 gestos...                                     //
	//os outros 2 gestos distinguem-se pelos momentos                     //
	////////////////////////////////////////////////////////////////////////

	cv::RotatedRect rotRec;
		
	if( hull.size()>0 )
		rotRec = cv::minAreaRect( cv::Mat( hull.at( max_c3 ) ) );

	cv::Point2f rect_points[4];
	rotRec.points( rect_points );

	float 
		cart_distance = 0.0f,
		dif_x = 0.0f,
		dif_y = 0.0f,
		min_distance = 1000.0f;

	for( int j = 0; j < 4; j++ )
	{
		dif_x = rect_points[( j + 1 ) % 4].x - rect_points[j].x;
		dif_y = rect_points[( j + 1 ) % 4].y - rect_points[j].y;

		cart_distance = sqrt( pow( dif_x, 2.0f ) + pow( dif_y, 2.0f ) );

		if( cart_distance<min_distance )
		{
			min_distance = cart_distance;
		}

	}

	bool aponta = ( static_cast<int>( min_distance ) < nWidthGesture.get() );

	////////////////////////////////////////////////////////////////////////

	double array_moms[7] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

	//calcula os momentos da mão
	cv::Moments gestureMoments = cv::moments( image_roi_red );
	image_roi_red.release();
		
	cv::HuMoments( gestureMoments, array_moms );
		
	double 
		dPower_M1 = -3.0,
		dPower_M2 = -3.0;

	dPower.lock();
	if( dPower.Obj.size() > 1 )
	{
		dPower_M1 = pow( 10.0, dPower.Obj.at( 0 ) );
		dPower_M2 = pow( 10.0, dPower.Obj.at( 1 ) );
	}
	dPower.unlock();

	//--------------------------------------------------------------------------
	// Search for side grip

	vMoments_Side.lock();

	double
		dM1_min = vMoments_Side.Obj.at( 0 )*dPower_M1,
		dM1_max = vMoments_Side.Obj.at( 1 )*dPower_M1,
		dM2_min = vMoments_Side.Obj.at( 2 )*dPower_M2,
		dM2_max = vMoments_Side.Obj.at( 3 )*dPower_M2;

	vMoments_Side.unlock();

	if( ( array_moms[0]>dM1_min && array_moms[0]<dM1_max ) &&
		( array_moms[1]>dM2_min && array_moms[1]<dM2_max ) &&
		!aponta )
	{
		uDetected_Gesture = Gesture::SIDE_GRIP;
	}
		
	//--------------------------------------------------------------------------
	// Search for above grip
	if (bDetect_AboveGrip.get())
	{
		vMoments_Above.lock();
		dM1_min = vMoments_Above.Obj.at(0)*dPower_M1;
		dM1_max = vMoments_Above.Obj.at(1)*dPower_M1;
		dM2_min = vMoments_Above.Obj.at(2)*dPower_M2;
		dM2_max = vMoments_Above.Obj.at(3)*dPower_M2;
		vMoments_Above.unlock();

		if ((array_moms[0] > dM1_min && array_moms[0]<dM1_max) &&
			(array_moms[1]>dM2_min && array_moms[1] < dM2_max) &&
			!aponta)
		{
			uDetected_Gesture = Gesture::ABOVE_GRIP;
		}
	}
	//--------------------------------------------------------------------------
	// Search for pointing

	vMoments_Point.lock();
	dM1_min = vMoments_Point.Obj.at( 0 )*dPower_M1;
	dM1_max = vMoments_Point.Obj.at( 1 )*dPower_M1;
	dM2_min = vMoments_Point.Obj.at( 2 )*dPower_M2;
	dM2_max = vMoments_Point.Obj.at( 3 )*dPower_M2;
	vMoments_Point.unlock();

	if( (array_moms[0]>dM1_min && array_moms[0]<dM1_max) &&
		(array_moms[1]>dM2_min && array_moms[1]<dM2_max) &&
		aponta && bDetect_Pointing.get() )
	{
		uDetected_Gesture = Gesture::POINTING;

		////////////////////////////////////////////////////////////////////

		cv::Point
			point_high( 0, 0 ),
			point_low(
				RoI_hand.x + static_cast<int>( RoI_hand.width / 2 ),//USADO BOUND_RECT (DA PULSEIRA)
				RoI_hand.y );

		////////verificar qual é o maior y -> sítio para onde aponta
		vSize = contoursConvex.at( max_c3 ).size();
		for( size_t i = 0; i<vSize; i++ )
		{
			if( contoursConvex.at( max_c3 ).at( i ).y > point_high.y )
			{
				point_high = contoursConvex.at( max_c3 ).at( i );
			}
		}

		// Transform the points from the region of the hand to the full image
		point_high.x += RoI_hand.x;
		point_high.y += RoI_hand.y;

		//////////////////////////AUMENTA LINHA ////////////////////////////
			
		cv::Point
			// Difference in X and Y
			ptDiff( point_high.x - point_low.x, point_high.y - point_low.y ),
			// Extension point, y coordinate is the edge of the window
			point_ext( point_high.x, img_Original.size().height );

		float
			m = 0.0f,
			b = 0.0f,
			m2_p1 = 0.0f;

		if( ptDiff.x != 0 )
		{
			cv::Point2f 
				ptDiff_f( static_cast<float>( ptDiff.x ), static_cast<float>( ptDiff.y ) ),
				ptHigh_f( static_cast<float>( point_high.x ), static_cast<float>( point_high.y ) );

			float dy = static_cast<float>( point_ext.y );

			m = ptDiff_f.y / ptDiff_f.x;
			b = ptHigh_f.y - m*ptHigh_f.x;
			m2_p1 = pow( m, 2.0f ) + 1.0f;

			point_ext.x = static_cast<int>( ( dy - b ) / m );
		}

		// Lines that show the pointing
			
		// From wrist to finger. Commented, only enable for debug purposes.
		//cv::line( img_Original, point_low, point_high, cv::Scalar( 255.0, 255.0, 0.0 ), 2 );
	
		// From wrist to the edge of the image
		cv::line( img_Original, point_low, point_ext, cv::Scalar( 255.0, 255.0, 0.0 ), 2 );

		////////////////////////////////////////////////////////////////////////
		// OPERAÇÕES DE CÁLCULO DA DISTÂNCIA AO CENTRO DO OBJECTO
		// MENOR DISTÂNCIA "GANHA", ISTO É, TRATA-SE DO OBJECTO PRETENDIDO
		const int nMinorDistance_Threshold = 1000;

		int 
			minorDistance = nMinorDistance_Threshold,
			distance = 0;

		cv::Point2i pt_obj( 0, 0 );

		vObjects.lock();

		PointedObject.lock();

		vSize = vObjects.Obj.size();
		for( size_t i = 0; i<vSize; i++ )
		{
			pt_obj = vObjects.Obj.at( i ).Coord_Image;

			//verificar se os centros se encontram FORA do ROI: para não considerar a pulseira como objecto	
			if( !( RoI_hand.contains( pt_obj ) ) )
			{
				cv::Point2f ptObj_f( static_cast<float>( pt_obj.x ), static_cast<float>( pt_obj.y ) );

				// Test difference in X to assess the slope of the line
				if( ptDiff.x != 0 )
				{
					float modulo = abs( -m*ptObj_f.x + ptObj_f.y - b );
					distance = static_cast<int>( modulo / sqrt( m2_p1 ) );
				}
				else
				{
					// If the slope of the line is infinite, compute distance as the difference in X axis
					distance = abs( pt_obj.x - ptDiff.x );
				}

				if( distance<minorDistance )
				{
					// If this is the first time, push a new pointed object
					if( minorDistance == nMinorDistance_Threshold )
						PointedObject.Obj.emplace_back( CObjectBasic() );

					minorDistance = distance;
						
					PointedObject.Obj.back() = vObjects.Obj.at( i );
				}
			}
		}
			
		vObjects.unlock();

		// Paint a mark in the pointed object
		if( PointedObject.Obj.size() > 0 )
		{	
			cv::circle( img_Original, PointedObject.Obj.back().Coord_Image, 3, cv::Scalar::all( 255.0 ), -1 );
			cv::circle( img_Original, PointedObject.Obj.back().Coord_Image, 10, cv::Scalar::all( 255.0 ), 1 );
		}

		PointedObject.unlock();
	}

	// Add the detected gesture to the vector containing all the detected gestures
	vGestures.lock();
	vGestures.Obj.emplace_back( uDetected_Gesture );
	vGestures.unlock();

	//--------------------------------------------------------------------------
	return uDetected_Gesture;
}
