
// Game2048.h : Game2048 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号
#include "GameBoard.h"
#include "GameAI.h"

// CGame2048App:
class CGame2048App : public CWinAppEx
{
public:
	CGame2048App();

// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGame2048App theApp;
