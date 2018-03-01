#include "StereoVidereDumbo.h"
#include "MathDefines.h"

CStereoVidereDumbo::CStereoVidereDumbo( void )
: CStereoVidere()
{
	Instantiate_SVS_Object();
	Load_Constant_Transformation_Parameters();
}

CStereoVidereDumbo::~CStereoVidereDumbo( void )
{
	Destroy_SVS_Object();
}

void CStereoVidereDumbo::Load_Constant_Transformation_Parameters( void )
{
	// It takes 5 transformations to go from vision axis to world axis.
	Resize_Transformation_Arrays( 5 );

	Baseline = 60.0f;
	
	K1 = -Baseline/2.0f;
	K2 = 63.0f;		// Distance from eyes to tilt rotation axis
	K3 = 835.0f;	// Height from the floor to the Neck (on Tilt axis) -> 733 + 9 + 93
	K4 = 212.0f;	// Distance from the Pan/Tilt to the center of the robot
	
	// Assign non-zero values to transformation arrays
	Alpha.at( 2 ) = -M_PI_2_F;
	Alpha.at( 3 ) = -M_PI_2_F;
	Alpha.at( 4 ) = M_PI_2_F;

	A.at( 0 ) = K4;
	A.at( 4 ) = K1;
	
	D.at( 0 ) = K3;
	D.at( 3 ) = -K2;

	Theta.at( 0 ) = M_PI_2_F;
	//dTheta.at( 1 ) = ;	//defined in function: "Setup_Transformation_Parameters"
	//dTheta.at( 2 ) = ;	//defined in function: "Setup_Transformation_Parameters"
	Theta.at( 3 ) = -M_PI_2_F;

	Offset[0] = 10.0f;	//mm
	Offset[1] = 0.0f;	//mm
	Offset[2] = 0.0f;	//mm
}

void CStereoVidereDumbo::Compensate_Coordinates_Vision( float *tX, float *tY, float *tZ )
{
	// Compensate calculated coordinates if necessary

	//* The function used to aproach the values was calculated to use         *
	//* with milimeters, the values returned by 'CalcPoint3D' are im meters,  *
	//* so I had to multiply aux values by 1000 to convert them to milimeters *

	const float scale = 1000.0f;

	float 
		X_aux = *tX * scale,
		Y_aux = *tY * scale,
		Z_aux = *tZ * scale;

	// Equations to compensate error calculation of vision coordinates

	*tX = ( 0.8467541032f*X_aux + 1.4329684824f ) / scale;

	*tY = ( 0.812604616f*Y_aux - 1.4908296603f ) / scale;

	*tZ = ( -0.000098146f * pow( Z_aux, 2.0f ) + 0.9333950613f * Z_aux + 40.2438131585f ) / scale; // Second order polynomial
}
