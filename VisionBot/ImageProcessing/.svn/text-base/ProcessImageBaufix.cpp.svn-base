#include "ProcessImageBaufix.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include "range.hpp"

CProcessImageBaufix::CProcessImageBaufix( int width, int height )
: CProcessImage( width, height )
{
	sSegFile += "AVT/Segmentation_Parameters_(TUM).ini";

	SetupProcessing( width, height );

	nProcessImageID = ProcessImageID::PI_Baufix;
}

CProcessImageBaufix::~CProcessImageBaufix( void )
{
	Clean_TaskSpecific();
}

void CProcessImageBaufix::SetupProcessing_TaskSpecific( int width, int height )
{
	img_gray.create( height, width, CV_8UC1 );
	img_gray.setTo( 0 );

	img_canny.create( height, width, CV_8UC1 );
	img_canny.setTo( 0 );

	img_segmented.Setup( nWidth, nHeight, COLOUR_K );
	img_segmented.uMorph[Seg::OP].set( MORPH_OPERATION_CLOSE );
	img_segmented.uMorph[Seg::ERODE].set( 4 );
	img_segmented.uMorph[Seg::DILATE].set( 5 );
}

void CProcessImageBaufix::Clean_TaskSpecific( void )
{
	if( !img_gray.empty() )
		img_gray.release();

	if( !img_canny.empty() )
		img_canny.release();

	img_segmented.Clean();
}

bool CProcessImageBaufix::SearchAllObjects( void )
{
	bool bFound = false;

	//--------------------------------------------------------------------------
	// Detect the objects
	
	bFound |= Detect_Object_From_Primary_Colour( OBJECT_BOLT_RED, 50, 300 );

	bFound |= Detect_Object_From_Primary_Colour( OBJECT_CUBE_RED, 301, 1000 );
	
	bFound |= Detect_Object_From_Primary_Colour( OBJECT_BOLT_GREEN, 30, 300 );

	bFound |= Detect_Object_From_Primary_Colour( OBJECT_CUBE_GREEN, 301, 1000 );

	bFound |= Detect_Object_From_Primary_Colour( OBJECT_CUBE_BLUE, 50, 500 );
	
	bFound |= Detect_Object_From_Primary_Colour( OBJECT_NUT_ORANGE, 50, 400 );
	
	bFound |= Detect_Object_From_Primary_Colour( OBJECT_BOLT_YELLOW, 50, 300 );

	bFound |= Detect_Object_From_Primary_Colour( OBJECT_CUBE_YELLOW, 301, 1000 );

	bFound |= Detect_Object_From_Primary_Colour( OBJECT_HAND, 300 );

	bFound |= Detect_Object_From_Primary_Colour( OBJECT_SLAT_SHORT, 0, 1200 );

	bFound |= Detect_Object_From_Primary_Colour( OBJECT_SLAT_MEDIUM, 1201, 1600 );
	
	bFound |= Detect_Object_From_Primary_Colour( OBJECT_SLAT_LONG, 1601, 5000 );

	//Define_New_Region_of_Interest( OBJECT_HAND, cvRect( -90, -90, 130, 130 ) );

	//--------------------------------------------------------------------------

	return bFound;
}

void CProcessImageBaufix::ObjectPreProcessing( void )
{
}

void CProcessImageBaufix::ObjectPostProcessing( void )
{
	// Define an image with objects on the robot's workspace

	// Draw a line in the middle of the image
	//cvLine( img_proc, cvPoint( 0, img_proc->height/2 ), cvPoint( img_proc->width, img_proc->height/2 ), CV_RGB( 255, 255, 255 ) );

	// Define an image with objects on the human's workspace
}

// Process the image and retrieve data
void CProcessImageBaufix::ProcessTask( cv::Mat * mat_ptr )
{
	//Convert source image to Grayscale
	cv::cvtColor( *mat_ptr, img_gray, CV_BGR2GRAY );

	//Preprocessing - Contour detection
	cv::Canny( img_gray, img_canny, 255.0, 0.0 );

	img_canny.copyTo( img_segmented.Image );

	img_segmented.doMorphologicalOperations();
		
	// Erode the original segmented image, in an atempt to compensate for
	// the strange colours on the edge of objects
	cv::erode( img_segmented.Image, img_segmented.Image, cv::Mat() );

#if SHOW_CANNY_IMAGE
	cv::imshow( "Canny", img_segmented.Image );
#endif

	img_segmented.DetectSpots_FloodFill();
		
	std::vector< Pixel_Pair >
		limit( 7, Pixel_Pair( Pixel3( 0, 0, 0 ), Pixel3( 0, 0, 0 ) ) );
	
	vSegmentedImage.lock();
		
	size_t nColours = vSegmentedImage.size();
	for( size_t iSeg=0 ; iSeg<nColours ; iSeg++ )
	{
		//vSegmentedImage.at( iSeg ).Image = cv::Mat::zeros( vSegmentedImage.at( iSeg ).Image.size(), vSegmentedImage.at( iSeg ).Image.type() );
		vSegmentedImage.at( iSeg ).Image.setTo( 0 );
		vSegmentedImage.at( iSeg ).vSpots.clear();

		limit.at( iSeg ).first.x = static_cast<unsigned char>( vSegmentedImage.at( iSeg ).uHue[Seg::MIN].get() );
		limit.at( iSeg ).second.x = static_cast<unsigned char>( vSegmentedImage.at( iSeg ).uHue[Seg::MAX].get() );
		limit.at( iSeg ).first.y = static_cast<unsigned char>( vSegmentedImage.at( iSeg ).uSat[Seg::MIN].get() );
		limit.at( iSeg ).second.y = static_cast<unsigned char>( vSegmentedImage.at( iSeg ).uSat[Seg::MAX].get() );
		limit.at( iSeg ).first.z = static_cast<unsigned char>( vSegmentedImage.at( iSeg ).uVal[Seg::MIN].get() );
		limit.at( iSeg ).second.z = static_cast<unsigned char>( vSegmentedImage.at( iSeg ).uVal[Seg::MAX].get() );
	}

	//--------------------------------------------------------------------------
	// Variables declaration

	cv::Mat img_aux;

	int
		nRow = 0,
		nCol = 0,
		nRoI_Row = 0,
		nRoI_Col = 0,
		nRect_Width = 0,
		nRect_Height = 0;
		
	int
		nIndex_Colour = 0,
		nIndex_Gray = 0;

	bool bDetected = false;

	float 
		fSat_Sum = 0.0f, 
		fVal_Sum = 0.0f, 
		fNum_Pixel = 0.0f;

	cv::Rect 
		RoI,
		RoI2;

	//--------------------------------------------------------------------------
	size_t spots_size = img_segmented.vSpots.size();
	for( size_t iSpot=0 ; iSpot<spots_size ; iSpot++ )
	{
		RoI = img_segmented.vSpots.at( iSpot );

		nRect_Width = RoI.width;;
		nRect_Height = RoI.height;

		img_aux.create( nRect_Height, nRect_Width, CV_8UC3 );

		// Convert the region of the image from BGR to HSV, and store it on 'img_aux'
		cv::cvtColor( (*mat_ptr)( RoI ), img_aux, CV_BGR2HSV );
			
		fSat_Sum = 0.0f;
		fVal_Sum = 0.0f;
		fNum_Pixel = 0.0f;

		unsigned char aHue[HIST_LENGTH];

		//initialize the array
		for( unsigned int iHueHist=0 ; iHueHist<HIST_LENGTH ; iHueHist++ )
			aHue[iHueHist] = 0;

		unsigned int nCyanPixelCount = 0;
		bDetected = false;

		Pixel3 pixel( 0, 0, 0 );

		// Go through the Region of Interest
		for( nRow=0 ; nRow<img_aux.rows ; nRow++ )
		{
			for( nCol=0 ; nCol<img_aux.cols ; nCol++ )
			{
				nIndex_Colour = static_cast<int>( *img_aux.step.p )*nRow + nCol*img_aux.channels();

				pixel.x = img_aux.data[nIndex_Colour];
				pixel.y = img_aux.data[nIndex_Colour + 1];
				pixel.z = img_aux.data[nIndex_Colour + 2];

				//Test the background (low saturation and value)
				if( pixel.y<10 && pixel.z<10 ) continue; //Dark enough to be considered background
					
				nRoI_Row = nRow + RoI.y;
				nRoI_Col = nCol + RoI.x;
					
				// Test for cyan pixels to look for the wrist
				if( in_range(pixel, limit.at( COLOUR_C ) ) )
				{

					nIndex_Gray = 
						static_cast<int>( *vSegmentedImage.at( COLOUR_C ).Image.step.p ) * nRoI_Row + 
						nRoI_Col * vSegmentedImage.at( COLOUR_C ).Image.channels();

					vSegmentedImage.at( COLOUR_C ).Image.data[nIndex_Gray] = segmented;

					nCyanPixelCount++;
				}

				nIndex_Gray = 
					static_cast<int>( *img_segmented.Image.step.p ) * nRoI_Row + 
					nRoI_Col * img_segmented.Image.channels();

				if( img_segmented.Image.data[nIndex_Gray] == 0 ) continue;

				aHue[pixel.x]++;

				fSat_Sum += pixel.y;
				fVal_Sum += pixel.z;
				fNum_Pixel ++;

				bDetected = true;
				
			} // for( nCol<img_aux->width )

		} // for( nRow<img_aux->height )
			
		img_aux.release();

		if( !bDetected ) continue;

		// Calculate Hue using mode of pixels to get the most frequent colour
		pixel.x = CalculateMode( aHue );

		// Calculate Saturation and Value using an average of detected pixels
		pixel.y = static_cast<unsigned char>( fSat_Sum/fNum_Pixel );
		pixel.z = static_cast<unsigned char>( fVal_Sum/fNum_Pixel );

	//--------------------------------------------------------------------------
		// Test the colours
		int 
			nIndex = -1,
			iColour = COLOUR_R;

		do
		{
			if( in_range( pixel, limit.at( iColour ) ) )
			{
				if( nIndex == -1 ) nIndex = iColour;
			}
			iColour++;
		} while( nIndex == -1 && iColour <= COLOUR_W );

		//-------------------------------------------------------------------
		if( nIndex >= 0 )
		{
			RoI2 = img_segmented.vSpots.at( iSpot );
				
			img_segmented.Image( RoI2 ).copyTo( vSegmentedImage.at( nIndex ).Image( RoI2 ) );

			vSegmentedImage.at( nIndex ).vSpots.emplace_back( img_segmented.vSpots.at( iSpot ) );
		}

		// if there are more than 20 cyan pixels, look for a hand
		if( nCyanPixelCount > 20 )
		{
			// Detect cyan colour spots
			vSegmentedImage.at( COLOUR_C ).doMorphologicalOperations();
			vSegmentedImage.at( COLOUR_C ).DetectSpots_FloodFill( true );

#if SHOW_DEBUG_IMAGE
			cv::imshow( "Debug", vSegmentedImage.at( COLOUR_C ).Image );
#endif

			cv::Rect rect_max;
				
			int 
				sum_w = 0, 
				sum_h = 0;
			size_t spots_c_size = vSegmentedImage.at( COLOUR_C ).vSpots.size();
			vWrist.assign( spots_c_size, cv::Rect( 0, 0, 0, 0 ) );

			for( size_t iIndex=0 ; iIndex<spots_c_size ; iIndex++ )
			{
				rect_max = MaxRect( vSegmentedImage.at( COLOUR_C ).vSpots.at( iIndex ), img_segmented.vSpots.at( iSpot ) );
				sum_w = vSegmentedImage.at( COLOUR_C ).vSpots.at( iIndex ).width + img_segmented.vSpots.at( iSpot ).width;
				sum_h = vSegmentedImage.at( COLOUR_C ).vSpots.at( iIndex ).height + img_segmented.vSpots.at( iSpot ).height;

				if( rect_max.width <= sum_w && rect_max.height <= sum_h )
				{
					RoI2 = img_segmented.vSpots.at( iSpot );
						
					img_segmented.Image( RoI2 ).copyTo( vSegmentedImage.at( COLOUR_C ).Image( RoI2 ) );

					vWrist.at( iIndex ) = vSegmentedImage.at( COLOUR_C ).vSpots.at( iIndex );

					vSegmentedImage.at( COLOUR_C ).vSpots.at( iIndex ) = RoI2;
				}

			}	// for ( spots_c_size )

		} // if( nCyanPixelCount > 20 )

	} // for( iSpot<spots_size )

	SearchAllObjects();

	Calculate_Pointing_Directions();

	vSegmentedImage.unlock();

	//ObjectPostProcessing();
}

void CProcessImageBaufix::Calculate_Pointing_Directions( void )
{
	// Here I calculate the pointing direction for each detected hand and wrist

	unsigned char uPixel = 0;

	int
		nRow = 0, 
		nRow_first = 0, 
		nRow_last = 0,
		nCol = 0, 
		nCol_first = 0, 
		nCol_last = 0;

	long lIndex_Gray = 0;

	float 
		fPoint = 0.0f, 
		fCoord = 0.0f, 
		fLenght = 0.0f,
		fSum = 0.0f, 
		fNum = 0.0f;

	cv::Point
		pt( 0, 0 ),
		pt_wrist( 0, 0 ),
		pt_finger( 0, 0 );

	size_t spots_size = vSegmentedImage.at( COLOUR_C ).vSpots.size();
	for( size_t iHand=0 ; iHand<spots_size ; iHand++ )
	{
		// Calculate wrist coordinates
		fCoord = static_cast<float>( vWrist.at( iHand ).x );
		fLenght = static_cast<float>( vWrist.at( iHand ).width );
		fPoint = fCoord + fLenght/2.0f;
		pt_wrist.x = static_cast<int>( fPoint );

		fCoord = static_cast<float>( vWrist.at( iHand ).y );
		fLenght = static_cast<float>( vWrist.at( iHand ).height );
		fPoint = fCoord + fLenght/2.0f;
		pt_wrist.y = static_cast<int>( fPoint );

		cv::circle( mat_ImageProcessed.Obj, pt_wrist, 3, CV_RGB( 255, 0, 0 ), 3 );
		
		// Calculate coordinates to go through the image
		nRow_first = vSegmentedImage.at( COLOUR_C ).vSpots.at( iHand ).y;
		nRow_last = nRow_first + vSegmentedImage.at( COLOUR_C ).vSpots.at( iHand ).height;

		nCol_first = vSegmentedImage.at( COLOUR_C ).vSpots[iHand].x;
		nCol_last = nCol_first + vSegmentedImage.at( COLOUR_C ).vSpots.at( iHand ).width;
		
		for( nRow = nRow_last ; nRow>nRow_first ; nRow-- )
		{
			fSum = 0.0f;
			fNum = 0.0f;

			for( nCol = nCol_first ; nCol<nCol_last ; nCol++ )
			{
				lIndex_Gray = static_cast<long>( nRow*vSegmentedImage.at( COLOUR_C ).Image.step + nCol*vSegmentedImage.at( COLOUR_C ).Image.channels() );
				uPixel = static_cast<unsigned char>( vSegmentedImage.at( COLOUR_C ).Image.data[lIndex_Gray] );

				if( uPixel > 0 )
				{
					//fSum = nCol;
					fSum += nCol;
					fNum ++;
				}

			} // for( nCol<nCol_last )

			if( fNum > 0.0f ) break;

		} // for( nRow>nRow_first )

		if( fNum == 0.0f ) continue;

		pt_finger.x = static_cast<int>( fSum/fNum );
		pt_finger.y = nRow;

		float 
			diff_x = static_cast<float>( pt_finger.x - pt_wrist.x ),
			diff_y = static_cast<float>( pt_finger.y - pt_wrist.y );
		
		float fm = diff_y/diff_x;

		float
			fx = static_cast<float>( pt_wrist.x ),
			fy = static_cast<float>( pt_wrist.y );

		float fb = fy - fm*fx;

		pt.y = mat_ImageProcessed.Obj.rows;
		fy = static_cast<float>( pt.y );
		float faux = (fy - fb )/fm;

		pt.x = static_cast<int>( faux );

		cv::line( mat_ImageProcessed.Obj, pt_wrist, pt, CV_RGB( 255, 0, 0 ) );
		
	} // for( iHand<spots_size )
}
