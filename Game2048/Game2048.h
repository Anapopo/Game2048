
// Game2048.h: Game2048 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号
#include "GameRule.cpp"

// CGame2048App:
// 有关此类的实现，请参阅 Game2048.cpp
//

class CGame2048App : public CWinApp
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
