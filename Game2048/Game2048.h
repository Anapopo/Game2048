
// Game2048.h : Game2048 Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include "GameBoard.h"
#include "GameAI.h"

// CGame2048App:
class CGame2048App : public CWinAppEx
{
public:
	CGame2048App();

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGame2048App theApp;
