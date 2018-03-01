#ifndef _AMARIFIELD2D_HPP_INCLUDED_
#define _AMARIFIELD2D_HPP_INCLUDED_

#include "AmariField2D.h"

typedef CAmariField2D<float> CAmariField2D_f;
typedef CAmariField2D<double> CAmariField2D_d;

#include <algorithm>

template <typename Type>
CAmariField2D<Type>::CAmariField2D( Type var_x_min, Type var_x_max, Type sampling_x, Type var_y_min, Type var_y_max, Type sampling_y )
: Tau( 0.1f )
, Time( 10.0f*Tau )
, dt( Tau/5.0f )
{
	Initialize_Field( var_x_min, var_x_max, sampling_x, var_y_min, var_y_max, sampling_y );
}

template <typename Type>
CAmariField2D<Type>::CAmariField2D( const CAmariField2D<Type> &field )
: X( field.X ), Y( field.Y )
, Sampling_X( field.Sampling_X ), Sampling_Y( field.Sampling_Y )
, Range_X( field.Range_X ), Range_Y( field.Range_Y )
, nElements_X( field.nElements_X ), nElements_Y( field.nElements_Y )
, lwx( field.lwx ), lwy( field.lwy ), lwx2( field.lwx2 ), lwy2( field.lwy2 )
, Label_X( field.Label_X ), Label_Y( field.Label_Y )
, Tau( field.Tau ), Time( field.Time ), dt( field.dt )
, u( field.u )
, up( field.up )
, u_ini( field.u_ini )
, u_conv( field.u_conv )
, u_past( field.u_past )
, f_u_conv( field.f_u_conv )
, convx( field.convx )
, h( field.h )
, S( field.S )
, Kernel( field.Kernel )
, kSigma( field.kSigma ), kCenter( field.kCenter )
, Noise( field.Noise )
{
	if( s_field_input.size() != field.s_field_input.size() )
		s_field_input.resize( field.s_field_input.size() );

	std::copy( field.s_field_input.begin(), field.s_field_input.end(), s_field_input.begin() );
}

template <typename Type>
inline void CAmariField2D<Type>::ClearInputs( void )
{
	s_field_input.clear();
	S.set_Data( 0.0f );
}

template <typename Type>
void CAmariField2D<Type>::Initialize_Field( Type var_x_min, Type var_x_max, Type sampling_x, Type var_y_min, Type var_y_max, Type sampling_y )
{
	Sampling_X = sampling_x;
	Sampling_Y = sampling_y;

	Label_X = "Pan";
	Label_Y = "Tilt";

	Range_X.min = std::min( var_x_min, var_x_max );
	Range_X.max = std::max( var_x_min, var_x_max );

	Range_Y.min = std::min( var_y_min, var_y_max );
	Range_Y.max = std::max( var_y_min, var_y_max );

	nElements_X = (int) ( ( Range_X.max - Range_X.min )/Sampling_X );
	nElements_Y = (int) ( ( Range_Y.max - Range_Y.min )/Sampling_Y );

	lwx = nElements_X;
	lwy = nElements_Y;
	lwx2 = (int) ( nElements_X/2 + 0.5f );
	lwy2 = (int) ( nElements_Y/2 + 0.5f );

	//-----------------------------------------------------------------------
	// Create Behavioral variables
	X.set_Dimensions( nElements_X, 1 );
	Y.set_Dimensions( nElements_Y, 1 );

	int i=0;
	
	for( i=0 ; i<nElements_X ; i++ )
	{
		X( i, 0 ) = Range_X.min + i*Sampling_X;
	}

	for( i=0 ; i<nElements_Y ; i++ )
	{
		Y( i, 0 ) = Range_Y.min + i*Sampling_Y;
	}
	
	//-----------------------------------------------------------------------
	CMatrix<Type> init( nElements_Y, nElements_X, 0.0f );

	u.set_Data( &init );

	up.set_Data( &init );

	u_ini.set_Data( &init );

	u_past.set_Data( &init );

	Noise.set_Data( &init );

	u_conv.set_Dimensions( nElements_Y + lwy, nElements_X + lwx );
	u_conv.set_Data( 0.0f );
	
	f_u_conv.set_Dimensions( nElements_Y + lwy, nElements_X + lwx );
	f_u_conv.set_Data( 0.0f );

	// Define Kernel size
	Kernel.set_Dimensions( nElements_Y, nElements_X );

	kIntSamples = 20;
	Kernel_Integral.set_Dimensions( kIntSamples, kIntSamples );

	// Initialize inputs
	S.set_Dimensions( nElements_Y, nElements_X );
	ClearInputs();

	// Define resting level
	h = -1.0f;
}

// gaussian kernel with global inhibition
template <typename Type>
void CAmariField2D<Type>::Calculate_Kernel_Gaussian( void )
{
	const Type
		kp = 1.5f,
		kn = -0.25f;

	kSigma.x = 5.0f;
	kSigma.y = 2.0f;

	kCenter.x = Range_X.min + abs( Range_X.max - Range_X.min )/2.0f; //Make sure the kernel is centered in X
	kCenter.y = Range_Y.min + abs( Range_Y.max - Range_Y.min )/2.0f; //Make sure the kernel is centered in Y

	gauss2d2( &X, &Y, &kCenter, &kSigma, kp+abs(kn), kn, &Kernel );
}

// circular kernel with global inhibition
template <typename Type>
void CAmariField2D<Type>::Calculate_Kernel_Circular( void )
{
	const Type
		lcoop = 15.0f,	
        kp = 0.7f,	// 10
        kn = -1.0f,	// -4 -3.5 -2.5
		HalfRange_x = Range_X.max/2.0f,
		HalfRange_y = Range_Y.max/2.0f;

	Type 
		r = 0.0f,
		p = 2.0f;

	Kernel.set_Data( kn );

	for( int ix=0 ; ix<Kernel.get_Width() ; ix++ )
	{
		for( int iy=0 ; iy<Kernel.get_Height() ; iy++ )
		{
			r = sqrt( pow( ( X( ix, 0 ) - HalfRange_x ), p ) + pow( ( Y( iy, 0 ) - HalfRange_y ), p ) );

			if( r<lcoop )
			{
				Kernel( ix, iy ) = kp;
			}
		}
	}
}

template <typename Type>
void CAmariField2D<Type>::AddInput( CPoint<Type> center, CPoint<Type> sigma, Type a, Type k )
{
	s_field_input.push_back( CFieldInput<Type>( center, sigma, a, k ) );
}

template <typename Type>
void CAmariField2D<Type>::ProcessInputs( void )
{
	CMatrix<Type> 
		input_field( S.get_Height(), S.get_Width() ),
		input( S.get_Height(), S.get_Width() );

	unsigned int 
		i = 0,
		nInputs = s_field_input.size();

	for( i=0 ; i<nInputs ; i++ )
	{
		gauss2d2( &X, &Y, &s_field_input[i].Center, &s_field_input[i].Sigma, s_field_input[i].A, s_field_input[i].K, &input );
	
		// Add current input to field 'S'
		S.Add( &input );
	}
}

template <typename Type>
void CAmariField2D<Type>::Iterate( void )
{
	// u_past = u
	u_past.set_Data( &u );

	int 
		nx = lwx,
		ny = lwy;

	//----------------------------------------
	// Implement periodic boundary conditions

    //u_conv( 1:lwx2, 1:lwy2 ) = u_past( nx-lwx2+1:nx, ny-lwy2+1:ny );    % 4.1
	u_conv( 0, 0, lwx2, lwy2 ) = u_past( nx-lwx2+1, ny-lwy2+1, nx, ny );
	
    //u_conv( 1:lwx2, lwy2+1:ny+lwy2 ) = u_past( nx-lwx2+1:nx, 1:ny );            % 2.2 e 4.2
	u_conv( 0, lwy2+1, lwx2, ny+lwy2 ) = u_past( nx-lwx2+1, 0, nx, ny );

	//u_conv( 1:lwx2, ny+lwy2+1:ny+lwy ) = u_past( nx-lwx2+1:nx, 1:lwy2+1 );        % 2.1
	u_conv( 0, ny+lwy2+1, lwx2, ny+lwy ) = u_past( nx-lwx2+1, 0, nx, lwy2+1 );

	//u_conv( lwx2+1:nx+lwx2, 1:lwy2 ) = u_past( 1:nx, ny-lwy2+1:ny );            % 3.4 e 4.4
	u_conv( lwx2+1, 0, nx+lwx2, lwy2 ) = u_past( 0, ny-lwy2+1, nx, ny );

	//u_conv( lwx2+1:nx+lwx2, lwy2+1:lwy2+ny ) = u_past( 1:nx, 1:ny );                    % 1 2 3 4
	u_conv( lwx2+1, lwy2+1, nx+lwx2, lwy2+ny ) = u_past( 0, nx, 0, ny );

	//u_conv( lwx2+1:nx+lwx2, lwy2+ny+1:ny+lwy ) = u_past( 1:nx, 1:lwy2+1 );                % 1.3 e 2.3
	u_conv( lwx2+1, lwy2+ny+1, nx, ny+lwy ) = u_past( 0, 0, nx, lwy2+1 );

	//u_conv( nx+lwx2+1:nx+lwx, 1:lwy2 ) = u_past( 1:lwx2+1, ny-lwy2+1:ny );        % 3.1
	u_conv( nx+lwx2+1, 0, nx+lwx, lwy2 ) = u_past( 0, ny-lwy2+1, lwx2+1, ny );

	//u_conv( nx+lwx2+1:nx+lwx, lwy2+1:ny+lwy2 ) = u_past( 1:lwx2+1, 1:ny );                % 3.2 e 1.2
	u_conv( nx+lwx2+1, lwy2+1, nx+lwx, ny+lwy2 ) = u_past( 0, 0, lwx2+1, ny );

	//u_conv( nx+lwx2+1:nx+lwx, ny+lwy2+1:ny+lwy ) = u_past( 1:lwx2+1, 1:lwy2+1 );            % 1.1
	u_conv( nx+lwx2+1, ny+lwy2+1, nx+lwx, ny+lwy ) = u_past( 0, 0, lwx2+1, lwy2+1 );

	//----------------------------------------

	// applies non-linearity to u_conv  (i.e f(u(x,t) ) :
	Type K = 0.5f;

	sigmoid2d( &u_conv, K, &f_u_conv );

    // compute convolution
    // w - kernel
	//convx = conv2( f_u_conv, w ); 
	f_u_conv.Conv2D( &Kernel, &convx );
	
  	//----------------------------------------
	//% Follows fields iteration:
	// u = u_past + dt/Tau *( -u_past + convx( lwx:nx+lwx-1, lwy:ny+lwy-1 )+ h + S ) + 0.05*randn( nx, ny ); %0.01

	u.set_Data( &convx( lwx, lwy, nx+lwx-1, ny+lwy-1 ) );
	u.Sub( &u_past );
	u.Add( h );
	u.Add( &S );

	u.Mul( dt/Tau );

	u.Add( &u_past );

	int
		width = u.get_Width(),
		height = u.get_Height();

	// Add noise as a random gaussian
	Noise.RandomizeGaussian( 0.0f, 0.05f );
	u.Add( &Noise );
	
	// computes output of field:
	for( int i=0 ; i<height ; i++ )
	{
		for( int j=0 ; j<width ; j++ )
		{
			if( u( i, j ) > 0.0f )
			{
				up( i, j ) = u( i, j );
			}
			else
			{
				up( i, j ) = 0.0f;
			}
		}
	}
}

template <typename Type>
void CAmariField2D<Type>::IterateField( unsigned int uIterations, unsigned int uKernel )
{
	switch( uKernel )
	{
	default:
	case KernelType::GAUSSIAN:
		Calculate_Kernel_Gaussian();
		break;

	case KernelType::CIRCULAR:
		Calculate_Kernel_Circular();
		break;
	}

	ProcessInputs();

	u_conv.set_Data( 0.0f );
	u.set_Data( 0.0f );

	for( unsigned int i=0 ; i<uIterations ; i++ )
	{
		Iterate();
	}
}

#endif //_AMARIFIELD2D_HPP_INCLUDED_
