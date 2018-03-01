#ifndef _PREPROCESSOR_MACROS_H_
#define _PREPROCESSOR_MACROS_H_

#pragma once

#define SIZE_OF_ARRAY( array_name ) sizeof( array_name ) / sizeof( array_name[0] )

#define isIndexInRange( array_name, index ) ( ( index>=0 ) && ( index<SIZE_OF_ARRAY( array_name ) ) )

#define GET_VALID_ArrayItem( array_name, index ) isIndexInRange( array_name, index ) ? array_name[index] : array_name[SIZE_OF_ARRAY( array_name ) - 1]

#endif // _PREPROCESSOR_MACROS_H_
