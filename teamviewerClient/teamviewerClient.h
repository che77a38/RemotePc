
// teamviewerClient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CteamviewerClientApp: 
// �йش����ʵ�֣������ teamviewerClient.cpp
//

class CteamviewerClientApp : public CWinApp
{
public:
	CteamviewerClientApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CteamviewerClientApp theApp;