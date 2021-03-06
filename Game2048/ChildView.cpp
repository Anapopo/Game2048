
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
	this->rule = new GameBoard();
	this->ai = new GameAI(rule);
	this->isAIPlaying = false;
}

CChildView::~CChildView()
{
	//delete rule;
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_BTN_START, &CChildView::OnBtnStart)
	ON_COMMAND(ID_BTN_ACE, &CChildView::OnBtnAce)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_BTN_RESTART, &CChildView::OnBtnRestart)
	ON_COMMAND(ID_BTN_WIN_TEST, &CChildView::OnBtnWinTest)
	ON_COMMAND(ID_BTN_AUTORUN, &CChildView::OnBtnAutorun)
	ON_WM_TIMER()
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
	/*int sm = rule->Smoothness();
	int mo = rule->Monotonicity();
	int ma = rule->MaxValue();
	CString out;
	out.Format(L"平滑性：%d\n单调性：%d\n最大值：%d", sm, mo, ma);

	MessageBox(out);*/
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
	if (rule->isGameRunning && !isAIPlaying)
	{
		switch (nChar)
		{
		case 38:
			rule->MoveTo(GameBoard::UP);
			break;
		case 40:
			rule->MoveTo(GameBoard::DOWN);
			break;
		case 37:
			rule->MoveTo(GameBoard::LEFT);
			break;
		case 39:
			rule->MoveTo(GameBoard::RIGHT);
			break;
		default:
			break;
		}
		this->rule->generateNewBlock();
		Invalidate();
		if (rule->isLose()) {
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
	//rule->GameOver();
	//rule->GameStart();
	//rule->generateNewBlock(1024);
	//rule->generateNewBlock(1024);
	//Invalidate();
}

void CChildView::AIDoMove()
{
	if(isAIPlaying){
		switch ((ai->GetBest(3, -10000, 10000)).first)
		{
			case 1:
				this->rule->MoveTo(1);//上
				break;
			case 2:
				this->rule->MoveTo(2);//下
				break;
			case 3:
				this->rule->MoveTo(3);//左
				break;
			case 4:
				this->rule->MoveTo(4);//右
				break;
			case 5:
				break;
			default:
				break;

		}
		if(ai->count > 0){
			ai->count--;
		}

		this->rule->generateNewBlock();
		Invalidate();
	}

}
// 启动AI自动游戏
void CChildView::OnBtnAutorun()
{
	this->rule->isGameRunning = true;
	this->isAIPlaying = true;
	SetTimer(1, 100, NULL);
}


void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	if(this->rule->isGameRunning)
	{
		this->AIDoMove();
	} else {
		KillTimer(1);
	}
	CWnd::OnTimer(nIDEvent);
}


