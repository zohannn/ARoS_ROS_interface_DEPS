#include "Message.h"
#include <sstream>

CMessage::CMessage( void )
{
	clear();
}

CMessage::CMessage( unsigned int command )
{
	clear();
	uCommand = command;
}

CMessage::CMessage( const CMessage &msg )
{
	copy_from( const_cast<CMessage*>( &msg ) );
}

CMessage::~CMessage( void )
{
	clear();
}

void CMessage::clear( void )
{
	uCommand = 0;
	iErrorCode = 0;
	bin_size = 0;
	bin = nullptr;
	strText.clear();
	uParam.clear();
	fData.clear();
}

void CMessage::copy( CMessage *src, CMessage *dst )
{
	dst->strText = src->strText;
	dst->uCommand = src->uCommand;
	dst->uParam = src->uParam;
	dst->fData = src->fData;
	dst->iErrorCode = src->iErrorCode;
	dst->bin = src->bin;
	dst->bin_size = src->bin_size;
}

void CMessage::copy_from( CMessage * src )
{
	copy( src, this );
}

CMessage CMessage::operator=( CMessage rightOp )
{
	copy_from( &rightOp );
	return *this;
}

std::string CMessage::asString( void )
{
	std::stringstream strm_Msg;
	strm_Msg << "error:" << iErrorCode << ' ';

	if( strText.size() > 0 )
	{
		strm_Msg << "text:" << strText << ' ';
	}

	strm_Msg << "command:" << uCommand;

	size_t vSize = uParam.size();
	for( size_t i = 0; i < vSize; i++ )
	{
		strm_Msg << " param" << i << ':' << uParam.at( i );
	}

	vSize = fData.size();
	for( size_t i = 0; i < vSize; i++ )
	{
		strm_Msg << " fdata" << i << ':' << fData.at( i );
	}

	return strm_Msg.str();
}

void CMessage::fromBottle( yarp::os::Bottle &bottleMsg )
{
	int index = 0;

	iErrorCode = bottleMsg.get( index ).asInt();
	index++;

	strText = bottleMsg.get( index ).asString();
	index++;

	uCommand =  static_cast<unsigned int>( bottleMsg.get( index ).asInt() );
	index++;

	int numParam = bottleMsg.get( index ).asInt();
	index++;
	uParam.assign( numParam, 0 );
	for( int i = 0; i < numParam; i++ )
	{
		uParam.at( i ) = static_cast<unsigned int>( bottleMsg.get( index ).asInt() );
		index++;
	}

	int numData = bottleMsg.get( index ).asInt();
	index++;
	fData.assign( numData, 0.0f );
	for( int i = 0; i < numData; i++ )
	{
		fData.at( i ) = static_cast<float>( bottleMsg.get( index ).asDouble() );
		index++;
	}

	bin_size = bottleMsg.get( index ).asBlobLength();
	bin = ( char* ) bottleMsg.get( index ).asBlob();
}

void CMessage::toBottle( yarp::os::Bottle &bottleMsg )
{
	bottleMsg.clear();

	//Bottle will be responsible for freeing up this memory
	bottleMsg.add( yarp::os::Value::makeInt( iErrorCode ) );

	bottleMsg.add( yarp::os::Value::makeString( strText.c_str() ) );

	bottleMsg.add( yarp::os::Value::makeInt( static_cast<int>( uCommand ) ) );

	size_t sizeParam = uParam.size();
	bottleMsg.add( yarp::os::Value::makeInt( static_cast<int>( sizeParam ) ) );
	
	for( size_t i = 0; i < sizeParam; i++ )
	{
		bottleMsg.add( yarp::os::Value::makeInt( static_cast<int>( uParam.at( i ) ) ) );
	}

	size_t sizeData = fData.size();
	bottleMsg.add( yarp::os::Value( static_cast<int>( sizeData  )) );
	
	for( size_t i = 0; i < sizeData; i++ )
	{
		bottleMsg.add( yarp::os::Value::makeDouble( static_cast<double>( fData.at( i ) ) ) );
	}

	bottleMsg.add( yarp::os::Value::makeBlob( (void*) bin, static_cast<int>( bin_size ) ) );
}

yarp::os::Bottle CMessage::toBottle( void )
{
	yarp::os::Bottle bottleOut;

	toBottle( bottleOut );

	return bottleOut;
}

bool CMessage::isValid( void )
{
	return ( ( uCommand > 0 ) && ( uCommand < UINT_MAX ) );
}
