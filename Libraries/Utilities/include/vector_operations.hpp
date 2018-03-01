#ifndef _VECTOR_OPERATIONS_HPP_
#define _VECTOR_OPERATIONS_HPP_

#pragma once

#include <vector>

///	<summary>Makes sure a vector has no more than a given number of elements.</summary>
///	<param name="vec">Vector to test.</param>
///	<param name="max_elements">Maximum number of elements.</param>
template<typename T>
void vector_enforce_number_elements( std::vector<T> &vec, size_t max_elements )
{
	size_t 
		n_elements = vec.size(),
		offset = n_elements - max_elements;

	if( ( offset > 0 ) && ( offset < n_elements ) )
	{
		vec.erase( vec.begin(), vec.begin() + offset );
	}
}

///	<summary>Copy the contents of one vector to another.
/// <para>Makes sure that the destination vector has enough space.</para></summary>
///	<param name="vec_src">Source vector.</param>
///	<param name="vec_dst">Destination vector.</param>
template<typename T>
void vector_copy( const std::vector<T> &vec_src, std::vector<T> &vec_dst )
{
	vec_dst.assign( vec_src.begin(), vec_src.end() );
}

#endif // _VECTOR_OPERATIONS_HPP_
