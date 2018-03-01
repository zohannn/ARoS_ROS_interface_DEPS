#ifndef _AMARIFIELD2D_H_
#define _AMARIFIELD2D_H_

#pragma once

#include <Matrix.h>

#include "FieldDataTypes.h"

namespace KernelType
{
	enum _KERNEL_TYPE_{
		GAUSSIAN = 0,
		CIRCULAR,
		// Add more Kernel defs here, before NUMBER_OF_KERNELS ...
		NUMBER_OF_KERNELS,
	};
}

#ifndef KERNEL_GAUSSIAN
#define KERNEL_GAUSSIAN	0
#endif

template <typename Type> class CAmariField2D
{
protected:
	//----------------------------------------------------------
	// Data variables
	CMatrix<Type> 
		X,
		Y;

	Type
		Sampling_X,
		Sampling_Y;

	CRange<Type>
		Range_X,
		Range_Y;

	int 
		nElements_X,
		nElements_Y,
		lwx,
		lwy,
		lwx2,
		lwy2;

	std::string
		Label_X,
		Label_Y;

	//----------------------------------------------------------
	// Simulation control variables
	Type 
		Tau,
		Time, // tempo total de simulação
		dt; // Euler time step

	//----------------------------------------------------------
	// Field excitation variables
	CMatrix<Type>  
		u,
		up,
		u_ini,
		u_conv,
		u_past,
		f_u_conv,
		convx;

	Type h;	// Resting level
				
	//----------------------------------------------------------
	// Field input variables
	CMatrix<Type> S;
	std::vector< CFieldInput<Type> > s_field_input;

	//----------------------------------------------------------
	// Kernel

	CMatrix<Type> 
		Kernel,
		Kernel_Integral;

	int kIntSamples;
	
	CPoint<Type>
		kSigma,
		kCenter;

	//----------------------------------------------------------
	// Noise
	CMatrix<Type> Noise;

public:
	//----------------------------------------------------------
	//					Constructors
	//----------------------------------------------------------
	CAmariField2D( Type var_x_min, Type var_x_max, Type sampling_x, Type var_y_min, Type var_y_max, Type sampling_y );

	CAmariField2D( const CAmariField2D<Type> &field );

	~CAmariField2D( void ){};

	//----------------------------------------------------------
	// Initialization
	void Initialize_Field( Type var_x_min, Type var_x_max, Type sampling_x, Type var_y_min, Type var_y_max, Type sampling_y );

	//----------------------------------------------------------
	// Field inputs
	
	void ClearInputs( void );

	void AddInput( CPoint<Type> center, CPoint<Type> sigma, Type a, Type k );

	void ProcessInputs( void );

protected:
	//----------------------------------------------------------
	// Kernel setup
	void Calculate_Kernel_Gaussian( void );
	void Calculate_Kernel_Circular( void );

	void Calculate_Kernel_Integral( int nSamples ){};

public:
	void Iterate( void );

	void IterateField( unsigned int uIterations = 1, unsigned int uKernel = KernelType::GAUSSIAN );

};

#endif //_AMARIFIELD2D_H_
