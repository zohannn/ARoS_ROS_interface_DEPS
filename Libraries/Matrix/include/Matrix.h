#ifndef _MATRIX_H_
#define _MATRIX_H_

#pragma once

#include <ippdefs.h>
#include <vector>

/// <summary>CMatrix&lt;Type&gt; is a template class for matrix operations.
/// <para>Uses IPP to perform operations on matrices.</para>
/// <para>It has two specialized templates for native types 'float' and 'double'
/// to take advantage of specific functions contained by IPP.</para></summary>
template <typename Type>
class CMatrix
{
	// Matrix parameters
protected:
	int 
		/// <summary>Width of the matrix.</summary>
		Width,
		/// <summary>Height of the matrix.</summary>
		Height,
		/// <summary>Stride between matrices, vectors, or constants in the array.</summary>
		Stride0,
		/// <summary>Stride between matrix rows.</summary>
		Stride1,
		/// <summary>Stride between vector elements or matrix row elements.</summary>
		Stride2,
		/// <summary>Number of elements in the matrix.</summary>
		nElements,
		/// <summary>Size of each element in the matrix (bytes).</summary>
		nElement_Size;
		  
		// <summary>Pointer to array that will hold the data fin the matrix.</summary>
		Type * pData;

public:
	//-----------------------------------------------------------------------
	// 				Constructors
	//-----------------------------------------------------------------------

	/// <summary>Default constructor. Creates an instance of CMatrix<Type>.</summary>
	/// <param name="nHeight">Height of the matrix (number of rows).</param>
	/// <param name="nWidth">Width of the matrix (number of columns).</param>
	/// <param name="scalar">Value to initialize all elements in the matrix (0 by default).</param>
	CMatrix( int nHeight=0, int nWidth=0, Type scalar=static_cast<Type>( 0 ) );
	
	/// <summary>Data array constructor. Creates an instance of CMatrix<Type>
	/// and copies a data array.</summary>
	/// <param name="nHeight">Height of the matrix (number of rows).</param>
	/// <param name="nWidth">Width of the matrix (number of columns).</param>
	/// <param name="pData_Array">Pointer to data array to be copied.</param>
	CMatrix( int nHeight, int nWidth, Type * pData_Array );
	
	/// <summary>Data matrix constructor. Creates an instance of CMatrix<Type>
	/// and copies a data matrix.</summary>
	/// <param name="nHeight">Height of the matrix (number of rows).</param>
	/// <param name="nWidth">Width of the matrix (number of columns).</param>
	/// <param name="pData_Array">Pointer to data matrix to be copied.</param>
	CMatrix( int nHeight, int nWidth, Type ** pData_Array );
	
	/// <summary>CMatrix data constructor. Creates an instance of CMatrix<Type>
	/// and copies data from another CMatrix object.</summary>
	/// <param name="nHeight">Height of the matrix (number of rows).</param>
	/// <param name="nWidth">Width of the matrix (number of columns).</param>
	/// <param name="mat">Object were to copy the data from.</param>
	CMatrix( int nHeight, int nWidth, CMatrix<Type> mat );
	
	/// <summary>Quaternion constructor. Creates an instance of CMatrix<Type>
	/// by using a quaternion.</summary>
	/// <param name="pQuat">Pointer to an array containing quaternion information.</param>
	/// <param name="pPos">Pointer to an array containing quaternion information..</param>
	CMatrix( Type *pQuat, Type *pPos = nullptr );
	
	/// <summary>Copy constructor. Creates an instance of CMatrix<Type>
	/// from another CMatrix object.</summary>
	/// <param name="matrix_ref">Object to be copied</param>
	CMatrix( const CMatrix<Type> &matrix_ref );

	/// <summary>Destructor. Destroys the instance of CMatrix<Type>.</summary>
	~CMatrix( void );

public:
	//-----------------------------------------------------------------------
	// Matrix data handling, these functions returns true if successful
	
	/// <summary>Defines matrix parameters and copies the data from an 
	/// array to the CMatrix object.</summary>
	/// <param name="nHeight">Height of the matrix (number of rows).</param>
	/// <param name="nWidth">Width of the matrix (number of columns).</param>
	/// <param name="pData_Array">Pointer to data array to be copied.</param>
	/// <returns>true if successful, otherwise false.</returns>
	bool set_Data( int nHeight, int nWidth, Type * pData_Array );

	/// <summary>Copies the data from an array to the CMatrix object.</summary>
	/// <param name="pData_Array">Pointer to data array to be copied.</param>
	/// <returns>true if successful, otherwise false.</returns>
	bool set_Data( Type * pData_Array );

	/// <summary>Copies data from another CMatrix object.</summary>
	/// <param name="mat">Pointer to CMatrix object to be copied.</param>
	/// <returns>true if successful, otherwise false.</returns>
	bool set_Data( CMatrix<Type> * mat );

	/// <summary>Assigns a value to all elements in the matrix.</summary>
	/// <param name="value">Value to assign.</param>
	/// <returns>true if successful, otherwise false.</returns>
	bool set_Data( Type value );

	/// <summary>Copies data from an array</summary>
	/// <param name="pData_Array">Pointer to data array to be copied.</param>
	/// <param name="begin">Start position in destination.</param>
	/// <param name="len">Number of elementes to copy.</param>
	/// <returns>true if successful, otherwise false.</returns>
	bool Copy_Data(Type * pData_Array, int begin, int len );

	/// <summary>Copies data from an array</summary>
	/// <param name="pData_Array">CMatrix to copy from.</param>
	/// <param name="begin">Start position in destination.</param>
	/// <param name="len">Number of elementes to copy.</param>
	/// <returns>true if successful, otherwise false.</returns>
	bool Copy_Data(CMatrix<Type> * pSrc, int begin, int len);


	//-----------------------------------------------------------------------
	// Setup Matrix parameters
protected:
	/// <summary>Determine matrix parameters from width and height:
	/// <para><see cref="CMatrix<Type>::Stride0"/>.</para></summary>
	/// <para><see cref="CMatrix<Type>::Stride1"/>.</para></summary>
	/// <para><see cref="CMatrix<Type>::Stride2"/>.</para></summary>
	/// <para><see cref="CMatrix<Type>::nElements"/>.</para></summary>
	/// <para><see cref="CMatrix<Type>::nElement_Size"/>.</para></summary>
	/// <param name="height">Height of the matrix (number of rows).</param>
	/// <param name="width">Width of the matrix (number of columns).</param>
	void Setup_Matrix_Parameters( int height, int width );

	/// <summary>Determine matrix parameters from the already defined width and height:
	/// <para><see cref="CMatrix<Type>::Stride0"/>.</para></summary>
	/// <para><see cref="CMatrix<Type>::Stride1"/>.</para></summary>
	/// <para><see cref="CMatrix<Type>::Stride2"/>.</para></summary>
	/// <para><see cref="CMatrix<Type>::nElements"/>.</para></summary>
	/// <para><see cref="CMatrix<Type>::nElement_Size"/>.</para></summary>
	void Setup_Matrix_Parameters( void );

	//-----------------------------------------------------------------------
	// Data Memory management
protected:
	/// <summary>Alloc memory for internal data array where the 
	/// matrix data will be stored.</summary>
	/// <returns>true if successful, otherwise false</returns>
	bool Alloc_Data_Memory( void );
	
	/// <summary>Release memory for internal data array where the 
	/// matrix data is stored.</summary>
	void Free_Data_Memory( void );

	/// <summary>Checks if CMatrix has a valid allocated memory.</summary> 
	/// <returns>true if is valid, otherwise false.</returns>
	bool Valid_Data_Memory( void );

	//-----------------------------------------------------------------------
	// Error management
protected:
	IppStatus 
		/// <summary>Holds the last error that an IPP function had.</summary> 
		Status_Last_Error,
		/// <summary>Holds status of the last IPP function invoked.</summary> 
		Current_Status;

public:
	/// <summary>Access to last IPP error.</summary>
	/// <returns>IPP error code of the last error.</returns>
	IppStatus get_Last_Error( void );

	/// <summary>Checks current status for errors.</summary>
	/// <returns>true if current status is ok, false if it's an error.</returns>
	bool isStatusOK( void );

	//-----------------------------------------------------------------------

public:
	/// <summary>Changes matrix dimensions, reallocating memory if necessary.</summary>
	/// <param name="nHeight">Height of the matrix (number of rows).</param>
	/// <param name="nWidth">Width of the matrix (number of columns).</param>
	/// <returns>true if is valid, otherwise false.</returns>
	bool set_Dimensions( int nHeight, int nWidth );

	//-----------------------------------------------------------------------
	// 				Accessors
	//-----------------------------------------------------------------------
public:	
	/// <summary>Accessor for height of the matrix.</summary>
	/// <returns>Height of the matrix (number of rows).</returns>
	int get_Height( void ) const;

	/// <summary>Accessor for width of the matrix.</summary>
	/// <returns>Width of the matrix (number of columns).</returns>
	int get_Width( void ) const;

	/// <summary>Accessor for number of elements in matrix.</summary>
	/// <returns>Number of elements.</returns>
	int get_NumberElements( void ) const;

	/// <summary>Get matrix data in a vector. Similar to <see cref="CMatrix&lt;Type&gt;::get_DataArray"/>.</summary>
	/// <param name="v_data">Vector to where the data will be copied.</param>
	void get_DataVector( std::vector<Type> &v_data );
	
	/// <summary>Get matrix data in an array. Similar to <see cref="CMatrix&lt;Type&gt;::get_DataVector"/>.</summary>
	/// <param name="p_data">Array to where the data will be copied.</param>
	bool get_DataArray( Type * p_data );

	/// <summary>Print all of matrix elements in the console.</summary>
	void print( void );

	/// <summary>Sets the value of one element in the matrix at ( nLine, nColumn ).</summary>
	/// <param name="nLine">Index indentifying the line.</param>
	/// <param name="nColumn">Index indentifying the column.</param>
	/// <param name="value">Value to assign to element.</param>
	void set_Element( int nLine, int nColumn, Type value );

	/// <summary>Gets the value of one element in the matrix at ( nLine, nColumn ).
	/// Same as <see cref="CMatrix&lt;Type&gt;::operator()(int,int)"/>.</summary>
	/// <param name="nLine">Index indentifying the line.</param>
	/// <param name="nColumn">Index indentifying the column.</param>
	/// <returns>The element at the position ( nLine, nColumn ).</returns>
	Type get_Element( int nLine, int nColumn ) const;

	//-----------------------------------------------------------------------
	// 				Matrix Operators
	//-----------------------------------------------------------------------
public:
	/// <summary>Addition operator, with another matrix.
	/// Adds each element of both matrices.
	/// <para>mat_res = this + mat.</para></summary>
	/// <para>See also <see cref="CMatrix::Add"/></para></summary>
	/// <param name="mat">Second operand: CMatrix</param>
	/// <returns>Matrix with the result of the operation.</returns>
	CMatrix<Type> operator+( CMatrix<Type> mat );

	/// <summary>Addition operator, with a scalar.
	/// Adds the value of scalar to all elements of the matrix .
	/// <para>mat_res = this + scalar.</para></summary>
	/// <para>See also <see cref="CMatrix::Add"/></para></summary>
	/// <param name="scalar">Second operand: scalar</param>
	/// <returns>Matrix with the result of the operation.</returns>
	CMatrix<Type> operator+( Type scalar );

	/// <summary>Addition operator (in place), with another matrix. 
	/// Changes the content of first operand.
	/// <para>this = this + mat.</para></summary>
	/// <para>See also <see cref="CMatrix::Add"/></para></summary>
	/// <param name="mat">Second operand: CMatrix.</param>
	/// <returns>Matrix with the result of the operation.</returns>
	CMatrix<Type> operator+=( CMatrix<Type> mat );

	/// <summary>Addition operator (in place), with a scalar. 
	/// Changes the contents of first operand.
	/// <para>this = this + scalar.</para></summary>
	/// <para>See also <see cref="CMatrix::Add"/></para></summary>
	/// <param name="scalar">Second operand: scalar</param>
	/// <returns>Matrix with the result of the operation.</returns>
	CMatrix<Type> operator+=( Type scalar );

	/// <summary>Subtract operator, with another matrix.
	/// Subtracts the value of scalar to all elements of the matrix.
	/// <para>mat_res = this - mat.</para></summary>
	/// <para>See also <see cref="CMatrix::Sub"/></para></summary>
	/// <param name="mat">Second operand: CMatrix</param>
	/// <returns>Matrix with the result of the operation.</returns>
	CMatrix<Type> operator-( CMatrix<Type> mat );

	/// <summary>Subtract operator, with a scalar.
	/// <para>mat_res = this - scalar.</para></summary>
	/// <para>See also <see cref="CMatrix::Sub"/></para></summary>
	/// <param name="scalar">Second operand: scalar</param>
	/// <returns>Matrix with the result of the operation.</returns>
	CMatrix<Type> operator-( Type scalar );
	
	/// <summary>Subtract operator (in place), with another matrix. 
	/// Changes the content of first operand.
	/// <para>this = this - mat.</para></summary>
	/// <para>See also <see cref="CMatrix::Sub"/></para></summary>
	/// <param name="mat">Second operand: CMatrix.</param>
	/// <returns>Matrix with the result of the operation.</returns>
	CMatrix<Type> operator-=( CMatrix<Type> mat );

	/// <summary>Subtract operator (in place), with a scalar. 
	/// Changes the contents of first operand.
	/// <para>this = this + scalar.</para></summary>
	/// <para>See also <see cref="CMatrix::Sub"/></para></summary>
	/// <param name="scalar">Second operand: scalar</param>
	/// <returns>Matrix with the result of the operation.</returns>
	CMatrix<Type> operator-=( Type scalar );

	/// <summary>Multiply operator, with another matrix.
	/// <para>mat_res = this * mat.</para></summary>
	/// <para>See also <see cref="CMatrix::Mul"/></para></summary>
	/// <param name="mat">Second operand: CMatrix</param>
	/// <returns>Matrix with the result of the operation.</returns>
	CMatrix<Type> operator*( CMatrix<Type> mat );

	/// <summary>Multiply operator, with a scalar.
	/// Multiplies the value of scalar to all elements of the matrix.
	/// <para>mat_res = this * scalar.</para></summary>
	/// <para>See also <see cref="CMatrix::Mul"/></para></summary>
	/// <param name="scalar">Second operand: scalar</param>
	/// <returns>Matrix with the result of the operation.</returns>
	CMatrix<Type> operator*( Type scalar );

	/// <summary>Multiply operator (in place), with another matrix. 
	/// Changes the content of first operand.
	/// <para>this = this * mat.</para></summary>
	/// <para>See also <see cref="CMatrix::Mul"/></para></summary>
	/// <param name="mat">Second operand: CMatrix.</param>
	/// <returns>Matrix with the result of the operation.</returns>
	CMatrix<Type> operator*=( CMatrix<Type> mat );

	/// <summary>Multiply operator (in place), with a scalar. 
	/// Changes the contents of first operand.
	/// <para>this = this * scalar.</para></summary>
	/// <para>See also <see cref="CMatrix::Mul"/></para></summary>
	/// <param name="scalar">Second operand: scalar</param>
	/// <returns>Matrix with the result of the operation.</returns>
	CMatrix<Type> operator*=( Type scalar );

	/// <summary>Divide operator (in place), with a scalar.
	/// Divides all elements in the matrix by a scalar.
	/// <para>mat_res = this / scalar.</para></summary>
	/// <para>See also <see cref="CMatrix::Div"/></para></summary>
	/// <param name="scalar">Second operand: scalar</param>
	/// <returns>Matrix with the result of the operation.</returns>
	CMatrix<Type> operator/( Type scalar );

	/// <summary>Divide operator (in place), with a scalar. 
	/// Changes the contents of first operand.
	/// <para>this = this / scalar.</para></summary>
	/// <para>See also <see cref="CMatrix::Div"/></para></summary>
	/// <param name="scalar">Second operand: scalar</param>
	/// <returns>Matrix with the result of the operation.</returns>
	CMatrix<Type> operator/=( Type dScalar );

	/// <summary>Element accessor operator.
	/// <para>returns the element of the matrix in position (Line, Column).</para></summary>
	/// <param name="nLine">Index indentifying the line.</param>
	/// <param name="nColumn">Index indentifying the column.</param>
	/// <returns>Returns the element of the matrix in position (Line, Column).</returns>
	Type& operator()( int nLine, int nColumn );

	//returns a matrix defined from (Line_first, Col_first) to (Line_last, Col_last)
	/// <summary>Sub-matrix operator.
	/// <para>Creates a matrix inside this matrix defined by two points.</para></summary>
	/// <param name="Line_first">Index indentifying the line of the top right point.</param>
	/// <param name="Col_first">Index indentifying the column of the top right point.</param>
	/// <param name="Line_last">Index indentifying the line of the bottom left point.</param>
	/// <param name="Col_last">Index indentifying the column of the bottom left point.</param>
	/// <returns>Returns a sub-matrix defined between 
	/// ( Line_first, Col_first ) and ( Line_last, Col_last ).</returns>
	CMatrix<Type> operator()( int Line_first, int Col_first, int Line_last, int Col_last );

	/// <summary>Comparison operator, with another matrix.
	/// <para>this == mat.</para></summary>
	/// <param name="mat">Second operand: CMatrix</param>
	/// <returns>true if the two matrices are equal, false if different.</returns>
	bool operator==( CMatrix<Type> mat );

	/// <summary>Attribution operator. Assigns second operand to first operand.
	/// <para>this = mat.</para></summary>
	/// <param name="mat">Second operand: CMatrix</param>
	/// <returns>Matrix with new data.</returns>
	CMatrix<Type> operator=( CMatrix<Type> mat );

	//-----------------------------------------------------------------------
	// 	Matrix Operations - Turbo Mode
	//
	// This assumes the first operand as 'this' matrix,
	// The second operand is passed as a pointer in the
	// function's first parameter.
	// The result is saved in a matrix passed as a pointer
	// in the function's second parameter
	//  The exceptions are the functions with only one parameter
	//  or none. In these cases operations are made 'in place',
	//  the result is saved in 'this' matrix
	//  When a return type bool exists, its true if the operation
	//  was successfull
	//-----------------------------------------------------------------------
public:
	/// <summary>Addition operator, with another matrix.
	/// Adds each element of both matrices.
	/// <para>result = this + operand2.</para></summary>
	/// <param name="operand2">Second operand: CMatrix.</param>
	/// <param name="result">Pointer to a CMatrix where the result will be stored.</param>
	/// <returns>true if the operation was successful, false if fails.</returns>
	bool Add( CMatrix<Type> * operand2, CMatrix<Type> * result );

	/// <summary>Addition operator, with a scalar.
	/// Adds the value of scalar to each element of 'this' matrix.
	/// <para>result = this + value.</para></summary>
	/// <param name="value">Second operand: scalar.</param>
	/// <param name="result">Pointer to a CMatrix where the result will be stored.</param>
	/// <returns>true if the operation was successful, false if fails.</returns>
	bool Add( Type value, CMatrix<Type> * result );

	/// <summary>Addition operator (in place), with another matrix.
	/// Adds each element of both matrices, stores the result in 'this'.
	/// <para>this = this + operand2.</para></summary>
	/// <param name="operand2">Second operand: CMatrix.</param>
	/// <returns>true if the operation was successful, false if fails.</returns>
	bool Add( CMatrix<Type> * operand2 );

	/// <summary>Addition operator (in place), with a scalar.
	/// Adds the value of scalar to each element of 'this' matrix.
	/// <para>this = this + value.</para></summary>
	/// <param name="value">Second operand: scalar.</param>
	/// <returns>true if the operation was successful, false if fails.</returns>
	bool Add( Type value );

	/// <summary>Subtraction operator, with another matrix.
	/// Subtracts each element of both matrices.
	/// <para>result = this - operand2.</para></summary>
	/// <param name="operand2">Second operand: CMatrix.</param>
	/// <param name="result">Pointer to a CMatrix where the result will be stored.</param>
	/// <returns>true if the operation was successful, false if fails.</returns>
	bool Sub( CMatrix<Type> * operand2, CMatrix<Type> * result );

	/// <summary>Subtraction operator, with a scalar.
	/// Subtracts the value of scalar to each element of 'this' matrix.
	/// <para>result = this - value.</para></summary>
	/// <param name="value">Second operand: scalar.</param>
	/// <param name="result">Pointer to a CMatrix where the result will be stored.</param>
	/// <returns>true if the operation was successful, false if fails.</returns>
	bool Sub( Type value, CMatrix<Type> * result );

	/// <summary>Subtraction operator (in place), with another matrix.
	/// Subtracts each element of both matrices, stores the result in 'this'.
	/// <para>this = this - operand2.</para></summary>
	/// <param name="operand2">Second operand: CMatrix.</param>
	/// <returns>true if the operation was successful, false if fails.</returns>
	bool Sub( CMatrix<Type> * operand2 );

	/// <summary>Subtraction operator (in place), with a scalar.
	/// Subtracts the value of scalar to each element of 'this' matrix.
	/// <para>this = this - value.</para></summary>
	/// <param name="value">Second operand: scalar.</param>
	/// <returns>true if the operation was successful, false if fails.</returns>
	bool Sub( Type value );

	/// <summary>Multiply operator, with another matrix.
	/// Performs multiplication of two matrices.
	/// <para>result = this * operand2.</para></summary>
	/// <param name="operand2">Second operand: CMatrix.</param>
	/// <param name="result">Pointer to a CMatrix where the result will be stored.</param>
	/// <returns>true if the operation was successful, false if fails.</returns>
	bool Mul( CMatrix<Type> * operand2, CMatrix<Type> * result );

	/// <summary>Multiply operator, with a scalar.
	/// Multiplies the value of scalar to each element of 'this' matrix.
	/// <para>result = this * value.</para></summary>
	/// <param name="value">Second operand: scalar.</param>
	/// <param name="result">Pointer to a CMatrix where the result will be stored.</param>
	/// <returns>true if the operation was successful, false if fails.</returns>
	bool Mul( Type value, CMatrix<Type> * result );

	/// <summary>Multiply operator (in place), with another matrix.
	/// Performs multiplication of two matrices.
	/// <para>this = this * operand2.</para></summary>
	/// <param name="operand2">Second operand: CMatrix.</param>
	/// <returns>true if the operation was successful, false if fails.</returns>
	bool Mul( CMatrix<Type> * operand2 );

	/// <summary>Multiply operator (in place), with a scalar.
	/// Multiplies the value of scalar to each element of 'this' matrix.
	/// <para>this = this * value.</para></summary>
	/// <param name="value">Second operand: scalar.</param>
	/// <returns>true if the operation was successful, false if fails.</returns>
	bool Mul( Type value );

	/// <summary>Multiply operator, with another matrix.
	/// Performs elementwise multiplication of two matrices.
	/// <para>this = this .* operand2.</para></summary>
	/// <param name="operand2">Second operand: CMatrix.</param>
	/// <returns>true if the operation was successful, false if fails.</returns>
	bool DotMul(CMatrix<Type> * operand2);

	/// <summary>Multiply operator, with another matrix.
	/// Performs elementwise multiplication of two matrices.
	/// <para>result = this .* operand2.</para></summary>
	/// <param name="operand2">Second operand: CMatrix.</param>
	/// <param name="result">Pointer to a CMatrix where the result will be stored.</param>
	/// <returns>true if the operation was successful, false if fails.</returns>
	bool DotMul(CMatrix<Type> * operand2, CMatrix<Type> *result);

	/// <summary>Division operator, with a scalar.
	/// Divides each element of 'this' matrix by the value of scalar.
	/// <para>result = this / value.</para></summary>
	/// <param name="value">Second operand: scalar.</param>
	/// <param name="result">Pointer to a CMatrix where the result will be stored.</param>
	/// <returns>true if the operation was successful, false if fails.</returns>
	bool Div( Type value, CMatrix<Type> * result );

	/// <summary>Division operator (in place), with a scalar.
	/// Divides each element of 'this' matrix by the value of scalar.
	/// <para>this = this / value.</para></summary>
	/// <param name="value">Second operand: scalar.</param>
	/// <returns>true if the operation was successful, false if fails.</returns>
	bool Div( Type value );

	/// <summary>Transpose matrix (in place).
	/// Turns lines into columns and vice-versa.</summary>
	/// <returns>true if the operation was successful, false if fails.</returns>
	bool Transpose( void );

	// turns lines into columns and vice-versa, result is stored in 'result'
	/// <summary>Transpose matrix.
	/// Turns lines into columns and vice-versa.</summary>
	/// <param name="value">Pointer to CMatrix where the result will be stored.</param>
	/// <returns>true if the operation was successful, false if fails.</returns>
	bool Transpose( CMatrix<Type> * result );

	/// <summary>Computes the heaviside of matrix.</summary>
	/// <param name="u0">Heaviside threshold.</param>
	/// <param name="result">Pointer to CMatrix where the result will be stored.</param>
	/// <returns>true if the operation was successful, false if fails.</returns>
	bool Heaviside(Type u0, CMatrix<Type> * result);

	/// <summary>Computes the heaviside of matrix.</summary>
	/// <param name="u0">Heaviside threshold.</param>
	/// <returns>true if the operation was successful, false if fails.</returns>
	bool Heaviside(Type u0);

	/// <summary>Computes the threshold of matrix.
	///<para> result[n] = {  lowerValue : src[n] &lt; lowerBound ;  src[n] : lowerBound &gt;= src[n] &lt;= upperBound; upperValue > upperBound }</para></summary>
	/// <param name="lowerBound">Lower bound for threshold</param>
	/// <param name="lowerValue">Value to set when source is below lower bound</param>
	/// <param name="lowerBound">Upper bound for threshold</param>
	/// <param name="lowerBound">Value to set when source is higher than upper bound</param>
	/// <param name="result">Pointer to CMatrix where the result will be stored.</param>
	/// <returns>true if the operation was successful, false if fails.</returns>
	bool Threshold(Type lowerBound, Type lowerValue, Type upperBound, Type upperValue, CMatrix<Type> *result);

	/// <summary>Applis predicate to each element of matrix
	///<para> result[n] = pred(src[n])</para></summary>
	/// <param name="result">Pointer to CMatrix where the result will be stored.</param>
	/// <param name="pred">predicate function to apply.</param>
	/// <returns>true if the operation was successful, false if fails.</returns>
	template<class _Pr>
	bool Apply(CMatrix<Type> *result, _Pr pred)
	{
		for (int i = 0; i < nElements; i++)
			result->pData[i] = pred(pData[i]);

		return isStatusOK();
	}

	/// <summary>Computes the sum of the matrix elements.</summary>
	/// <param name="begin">Start element (including).</param>
	/// <param name="end">End element (excluding).</param>
	/// <param name="dst">Pointer to where to store sum.</param>
	/// <returns>Returns the matrix sum.</returns>
	bool Sum( int begin, int end, Type *dst );

	/// <summary>Matrix determinant.</summary>
	/// <returns>Returns the matrix determinant.</returns>
	Type Determinant( void );

	/// <summary>Identity matrix.</summary>
	/// <returns>Returns the identity matrix.</returns>
	CMatrix<Type> Identity( void );

	/// <summary>Sets this matrix as an Identity matrix.</summary>
	/// <returns>true if the operation was successful, false if fails.</returns>
	bool set_Identity( void );

	//-----------------------------------------------------------------------
	// Statistical functions
	//-----------------------------------------------------------------------

	/// <summary>Maximun value of the matrices' elements.</summary>
	/// <returns>Returns the maximun value.</returns>
	Type Max( void );

	/// <summary>Minimun value of the matrices' elements.</summary>
	/// <returns>Returns the minimun value.</returns>
	Type Min( void );

	/// <summary>Sum value of the matrices' elements.</summary>
	/// <returns>Returns the sum of the elements.</returns>
	Type Sum( void );

	/// <summary>Randomize this matrix using a Gaussian distribution.</summary>
	/// <param name="Mean">The mean of the distribution.</param>
	/// <param name="Deviation">The deviation of the distribution.</param>
	/// <returns>true if the operation was successful, false if fails.</returns>
	bool RandomizeGaussian( Type Mean, Type Deviation );
	
	/// <summary>Randomize matrix using a Gaussian distribution.</summary>
	/// <param name="height">Height of the desired matrix.</param>
	/// <param name="width">Width of the desired matrix.</param>
	/// <param name="Mean">The mean of the distribution.</param>
	/// <param name="Deviation">The deviation of the distribution.</param>
	/// <returns>Returns the randomized matrix.</returns>
	static CMatrix<Type> RandomGaussian( int height, int width, Type Mean, Type Deviation );

	/// <summary>Randomize thhis matrix using a Uniform distribution.</summary>
	/// <param name="Low">Lower bound of the uniform distribution range.</param>
	/// <param name="High">Upper bound of the uniform distribution range..</param>
	/// <returns>true if the operation was successful, false if fails.</returns>
	bool RandomizeUniform( Type Low, Type High );
	
	/// <summary>Randomize matrix using a Uniform distribution.</summary>
	/// <param name="height">Height of the desired matrix.</param>
	/// <param name="width">Width of the desired matrix.</param>
	/// <param name="Low">Lower bound of the uniform distribution range.</param>
	/// <param name="High">Upper bound of the uniform distribution range..</param>
	/// <returns>Returns the randomized matrix.</returns>
	static CMatrix<Type> RandomUniform( int width, int height, Type Low, Type High );

	//-----------------------------------------------------------------------
	// 				Inverse Matrix
	// 	these functions return true if the operation
	// 	was well suceeded or false otherwise
	//-----------------------------------------------------------------------
	
	/// <summary>Computes the inverse of any square matrix.</summary>
	/// <returns>true if the operation was successful, false if fails.</returns>
	bool Inverse( void );

	/// <summary>Computes the inverse a square matrix, and stores the result in another matrix.</summary>
	/// <param name="result">Pointer to the matrix where the result will be stored.</param>
	/// <returns>true if the operation was successful, false if fails.</returns>
	bool Inverse( CMatrix<Type> * result );

	//-----------------------------------------------------------------------
	
	/// <summary>2D full convolution of two matrices. First operand is 'this' matrix.</summary>
	/// <param name="operand2">Second operand.</param>
	/// <param name="operand2">Pointer to where the result should be stored.</param>
	/// <returns>true if the operation was successful, false if fails.</returns>
	bool Conv2D( CMatrix<Type> * operand2, CMatrix<Type> * result );

	/// <summary>1D full convolution of two matrices. First operand is 'this' matrix.</summary>
	/// <param name="operand2">Second operand.</param>
	/// <param name="operand2">Pointer to where the result should be stored.</param>
	/// <returns>true if the operation was successful, false if fails.</returns>
	bool Conv(CMatrix<Type> * operand2, CMatrix<Type> * result);

	/// <summary>1D circular circular convolution of two vectors. First operand is 'this' vector. Vectors must
	/// have odd size. Method behavior for even size on either vector has undocumented behavior.</summary>
	/// <param name="operand2">Second operand.</param>
	/// <param name="operand2">Pointer to where the result should be stored.</param>
	/// <returns>true if the operation was successful, false if fails.</returns>
	bool ConvCirc(CMatrix<Type> * operand2, CMatrix<Type> * result);

	/// <summary>1D circular circular convolution of two vectors. First operand is 'this' vector. Vectors must
	/// have odd size. Method behavior for even size on either vector has undocumented behavior.
	/// This method allows the caller to give extra matrix to hold auxiliary variables, so that they are not required to be allocated
	///every call.</summary>
	/// <param name="operand2">Second operand.</param>
	/// <param name="result">Pointer to where the result should be stored.</param>
	/// <param name="extended_operand1">Pointer to where the auxiliary extended operand1. It is not required to prealloc space.
	///	if not available the first time, it will be created.</param>
	/// <param name="full_result">Pointer to where the auxiliary full result. It is not required to prealloc space.
	///	if not available the first call, it will be created.</param>
	/// <returns>true if the operation was successful, false if fails.</returns>
	bool ConvCircP(CMatrix<Type> * operand2, CMatrix<Type> * result, CMatrix<Type> * extended_operand1, CMatrix<Type> * full_result);

	//-----------------------------------------------------------------------
	// 				Vector Operations
	//-----------------------------------------------------------------------

	/// <summary>Cross product of two vectors.</summary>
	/// <param name="mat">Second operand.</param>
	/// <returns>Matrix with the result of the operation.</returns>
	CMatrix<Type> cross( CMatrix<Type> * mat );

	/// <summary>Cross product of two vectors.</summary>
	/// <param name="mat">Second operand.</param>
	/// <param name="result">Pointer to the matrix where the result will be stored.</param>
	/// <returns>true if the operation was successful, false if fails.</returns>
	bool cross( CMatrix<Type> * mat, CMatrix<Type> * result );

	/// <summary>Calculates the norm 2 of a vector.</summary>
	/// <returns>Norm 2.</returns>
	Type norm( void );

	/// <summary>Rotate the vector v, it doesn't apply position transformation.
	/// <para>Quaternion related operation.</para></summary>
	/// <param name="v">Vector to be rotated.</param>
	/// <param name="rotated_v">Pointer to where the rotated vector will be saved.</param>
	void RotateVector( Type *v, Type *rotated_v ) const;

	/// <summary>Invert the rotation of this matrix on the vector v
	/// <para>Quaternion related operation.</para></summary>
	/// <param name="v">Vector to be unrotated.</param>
	/// <param name="rotated_v">Pointer to where the unrotated vector will be saved.</param>
	void UnrotateVector( Type *v, Type *unrotated_v ) const;

	//-----------------------------------------------------------------------
	// 				Other Matrix Operations
	//-----------------------------------------------------------------------

	/// <summary>Searches any positive elements in the matrix, 
	/// and returns their indices.</summary>
	/// <param name="indices">Vector of integers where the indices will be returned.</param>
	/// <returns>true if the operation was successful, false if fails.</returns>
	bool AnyPositive( std::vector<int> &indices );

	//-----------------------------------------------------------------------

	/// <summary>Makes this matrix, a transformation matrix (for inverse kinematics).
	/// <para>Accepts a set of Denavit–Hartenberg parameters.</para></summary>
	/// <param name="Alpha">Rotation on the Z axis.</param>
	/// <param name="A">Translation along the Z axis.</param>
	/// <param name="D">Translation along the X axis.</param>
	/// <param name="Theta">Rotation on the X axis.</param>
	void set_TransfMatrix( Type Alpha, Type A, Type D, Type Theta );

	/// <summary>Makes this matrix, a transformation matrix (for inverse kinematics).
	/// <para>Accepts a set of Denavit–Hartenberg parameters.</para>
	/// <para>This function should only be used if the matrix already has the correct size.</para></summary>
	/// <param name="Alpha">Rotation on the Z axis.</param>
	/// <param name="A">Translation along the Z axis.</param>
	/// <param name="D">Translation along the X axis.</param>
	/// <param name="Theta">Rotation on the X axis.</param>
	void set_TransfMatrixData( Type Alpha, Type A, Type D, Type Theta );

	/// <summary>Makes this matrix, a transformation matrix (for inverse kinematics).
	/// <para>Accepts a sub set of the Denavit–Hartenberg parameters.</para>
	/// <para>The same as function <see cref="CMatrix::set_TransfMatrix", but
	/// just the rotation part of the matrix.</para></summary>
	/// <param name="Alpha">Rotation on the Z axis.</param>
	/// <param name="Theta">Rotation on the X axis.</param>
	void set_TransfMatrix_3x3( Type Alpha, Type Theta );

	/// <summary>Rotation matrix of dPhi around x, y OR z.
	/// <para>(axis == 1 - x_axis; == 2 - y_axis; == 3 - z_axis).</para>
	/// <para>Eliana 2009-01.</para></summary>
	/// <param name="Phi">Phi angle.</param>
	/// <param name="axis">ID of axis.</param>
	void set_RotationMatrix_xyz_3x3( Type Phi, int axis );

	/// <summary>Makes this matrix, a rotation matrix with parameters Theta, Psi, Phi.
	/// <para>Accepts the rotation angles on x, y and z axis.</para></summary>
	/// <param name="Theta">Rotation on the X axis.</param>
	/// <param name="Psi">Rotation on the Y axis.</param>
	/// <param name="Phi">Rotation on the Z axis.</param>
	void set_RotationMatrix_3x3( Type Theta, Type Psi, Type Phi );

	/// <summary>Makes this matrix, a rotation matrix with parameters Roll, Pitch, Yaw.</summary>
	/// <param name="Roll">Longitudinal axis.</param>
	/// <param name="Pitch">Lateral axis.</param>
	/// <param name="Yaw">Normal axis.</param>
	void set_RotationMatrix_RPY_3x3( Type Roll, Type Pitch, Type Yaw );

	/// <summary>Solve a 6x6 linear system</summary>
	/// <param name="A">First operand.</param>
	/// <param name="b">Second operand.</param>
	/// <returns>The result of the operation.</returns>
	CMatrix<Type> SolLinearSyst_6x6( CMatrix<Type> A, CMatrix<Type> b );

	/// <summary>Coeficient calculation.</summary>
	/// <param name="x_i"></param>
	/// <param name="v_i"></param>
	/// <param name="a_i"></param>
	/// <param name="x_f"></param>
	/// <param name="v_f"></param>
	/// <param name="a_f"></param>
	void Coef_calc( Type x_i, Type v_i, Type a_i, Type x_f, Type v_f, Type a_f );

	/// <summary>Sets CMatrix<Type> object from a Matrix with Quaternion and Position parameters.
	/// <para>1st line the Quaternion elements ( qw, qx, qy, qz ).</para>
	/// <para>2nd line the Position elements ( p_x, p_y, p_z, w ).</para></summary>
	/// <param name="pQuat"></param>
	/// <param name="pPos"></param>
	void QuaternionToMatrix( Type *pQuat, Type *pPos = nullptr );

	/// <summary>Calculate an orthonormal matrix with the Z vector pointing on the 'dir' direction,
	/// <para>and the Y and X are determined by using the Gramm-Schmidth procedure.</para></summary>
	/// <param name="dir">Direction.</param>
	void GrammSchmidt( Type *dir );
};

#endif //_MATRIX_H_
