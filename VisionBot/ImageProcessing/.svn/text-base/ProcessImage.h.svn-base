#ifndef _PROCESS_IMAGE_H_
#define _PROCESS_IMAGE_H_

#pragma once

#include <vector>

#include "Data.h"
#include "GestureClassifier.h"
#include "MovementQuantifier.h"
#include "ObjectTracker.h"
#include "SegmentedImage.h"

#define OBJECT_AREA_MIN 0
#define OBJECT_AREA_MAX 307200 //640*480

#ifndef SHOW_CANNY_IMAGE
	#define SHOW_CANNY_IMAGE 0
#endif

#ifndef SHOW_DEBUG_IMAGE
	#define SHOW_DEBUG_IMAGE 0
	#define DEBUG_WINDOW_NAME "Debug"
#endif

namespace ProcessImageID
{
	enum _PROCCESS_IMAGE_ID_
	{
		PI_BASE = -1,
		PI_Test,
		PI_ToyRobot_SVS,
		PI_Drinking,
		PI_Baufix,
		PI_ToyRobotFull_SVS,
		PI_ToyRobotFull_AVT,
		PI_ColourLearning,
		PI_FaceTracking,
		PI_MobileRobotTarget1,
		PI_ToyRobotLearning,
		PI_PlayPiano,
		PI_HRTransport,
		PI_DiffKin,
		PI_ComputerAssembly,
	};
};

class CProcessImage  
{
public:
	// Contruction
	CProcessImage( int width = 640, int height = 480 );
	virtual ~CProcessImage( void );

	// Set pointer to data source
	void set_DataSource( CData * pData );

	//--------------------------------------------------------------------------
	// Segmentation
public:
	// Perform segmentation on all colours manually
	void SegmentAllColours( void );

	// Perform segmentation on 'iColour' manually
	void SegmentColour( int nColour );

	// Perform segmentation in main colours
	void SegmentAllColours_cv( void );

	// Perform segmentation for 'iColour' using cv function
	void SegmentColour_cv( int iColour );
	
	// Perform segmentation in main colours ( R, G, B, C, M, Y )
	void SegmentMainColours_cv( void );

	// Load/Save segmentation parameters
	void LoadSegmentationParameters( std::string file );
	void SaveSegmentationParameters( std::string file );

	// Detect colour spots in all colours
	void DetectAllColourSpots( void );

	// Detect colour spots in main colours ( R, G, B, C, M, Y )
	void DetectMainColourSpots( void );

	//--------------------------------------------------------------------------
	//Accessors
public:
	bool isFeedbackActive( void );
	void doImageProcessing( bool doProc );
	bool useImageProcessing( void );

	int get_ProcessImageID( void );

	//--------------------------------------------------------------------------
	// Functions to interpret colour information
protected:
	// The name says it all!
	bool Associate_Colour_to_Object( int nColour, int nObject, unsigned int nMinArea = OBJECT_AREA_MIN, unsigned int nMaxArea = OBJECT_AREA_MAX );

	// Use function 'Associate_Colour_to_Object' to associate a colour to the object
	bool Detect_Object_From_Primary_Colour( int nObject, unsigned int nMinArea = OBJECT_AREA_MIN, unsigned int nMaxArea = OBJECT_AREA_MAX );

	//Change the region of interest of a type of objects, adding 'rect'
	void Define_New_Region_of_Interest( int nObject, cv::Rect rect );

	virtual void Action_Interpretation( void ) {};

	//--------------------------------------------------------------------------
	// Variable functions acording to the task
public:

	// Search for objects for the robot task
	virtual bool SearchAllObjects( void ) { return false; };

	virtual void ObjectPreProcessing( void ) {};
	
	virtual void ObjectPostProcessing( void ) {};

	// Process preconfigured image via 'set_New_Image_for_Processing' and retrieve data
	virtual void Process( void );

	// Process the image and retrieve data
	virtual void Process( cv::Mat * mat_ptr );

	virtual void ProcessTask( cv::Mat * mat_ptr ) {};

	virtual void Post_Process( void ) {};

	virtual void Calculate_Pointing_Directions( void ) {};

	//--------------------------------------------------------------------------
	// Function to interpret Object information
	void LookForNearObjects( int nObjectTypeSrc, int nObjectTypeDst );

public:
	//--------------------------------------------------------------------------
	// Visual feedback

	void CreateWindows( void );

	void DestroyWindows( void );

	void MarkFoundObjects( bool bText = false, cv::Mat * img = nullptr );

	void ShowSegmentedImages( void );
	
	void ShowSegmentationFeedback( void );

	void HideSegmentationFeedback( void );

	bool get_SegmentationWindowsStatus( void );

	void set_ImageDisplayStatus( bool bStatus );

	bool get_ImageDisplayStatus( void );

	void ShowSourceImageOnCVWindow( std::string strName );
	
	void ShowProcessedImageOnCVWindow( std::string strName );

	std::string get_MainWindowName( void );

	void Display_Processed_Image( void );

	CAttachMutex_n<bool>
		bPrintHandMovement,
		bPrintBodyMovement;

	void Display_Status_Bar_Information( void );

	CAttachMutex_t< cv::Point3i > ptMousePos;

protected:
	void set_SegmentationWindowsStatus( bool bStatus );

	std::string strWindowName;

	//--------------------------------------------------------------------------
	// Image file handling
public:
	void get_SourceImage( cv::Mat * mat_ptr );
	void get_ProcessedImage( cv::Mat * mat_ptr );

	void SaveProcessedImageToFile( std::string file_name );
	void SaveSourceImageToFile( std::string file_name );
	void SaveResizedImageToFile( std::string file_name );

	//--------------------------------------------------------------------------
	// Frame saving
public:
	void FrameSaving_On( void );
	void FrameSaving_Off( void );
	void SaveFrame( std::string ext = "bmp", bool bSave_dt = false, double dt = 0.0, bool bSave_PanTilt = false, float pan = 0.0f, float tilt = 0.0f );
	void set_FrameSavingStatus( bool bSave );
	bool get_FrameSavingStatus( void );
	void ToggleFrameSaving( void );

	//--------------------------------------------------------------------------
	// Hand Acquisition
public:
	void SaveHandImage( void );
	void set_HandAcquisition( bool bAcquire );
	bool get_HandAcquisition( void );

	//--------------------------------------------------------------------------
	// Setup processing
public:
	// Allocate memory for images and other objects
	void SetupProcessing( int width, int height );

	// Clear allocated memory
	void Clean( void );

	// Prepare a new image for processing
	void set_New_Image_for_Processing( cv::Mat * mat_ptr );

	// When specific initialization for a task is required
protected:
	virtual void SetupProcessing_TaskSpecific( int width, int height ) {};

	virtual void Clean_TaskSpecific( void ) {};
	
protected:
	//returns the position of the array with the most frequent value
	unsigned char CalculateMode( unsigned char * val );

private:
	void SegmentationValues_Store( void );
	void SegmentationValues_Load( void );

	//--------------------------------------------------------------------------
	// Functions to analyze image regions and spots
protected:
	// Verifies if contents of 'spot1' in 'img1' overlap with contents of 'spot2' int 'img2'
	bool CheckOverlapSpot( cv::Mat * img1, cv::Rect * spot1, cv::Mat * img2, cv::Rect * spot2 );

	//--------------------------------------------------------------------------
	// Object mask handling for stereo calculation taking into account the segmentation
protected:
	// Sets up the mask of each object
	void Generate_Object_Masks( void );

	// Generically generate a mask for any object
	void Generate_Mask_for_Object( CObject_f * pObj );

	// Specific function to generate the mask for columns
	//void Generate_Mask_for_Column( CObject_f * pCol );

	//--------------------------------------------------------------------------
	// Member variables
public:
	CAttachMutex_t< std::vector< CSegmentedImage > > vSegmentedImage;
	CData * pDataSource;

protected:
	CAttachMutex_n<bool>
		bFeedback,
		bFeedbackWindowsReady,
		bImageProcessing,
		bAcquireHand,
		bFrameSaving,
		bDisplayImage;

	unsigned char segmented;
	
	int 
		nWidth, 
		nHeight,
		nProcessImageID;

	std::string
		sSegFile,
		sSegmentationSearchDir,
		sBasePath_FrameSaving,
		sPath_FrameSaving;

	unsigned int 
		nObjectArea_Max, 
		nObjectArea_Min,
		uFrame,
		uDir;

	cv::Mat
		mat_ImageHSV,
		mat_ImageGray,
		aImg_gray_aux[3];

	CAttachMutex_o< cv::Mat >
		mat_ImageSource,
		mat_ImageProcessed;

public:
	// This object can be used by calling BodyMovementQuantifier.Setup( width, height ) 
	// on any derived class from CProcessImage in function SetupProcessing_TaskSpecific
	CMovementQuantifier BodyMovementQuantifier;

	CAttachMutex_n<bool> bUseBodyMovement;

	void Process_Movement( void );

public:
	// This object can be used by calling HandTracker.Setup( width, height ) 
	// on any derived class from CProcessImage in function SetupProcessing_TaskSpecific
	// Then HandTracker.TrackObjects( ... ); must be called with a binary image of the objects to be tracked
	CObjectTracker HandTracker;
	
	CAttachMutex_n<bool> bUseHandTracker;

	void Update_Coordinates_on_Tracked_Hands( void );

private:
	std::vector< std::vector< unsigned int > > vSegValues;

public:
	// Object for gesture classification
	CGestureClassifier Gesture;

	void Detect_Gestures( void );

	bool isHand_with_Gesture_Near_Object( unsigned int gesture, unsigned int object_type );
};

#endif // _PROCESS_IMAGE_H_
