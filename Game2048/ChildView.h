// ChildView.h: CChildView 类的接口
#pragma once

// CChildView 窗口
class CChildView : public CWnd
{
// 构造
public:
	CChildView();

// 特性
public:
	GameBoard * rule;
	GameAI *ai;
	bool isAIPlaying;
// 操作
public:

// 重写
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CChildView();

// 生成的消息映射函数
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBtnStart();
	afx_msg void OnBtnAce();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnBtnRestart();
	afx_msg void OnBtnWinTest();
	void AIDoMove();
	afx_msg void OnBtnAutorun();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

