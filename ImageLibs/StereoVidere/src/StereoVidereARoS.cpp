#include "StereoVidereARoS.h"
#include "MathDefines.h"

CStereoVidereARoS::CStereoVidereARoS( void )
: CStereoVidere()
{
	Instantiate_SVS_Object( true );
	Load_Constant_Transformation_Parameters();
}

CStereoVidereARoS::~CStereoVidereARoS( void )
{
	Destroy_SVS_Object();
}

void CStereoVidereARoS::Load_Constant_Transformation_Parameters( void )
{
	// It takes 5 transformations to go from vision axis to world axis.
	Resize_Transformation_Arrays( 5 );

	Baseline = 69.875881f;
	
	K1 = -Baseline/2.0f;
	K2 = 74.0f;		// Distance from eyes to tilt rotation axis
	K3 = 1765.694f;	// Height from the floor to the Neck (on Tilt axis) //, K5( 1560.0 + 40.0 + 51.0 + 91.694 )

	// Assign non-zero values to transformation arrays
	Alpha.at( 2 ) = -M_PI_2_F;
	Alpha.at( 3 ) = -M_PI_2_F;
	Alpha.at( 4 ) = M_PI_2_F;

	A.at( 4 ) = K1;
	
	D.at( 0 ) = K3;
	D.at( 3 ) = -K2;

	Theta.at( 0 ) = M_PI_2_F;
	//dTheta.at( 1 ) = ;	//defined in function: "Setup_Transformation_Parameters"
	//dTheta.at( 2 ) = ;	//defined in function: "Setup_Transformation_Parameters"
	Theta.at( 3 ) = -M_PI_2_F;

	Offset[0] = 10.0f;//mm
	Offset[1] = 0.0f;//mm
	Offset[2] = 0.0f;//mm
}

void CStereoVidereARoS::Compensate_Coordinates_Vision( float *tX, float *tY, float *tZ )
{
	//Compensate the value on the Z axis, aproaching it by a sum of exponentials
	//dZ = 937.7*exp( 0.0004467*dZ_aux ) - 998.5*exp( -0.0008334*dZ_aux );
	
	//* The function used to aproach the values was calculated to use        *
	//* with milimeters, the values returned by 'CalcPoint3D' are im meters, *
	//* so I had to multiply dZ_aux by 1000 to convert it to milimeters	     *

	//dZ = ( 937.7*exp( 0.4467*dZ_aux ) - 998.5*exp( -0.8334*dZ_aux ) ) / 1000.0;
	
	float Z_aux = *tZ * 1000.0f;

	*tZ = ( 1.03f*Z_aux - 0.6f ) / 1000.0f;
}

void CStereoVidereARoS::Compensate_Coordinates_World( float *tX, float *tY, float *tZ )
{
	float 
		fX = *tX,
		fY = *tY,
		fZ = *tZ;

	//*tX = 1.1018403243f*fX + 15.6294167663f;
	*tX = 1.05f*fX; // Add 5% to the X coordinate
	*tY = 1.0924774636f*fY + 15.834189448f;
}

void CStereoVidereARoS::CalculatePanTiltFromCoordinates( float X, float Y, float Z, float * pPan, float * pTilt )
{
	//In this function EVERYTHING is in milimeters

	// Use the same variables from the setup for the transformation matrix of coordinates
	// The variables K2 and K3 are not used because of the static nature of function 'CalculatePanTiltFromCoordinates'
	const float 
		AROS_NECK_HEIGHT = 1765.694f,	// K3
		AROS_NECK_EYES_DISTANCE = 74.0f;// K2

	float 
		RO = 0.0f,						// Distance from the robot to the Object (Distance measured on the floor)
		HON = 0.0f,						// Height difference from the object to ARoS's neck (On Tilt axis)
		NE = AROS_NECK_EYES_DISTANCE,	// mm Distance from the Neck to the Eyes
		NO = 0.0f,						// Distance from Neck to the Object
		fAlpha = 0.0f,					//Auxiliary angle
		fBeta = 0.0f;					//Auxiliary angle
	
	RO = sqrt( pow( X, 2.0f ) + pow( Y, 2.0f ) );
	HON = AROS_NECK_HEIGHT - Z;
	NO = sqrt( pow( RO, 2.0f ) + pow( HON, 2.0f ) );

	fAlpha = atan2( RO, HON );
	fBeta = acos( NE/NO );

	*pPan = atan2( Y, X ) * RAD_TO_DEG_F;
	
	// The tilt angle is negative on the pan-tilt, hence, 
	// the changing in the formula, relatively to what appears in the thesis Silva (2008)
	*pTilt = ( fAlpha + fBeta - M_PI_F ) * RAD_TO_DEG_F;
	
	if( *pTilt < -47.0f ) *pTilt = -47.0f; // -47º is the lower limit for tilt position
}
