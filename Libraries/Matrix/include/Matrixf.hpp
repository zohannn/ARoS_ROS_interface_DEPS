// Implementation of class template specialization for CMatrix<float>
#include "Matrix.h"

//---------------------------------------------------------------------------
// Intel IPP libraries
#include <ippi.h>
#include <ippm.h>
#include <ipps.h>

//---------------------------------------------------------------------------

template<>
bool CMatrix<float>::set_Data( int nHeight, int nWidth, float * pData_Array )
{
	set_Dimensions( nHeight, nWidth );

	Current_Status = ippmCopy_ma_32f_SS(	
		pData_Array, Stride0, Stride1, Stride2,
		pData, Stride0, Stride1, Stride2,
		Width, Height, 1 );

	return isStatusOK();
}

template<>
bool CMatrix<float>::set_Data( float * pData_Array )
{
	Current_Status = ippmCopy_ma_32f_SS(
		pData_Array, Stride0, Stride1, Stride2,
		pData, Stride0, Stride1, Stride2,
		Width, Height, 1 );

	return isStatusOK();
}

template<>
bool CMatrix<float>::set_Data( CMatrix<float> * mat )
{
	set_Dimensions( mat->Height, mat->Width );

	Current_Status = ippmCopy_ma_32f_SS(
		mat->pData, Stride0, Stride1, Stride2,
		pData, Stride0, Stride1, Stride2,
		Width, Height, 1 );

	return isStatusOK();
}

template<>
bool CMatrix<float>::set_Data( float value )
{
	Current_Status = ippsSet_32f( value, pData, nElements );

	return isStatusOK();
}

template<>
bool CMatrix<float>::Alloc_Data_Memory( void )
{
	pData = ippsMalloc_32f( nElements*sizeof( float ) );

	return Valid_Data_Memory();
}
	
template<>
bool CMatrix<float>::get_DataArray( float * p_data )
{
	if( p_data == nullptr ) return false;

	Current_Status = ippmCopy_ma_32f_SS (
		pData, Stride0, Stride1, Stride2,
		p_data, Stride0, Stride1, Stride2,
		Width, Height, 1 );

	return isStatusOK();
}

template<>
CMatrix<float> CMatrix<float>::operator()( int Line_first, int Col_first, int Line_last, int Col_last )
{
	int 
		nlines = Line_last - Line_first + 1, 
		ncols = Col_last - Col_first + 1;

	CMatrix<float> mtemp( nlines , ncols );
		
	Current_Status = ippmExtract_m_32f(
		&pData[Line_first*Width+Col_first],	// Starting point
		Stride1, Stride2,
		mtemp.pData,
		ncols, nlines );

	isStatusOK();

	return mtemp;
}

template<>
bool CMatrix<float>::Add( CMatrix<float> * operand2, CMatrix<float> * result )
{
	Current_Status = ippmAdd_mm_32f( 
		pData, Stride1, Stride2,								//Matrix 1
		operand2->pData, operand2->Stride1, operand2->Stride2,	//Matrix 2
		result->pData, result->Stride1, result->Stride2,		//Matrix dst
		Width, Height );

	return isStatusOK();
}

template<>
bool CMatrix<float>::Add( float value, CMatrix<float> * result )
{
	Current_Status = ippsAddC_32f( pData, value, result->pData, nElements );

	return isStatusOK();
}

template<>
bool CMatrix<float>::Add( CMatrix<float> * operand2 )
{
	Current_Status = ippsAdd_32f_I( operand2->pData, pData, nElements );

	return isStatusOK();
}

template<>
bool CMatrix<float>::Add( float value )
{
	Current_Status = ippsAddC_32f_I( value, pData, nElements );

	return isStatusOK();
}

template<>
bool CMatrix<float>::Sub( CMatrix<float> * operand2, CMatrix<float> * result )
{
	Current_Status = ippmSub_mm_32f( pData, Stride1, Stride2,			//Matrix 1
		operand2->pData, operand2->Stride1, Stride2,	//Matrix 2
		result->pData, result->Stride1, result->Stride2,//Matrix dst
		Width, Height );

	return isStatusOK();
}

template<>
bool CMatrix<float>::Sub( float value, CMatrix<float> * result )
{
	Current_Status = ippsSubC_32f( pData, value, result->pData, nElements );

	return isStatusOK();
}

template<>
bool CMatrix<float>::Sub( CMatrix<float> * operand2 )
{
	Current_Status = ippsSub_32f_I( operand2->pData, pData, nElements );

	return isStatusOK();
}

template<>
bool CMatrix<float>::Sub( float value )
{
	Current_Status = ippsSubC_32f_I( value, pData, nElements );

	return isStatusOK();
}

template<>
bool CMatrix<float>::Mul( CMatrix<float> * operand2, CMatrix<float> * result )
{
	Current_Status = ippmMul_mm_32f( 
		pData, Stride1, Stride2, Width, Height,
		operand2->pData, operand2->Stride1, operand2->Stride2, operand2->Width, operand2->Height,
		result->pData, result->Stride1, result->Stride2 );

	return isStatusOK();
}

template<>
bool CMatrix<float>::Mul( float value, CMatrix<float> * result )
{
	Current_Status = ippmMul_mc_32f( 
		pData, Stride1, Stride2, 
		value, 
		result->pData, Stride1, Stride2, 
		Width, Height );

	return isStatusOK();
}

template<>
bool CMatrix<float>::Mul( CMatrix<float> * operand2 )
{
	float * pData2 = ippsMalloc_32f( nElements*sizeof( float ) );

	// Copy data from this to aux array	
	Current_Status = ippmCopy_ma_32f_SS(
		pData, Stride0, Stride1, Stride2,
		pData2, Stride0, Stride1, Stride2,
		Width, Height, 1 );

	if( !isStatusOK() ) return false;

	Current_Status = ippmMul_mm_32f( 
		pData2, Stride1, Stride2, Width, Height,
		operand2->pData, operand2->Stride1, operand2->Stride2, operand2->Width, operand2->Height,
		pData, Stride1, Stride2 );

	ippsFree( pData2 );

	return isStatusOK();
}

template<>
bool CMatrix<float>::Mul( float value )
{
	Current_Status = ippsMulC_32f_I( value, pData, nElements );

	return isStatusOK();
}

template<>
bool CMatrix<float>::Div( float value, CMatrix<float> * result )
{
	Current_Status = ippsDivC_32f( pData, value, result->pData, nElements );

	return isStatusOK();
}

template<>
bool CMatrix<float>::Div( float value )
{
	Current_Status = ippsDivC_32f_I( value, pData, nElements );

	return isStatusOK();
}

template<>
bool CMatrix<float>::Transpose( void )
{
	float * pDst = ippsMalloc_32f( nElements*sizeof( float ) );

	if( pDst == nullptr ) return false;

	Current_Status = ippmTranspose_m_32f( 
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
float CMatrix<float>::Determinant( void )
{
	if( Width != Height ) return 0.0f;
		
	int buf_size = Width*Width + Width;

	float 
		fResult = 0.0f,
		*pBuffer = ippsMalloc_32f( buf_size*sizeof( float ) );
		
	if( pBuffer == nullptr ) return false;

	Current_Status = ippmDet_m_32f( 
		pData, Stride1, Stride2, 
		Width, 
		pBuffer, 
		&fResult );
		
	ippsFree( pBuffer );

	return isStatusOK() ? fResult : 0.0f;
}

template<>
float CMatrix<float>::Max( void )
{
	float fMax = 0.0f;
	Current_Status = ippsMax_32f( pData, nElements, &fMax );
	return isStatusOK() ? fMax : 0.0f;
}

template<>
float CMatrix<float>::Min( void )
{
	float fMin = 0.0f;
	Current_Status = ippsMin_32f( pData, nElements, &fMin );
	return isStatusOK() ? fMin : 0.0f;
}

template<>
float CMatrix<float>::Sum( void )
{
	float fSum = 0.0f;
	Current_Status = ippsSum_32f( pData, nElements, &fSum, ippAlgHintNone );
	return isStatusOK() ? fSum : 0.0f;
}

template<>
bool CMatrix<float>::RandomizeGaussian( float Mean, float Deviation )
{
	unsigned int Seed = 0;
	Current_Status = ippsRandGauss_Direct_32f( pData, nElements, Mean, Deviation, &Seed );
	return isStatusOK();
}
	
template<>
bool CMatrix<float>::RandomizeUniform( float Low, float High )
{
	unsigned int Seed = 0;
	Current_Status = ippsRandUniform_Direct_32f( pData, nElements, Low, High, &Seed );
	return isStatusOK();
}
	
template<>
CMatrix<float> CMatrix<float>::RandomUniform( int height, int width, float Low, float High )
{
	CMatrix<float> mtemp( height, width );
	mtemp.RandomizeUniform( Low, High );
	return mtemp;
}

template<>
bool CMatrix<float>::Inverse( void )
{
	if( Width != Height ) return false;

	int buf_size = nElements * nElements + nElements;

	float 
		*pInverse = ippsMalloc_32f( nElements*sizeof( float ) ),	//Data will be stored here temporarily			
		*pBuffer = ippsMalloc_32f( buf_size*sizeof( float ) );		//IPP requires this buffer to do the operation

	if( ( pInverse == nullptr ) || ( pBuffer == nullptr ) ) return false;

	Current_Status = ippmInvert_m_32f( 
		pData, Stride1, Stride2, 
		pBuffer, 
		pInverse, Stride1, Stride2, 
		Width );

	//verify if the inverse matrix was calculated successfuly
	if( isStatusOK() )
	{
		Current_Status = ippmCopy_ma_32f_SS(
			pInverse, Stride0, Stride1, Stride2,
			pData, Stride0, Stride1, Stride2,
			Width, Height, 1 );
	}

	ippsFree( pInverse );
	ippsFree( pBuffer );
		
	return isStatusOK();
}

template<>
CMatrix<float> CMatrix<float>::cross( CMatrix<float> * mat )
{
	CMatrix<float> mtemp( Height, Width );

	if( Width == 1 || Height == 1 )
	{
		int 
			val1 = 10*Height + Width,
			val2 = 10*mat->Height + mat->Width;

		if( ( val1 == val2 ) && ( nElements == 3 ) )
		{
			Current_Status = ippmCrossProduct_vv_32f( 
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
bool CMatrix<float>::cross( CMatrix<float> * mat, CMatrix<float> * result )
{
	if( Width == 1 || Height == 1 )
	{
		int 
			val1 = 10*Height + Width,
			val2 = 10*mat->Height + mat->Width;

		if( ( val1 == val2 ) && ( nElements == 3 ) )
		{
			Current_Status = ippmCrossProduct_vv_32f( 
				pData, Stride2, 
				mat->pData, mat->Stride2, 
				result->pData, result->Stride2 );
				
			return isStatusOK();
		}
	}

	return false;
}

template<>
float CMatrix<float>::norm( void )
{
	float fNorm = 0.0f;
	Current_Status = ippmL2Norm_va_32f( pData, Stride0, Stride2, &fNorm, nElements, 1 );
	return isStatusOK() ? fNorm : 0.0f;
}

template<>
CMatrix<float> CMatrix<float>::SolLinearSyst_6x6( CMatrix<float> A, CMatrix<float> b )
{
	CMatrix<float> C1( 6, 1 );
		
	int * index = ippsMalloc_32s( 200*sizeof( int ) );

	if( index == nullptr ) return false;

	Current_Status = ippmLUDecomp_m_32f(
		A.pData, A.Stride1,	A.Stride2,
		index,
		A.pData, A.Stride1,	A.Stride2,
		A.nElements );

	if( isStatusOK() )
	{
		Current_Status = ippmLUBackSubst_mv_32f( 
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
bool CMatrix<float>::Conv2D( CMatrix<float> * operand2, CMatrix<float> * result )
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
	// Make sure the result matrix has the proper dimensions
	result->set_Dimensions( size_res.height, size_res.width );
		
	// ----------------------------------------------------------------------
	// Calculate convolution
		
	Current_Status = ippiConvFull_32f_C1R( 
		pData, Stride1, size_op1,						// Operand 1
		operand2->pData, operand2->Stride1, size_op2,	// Operand 2
		result->pData, result->Stride1 );				// Result

	return isStatusOK();
}


template<>
bool CMatrix<float>::Conv(CMatrix<float> * operand2, CMatrix<float> * result)
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

	Current_Status = ippsConv_32f(
		pData, src1Len,						// Operand 1
		operand2->pData, src2Len,	// Operand 2
		result->pData);				// Result

	return isStatusOK();
}

//TODO: needs review for even operand2.
template<>
bool CMatrix<float>::ConvCirc(CMatrix<float> * operand2, CMatrix<float> * result)
{
	//Auxiliary variable for circular convolution
	CMatrix<float> extended_signal;
	CMatrix<float> full_result;

	// Define size variables
	int
		src1Len,
		src2Len;

	src1Len = Width;
	src2Len = operand2->Width;
	int hkernelWidth = src2Len / 2; //src2Len/2 will floor the value on pourpose

	// Copy the data
	extended_signal.set_Dimensions(1, src1Len + 2 * hkernelWidth);

	Current_Status = ippsCopy_32f(pData + (src1Len - hkernelWidth), extended_signal.pData, hkernelWidth);

	if (!isStatusOK())
		return false;

	Current_Status = ippsCopy_32f(pData, extended_signal.pData + hkernelWidth, src1Len);

	if (!isStatusOK())
		return false;

	Current_Status = ippsCopy_32f(pData, extended_signal.pData + src1Len + hkernelWidth, hkernelWidth);

	if (!isStatusOK())
		return false;


	// ----------------------------------------------------------------------
	// Make sure the result matrix has the proper dimensions
	full_result.set_Dimensions(1, src1Len + 2 * hkernelWidth + src2Len - 1);
	result->set_Dimensions(1, src1Len);
	// ----------------------------------------------------------------------
	// Calculate convolution

	Current_Status = ippsConv_32f(
		extended_signal.pData, src1Len + 2 * hkernelWidth,						// Operand 1
		operand2->pData, src2Len,	// Operand 2
		full_result.pData);				// Result

	if (!isStatusOK())
		return false;

	//Copy only the valid part to the result
	Current_Status = ippsCopy_32f(full_result.pData + 2 * hkernelWidth, result->pData, src1Len);

	return isStatusOK();
}


template<>
bool CMatrix<float>::ConvCircP(CMatrix<float> * operand2, CMatrix<float> * result, CMatrix<float> * extended_operand1, CMatrix<float> * full_result)
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

	Current_Status = ippsCopy_32f(pData + (src1Len - hkernelWidth), extended_operand1->pData, hkernelWidth);

	if (!isStatusOK())
		return false;

	Current_Status = ippsCopy_32f(pData, extended_operand1->pData + hkernelWidth, src1Len);

	if (!isStatusOK())
		return false;

	Current_Status = ippsCopy_32f(pData, extended_operand1->pData + src1Len + hkernelWidth, hkernelWidth);

	if (!isStatusOK())
		return false;


	// ----------------------------------------------------------------------
	// Make sure the result matrix has the proper dimensions
	full_result->set_Dimensions(1, src1Len + 2 * hkernelWidth + src2Len - 1);
	result->set_Dimensions(1, src1Len);
	// ----------------------------------------------------------------------
	// Calculate convolution

	Current_Status = ippsConv_32f(
		extended_operand1->pData, src1Len + 2 * hkernelWidth,						// Operand 1
		operand2->pData, src2Len,	// Operand 2
		full_result->pData);				// Result

	if (!isStatusOK())
		return false;

	//Copy only the valid part to the result
	Current_Status = ippsCopy_32f(full_result->pData + 2 * hkernelWidth, result->pData, src1Len);

	return isStatusOK();
}

//---------------------------------------------------------------------------
// Instantiate template for 'float'
template class CMatrix<float>;
