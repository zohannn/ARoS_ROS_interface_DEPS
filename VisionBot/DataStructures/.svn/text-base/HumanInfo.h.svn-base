#ifndef _HUMAN_INFO_H_
#define _HUMAN_INFO_H_

#pragma once

#include <vector>

#include "AttachMutex.hpp"
#include "ConfigFile.h"

class CHumanInfo
	: public CConfigFile
{
public:
	CHumanInfo( void );
	CHumanInfo( const CHumanInfo& obj );
	~CHumanInfo( void );

protected:
	void copy( CHumanInfo * obj );

public:
	CAttachMutex_n<float> 
		BodyMovement,
		HandMovement;

	// Parameters for movement classification
	CAttachMutex_o< std::vector<float> >
		vMovParameters_Body,
		vMovParameters_Hand;

	// Classify Body Movement in terms of ABSENT, IDLE, LOW, MEDIUM or HIGH
	void ClassifyBodyMovement( float fMov );
	
	// Classify Hand Movement in terms of ABSENT, IDLE, LOW, MEDIUM or HIGH
	void ClassifyHandMovement( size_t num_hands, float fMov );

protected:
	// Classify a movement in terms of IDLE, LOW, MEDIUM or HIGH
	void ClassifyMovement( CAttachMutex_o< std::vector<float> > * vParameters, CAttachMutex_n<int> * pMovStore, float fMov );

protected:
	bool Parse_Parameters_Load( void );
};

#endif // _HUMAN_INFO_H_
