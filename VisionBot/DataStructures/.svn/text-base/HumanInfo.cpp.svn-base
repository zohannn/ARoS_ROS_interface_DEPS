#include "HumanInfo.h"

#include <sstream>

#include "commands.h"
#include "MutexUtils.hpp"
#include "vector_operations.hpp"

CHumanInfo::CHumanInfo( void )
: BodyMovement( Quantity::ABSENT )
, HandMovement( Quantity::ABSENT )
{
}

CHumanInfo::CHumanInfo( const CHumanInfo& obj )
: CConfigFile( obj )
{
	copy( (CHumanInfo*) &obj );
}

CHumanInfo::~CHumanInfo( void )
{
	vMovParameters_Body.lock();
	vMovParameters_Body.Obj.clear();
	vMovParameters_Body.unlock();

	vMovParameters_Hand.lock();
	vMovParameters_Hand.Obj.clear();
	vMovParameters_Hand.unlock();
}

void CHumanInfo::copy( CHumanInfo * obj )
{
	if( obj == nullptr ) return;

	BodyMovement.set( obj->BodyMovement.get() );
	HandMovement.set( obj->HandMovement.get() );

	mutex_vector_copy( obj->vMovParameters_Body, vMovParameters_Body );

	mutex_vector_copy( obj->vMovParameters_Hand, vMovParameters_Hand );
}

bool CHumanInfo::Parse_Parameters_Load( void )
{
	bool bSuccess = true;

	std::string
		strMovement = "Movement_limits",
		strMovBody = "Body:",
		strMovHands = "Hands:";

	//--------------------------------------------------------------------------
	// Movement

	yarp::os::Bottle bot_mov = prop_ConfigFile.findGroup( strMovement.c_str() );
	
	int 
		num_params = 0,
		iPar = 0;

	if( bot_mov.size()>0 )
	{
		// ------------------------------------------------------------------
		// Config movement limits for body
		yarp::os::Bottle bot = bot_mov.findGroup( strMovBody.c_str() );
	
		num_params = bot.size() - 1;

		if( num_params > 0 )
		{
			vMovParameters_Body.lock();

			vMovParameters_Body.Obj.assign( num_params, 0.0f );

			for( iPar=0 ; iPar<num_params ; iPar++ )
			{
				vMovParameters_Body.Obj.at( iPar ) = static_cast<float>( bot.get( iPar+1 ).asDouble() );
			}

			vMovParameters_Body.unlock();
		}
		else bSuccess = false;

		// ------------------------------------------------------------------
		// Config movement limits for hands
		bot = bot_mov.findGroup( strMovHands.c_str() );
	
		num_params = bot.size() - 1;

		if( num_params > 0 )
		{
			vMovParameters_Hand.lock();

			vMovParameters_Hand.Obj.assign( num_params, 0.0f );

			for( iPar=0 ; iPar<num_params ; iPar++ )
			{
				vMovParameters_Hand.Obj.at( iPar ) = static_cast<float>( bot.get( iPar+1 ).asDouble() );
			}

			vMovParameters_Hand.unlock();
		}
		else bSuccess = false;
	}
	else bSuccess = false;

	return bSuccess;
}

void CHumanInfo::ClassifyMovement( CAttachMutex_o< std::vector<float> > * vParameters, CAttachMutex_n<int> * pMovStore, float fMov )
{
	vParameters->lock();
	
	int
		num_params = static_cast<int>( vParameters->Obj.size() ),
		Quant_Start = ( Quantity::IDLE >= 0 ) ? Quantity::IDLE : 0;

	for( int iQuant=Quant_Start ; iQuant<num_params ; iQuant++ )
	{
		if( fMov <= vParameters->Obj.at( iQuant ) )
		{
			pMovStore->set( iQuant );
			break;
		}
	}
	
	vParameters->unlock();
}

void CHumanInfo::ClassifyBodyMovement( float fMov )
{
	BodyMovement.set( fMov );
}

void CHumanInfo::ClassifyHandMovement( size_t num_hands, float fMov )
{
	float fHandMov = ( num_hands > 0 ) ? fMov : -1.0f;

	HandMovement.set( fHandMov );
}
