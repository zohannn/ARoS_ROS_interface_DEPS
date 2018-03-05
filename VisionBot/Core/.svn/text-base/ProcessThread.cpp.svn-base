//---------------------------------------------------------------------------
#include "ProcessThread.h"

//---------------------------------------------------------------------------
// Grabbers
#include "AllGrabbers.h"

//---------------------------------------------------------------------------
// Image processing classes
#include "AllProcessImage.h"
#include "FaceDetection.h"

//---------------------------------------------------------------------------
// Utility includes
#include "EnvironmentVars.h"
#include "Files_Directories.h"
#include "TimeCountOpenCV.h"

//---------------------------------------------------------------------------
// Handle events from OpenCV
#include "cvEventHandlers.h"

//---------------------------------------------------------------------------
// Stereo headers
#include "StereoProcessARoS.h"
#include "StereoProcessDumbo.h"

//---------------------------------------------------------------------------

CProcessThread::CProcessThread( int task )
: fFrameRate( 0.0f )
, nGrabberType( GrabberType::SVS_Video )
, grabberObject( nullptr )
#ifdef USE_StereoVidere
, pStereoProcess( nullptr )
#endif
, imgStream_orig( "/aros/vision/grabber/orig", 640, 480 )
, imgStream_proc( "/aros/vision/grabber/proc", 640, 480 )
, bStream_Image_Original( false )
, bStream_Image_Processed( false )
, bUse_Stereo( true )
{
	Instantiate_Image_Processing( task );
	
	image_procObject->CreateWindows();
}

CProcessThread::~CProcessThread( void )
{
	if( grabberObject != nullptr )
	{
		delete grabberObject;
		grabberObject = nullptr;
	}

	if( image_procObject != nullptr )
	{
		delete image_procObject;
		image_procObject = nullptr;
	}

	Clean_Stereo();
}

void CProcessThread::Instantiate_Image_Processing( int task )
{
	switch( task )
	{
	case Task::TEST:
		image_procObject = new CProcessImageTest();
		break;

	case Task::CONSTRUCTION_TOY_STEREO:
		image_procObject = new CProcessImageToyRobot_SVS();
		break;

	case Task::TUM_INTEGRATION:
		image_procObject = new CProcessImageBaufix();
		break;

	case Task::DRINKING:
		image_procObject = new CProcessImageDrinking();
		break;

	default:
	case Task::CONSTRUCTION_TOY_FULL_SVS:
		image_procObject = new CProcessImageToyRobotFull_SVS();
		break;

	case Task::CONSTRUCTION_TOY_FULL_AVT:
		image_procObject = new CProcessImageToyRobotFull_AVT();
		break;

	case Task::COLOUR_LEARNING:
		image_procObject = new CProcessImageColourLearning();
		break;

	case Task::FACE_TRACKING:
		image_procObject = new CProcessImageFaceTracking();
		break;

	case Task::MOBILE_ROBOT_TARGET_ACQUISITION:
		image_procObject = new CProcessImageMobileRobotTarget1();
		break;

	case Task::TOY_ROBOT_LEARNING:
		image_procObject = new CProcessImageToyRobotLearning();
		break;

	case Task::PLAY_PIANO:
		image_procObject = new CProcessImagePlayPiano();
		break;

	case Task::HR_TRANSPORT:
		image_procObject = new CProcessImageHRTransport();
		break;

	case Task::DIFF_KINEMATICS:
		image_procObject = new CProcessImageDiffKin();
		break;

	case Task::COMPUTER_ASSEMBLY:
		image_procObject = new CProcessImageComputerAssembly();
		break;

	}
}

void CProcessThread::Instantiate_Stereo()
{
	std::string strRobotName = image_procObject->pDataSource->get_RobotName();

	std::transform( strRobotName.begin(), strRobotName.end(), strRobotName.begin(), ::tolower );

#ifdef USE_StereoVidere
	bool isARoS = ( strRobotName.compare( "aros" ) == 0 );

	if( isARoS )
	{
		pStereoProcess = new CStereoProcessARoS();
	}
	else
	{
		pStereoProcess = new CStereoProcessDumbo();
	}
	
	if( pStereoProcess == nullptr ) return;

	pStereoProcess->set_DataSource( image_procObject->pDataSource );

	if( image_procObject->BodyMovementQuantifier.isReady() )
	{
		std::vector< std::string > 
			vDirs,
			vDirVar;

		vDirVar.emplace_back( "Config/Parameters" );

		std::string file_path;
		
		if( Find_File( "Human_Info.ini", file_path, vDirs, vDirVar ) )
			pStereoProcess->objStereo->Load_parameters_from_file( file_path );
	}
#endif
}

void CProcessThread::Clean_Stereo( void )
{
#ifdef USE_StereoVidere
	if( pStereoProcess != nullptr )
	{
		delete pStereoProcess;
		pStereoProcess = nullptr;
	}
#endif
}

// -----------------------------------------------
// yarp::os::thread -> function implementation

bool CProcessThread::threadInit( void )
{
	if( !grabberObject->Open() )
	{
		grabberObject->Close();
		return false;
	}

	//--------------------------------------------------------------------------
	// Configure Video parameters according to task
	int grabber_type = get_GrabberType();

	std::string file = "";

	if( ( grabber_type == GrabberType::SVS_Video ) || ( grabber_type == GrabberType::SVS_Image_File ) )
	{
		switch( image_procObject->pDataSource->get_Task() )
		{
		case Task::COLOUR_LEARNING:
			file = "Video_Parameters_Videre_STH-DCS-VAR-C_(Colour_Learning).ini";
			break;

		case Task::FACE_TRACKING:
			file = "Video_Parameters_Videre_STH-DCS-VAR-C_(Face_Tracking).ini";
			break;
	
		case Task::PLAY_PIANO:
			//file = "Video_Parameters_Videre_STOC-6cm-colour.ini";
			file = "Video_Parameters_Videre_STH-DCS-VAR-C_(Piano).ini";
			break;

		case Task::HR_TRANSPORT:
			file = "Video_Parameters_Videre_STOC-6cm-colour_Dumbo.ini";
			break;

		case Task::COMPUTER_ASSEMBLY:
			file = "Video_Parameters_Videre_STH-DCS-VAR-C_(ComputerAssembling).ini";
			break;

		default:
			file = "Video_Parameters_Videre_STH-DCS-VAR-C_(General).ini";
			break;
		}

		//-------------------------------------------------------------------
		std::vector< std::string > 
			vFileVideoDirs,
			vDirVar;
	
		vFileVideoDirs.emplace_back( get_Env_Var_ANSI( "VISIONBOT_DIR" ) );
		vFileVideoDirs.emplace_back( IMAGE_IO_DIR );
		vFileVideoDirs.emplace_back( "ImageIO" );

		vDirVar.emplace_back( "/Config/Video/" );
		vDirVar.emplace_back( "/config/Videre/Video/" );

		std::string sFilePathVideoParam = "";

		if( Find_File( file, sFilePathVideoParam, vFileVideoDirs, vDirVar ) )
		{
			grabberObject->Load_parameters_from_file( sFilePathVideoParam );
		}
	}

	//--------------------------------------------------------------------------

	int 
		width = grabberObject->get_Width(),
		height = grabberObject->get_Height();

	image_procObject->SetupProcessing( width, height );

	Instantiate_Stereo();

	imgStream_orig.set_Resolution( width, height );
	
	if( bStream_Image_Original.get() && !imgStream_orig.isRunning() )
		imgStream_orig.start();

	imgStream_proc.set_Resolution( width, height );
	
	if( bStream_Image_Processed.get() && !imgStream_proc.isRunning() )
		imgStream_proc.start();

	return true;
}

void CProcessThread::threadRelease( void )
{
	imgStream_orig.stop();
	imgStream_proc.stop();

	Clean_Stereo();

	grabberObject->Close();
}

void CProcessThread::run( void )
{
	cv::Size size( grabberObject->get_Width(), grabberObject->get_Height() );

	CAttachMutex_o< cv::Mat > the_Image;

	the_Image.lock();

	the_Image.Obj.create( size, CV_8UC3 );
	the_Image.Obj.setTo( 0 );

	the_Image.unlock();

	cv::Mat the_Image_proc( size, CV_8UC3 );
	the_Image_proc.setTo( 0 );
	
	CTimeCountOpenCV t_count;

	//--------------------------------------------------------------------------
	// Setup mouse callback
#ifdef USE_OPENCV_QT
	cv::setMouseCallback( image_procObject->get_MainWindowName(), onMouse, &image_procObject->ptMousePos );
#endif

	//---------------- Do acquisition, processing and display ----------------
	do
	{
		// get the next frame
		t_count.set_Time_Finish();

		fFrameRate.set( 1.0f/static_cast<float>( t_count.HowLong() ) );
		
		t_count.set_Time_Start();
	
		// Capture a frame from the device
		grabberObject->get_Frame();

		the_Image.lock();

		// Get the captured frame to a cv::Mat
		grabberObject->get_Image( &the_Image.Obj );

		image_procObject->set_New_Image_for_Processing( &the_Image.Obj );

		// Release the image
		the_Image.unlock();
	
		if( image_procObject->useImageProcessing() )
		{
			// Lock the objects
			image_procObject->pDataSource->vObjects.lock();

			// Process the frame
			image_procObject->Process();

			image_procObject->ObjectPreProcessing();

			// If the grabber is capable of stereo, do the stereo computation
		#ifdef USE_StereoVidere
			if( grabberObject->has_Stereo() && bUse_Stereo.get() )
			{
				// No one else touches the stereo frame
				( (CGrabberSVS*) grabberObject )->stereo_image.lock();

				// Set the pointer directly in the stereo object, no accessors because pointer context doesn't allows
				pStereoProcess->objStereo->pStereoImage = &( (CGrabberSVS*) grabberObject )->stereo_image;

				// Define pan and tilt angles to the matrix transformations
				pStereoProcess->objStereo->Pan.set( image_procObject->pDataSource->PanTilt.fPan_Angle.get() );
				pStereoProcess->objStereo->Tilt.set( image_procObject->pDataSource->PanTilt.fTilt_Angle.get() );

				pStereoProcess->Perform_Stereo_Process();
				
				// Release the stereo frame
				( (CGrabberSVS*) grabberObject )->stereo_image.unlock();

				if( image_procObject->BodyMovementQuantifier.isReady() )
				{
					pStereoProcess->objStereo->Create_3D_Segmentation_Image_Coord_Limits( &image_procObject->vSegmentedImage.at( COLOUR_K ).Image );
				}
			}
		#endif
			
			// After stereo calculation check if HandTracker is being used, and process data if needed
			image_procObject->Update_Coordinates_on_Tracked_Hands();
			
			// Apply image processing after stereo calculations
			image_procObject->Post_Process();

			// Process movement if enabled
			image_procObject->Process_Movement();

			// Apply any object post processing
			image_procObject->ObjectPostProcessing();

			// Release the objects
			image_procObject->pDataSource->vObjects.unlock();

			// Refresh information available to communication
			image_procObject->pDataSource->Refresh_Information();

			// Mark the objects in the processed image
			image_procObject->MarkFoundObjects( true );
		}

		// Display the processed image, if selected for displaying in default window
		image_procObject->Display_Processed_Image();

		//----------------------------------------------------------------------------
		// Image streaming

		// Send the original captured image to the network
		if( imgStream_orig.isRunning() && bStream_Image_Original.get() )
		{
			the_Image.lock();
			imgStream_orig.copyImage( &the_Image.Obj );
			the_Image.unlock();
		}

		// Send the processed image to the network
		if( imgStream_proc.isRunning() && bStream_Image_Processed.get() )
		{
			image_procObject->get_ProcessedImage( &the_Image_proc );
			imgStream_proc.copyImage( &the_Image_proc );
		}
		
		//----------------------------------------------------------------------------
		// Frame saving to the disk, keep this frame?
		if( image_procObject->get_FrameSavingStatus() )
		{
			image_procObject->SaveFrame(
				"png",
				true,
				1.0f / fFrameRate.get(),
				true,
				image_procObject->pDataSource->PanTilt.fPan_Angle.get(),
				image_procObject->pDataSource->PanTilt.fTilt_Angle.get() );
		}

	}while( !isStopping() );

	the_Image.Obj.release();
	the_Image_proc.release();
}

int CProcessThread::get_GrabberType( void )
{
	return nGrabberType.get();
}

void CProcessThread::ConfigureCurrentGrabber( void )
{
	set_GrabberType( nGrabberType.get() );
}

void CProcessThread::set_GrabberType( int type, std::string image_file_name )
{
	if( isRunning() ) return;

	sFileImage = image_file_name;

	if( grabberObject != nullptr )
	{
		delete grabberObject;
		grabberObject = nullptr;
	}

	const int
		default_width = 640,
		default_height = 480;

	const float default_framerate = 30.0f;

	switch( type )
	{
	case GrabberType::SVS_Video:
		grabberObject = new CGrabberSVSVideo();
		grabberObject->set_GrabberMode( default_width, default_height, default_framerate );
		break;

	case GrabberType::SVS_Image_File:
		grabberObject = new CGrabberSVSFileImage( sFileImage );
		break;

	case GrabberType::Common_Image_File:
		grabberObject = new CGrabberFileImage( sFileImage );
		break;

	case GrabberType::OpenCV_Camera:
		grabberObject = new CGrabberOpenCVCamera();
		grabberObject->set_GrabberMode( default_width, default_height, default_framerate );
		break;

	case GrabberType::YARP_Port:
		grabberObject = new CGrabberYARPPort();
		break;

	case GrabberType::CMU1394:
		grabberObject = new CGrabberCMU1394();
		break;

	case GrabberType::PS3Eye:
		grabberObject = new CGrabberPS3Eye();
		grabberObject->set_GrabberMode( default_width, default_height, 75.0f );
		break;

	case GrabberType::OpenCV_Video_File:
		grabberObject = new CGrabberOpenCVFileVideo( sFileImage );
		break;
	}

	if( grabberObject != nullptr ) nGrabberType.set( type );
}
