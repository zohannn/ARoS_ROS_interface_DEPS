#ifndef _OBJECT_VECTOR_H_
#define _OBJECT_VECTOR_H_

#pragma once

#include <vector>

template <class T>
class ObjectVector
	: public std::vector<T>
{
public:

	T *GetObjectByColor( int nColor ) {
		T temp;
		std::vector<T> vtemp;

		temp.aColor[0] = nColor;
		vtemp.emplace_back( temp );
		std::vector<T>::iterator F1 = begin();
		std::vector<T>::iterator L1 = end();
		std::vector<T>::iterator F2 = vtemp.begin();
		std::vector<T>::iterator L2 = vtemp.end();

		return  find_first_of( F1, L1, F2, L2, T::FindByColor );
	}

	T *GetObjectByType( int nType ) {
		T temp, *ptr = NULL;
		std::vector<T> vtemp;

		temp.nType = nType;
		vtemp.emplace_back( temp );
		std::vector<T>::iterator F1 = begin();
		std::vector<T>::iterator L1 = end();
		std::vector<T>::iterator F2 = vtemp.begin();
		std::vector<T>::iterator L2 = vtemp.end();

		ptr = find_first_of( F1, L1, F2, L2, T::FindByType );
		return ptr;
	}
};

#endif // _OBJECT_VECTOR_H_
