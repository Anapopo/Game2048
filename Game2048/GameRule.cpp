#include "stdafx.h"
#include <vector>
#include "FontFactory.h"
#include "SimpleMatrix.cpp"

using namespace std;

class GameRule {
public:
	int score = 0;
	bool isGameRunning = false;
	enum Direction { UP, DOWN, LEFT, RIGHT };

private:
	HWND hWnd;
	int offset_width = 10;
	CPoint pos_start;
	SimpleMatrix * board;
	CBitmap bitMap[12];
	int expMap[12] = { 0, 20, 40, 80, 160, 320, 640, 1280, 2560, 5120, 10240, 20480 };

public:
	GameRule(HWND hWnd) {
		this->hWnd = hWnd;
		board = new SimpleMatrix(4, 0);
		pos_start.SetPoint(55, 55);
		this->LoadBitMap();
	}
	// ��Ϸ��ʼ
	void GameStart() {
		if (!isGameRunning)
		{
			// �ı���Ϸ״̬
			isGameRunning = true;
			// ��շ���
			this->score = 0;
			// ��䷽��
			this->fillBoardWith(0);
			this->generateNewBlock();
			this->generateNewBlock();
			//this->board->set(0,1,1024);
			//this->board->set(0,2,1024);
			//this->board->set(0,3,2);
			//this->board->set(0,0,2);
		}
	}

	// �������
	void fillBoardWith(int element) {
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				this->board->set(i, j, element);
	}
	
	// ��Ϸ����
	void GameOver() {
		this->score = 0;
		isGameRunning = false;
		this->fillBoardWith(0);
	}

	// ������Ϸ
	void GameRestart() {
		if (isGameRunning) this->GameOver();
		this->GameStart();
	}

	// ���������
	inline double Rand(double L, double R) { return L + (R - L) * rand() * 1.0 / RAND_MAX; }
	// log2��N
	inline int log2(int n) { return log10(n) / log10(2); }

	// �ж���Ϸ�Ƿ�ʤ��
	bool isWin() {
		int max = 0;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (max < board->element[i][j]) {
					max = board->element[i][j];
				}
			}
		}
		return (max == 2048);
	}
	// �ж���Ϸ�Ƿ����
	bool isDead() {
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				if (board->element[i][j] == 0)
					return false;
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 4; ++j)
				if (board->element[i][j] == board->element[i + 1][j])
					return false;
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 3; ++j)
				if (board->element[i][j] == board->element[i][j + 1])
					return false;
		isGameRunning = false;
		return true;
	}
	// ��ȡBoard
	SimpleMatrix* getBoard() {
		return this->board;
	}
	// ��������µķ��飬2��4
	void generateNewBlock()
	{
		int new_pos = this->getRandomBlankPos();
		if (new_pos == -1) return;
		int row = new_pos / 4;
		int col = new_pos % 4;
		board->element[row][col] = Rand(1, 100) > 90 ? 4 : 2;	//�����ʾ�������2��4

	}
	//  �����������µķ���
	void generateNewBlock(int element) {
		int new_pos = this->getRandomBlankPos();
		if (new_pos == -1) return;
		int row = new_pos / 4;
		int col = new_pos % 4;
		this->board->element[row][col] = element;
	}
	// �����ƶ�Ԫ�ط���
	void MoveTo(Direction d) {
		switch (d) {
			case UP:
				this->Action2048(0);
				break;
			case DOWN:
				this->Action2048(1);
				break;
			case LEFT:
				this->Action2048(2);
				break;
			case RIGHT:
				this->Action2048(3);
				break;
		}
		this->generateNewBlock();
	}

	/*
	//// ���»���
	//bool Update() {
	//	if (animeBlock != CPoint(-1, -1))
	//	{
	//		int i = animeBlock.x;
	//		int j = animeBlock.y;
	//		//������Ӿ��β���������
	//		RECT blockRect;
	//		blockRect.left = pos_start.x + i * (100 + offset_width);
	//		blockRect.top = pos_start.y + j * (100 + offset_width);
	//		blockRect.right = blockRect.left + 100;
	//		blockRect.bottom = blockRect.top + 100;

	//		if (CPoint(i, j) == animeBlock) {
	//			const static int zoomArg[] = {			//ƫ��ԭ���ӱ��صľ���
	//				-24, -20, -16, -12, -8, -4, 0,
	//			};
	//			int delta = zoomArg[zoomStep];			//��ǰƫ�ƾ���
	//			blockRect.left -= delta;				//ƫ������
	//			blockRect.right += delta;
	//			blockRect.top -= delta;
	//			blockRect.bottom += delta;

	//			++zoomStep;//�Ŵ����Ÿ���

	//					   //�Ŵ󶯻����������� animeBlock Ϊ���ɴ�
	//			if (zoomStep >= sizeof(zoomArg) / sizeof(int))
	//				animeBlock.SetPoint(-1, -1);
	//			return true;
	//		}
	//		
	//	}
	//	return false;
	//}
	*/

	void LoadBitMap() {
		for (int i = 0; i < 12; ++i)
			bitMap[i].LoadBitmapW(i + 1000);
	}
	// ���Ʒ���
	void DrawScore(CDC &dc) {
		CFont f1;
		f1.CreateFontIndirectW(FontFactory::CreateFontW(32, L"΢���ź�"));
		dc.SelectObject(&f1);
		CString str;
		str.Format(L"Score: %d", this->score);
		dc.SetTextColor(RGB(205, 193, 180));
		dc.TextOutW(pos_start.x, 0, str);
	}

	void Draw(CDC &dc) {
		CDC MemDC;
		HDC hdc = dc.GetSafeHdc();
		CDC *pDC = new CDC;
		pDC->Attach(hdc);
		MemDC.CreateCompatibleDC(pDC);

		this->DrawScore(dc);


		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			{
				int block_value= this->board->get(i, j);
				int pos;
				if (block_value == 0) pos = 0;
				else pos = log2(block_value);

				MemDC.SelectObject(&bitMap[pos]);
				BITMAP bm;
				bitMap[pos].GetObject(sizeof(BITMAP), &bm);

				// ����ϵ��ͬ������ i �� j
				pDC->BitBlt(pos_start.x + j * 120, pos_start.y + i * 120, bm.bmWidth, bm.bmHeight, &MemDC, 0, 0, SRCCOPY);
			}
		ReleaseDC(hWnd, MemDC);
	}

	~GameRule() {
		delete this->board;
	}
private:
	// �������һ����λ
	int getRandomBlankPos() {
		vector<int> blankBlock;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				if (board->element[i][j] == 0)
					blankBlock.push_back(i * 4 + j);
		if (blankBlock.empty()) return -1;
		int randomIdx = Rand(0, blankBlock.size());
		return blankBlock[randomIdx];//�����λ��
	}
	// Ԫ���ƶ�&�ں�
	void Action2048(int direction)
	{
		switch (direction) {
			// ��
		case 0:
			board->rotate3();
			this->Action2048(1);
			board->rotate3();
			break;
			// �� ��ʱ�� + ˳ʱ��
		case 2:
			board->rotate1();
			this->Action2048(1);
			board->rotate2();
			break;
			// ��
		case 3:
			board->rotate2();
			this->Action2048(1);
			board->rotate1();
			break;
			// ��
		case 1:
			int i, j, k;
			for (k = 0; k < 3; ++k)
			{
				//int merge_times = 2;
				for (i = 0; i<4; ++i)
					for (j = 3; j>0; --j)
					{
						if (board->element[j][i] == 0)
						{
							int temp = board->element[j][i];
							board->element[j][i] = board->element[j - 1][i];
							board->element[j - 1][i] = temp;
						}
						if (board->element[j][i] == board->element[j - 1][i])
						{
							board->element[j][i] *= 2;
							//--merge_times;
							this->score += expMap[log2(board->element[j][i])];
							board->element[j - 1][i] = 0;
						}
					}
			}
			break;
		}
	}
};