
// ChildView.cpp: CChildView 类的实现
//

#include "stdafx.h"
#include "Game2048.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{	
	//int a[16] = { 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 0, 0, 0, 0, 0 };
	//board = new SimpleMatrix(4, a);
	rule = new GameRule(m_hWnd);
}

CChildView::~CChildView()
{
	delete rule;
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_BTN_START, &CChildView::OnBtnStart)
	ON_COMMAND(ID_BTN_ACE, &CChildView::OnBtnAce)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_BTN_RESTART, &CChildView::OnBtnRestart)
	ON_COMMAND(ID_BTN_WIN_TEST, &CChildView::OnBtnWinTest)
END_MESSAGE_MAP()

// CChildView 消息处理程序
BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this);             // 用于绘制的设备上下文
	dc.SetBkMode(TRANSPARENT);     // 透明背景

	rule->Draw(dc);
	ReleaseDC(&dc);
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	// pDC->SetBkColor(getAlphaColor(RGB(238, 228, 218), 0.35));
	//设置brush为希望的背景颜色 
	CBrush backBrush(RGB(187, 173, 160));

	//保存旧的brush 
	CBrush* pOldBrush = pDC->SelectObject(&backBrush);
	CRect rect;
	pDC->GetClipBox(&rect);

	//画需要的区域 
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);

	return TRUE;
	//return CWnd::OnEraseBkgnd(pDC);
}

// 开始游戏
void CChildView::OnBtnStart()
{
	rule->GameStart();
	Invalidate();
}

// 重新开始游戏
void CChildView::OnBtnRestart()
{
	rule->GameRestart();
	Invalidate();
}

void CChildView::OnBtnAce()
{
	rule->GameOver();
	rule->score = INT_MAX;
	rule->fillBoardWith(2048);
	Invalidate();
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (rule->isGameRunning)
	{
		switch (nChar)
		{
		case 38:
			rule->MoveTo(GameRule::UP);
			break;
		case 40:
			rule->MoveTo(GameRule::DOWN);
			break;
		case 37:
			rule->MoveTo(GameRule::LEFT);
			break;
		case 39:
			rule->MoveTo(GameRule::RIGHT);
			break;
		default:
			break;
		}
		Invalidate();
		if (rule->isDead()) {
			MessageBox(L"You Dead. Click Restart To Continue.", L"2048 INFO", MB_OK);
			rule->GameOver();
		}
		if (rule->isWin()) {
			MessageBox(L"You Win. Click Restart To Continue.", L"2048 INFO", MB_OK);
			rule->GameOver();
		}
	}
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CChildView::OnBtnWinTest()
{
	// rule->GameOver();
	rule->GameStart();
	rule->generateNewBlock(1024);
	rule->generateNewBlock(1024);
	Invalidate();
}
