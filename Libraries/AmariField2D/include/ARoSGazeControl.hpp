#include "ARoSGazeControl.h"

typedef CARoSGazeControl<float> CARoSGazeControl_f;
typedef CARoSGazeControl<double> CARoSGazeControl_d;

template <typename T>
CARoSGazeControl<T>::CARoSGazeControl( void )
: CAmariField2D<T>( -90, 90, 1, -50, 10, 1 )
, iSigma( 5, 5 )
, iK( -0.15f )
, iaA1( 20 )
, iaBeta1( 2 )
{
	Calculate_Kernel_Gaussian();
}

template <typename T>
T CARoSGazeControl<T>::InputAmplitude( T distance )
{
	return ( iaA1 * exp( -distance/iaBeta1 ) );
}

template <typename T>
void CARoSGazeControl<T>::AddInput( CPoint<T> pantilt, T distance )
{
	T amp = InputAmplitude( distance );

	CAmariField2D<T>::AddInput( pantilt, iSigma, amp, iK );
}
