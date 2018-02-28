#ifndef _MUTEX_UTILS_HPP_
#define _MUTEX_UTILS_HPP_

#include "AttachMutex.hpp"

/// <summary>Copy two vectors wrapped by CAttachMutex_n.</summary>
/// <param name="vec_src">Source vector.</param>
/// <param name="vec_dst">Destination vector.</param>
template <typename T>
void mutex_vector_copy( CAttachMutex_n< std::vector<T> > &vec_src, CAttachMutex_n< std::vector<T> > &vec_dst )
{
	vec_src.lock();
	vec_dst.lock();
	vec_dst.assign( vec_src.begin(), vec_src.end() );
	vec_dst.unlock();
	vec_src.unlock();
}

/// <summary>Copy a vector wrapped by CAttachMutex_n to a vector.</summary>
/// <param name="vec_src">Source vector.</param>
/// <param name="vec_dst">Destination vector.</param>
template <typename T>
void mutex_vector_copy( CAttachMutex_n< std::vector<T> > &vec_src, std::vector<T> &vec_dst )
{
	vec_src.lock();
	vec_dst.assign( vec_src.begin(), vec_src.end() );
	vec_src.unlock();
}

/// <summary>Copy a vector to a vector wrapped by CAttachMutex_n.</summary>
/// <param name="vec_src">Source vector.</param>
/// <param name="vec_dst">Destination vector.</param>
template <typename T>
void mutex_vector_copy( std::vector<T> &vec_src, CAttachMutex_n< std::vector<T> > &vec_dst )
{
	vec_dst.lock();
	vec_dst.assign( vec_src.begin(), vec_src.end() );
	vec_dst.unlock();
}

/// <summary>Copy two vectors wrapped by CAttachMutex_t.</summary>
/// <param name="vec_src">Source vector.</param>
/// <param name="vec_dst">Destination vector.</param>
template <typename T>
void mutex_vector_copy( CAttachMutex_t< std::vector<T> > &vec_src, CAttachMutex_t< std::vector<T> > &vec_dst )
{
	vec_src.lock();
	vec_dst.lock();
	vec_dst.assign( vec_src.begin(), vec_src.end() );
	vec_dst.unlock();
	vec_src.unlock();
}

/// <summary>Copy a vector wrapped by CAttachMutex_t to a vector.</summary>
/// <param name="vec_src">Source vector.</param>
/// <param name="vec_dst">Destination vector.</param>
template <typename T>
void mutex_vector_copy( CAttachMutex_t< std::vector<T> > &vec_src, std::vector<T> &vec_dst )
{
	vec_src.lock();
	vec_dst.assign( vec_src.begin(), vec_src.end() );
	vec_src.unlock();
}

/// <summary>Copy a vector to a vector wrapped by CAttachMutex_t.</summary>
/// <param name="vec_src">Source vector.</param>
/// <param name="vec_dst">Destination vector.</param>
template <typename T>
void mutex_vector_copy( std::vector<T> &vec_src, CAttachMutex_t< std::vector<T> > &vec_dst )
{
	vec_dst.lock();
	vec_dst.assign( vec_src.begin(), vec_src.end() );
	vec_dst.unlock();
}

/// <summary>Copy two vectors wrapped by CAttachMutex_o.</summary>
/// <param name="vec_src">Source vector.</param>
/// <param name="vec_dst">Destination vector.</param>
template <typename T>
void mutex_vector_copy( CAttachMutex_o< std::vector<T> > &vec_src, CAttachMutex_o< std::vector<T> > &vec_dst )
{
	vec_src.lock();
	vec_dst.lock();
	vec_dst.Obj.assign( vec_src.Obj.begin(), vec_src.Obj.end() );
	vec_dst.unlock();
	vec_src.unlock();
}

/// <summary>Copy a vector wrapped by CAttachMutex_o to a vector.</summary>
/// <param name="vec_src">Source vector.</param>
/// <param name="vec_dst">Destination vector.</param>
template <typename T>
void mutex_vector_copy( CAttachMutex_o< std::vector<T> > &vec_src, std::vector<T> &vec_dst )
{
	vec_src.lock();
	vec_dst.assign( vec_src.Obj.begin(), vec_src.Obj.end() );
	vec_src.unlock();
}

/// <summary>Copy a vector to a vector wrapped by CAttachMutex_o.</summary>
/// <param name="vec_src">Source vector.</param>
/// <param name="vec_dst">Destination vector.</param>
template <typename T>
void mutex_vector_copy( std::vector<T> &vec_src, CAttachMutex_o< std::vector<T> > &vec_dst )
{
	vec_dst.lock();
	vec_dst.Obj.assign( vec_src.begin(), vec_src.end() );
	vec_dst.unlock();
}

/// <summary>Copy two vectors wrapped by CAttachMutexPtr_n.</summary>
/// <param name="vec_src">Source vector.</param>
/// <param name="vec_dst">Destination vector.</param>
template <typename T>
void mutex_vector_copy( CAttachMutexPtr_n< std::vector<T> > &vec_src, CAttachMutexPtr_n< std::vector<T> > &vec_dst )
{
	vec_src.lock();
	vec_dst.lock();
	vec_dst.assign( vec_src.begin(), vec_src.end() );
	vec_dst.unlock();
	vec_src.unlock();
}

/// <summary>Copy a vector wrapped by CAttachMutexPtr_n to a vector.</summary>
/// <param name="vec_src">Source vector.</param>
/// <param name="vec_dst">Destination vector.</param>
template <typename T>
void mutex_vector_copy( CAttachMutexPtr_n< std::vector<T> > &vec_src, std::vector<T> &vec_dst )
{
	vec_src.lock();
	vec_dst.assign( vec_src.begin(), vec_src.end() );
	vec_src.unlock();
}

/// <summary>Copy a vector to a vector wrapped by CAttachMutexPtr_n.</summary>
/// <param name="vec_src">Source vector.</param>
/// <param name="vec_dst">Destination vector.</param>
template <typename T>
void mutex_vector_copy( std::vector<T> &vec_src, CAttachMutexPtr_n< std::vector<T> > &vec_dst )
{
	vec_src.lock();
	vec_dst.assign( vec_src.begin(), vec_src.end() );
	vec_src.unlock();
}

/// <summary>Copy two vectors wrapped by CAttachMutexPtr_t.</summary>
/// <param name="vec_src">Source vector.</param>
/// <param name="vec_dst">Destination vector.</param>
template <typename T>
void mutex_vector_copy( CAttachMutexPtr_t< std::vector<T> > &vec_src, CAttachMutexPtr_t< std::vector<T> > &vec_dst )
{
	vec_src.lock();
	vec_dst.lock();
	vec_dst.assign( vec_src.begin(), vec_src.end() );
	vec_dst.unlock();
	vec_src.unlock();
}

/// <summary>Copy a vector wrapped by CAttachMutexPtr_t to a vector.</summary>
/// <param name="vec_src">Source vector.</param>
/// <param name="vec_dst">Destination vector.</param>
template <typename T>
void mutex_vector_copy( CAttachMutexPtr_t< std::vector<T> > &vec_src, std::vector<T> &vec_dst )
{
	vec_src.lock();
	vec_dst.assign( vec_src.begin(), vec_src.end() );
	vec_src.unlock();
}

/// <summary>Copy a vector to a vector wrapped by CAttachMutexPtr_t.</summary>
/// <param name="vec_src">Source vector.</param>
/// <param name="vec_dst">Destination vector.</param>
template <typename T>
void mutex_vector_copy( std::vector<T> &vec_src, CAttachMutexPtr_t< std::vector<T> > &vec_dst )
{
	vec_dst.lock();
	vec_dst.assign( vec_src.begin(), vec_src.end() );
	vec_dst.unlock();
}

/// <summary>Copy two vectors wrapped by CAttachMutexPtr_o.</summary>
/// <param name="vec_src">Source vector.</param>
/// <param name="vec_dst">Destination vector.</param>
template <typename T>
void mutex_vector_copy( CAttachMutexPtr_o< std::vector<T> > &vec_src, CAttachMutexPtr_o< std::vector<T> > &vec_dst )
{
	vec_src.lock();
	vec_dst.lock();
	vec_dst.Obj.assign( vec_src.Obj.begin(), vec_src.Obj.end() );
	vec_dst.unlock();
	vec_src.unlock();
}

/// <summary>Copy a vector wrapped by CAttachMutexPtr_o to a vector.</summary>
/// <param name="vec_src">Source vector.</param>
/// <param name="vec_dst">Destination vector.</param>
template <typename T>
void mutex_vector_copy( CAttachMutexPtr_o< std::vector<T> > &vec_src, std::vector<T> &vec_dst )
{
	vec_src.lock();
	vec_dst.assign( vec_src.Obj.begin(), vec_src.Obj.end() );
	vec_src.unlock();
}

/// <summary>Copy a vector to a vector wrapped by CAttachMutexPtr_o.</summary>
/// <param name="vec_src">Source vector.</param>
/// <param name="vec_dst">Destination vector.</param>
template <typename T>
void mutex_vector_copy( std::vector<T> &vec_src, CAttachMutexPtr_o< std::vector<T> > &vec_dst )
{
	vec_dst.lock();
	vec_dst.Obj.assign( vec_src.begin(), vec_src.end() );
	vec_dst.unlock();
}

#endif // _MUTEX_UTILS_HPP_
