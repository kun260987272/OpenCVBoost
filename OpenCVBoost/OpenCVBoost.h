
// OpenCVBoost.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// COpenCVBoostApp: 
// �йش����ʵ�֣������ OpenCVBoost.cpp
//

class COpenCVBoostApp : public CWinApp
{
public:
	COpenCVBoostApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern COpenCVBoostApp theApp;