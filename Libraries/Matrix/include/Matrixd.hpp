// Implementation of class template specialization for CMatrix<double>
#include "Matrix.h"

//---------------------------------------------------------------------------
// Intel IPP libraries
#include <ippi.h>
#include <ippm.h>
#include <ipps.h>

//---------------------------------------------------------------------------

template<>
bool CMatrix<double>::set_Data( int nHeight, int nWidth, double * pData_Array )
{
	set_Dimensions( nHeight, nWidth );

	Current_Status = ippmCopy_ma_64f_SS(	
		pData_Array, Stride0, Stride1, Stride2,
		pData, Stride0, Stride1, Stride2,
		Width, Height, 1 );

	return isStatusOK();
}

template<>
bool CMatrix<double>::set_Data( double * pData_Array )
{
	Current_Status = ippmCopy_ma_64f_SS(
		pData_Array, Stride0, Stride1, Stride2,
		pData, Stride0, Stride1, Stride2,
		Width, Height, 1 );

	return isStatusOK();
}

template<>
bool CMatrix<double>::set_Data( CMatrix<double> * mat )
{
	set_Dimensions( mat->Height, mat->Width );

	Current_Status = ippmCopy_ma_64f_SS(
		mat->pData, Stride0, Stride1, Stride2,
		pData, Stride0, Stride1, Stride2,
		Width, Height, 1 );

	return isStatusOK();
}

template<>
bool CMatrix<double>::set_Data( double value )
{
	Current_Status = ippsSet_64f( value, pData, nElements );

	return isStatusOK();
}

template<>
bool CMatrix<double>::Copy_Data(double * pData_Array, int begin, int len)
{
	Current_Status = ippsCopy_64f(pData_Array, pData + begin, len);

	return isStatusOK();
}

template<>
bool CMatrix<double>::Copy_Data(CMatrix<double> * pSrc, int begin, int len)
{
	Current_Status = ippsCopy_64f(pSrc->pData, pData + begin, len);

	return isStatusOK();
}

template<>
bool CMatrix<double>::Alloc_Data_Memory( void )
{
	pData = ippsMalloc_64f( nElements*sizeof( double ) );

	return Valid_Data_Memory();
}
	
template<>
bool CMatrix<double>::get_DataArray( double * p_data )
{
	if( p_data == nullptr ) return false;

	Current_Status = ippmCopy_ma_64f_SS (
		pData, Stride0, Stride1, Stride2,
		p_data, Stride0, Stride1, Stride2,
		Width, Height, 1 );

	return isStatusOK();
}

template<>
CMatrix<double> CMatrix<double>::operator()( int Line_first, int Col_first, int Line_last, int Col_last )
{
	int 
		nlines = Line_last - Line_first + 1, 
		ncols = Col_last - Col_first + 1;

	CMatrix<double> mtemp( nlines , ncols );
		
	Current_Status = ippmExtract_m_64f(
		&pData[Line_first*Width+Col_first],	// Starting point
		Stride1, Stride2,
		mtemp.pData,
		ncols, nlines );

	isStatusOK();

	return mtemp;
}

template<>
bool CMatrix<double>::Add( CMatrix<double> * operand2, CMatrix<double> * result )
{
	Current_Status = ippmAdd_mm_64f( 
		pData, Stride1, Stride2,								//Matrix 1
		operand2->pData, operand2->Stride1, operand2->Stride2,	//Matrix 2
		result->pData, result->Stride1, result->Stride2,		//Matrix dst
		Width, Height );

	return isStatusOK();
}

template<>
bool CMatrix<double>::Add( double value, CMatrix<double> * result )
{
	Current_Status = ippsAddC_64f( pData, value, result->pData, nElements );

	return isStatusOK();
}

template<>
bool CMatrix<double>::Add( CMatrix<double> * operand2 )
{
	Current_Status = ippsAdd_64f_I( operand2->pData, pData, nElements );

	return isStatusOK();
}

template<>
bool CMatrix<double>::Add( double value )
{
	Current_Status = ippsAddC_64f_I( value, pData, nElements );

	return isStatusOK();
}

template<>
bool CMatrix<double>::Sub( CMatrix<double> * operand2, CMatrix<double> * result )
{
	Current_Status = ippmSub_mm_64f( pData, Stride1, Stride2,			//Matrix 1
		operand2->pData, operand2->Stride1, Stride2,	//Matrix 2
		result->pData, result->Stride1, result->Stride2,//Matrix dst
		Width, Height );

	return isStatusOK();
}

template<>
bool CMatrix<double>::Sub( double value, CMatrix<double> * result )
{
	Current_Status = ippsSubC_64f( pData, value, result->pData, nElements );

	return isStatusOK();
}

template<>
bool CMatrix<double>::Sub( CMatrix<double> * operand2 )
{
	Current_Status = ippsSub_64f_I( operand2->pData, pData, nElements );

	return isStatusOK();
}

template<>
bool CMatrix<double>::Sub( double value )
{
	Current_Status = ippsSubC_64f_I( value, pData, nElements );

	return isStatusOK();
}

template<>
bool CMatrix<double>::Mul( CMatrix<double> * operand2, CMatrix<double> * result )
{
	Current_Status = ippmMul_mm_64f( 
		pData, Stride1, Stride2, Width, Height,
		operand2->pData, operand2->Stride1, operand2->Stride2, operand2->Width, operand2->Height,
		result->pData, result->Stride1, result->Stride2 );

	return isStatusOK();
}

template<>
bool CMatrix<double>::Mul( double value, CMatrix<double> * result )
{
	Current_Status = ippmMul_mc_64f( 
		pData, Stride1, Stride2, 
		value, 
		result->pData, Stride1, Stride2, 
		Width, Height );

	return isStatusOK();
}

template<>
bool CMatrix<double>::Mul( CMatrix<double> * operand2 )
{
	double * pData2 = ippsMalloc_64f( nElements*sizeof( double ) );

	// Copy data from this to aux array	
	Current_Status = ippmCopy_ma_64f_SS(
		pData, Stride0, Stride1, Stride2,
		pData2, Stride0, Stride1, Stride2,
		Width, Height, 1 );

	if( !isStatusOK() ) return false;

	Current_Status = ippmMul_mm_64f( 
		pData2, Stride1, Stride2, Width, Height,
		operand2->pData, operand2->Stride1, operand2->Stride2, operand2->Width, operand2->Height,
		pData, Stride1, Stride2 );

	ippsFree( pData2 );

	return isStatusOK();
}

template<>
bool CMatrix<double>::Mul( double value )
{
	Current_Status = ippsMulC_64f_I( value, pData, nElements );

	return isStatusOK();
}

template<>
bool CMatrix<double>::DotMul(CMatrix<double> * operand2)
{
	Current_Status = ippsMul_64f_I(operand2->pData, pData, nElements);

	return isStatusOK();
}

template<>
bool CMatrix<double>::DotMul(CMatrix<double> * operand2, CMatrix<double> *result)
{
	Current_Status = ippsMul_64f(pData, operand2->pData, result->pData, nElements);

	return isStatusOK();
}

template<>
bool CMatrix<double>::Div( double value, CMatrix<double> * result )
{
	Current_Status = ippsDivC_64f( pData, value, result->pData, nElements );

	return isStatusOK();
}

template<>
bool CMatrix<double>::Div( double value )
{
	Current_Status = ippsDivC_64f_I( value, pData, nElements );

	return isStatusOK();
}

template<>
bool CMatrix<double>::Transpose( void )
{
	double * pDst = ippsMalloc_64f( nElements*sizeof( double ) );

	if( pDst == nullptr ) return false;

	Current_Status = ippmTranspose_m_64f( 
		pData, Stride1, Stride2, 
		Width, Height, 
		pDst, Height*nElement_Size, Stride2 );

	bool bOK = isStatusOK();
	
	if( bOK )
	{
		Setup_Matrix_Parameters( Width, Height );
		set_Data( pDst );
	}

	ippsFree( pDst );

	return bOK;
}

template<>
bool CMatrix<double>::Heaviside(double u0, CMatrix<double> * result)
{
	Current_Status = ippsThreshold_LTValGTVal_64f(pData, result->pData, nElements,
		u0, 0.0, u0, 1.0);

	return isStatusOK();
}

template<>
bool CMatrix<double>::Heaviside(double u0)
{
	Current_Status = ippsThreshold_LTValGTVal_64f_I(pData, nElements, 
		u0, 0.0, u0, 1.0);

	return isStatusOK();
}

template<>
bool CMatrix<double>::Threshold(double lowerBound, double lowerValue, double upperBound, double upperValue, CMatrix<double> *result)
{
	Current_Status = ippsThreshold_LTValGTVal_64f(pData, result->pData, nElements,
		lowerBound, lowerValue, upperBound, upperValue);

	return isStatusOK();
}

//template<class _Pr>
//bool CMatrix<double>::Apply(CMatrix<double> *result, _Pr pred)
//{
//	for (int i = 0; i < nElements; i++)
//		result->pData[i] = pred(pData[i]);
//
//	return isStatusOK();
//}

template<>
bool CMatrix<double>::Sum(int begin, int end, double *dst)
{
	Current_Status = ippsSum_64f(pData + begin, end - begin, dst);

	return isStatusOK();
}

template<>
double CMatrix<double>::Determinant( void )
{
	if( Width != Height ) return 0.0;
		
	int buf_size = Width*Width + Width;

	double 
		dResult = 0.0,
		*pBuffer = ippsMalloc_64f( buf_size*sizeof( double ) );
		
	if( pBuffer == nullptr ) return false;

	Current_Status = ippmDet_m_64f( 
		pData, Stride1, Stride2, 
		Width, 
		pBuffer, 
		&dResult );
		
	ippsFree( pBuffer );

	return isStatusOK() ? dResult : 0.0;
}

template<>
double CMatrix<double>::Max( void )
{
	double dMax = 0.0;
	Current_Status = ippsMax_64f( pData, nElements, &dMax );
	return isStatusOK() ? dMax : 0.0;
}

template<>
double CMatrix<double>::Min( void )
{
	double dMin = 0.0;
	Current_Status = ippsMin_64f( pData, nElements, &dMin );
	return isStatusOK() ? dMin : 0.0;
}

template<>
double CMatrix<double>::Sum( void )
{
	double dSum = 0.0;
	Current_Status = ippsSum_64f( pData, nElements, &dSum );
	return isStatusOK() ? dSum : 0.0;
}

template<>
bool CMatrix<double>::RandomizeGaussian( double Mean, double Deviation )
{
	unsigned int Seed = 0;
	Current_Status = ippsRandGauss_Direct_64f( pData, nElements, Mean, Deviation, &Seed );
	return isStatusOK();
}
	
template<>
bool CMatrix<double>::RandomizeUniform( double Low, double High )
{
	unsigned int Seed = 0;
	Current_Status = ippsRandUniform_Direct_64f( pData, nElements, Low, High, &Seed );
	return isStatusOK();
}
	
template<>
CMatrix<double> CMatrix<double>::RandomUniform( int height, int width, double Low, double High )
{
	CMatrix<double> mtemp( height, width );
	mtemp.RandomizeUniform( Low, High );
	return mtemp;
}

template<>
bool CMatrix<double>::Inverse( void )
{
	if( Width != Height ) return false;

	int buf_size = nElements * nElements + nElements;

	double 
		*pInverse = ippsMalloc_64f( nElements*sizeof( double ) ),	//Data will be stored here temporarily			
		*pBuffer = ippsMalloc_64f( buf_size*sizeof( double ) );		//IPP requires this buffer to do the operation

	if( ( pInverse == nullptr ) || ( pBuffer == nullptr ) ) return false;
	
	Current_Status = ippmInvert_m_64f( 
		pData, Stride1, Stride2, 
		pBuffer, 
		pInverse, Stride1, Stride2, 
		Width );

	//verify if the inverse matrix was calculated successfuly
	if( isStatusOK() )
	{
		Current_Status = ippmCopy_ma_64f_SS(
			pInverse, Stride0, Stride1, Stride2,
			pData, Stride0, Stride1, Stride2,
			Width, Height, 1 );
	}

	ippsFree( pInverse );
	ippsFree( pBuffer );
		
	return isStatusOK();
}

template<>
CMatrix<double> CMatrix<double>::cross( CMatrix<double> * mat )
{
	CMatrix<double> mtemp( Height, Width );

	if( Width == 1 || Height == 1 )
	{
		int 
			val1 = 10*Height + Width,
			val2 = 10*mat->Height + mat->Width;

		if( ( val1 == val2 ) && ( nElements == 3 ) )
		{
			Current_Status = ippmCrossProduct_vv_64f( 
				pData, Stride2, 
				mat->pData, mat->Stride2, 
				mtemp.pData, mtemp.Stride2 );

			isStatusOK();
		}
	}

	set_Data( &mtemp );

	return mtemp;
}

template<>
bool CMatrix<double>::cross( CMatrix<double> * mat, CMatrix<double> * result )
{
	if( Width == 1 || Height == 1 )
	{
		int 
			val1 = 10*Height + Width,
			val2 = 10*mat->Height + mat->Width;

		if( ( val1 == val2 ) && ( nElements == 3 ) )
		{
			Current_Status = ippmCrossProduct_vv_64f( 
				pData, Stride2, 
				mat->pData, mat->Stride2, 
				result->pData, result->Stride2 );
				
			return isStatusOK();
		}
	}
	
	return false;
}

template<>
double CMatrix<double>::norm( void )
{
	double dNorm = 0.0;
	Current_Status = ippmL2Norm_va_64f( pData, Stride0, Stride2, &dNorm, nElements, 1 );
	return isStatusOK() ? dNorm : 0.0;
}

template<>
CMatrix<double> CMatrix<double>::SolLinearSyst_6x6( CMatrix<double> A, CMatrix<double> b )
{
	CMatrix<double> C1( 6, 1 );
		
	int * index = ippsMalloc_32s( 200*sizeof( int ) );

	if( index == nullptr ) return false;

	Current_Status = ippmLUDecomp_m_64f(
		A.pData, A.Stride1,	A.Stride2,
		index,
		A.pData, A.Stride1,	A.Stride2,
		A.nElements );

	if( isStatusOK() )
	{
		Current_Status = ippmLUBackSubst_mv_64f( 
		A.pData, A.Stride1,	A.Stride2,
		index,
		b.pData, b.Stride2,
		C1.pData, C1.Stride2,
		C1.nElements );

		isStatusOK();
	}

	ippsFree( index );

	return C1;
}	

template<>
bool CMatrix<double>::Conv2D( CMatrix<double> * operand2, CMatrix<double> * result )
{
	// Define size variables
	IppiSize
		size_op1,
		size_op2,
		size_res;

	size_op1.height = Height;
	size_op1.width = Width;

	size_op2.height = operand2->Height;
	size_op2.width = operand2->Width;

	size_res.height = Height + operand2->Height - 1;
	size_res.width = Width + operand2->Width - 1;
		
	// ----------------------------------------------------------------------
	// Memory allocation
	int
		fM1_Step = 0,
		fM2_Step = 0,
		fM3_Step = 0;

	float
		*fM1 = ippiMalloc_32f_C1( Width, Height, &fM1_Step ),
		*fM2 = ippiMalloc_32f_C1( operand2->Width, operand2->Height, &fM2_Step ),
		*fM3 = ippiMalloc_32f_C1( size_res.width, size_res.height, &fM3_Step );

	if( ( fM1 == nullptr ) || ( fM2 == nullptr ) || ( fM3 == nullptr ) ) return false;

	// ----------------------------------------------------------------------
	// Get data ready to process (type conversion required)

	Current_Status = ippsConvert_64f32f( pData, fM1, nElements );
	if( !isStatusOK() ) return false;

	Current_Status = ippsConvert_64f32f( operand2->pData, fM2, operand2->nElements );
	if( !isStatusOK() ) return false;

	// ----------------------------------------------------------------------
	// Calculate convolution
		
	int
		stride_op1 = size_op1.width * sizeof( float ),
		stride_op2 = size_op2.width * sizeof( float ),
		stride_res = size_res.width * sizeof( float );
		
	Current_Status = ippiConvFull_32f_C1R( 
		fM1, stride_op1, size_op1,	// Operand 1
		fM2, stride_op2, size_op2,	// Operand 2
		fM3, stride_res );			// Result

	if( !isStatusOK() ) return false;

	// ----------------------------------------------------------------------
	// Make sure the result matrix has the proper dimensions
	result->set_Dimensions( size_res.height, size_res.width );
	
	// Type conversion from float to double and data copying
	Current_Status = ippsConvert_32f64f( fM3, result->pData, size_res.height*size_res.width );

	// ----------------------------------------------------------------------
	// Release allocated memory

	ippiFree( fM1 );
	ippiFree( fM2 );
	ippiFree( fM3 );

	return isStatusOK();
}

template<>
bool CMatrix<double>::Conv(CMatrix<double> * operand2, CMatrix<double> * result)
{
	// Define size variables
	int
		src1Len,
		src2Len;

	src1Len = Width;
	src2Len = operand2->Width;

	// ----------------------------------------------------------------------
	// Make sure the result matrix has the proper dimensions
	result->set_Dimensions(1, src1Len + src2Len - 1);

	// ----------------------------------------------------------------------
	// Calculate convolution

	Current_Status = ippsConv_64f(
		pData, src1Len,						// Operand 1
		operand2->pData, src2Len,	// Operand 2
		result->pData);				// Result

	return isStatusOK();
}

//TODO: needs review for even operand2.
template<>
bool CMatrix<double>::ConvCirc(CMatrix<double> * operand2, CMatrix<double> * result)
{
	//Auxiliary variable for circular convolution
	CMatrix<double> extended_signal;
	CMatrix<double> full_result;

	// Define size variables
	int
		src1Len,
		src2Len;

	src1Len = Width;
	src2Len = operand2->Width;
	int hkernelWidth = src2Len / 2; //src2Len/2 will floor the value on pourpose

	// Copy the data
	extended_signal.set_Dimensions(1, src1Len + 2 * hkernelWidth);

	Current_Status = ippsCopy_64f(pData + (src1Len - hkernelWidth), extended_signal.pData, hkernelWidth);

	if (!isStatusOK())
		return false;

	Current_Status = ippsCopy_64f(pData, extended_signal.pData + hkernelWidth, src1Len);

	if (!isStatusOK())
		return false;

	Current_Status = ippsCopy_64f(pData, extended_signal.pData + src1Len + hkernelWidth, hkernelWidth);

	if (!isStatusOK())
		return false;


	// ----------------------------------------------------------------------
	// Make sure the result matrix has the proper dimensions
	full_result.set_Dimensions(1, src1Len + 2 * hkernelWidth + src2Len - 1);
	result->set_Dimensions(1, src1Len);
	// ----------------------------------------------------------------------
	// Calculate convolution

	Current_Status = ippsConv_64f(
		extended_signal.pData, src1Len + 2 * hkernelWidth,						// Operand 1
		operand2->pData, src2Len,	// Operand 2
		full_result.pData);				// Result

	if (!isStatusOK())
		return false;

	//Copy only the valid part to the result
	Current_Status = ippsCopy_64f(full_result.pData + 2 * hkernelWidth, result->pData, src1Len);

	return isStatusOK();
}


template<>
bool CMatrix<double>::ConvCircP(CMatrix<double> * operand2, CMatrix<double> * result, CMatrix<double> * extended_operand1, CMatrix<double> * full_result)
{

	// Define size variables
	int
		src1Len,
		src2Len;

	src1Len = Width;
	src2Len = operand2->Width;
	int hkernelWidth = src2Len / 2; //src2Len/2 will floor the value on pourpose

	// Copy the data
	extended_operand1->set_Dimensions(1, src1Len + 2 * hkernelWidth);

	Current_Status = ippsCopy_64f(pData + (src1Len - hkernelWidth), extended_operand1->pData, hkernelWidth);

	if (!isStatusOK())
		return false;

	Current_Status = ippsCopy_64f(pData, extended_operand1->pData + hkernelWidth, src1Len);

	if (!isStatusOK())
		return false;

	Current_Status = ippsCopy_64f(pData, extended_operand1->pData + src1Len + hkernelWidth, hkernelWidth);

	if (!isStatusOK())
		return false;


	// ----------------------------------------------------------------------
	// Make sure the result matrix has the proper dimensions
	full_result->set_Dimensions(1, src1Len + 2 * hkernelWidth + src2Len - 1);
	result->set_Dimensions(1, src1Len);
	// ----------------------------------------------------------------------
	// Calculate convolution

	Current_Status = ippsConv_64f(
		extended_operand1->pData, src1Len + 2 * hkernelWidth,						// Operand 1
		operand2->pData, src2Len,	// Operand 2
		full_result->pData);				// Result

	if (!isStatusOK())
		return false;

	//Copy only the valid part to the result
	Current_Status = ippsCopy_64f(full_result->pData + 2 * hkernelWidth, result->pData, src1Len);

	return isStatusOK();
}
// --------------------------------------------------------------------------
// Instantiate template for 'double'
template class CMatrix<double>;
