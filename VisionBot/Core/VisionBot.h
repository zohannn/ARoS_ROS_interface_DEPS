#if !defined(AFX_VISIONBOT_H__804408D5_F3FF_44D7_9832_26B95FA4862C__INCLUDED_)
#define AFX_VISIONBOT_H__804408D5_F3FF_44D7_9832_26B95FA4862C__INCLUDED_

#pragma once

#include "stdafx.h"

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

// CVisionBotApp:
// See VisionBot.cpp for the implementation of this class
//

class CVisionBotApp : public CWinApp
{
public:
	CVisionBotApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CVisionBotApp theApp;

#endif // !defined(AFX_VISIONBOT_H__804408D5_F3FF_44D7_9832_26B95FA4862C__INCLUDED_)
