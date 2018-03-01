// Implementation of class template CMatrix<Type>
//---------------------------------------------------------------------------
// Specialized implementations for 'float' and 'double' types
#include "Matrixf.hpp"
#include "Matrixd.hpp"

//---------------------------------------------------------------------------
// System libraries
#include <iostream>
#include <iomanip>

//---------------------------------------------------------------------------
// Include required by Visual Studio 9 for trigonometry functions
#if MSVC_VERSION<1600
	#include <complex>
#endif

//---------------------------------------------------------------------------

template<typename Type>
CMatrix<Type>::CMatrix( int nHeight, int nWidth, Type value )
: pData( nullptr )
{
	Setup_Matrix_Parameters( nHeight, nWidth );
	
	if( ( nHeight>0 ) && ( nWidth>0 ) )
	{
		if( Alloc_Data_Memory() )
			set_Data( value );
	}
}

template<typename Type>
CMatrix<Type>::CMatrix( int nHeight, int nWidth, Type * pData_Array )
: pData( nullptr )
{
	Setup_Matrix_Parameters( nHeight, nWidth );
	
	if( Alloc_Data_Memory() )
		set_Data( pData_Array );
}

template<typename Type>
CMatrix<Type>::CMatrix( int nHeight, int nWidth, Type ** pData_Array )
: pData( nullptr )
{
	if( ( nHeight<1 ) || ( nWidth<1 ) || ( pData_Array==nullptr ) ) return;

	Setup_Matrix_Parameters( nHeight, nWidth );
	
	if( Alloc_Data_Memory() )
	{
		for( register int i=0 ; i<nWidth ; i++ )
		{
			for( register int j=0 ; j<nHeight ; j++ )
			{
				set_Element( i, j, pData_Array[i][j] );
			}
		}
	}
}

template<typename Type>
CMatrix<Type>::CMatrix( int nHeight, int nWidth, CMatrix<Type> mat )
: pData( nullptr )
{
	Setup_Matrix_Parameters( nHeight, nWidth );
	
	if( Alloc_Data_Memory() )
		set_Data( &mat );
}

template<typename Type>
CMatrix<Type>::CMatrix( const CMatrix<Type> &matrix_ref )
: pData( nullptr )
{
	Setup_Matrix_Parameters( matrix_ref.Height, matrix_ref.Width );
	
	if( Alloc_Data_Memory() )
		set_Data( matrix_ref.pData );
}

template<typename Type>
CMatrix<Type>::CMatrix( Type *pQuat, Type *pPos )
: pData( nullptr )
{
	Type	xx, xy, xz, xw,
			yy, yz, yw,
			zz, zw;
	
	xx = pQuat[1] * pQuat[1];
	xy = pQuat[1] * pQuat[2];
	xz = pQuat[1] * pQuat[3];
	xw = pQuat[1] * pQuat[0];

	yy = pQuat[2] * pQuat[2];
	yz = pQuat[2] * pQuat[3];
	yw = pQuat[2] * pQuat[0];
 
	zz = pQuat[3] * pQuat[3];
	zw = pQuat[3] * pQuat[0];
	// INVARIANT: abs( pQuat[0] * pQuat[0] + xx + yy + zz - 2.0 ) < 1.5e-3f;

	const Type
		t_0 = static_cast<Type>( 0 ),
		t_1 = static_cast<Type>( 1 ),
		t_2 = static_cast<Type>( 2 );

	if( pPos != nullptr ) {
		Type Matrix[16] = {	t_1-t_2*(yy+zz),		t_2*(xy-zw),		t_2*(xz+yw), pPos[0],
								t_2*(xy+zw),	t_1-t_2*(xx+zz),		t_2*(yz-xw), pPos[1],
								t_2*(xz-yw),		t_2*(yz+xw),	t_1-t_2*(xx+yy), pPos[2],
										t_0,				t_0,				t_0,    t_1};
		set_Dimensions( 4, 4 );
		set_Data( 4, 4, Matrix );
	} else {
		Type Matrix[9] = {	t_1-t_2*(yy+zz),		t_2*(xy-zw),		t_2*(xz+yw),
								t_2*(xy+zw),	t_1-t_2*(xx+zz),		t_2*(yz-xw),
								t_2*(xz-yw),		t_2*(yz+xw),	t_1-t_2*(xx+yy)};

		set_Data( 3, 3, Matrix );
	}
}

template<typename Type>
CMatrix<Type>::~CMatrix( void )
{
	Free_Data_Memory();
}

template<typename Type>
void CMatrix<Type>::Free_Data_Memory( void )
{
	if( Valid_Data_Memory() )
	{
		ippsFree( pData );
		pData = nullptr;
	}

	Setup_Matrix_Parameters( 0, 0 );
}

template<typename Type>
inline bool CMatrix<Type>::Valid_Data_Memory( void )
{
	return ( pData != nullptr );
}


template<typename Type>
inline IppStatus CMatrix<Type>::get_Last_Error( void )
{
	return Status_Last_Error;
}

template<typename Type>
inline bool CMatrix<Type>::isStatusOK( void )
{
	if( Current_Status == ippStsOk ) return true;
	else
	{
		Status_Last_Error = Current_Status;
		return false;
	}
}

template<typename Type>
void CMatrix<Type>::Setup_Matrix_Parameters( int height, int width )
{
	nElement_Size = sizeof( Type );
	Height = height;
	Width = width;

	Setup_Matrix_Parameters();
}

template<typename Type>
void CMatrix<Type>::Setup_Matrix_Parameters( void )
{
	nElements = Height*Width;
	Stride0 = nElements*nElement_Size;
	Stride1 = Width*nElement_Size;
	Stride2 = nElement_Size;
}

template<typename Type>
bool CMatrix<Type>::set_Dimensions( int nHeight, int nWidth )
{
	if( nHeight != Height || nWidth != Width )
	{
		Free_Data_Memory();
		Setup_Matrix_Parameters( nHeight, nWidth );
		
		return Alloc_Data_Memory();
	}
	else return true;
}

template<typename Type>
int CMatrix<Type>::get_Height( void ) const
{ 
	return Height;
}

template<typename Type>
int CMatrix<Type>::get_Width( void ) const
{ 
	return Width;
}

template<typename Type>
int CMatrix<Type>::get_NumberElements( void ) const
{ 
	return nElements;
}

template<typename Type>
void CMatrix<Type>::set_Element( int nLine, int nColumn, Type value )
{ 
	pData[nLine*Width+nColumn] = value;
}

template<typename Type>
Type CMatrix<Type>::get_Element( int nLine, int nColumn ) const
{ 
	return pData[nLine*Width+nColumn];
}

template<typename Type>
void CMatrix<Type>::print( void )
{
	int 
		iRow = 0,
		iCol = 0,
		rows = Height,
		cols = Width;

	std::cout << std::endl;
	for( iRow=0 ; iRow<rows ; iRow++ )
	{
		for( iCol=0 ; iCol<cols ; iCol++ )
		{
			std::cout << get_Element( iRow, iCol ) << "\t";
		}
		std::cout << std::endl;
	}
}

template<typename Type>
void CMatrix<Type>::get_DataVector( std::vector<Type> &v_data )
{
	if( v_data.size() != nElements )
		v_data.assign( nElements, static_cast<Type>( 0 ) );
	
	for( register int i=0 ; i<nElements ; i++ )
	{
		v_data.at( i ) = pData[i];
	}
}

template<typename Type>
CMatrix<Type> CMatrix<Type>::operator=( CMatrix<Type> mat )
{
	set_Data( &mat );
	return *this;
}

template<typename Type>
CMatrix<Type> CMatrix<Type>::operator+( CMatrix<Type> mat )
{
	CMatrix<Type> mtemp( Height, Width );
	Add( &mat, &mtemp );
	return mtemp;
}

template<typename Type>
CMatrix<Type> CMatrix<Type>::operator+( Type value )
{
	CMatrix<Type> mtemp( Height, Width );
	Add( value, &mtemp );
	return mtemp;
}

template<typename Type>
CMatrix<Type> CMatrix<Type>::operator+=( CMatrix<Type> mat )
{
	Add( &mat );
	return *this;
}

template<typename Type>
CMatrix<Type> CMatrix<Type>::operator+=( Type value )
{
	Add( value );
	return *this;
}

template<typename Type>
CMatrix<Type> CMatrix<Type>::operator-( CMatrix<Type> mat )
{
	CMatrix<Type> mtemp( Height, Width );
	Sub( &mat, &mtemp );
	return mtemp;
}
	
template<typename Type>
CMatrix<Type> CMatrix<Type>::operator-( Type value )
{
	CMatrix<Type> mtemp( Height, Width );
	Sub( value, &mtemp );
	return mtemp;
}
	
template<typename Type>
CMatrix<Type> CMatrix<Type>::operator-=( CMatrix<Type> mat )
{
	Sub( &mat );
	return *this;
}

template<typename Type>
CMatrix<Type> CMatrix<Type>::operator-=( Type value )
{
	Sub( value );
	return *this;
}

template<typename Type>
CMatrix<Type> CMatrix<Type>::operator*( CMatrix<Type> mat )
{
	CMatrix<Type> mtemp( Height, mat.Width );
	Mul( &mat, &mtemp );
	return mtemp;
}

template<typename Type>
CMatrix<Type> CMatrix<Type>::operator*( Type value )
{
	CMatrix<Type> mtemp( Height, Width );
	Mul( value, &mtemp );
	return mtemp;
}

template<typename Type>
CMatrix<Type> CMatrix<Type>::operator*=( CMatrix<Type> mat )
{
	Mul( &mat );
	return *this;
}

template<typename Type>
CMatrix<Type> CMatrix<Type>::operator*=( Type value )
{
	Mul( value );
	return *this;
}

template<typename Type>
CMatrix<Type> CMatrix<Type>::operator/( Type value )
{
	CMatrix<Type> mtemp( Height, Width );
	Div( value, &mtemp );
	return mtemp;
}

template<typename Type>
CMatrix<Type> CMatrix<Type>::operator/=( Type value )
{
	Div( value );
	return *this;
}

template<typename Type>
Type& CMatrix<Type>::operator()( int nLine, int nColumn )
{ 
	return pData[nLine*Width+nColumn];
}

template<typename Type>
bool CMatrix<Type>::operator==( CMatrix<Type> mat )
{
	int 
		val1 = 10*Height + Width,
		val2 = 10*mat.Height + mat.Width;
	
	if( val1 == val2 )
	{
		for( register int i=0 ; i<Height ; i++ )
		{
			for( register int j=0 ; j<Width ; j++ )
			{
				if( pData[i*Width+j] != mat.pData[i*Width+j] )
				{
					return false;
				}
			}
		}
		return true;
	}
	else return false;
}

//template<typename Type>
//template<class _Pr>
//bool CMatrix<Type>::Apply(CMatrix<Type> *result, _Pr pred)
//{
//	for (int i = 0; i < nElements; i++)
//		result->pData[i] = pred(pData[i]);
//
//	return isStatusOK();
//}

template<typename Type>
bool CMatrix<Type>::Inverse( CMatrix<Type> * result )
{
	result->set_Data( this );
	return result->Inverse();
}

template<typename Type>
bool CMatrix<Type>::set_Identity( void )
{
	// Test if the matrix has conditions to be an identity matrix
	if( Height != Width ) return false;
	if( Height < 1 ) return false;

	const Type 
		tZero = static_cast<Type>( 0 ),
		tOne = static_cast<Type>( 1 );
	
	// Make sure there is memory for the matrix and the parameters are all correctly set
	set_Dimensions( Height, Width );
	
	if( pData == nullptr ) return false;

	// Initialize the matrix with '0'
	set_Data( tZero );

	// Go through the diagonal values and set them to '1'
	for( int i=0 ; i<Width ; i++ )
	{
		pData[ i*Width + i ] = tOne;
	}

	// Success
	return true;
}

template<typename Type>
CMatrix<Type> CMatrix<Type>::Identity( void )
{
	CMatrix<Type> mat( Height, Width );
	mat.set_Identity();
	return mat;
}

template<typename Type>
void CMatrix<Type>::Coef_calc( Type x_i, Type v_i, Type a_i, Type x_f, Type v_f, Type a_f )
{
	Type
		v0 = static_cast<Type>( 0 ),
		v1 = static_cast<Type>( 1 ),
		v2 = static_cast<Type>( 2 ),
		v3 = static_cast<Type>( 3 ),
		v4 = static_cast<Type>( 4 ),
		v5 = static_cast<Type>( 5 ),
		v6 = static_cast<Type>( 6 ),
		v12 = static_cast<Type>( 12 ),
		v20 = static_cast<Type>( 20 );

	Type Array[36] = {	
		v1, v0, v0, v0, v0, v0, 
		v0, v1, v0, v0, v0, v0,  
		v0, v0, v2, v0, v0, v0,
		v1, v1, v1, v1, v1, v1,
		v0, v1, v2, v3, v4, v5,
		v0, v0, v2, v6, v12,v20 };

	Type Vect[6] = { x_i, v_i, a_i, x_f, v_f, a_f };
	
	CMatrix<Type> 
		A( 6, 6, Array ),
		b( 6, 1, Vect );

	*this = SolLinearSyst_6x6( A, b );
}

template<typename Type>
void CMatrix<Type>::set_TransfMatrix( Type Alpha, Type A, Type D, Type Theta )
{
	set_Dimensions( 4, 4 );
	set_TransfMatrixData( Alpha, A, D, Theta );
}

template<typename Type>
void CMatrix<Type>::set_TransfMatrixData( Type Alpha, Type A, Type D, Type Theta )
{
	Type 
		c_theta = cos( Theta ),
		s_theta = sin( Theta ),
		c_alpha = cos( Alpha ),
		s_alpha = sin( Alpha ),
		v0 = static_cast<Type>( 0 ),
		v1 = static_cast<Type>( 1 );

	Type Temp[16] = {
		c_theta,				-s_theta,		v0,			A,
		s_theta*c_alpha,	c_theta*c_alpha,	-s_alpha,	-s_alpha*D,
		s_theta*s_alpha,	c_theta*s_alpha,	c_alpha,	c_alpha*D,
		v0,					v0,					v0,			v1 };

	set_Data( Temp );
}

template<typename Type>
void CMatrix<Type>::set_TransfMatrix_3x3( Type Alpha, Type Theta )
{
	Type 
		c_theta = cos( Theta ),
		s_theta = sin( Theta ),
		c_alpha = cos( Alpha ),
		s_alpha = sin( Alpha );

	Type Temp[9] = {
		c_theta,				-s_theta,		static_cast<Type>( 0 ),	
		s_theta*c_alpha,	c_theta*c_alpha,	-s_alpha,
		s_theta*s_alpha,	c_theta*s_alpha,	c_alpha };

	set_Data( 3, 3, Temp );
}

template<typename Type>
void CMatrix<Type>::set_RotationMatrix_xyz_3x3( Type Phi, int axis )
{
	Type 
		c_phi = cos( Phi ),
		s_phi = sin( Phi );

	set_Dimensions( 3, 3 );
	set_Identity();

	if( axis == 1 )// x_axis rotation
	{
		pData[4] = c_phi;
		pData[5] = -s_phi;
		pData[7] = s_phi;
		pData[8] = c_phi;
	}
	if( axis == 2 )// y_axis rotation
	{
		pData[0] = c_phi;
		pData[2] = s_phi;
		pData[6] = -s_phi;
		pData[8] = c_phi;
	}
	if( axis == 3 )// z_axis rotation
	{
		pData[0] = c_phi;
		pData[1] = -s_phi;
		pData[3] = s_phi;
		pData[4] = c_phi;
	}
}

template<typename Type>
void CMatrix<Type>::set_RotationMatrix_3x3( Type Theta, Type Psi, Type Phi )
{
	Type 
		c_the = cos( Theta ),
		s_the = sin( Theta ),
		c_psi = cos( Psi ),
		s_psi = sin( Psi ),
		c_phi = cos( Phi ),
		s_phi = sin( Phi );

	Type Rot_Matrix[9] = {	c_phi*c_psi,	-s_phi*c_the+c_phi*s_psi*s_the,	s_phi*s_the+c_phi*s_psi*c_the,
							s_phi*c_psi,	c_phi*c_the+s_phi*s_psi*s_the,	-c_phi*s_the+s_phi*s_psi*c_the,
							-s_psi,			c_psi*s_the,					c_psi*c_the};

	set_Data( 3, 3, Rot_Matrix );
}

template<typename Type>
void CMatrix<Type>::set_RotationMatrix_RPY_3x3( Type Roll, Type Pitch, Type Yaw )
{
	set_RotationMatrix_3x3( Yaw, Pitch, Roll );
}

template<typename Type>
bool CMatrix<Type>::AnyPositive( std::vector<int> &indices )
{
	indices.clear();

	for( int i=0 ; i<nElements ; i++ )
	{
		if( pData[i]>0 ) indices.emplace_back( i );
	}

	return ( indices.size() > 0 );
}

template<typename Type>
bool CMatrix<Type>::Transpose( CMatrix<Type> * result )
{
	result->set_Data( this );
	return result->Transpose();
}

template<typename Type>
CMatrix<Type> CMatrix<Type>::RandomGaussian( int height, int width, Type Mean, Type Deviation )
{
	CMatrix<Type> mtemp( height, width );
	mtemp.RandomizeGaussian( Mean, Deviation );
	return mtemp;
}

template<typename Type>
void CMatrix<Type>::QuaternionToMatrix( Type *pQuat, Type *pPos )
{
	Type	xx, xy, xz, xw,
			yy, yz, yw,
			zz, zw;
	
	xx = pQuat[1] * pQuat[1];
	xy = pQuat[1] * pQuat[2];
	xz = pQuat[1] * pQuat[3];
	xw = pQuat[1] * pQuat[0];

	yy = pQuat[2] * pQuat[2];
	yz = pQuat[2] * pQuat[3];
	yw = pQuat[2] * pQuat[0];
 
	zz = pQuat[3] * pQuat[3];
	zw = pQuat[3] * pQuat[0];
	// INVARIANT: abs( pQuat[0] * pQuat[0] + xx + yy + zz - 2.0 ) < 1.5e-3f;

	const Type
		t_0 = static_cast<Type>( 0 ),
		t_1 = static_cast<Type>( 1 ),
		t_2 = static_cast<Type>( 2 );

	if( pPos != nullptr ) {
		Type Matrix[16] = {	t_1-t_2*(yy+zz),		t_2*(xy-zw),		t_2*(xz+yw), pPos[0],
								t_2*(xy+zw),	t_1-t_2*(xx+zz),		t_2*(yz-xw), pPos[1],
								t_2*(xz-yw),		t_2*(yz+xw),	t_1-t_2*(xx+yy), pPos[2],
										t_0,				t_0,				t_0,    t_1};
		set_Dimensions( 4, 4 );
		set_Data( 4, 4, Matrix );
	} else {
		Type Matrix[9] = {	t_1-t_2*(yy+zz),		t_2*(xy-zw),		t_2*(xz+yw),
								t_2*(xy+zw),	t_1-t_2*(xx+zz),		t_2*(yz-xw),
								t_2*(xz-yw),		t_2*(yz+xw),	t_1-t_2*(xx+yy)};

		set_Data( 3, 3, Matrix );
	}
}

template<typename Type>
void CMatrix<Type>::RotateVector( Type *v, Type *rotated_v ) const 
{
	rotated_v[0] = v[0] * get_Element(0,0) + v[1] * get_Element(0,1) + v[2] * get_Element(0,2);
	rotated_v[1] = v[0] * get_Element(1,0) + v[1] * get_Element(1,1) + v[2] * get_Element(1,2);
	rotated_v[2] = v[0] * get_Element(2,0) + v[1] * get_Element(2,1) + v[2] * get_Element(2,2);
	rotated_v[3] = static_cast<Type>( 1 );
}

template<typename Type>
void CMatrix<Type>::UnrotateVector( Type *v, Type *unrotated_v ) const 
{
	unrotated_v[0] = v[0] * get_Element(0,0) + v[1] * get_Element(1,0) + v[2] * get_Element(2,0);
	unrotated_v[1] = v[0] * get_Element(0,1) + v[1] * get_Element(1,1) + v[2] * get_Element(2,1);
	unrotated_v[2] = v[0] * get_Element(0,2) + v[1] * get_Element(1,2) + v[2] * get_Element(2,2);
	unrotated_v[3] = static_cast<Type>( 1 );
}

template<typename Type>
void CMatrix<Type>::GrammSchmidt( Type *dir )
{
	const Type 
		t_0 = static_cast<Type>( 0 ),
		t_1 = static_cast<Type>( 1 );

	Type X[4] = { t_0, t_0, t_0, t_0 };
	Type Y[4] = { t_0, t_0, t_0, t_0 };
	Type Z[4] = { dir[0], dir[1], dir[2], dir[3] };

	//Z norm
	Type Z_norm = sqrt( Z[0]*Z[0] + Z[1]*Z[1] + Z[2]*Z[2] );
	Z[0] /= Z_norm;
	Z[1] /= Z_norm;
	Z[2] /= Z_norm;
	

	if( abs( Z[2] ) > static_cast<Type>(0.577f) ) {
		X[0] = -Z[2]*Z[2];
		X[1] = -Z[2]*Z[1];
		X[2] =  Z[0]*Z[2] + Z[1]*Z[1];
	} else {
		X[0] = -Z[2]*Z[0];
		X[1] = -Z[2]*Z[1];
		X[2] =  Z[0]*Z[0] + Z[1]*Z[1];
	}
	//X norm
	Type X_norm = sqrt( X[0]*X[0] + X[1]*X[1] + X[2]*X[2] );
	X[0] /= X_norm;
	X[1] /= X_norm;
	X[2] /= X_norm;
	
	//Y = Z * X;
	Y[0] = Z[1]*X[2] - Z[2]*X[1];
	Y[1] = Z[2]*X[0] - Z[0]*X[2];
	Y[2] = Z[0]*X[1] - Z[1]*X[0];

	Type gSM[16] = {	X[0],	Y[0],	Z[0],	t_0,
						X[1],	Y[1],	Z[1],	t_0,
						X[2],	Y[2],	Z[2],	t_0,
						 t_0,	 t_0,	 t_0,	t_1};
	
	set_Data(4,4,gSM);
}

//---------------------------------------------------------------------------
// Instantiate used templates
template class CMatrix<float>;
template class CMatrix<double>;
