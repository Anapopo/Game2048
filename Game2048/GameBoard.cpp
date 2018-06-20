#include "stdafx.h"
#include <list>
#include <utility>
#include <time.h>
#include "GameBoard.h"

using namespace std;

// 构造、析构、重载
GameBoard::GameBoard() {
	this->score = 0;
	this->isGameRunning = false;
	this->isPlayerTurn = false;
	//this->offset_width = 20;
	board = new SimpleMatrix(4, 0);
	//last_board = new SimpleMatrix(4, 0);
	//pos_start.SetPoint(55, 55);
	this->LoadBitMap();
}
GameBoard::GameBoard(GameBoard &rule)
{
	this->score = rule.score;
	this->isGameRunning = rule.isGameRunning;
	this->isPlayerTurn = rule.isPlayerTurn;
	//this->offset_width = 10;
	//pos_start.SetPoint(55, 55);
	this->LoadBitMap();

	SimpleMatrix temp(rule.GetBoard());
	this->board = new SimpleMatrix(4, 0);

	for (int i=0;i<4;i++)
		for (int j=0;j<4;j++)
			this->board->set(i, j, temp.get(i, j));
	//this->board->Copy(temp);
}
GameBoard::~GameBoard() {
	if (this->board != NULL)
		delete this->board;
	//delete this->last_board;
}
//GameRule& GameRule::operator=(GameRule& rule)
//{
//	if (&rule != this) {
//		delete this->board;
//		this->board = new SimpleMatrix(4, 0);
//
//		SimpleMatrix temp = rule.GetBoard();
//		//SimpleMatrix tempBefore = rule.GetLastBoard();
//		if (!rule.isPlayerTurn) rule.SwitchPlayer();
//
//		this->board = &temp;
//		//this->last_board = &tempBefore;
//	}
//	return *this;
//}

// 开始、重启、结束游戏
void GameBoard::GameStart()
{
	if (!isGameRunning)
	{
		// 改变游戏状态
		isGameRunning = true;
		// 清空分数
		this->score = 0;
		// 填充方块
		this->fillBoardWith(0);
		this->generateNewBlock();
		this->generateNewBlock();
		isPlayerTurn = true;
	}
}
void GameBoard::GameRestart() {
	if (isGameRunning) this->GameOver();
	this->GameStart();
}
void GameBoard::GameOver()
{
	this->score = 0;
	isGameRunning = false;
	this->fillBoardWith(0);
}

// 绘图函数
void GameBoard::LoadBitMap() {
	for (int i = 0; i < 12; ++i)
		bitMap[i].LoadBitmapW(i + 1000);
}
void GameBoard::DrawScore(CDC &dc) {
	CFont f1;
	f1.CreateFontIndirectW(FontFactory::CreateFontW(32, L"微软雅黑"));
	dc.SelectObject(&f1);
	CString str;
	str.Format(L"Score: %d", this->score);
	dc.SetTextColor(RGB(205, 193, 180));
	dc.TextOutW(55, 0, str);
}
void GameBoard::Draw(CDC &dc) {
	CDC MemDC;
	HDC hdc = dc.GetSafeHdc();
	CDC *pDC = new CDC;
	pDC->Attach(hdc);
	MemDC.CreateCompatibleDC(pDC);

	this->DrawScore(dc);

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			int block_value = this->board->get(i, j);
			int pos;
			if (block_value == 0) pos = 0;
			else pos = log2(block_value);

			MemDC.SelectObject(&bitMap[pos]);
			BITMAP bm;
			bitMap[pos].GetObject(sizeof(BITMAP), &bm);

			// 坐标系不同，交换 i 和 j
			pDC->BitBlt(55 + j * 120, 55 + i * 120, bm.bmWidth, bm.bmHeight, &MemDC, 0, 0, SRCCOPY);
		}
		ReleaseDC(NULL, MemDC);
}


// 辅助函数
bool GameBoard::MoveTo(int d) {
	if (isPlayerTurn)
	{
		switch (d) {
		case UP:
			HandleUp();
			break;
		case DOWN:
			HandleDown();
			break;
		case LEFT:
			HandleLeft();
			break;
		case RIGHT:
			HandleRight();
			break;
		}
		this->SwitchPlayer();
		return true;
	}
	return false;
	
}
void GameBoard::fillBoardWith(int element)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			this->board->set(i, j, element);
}
inline double GameBoard::Rand(double L, double R) { return L + (R - L) * rand() * 1.0 / RAND_MAX; }
inline int GameBoard::log2(int n) { return log10(n*1.0) / log10(2.0); }
void GameBoard::SwitchPlayer()
{
	if (this->isPlayerTurn)
		this->isPlayerTurn = false;
	else
		this->isPlayerTurn = true;
}
bool GameBoard::isWin() {
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
bool GameBoard::isLose() {
	if (this->GetEmptyNumber() > 0) 
		return false;
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


// 评估函数
// 评估插入2或4
int GameBoard::EvaluateInsert(int position, int value)
{
	int x = position/10;
	int y = position%10;
	int temp = 9999;//init it big enough

	//up
	for(int i=x ; i>0 ; i--){
		if(this->board->get(i, y) != 0){
			if(abs( (this->board->get(i, y) - value)) < temp ){
				temp = abs((this->board->get(i, y) - value));
			}
			break;
		}
	}
	//down
	for(int i=x ; i<4 ; i++){
		if(this->board->get(i, y) != 0){
			if(abs( (this->board->get(i, y) - value)) < temp ){
				temp = abs((this->board->get(i, y) - value));
			}
			break;
		}
	}
	//left
	for(int i=y ; i>0 ; i--){
		if(this->board->get(x, i) != 0){
			if(abs( (this->board->get(x, i) - value)) < temp ){
				temp = abs((this->board->get(x, i) - value));
			}
			break;
		}
	}
	//right
	for(int i=y ; i<4 ; i++){
		if(this->board->get(x, i) != 0){
			if(abs( (this->board->get(x, i) - value)) < temp ){
				temp = abs((this->board->get(x, i) - value));
			}
			break;
		}
	}
	return (temp == 9999 ? 0 : temp);
}
// 插入值
void GameBoard::InsertTile(int position, int value)
{
	int i = position / 10;
	int j = position % 10;

	if(this->board->get(i, j) == 0 )
		this->board->set(i, j, value);
	else
		exit(1);
}
// 评估平滑性
int GameBoard::Smoothness() {
	int smoothness = 0;
	int value = 0;
	int targetValue = 0;

	for (int i = 0; i<4; i++) {
		for (int j = 0; j<4; j++) {
			value = (this->board->get(i, j) == 0 ? 0 : (log(this->board->get(i, j)*1.0) / log(2.0)));
			//in row
			for (int x = j + 1; x<4; x++) {
				if (this->board->get(i, x) != 0) {
					targetValue = (this->board->get(i, x) == 0 ? 0 : (log(this->board->get(i, x)*1.0) / log(2.0)));
					smoothness -= abs(value - targetValue);
					break;
				}
			}
			//in line
			for (int x = i + 1; x<4; x++) {
				if (this->board->get(x, j) != 0) {
					targetValue = (this->board->get(x, j) == 0 ? 0 : (log(this->board->get(x, j)*1.0) / log(2.0)));
					smoothness -= abs(value - targetValue);
					break;
				}
			}
		}
	}
	return smoothness;
}
// 评估单调性
int GameBoard::Monotonicity() {
	int monotonicity = 0;
	int mono1 = 0;
	int mono2 = 0;
	int value = 0;
	int targetValue = 0;
	for (int i = 0; i<4; i++) {
		for (int j = 0; j<4; j++) {
			value = this->board->get(i, j) == 0 ? 0 : (log(this->board->get(i, j)*1.0) / log(2.0));
			for (int x = j + 1; x<4; x++) {
				if (this->board->get(i, x) != 0) {
					targetValue = log(this->board->get(i, x)*1.0) / log(2.0);
				}
				else if (x = 3) {
					targetValue = 0;
				}
				else {
					continue;
				}

				if (value > targetValue) {
					mono1 += targetValue - value;
				}
				else {
					mono2 += value - targetValue;
				}
				j = x - 1;
				break;
			}
		}
	}
	monotonicity += mono1>mono2 ? mono1 : mono2;

	for (int j = 0; j<4; j++) {
		for (int i = 0; i<4; i++) {
			value = this->board->get(i, j) == 0 ? 0 : (log(this->board->get(i, j)*1.0) / log(2.0));
			for (int x = i + 1; x<4; x++) {
				if (this->board->get(x, j) != 0) {
					targetValue = log(this->board->get(x, j)*1.0) / log(2.0);
				}
				else if (x = 3) {
					targetValue = 0;
				}
				else {
					continue;
				}

				if (value > targetValue) {
					mono1 += targetValue - value;
				}
				else {
					mono2 += value - targetValue;
				}
				i = x - 1;
				break;
			}
		}
	}
	monotonicity += mono1>mono2 ? mono1 : mono2;
	return monotonicity;
}


// 获取所有空格的链表
list<int>& GameBoard::getEmptyGrids()
{
	this->empty_grids.clear();
	for(int i=0;i<4;i++)
		for(int j = 0;j<4;j++)
			if(this->board->get(i, j) == 0)
				this->empty_grids.push_back(i*10+j);
	return this->empty_grids;
}
// 获取Board
SimpleMatrix& GameBoard::GetBoard() {
	return *this->board;
}
// 获取上一步的Board
//SimpleMatrix GameRule::GetLastBoard() {
//	return *this->last_board;
//}
// 随机生成新的方块，2或4
void GameBoard::generateNewBlock()
{
	GetEmpty();
	if (empty_grids.size() == 0) {
		if (isLose()) {
			isPlayerLose = true;
			isGameRunning = false;
			return;
		}
	}
	int new_pos = this->getRandomEmptyPos();
	if (new_pos == -1) return;
	int row = new_pos / 10;
	int col = new_pos % 10;
	this->board->element[row][col] = Rand(1, 100) > 90 ? 4 : 2;	//按概率决定生成2或4
	this->isPlayerTurn = true;
	if (empty_grids.size() == 1) {
		if (isLose()) {
			isPlayerLose = true;
			isGameRunning = false;
		}
	}
	
}
// 按需求生成新的方块
void GameBoard::generateNewBlock(int element) {
	int new_pos = this->getRandomEmptyPos();
	if (new_pos == -1) return;
	int row = new_pos / 10;
	int col = new_pos % 10;
	this->board->element[row][col] = element;
}

// 获取最大值
int GameBoard::MaxValue() {
	return this->board->getMaxValue();
}

// private
// 随机生成即将要填充的空位
int GameBoard::getRandomEmptyPos() {
	vector<int> blankBlock;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (board->element[i][j] == 0)
				blankBlock.push_back(i * 10 + j);
	if (blankBlock.empty()) return -1;
	int randomIdx = Rand(0, blankBlock.size());
	return blankBlock[randomIdx];//随机新位置
}
// 元素移动&融合 弃用
void GameBoard::Action2048(int direction)
{
	switch (direction) {
		// 上
	case 0:
		board->rotate3();
		this->Action2048(1);
		board->rotate3();
		break;
		// 左 逆时针 + 顺时针
	case 2:
		board->rotate1();
		this->Action2048(1);
		board->rotate2();
		break;
		// 右
	case 3:
		board->rotate2();
		this->Action2048(1);
		board->rotate1();
		break;
		// 下
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
						this->score += board->element[j][i];
						board->element[j - 1][i] = 0;
					}
				}
		}
		break;
	}
}
// 获取当前空位数目
int GameBoard::GetEmptyNumber()
{
	return this->board->getElementCount(0);
}

// 压缩
int* GameBoard::compress(int *t)
{
	int container[4] = { 0,0,0,0 };
	int turn = 0;
	for (int i = 0; i<4; i++) {
		if (t[i] != 0) {
			container[turn] = t[i];
			turn++;
		}
	}
	for (int i = 0; i<4; i++) {
		t[i] = container[i];
	}
	return t;
}


int* GameBoard::Combine(int *temp) {
	compress(temp);
	for (int i = 0; i<4; i++) {
		if (temp[i] != 0 && temp[i] == temp[i + 1]) {
			temp[i] = 2 * temp[i];
			score += temp[i];
			temp[i + 1] = 0;
		}
		if (temp[i] >= 2048) {
			isPlayerWin = true;
		}
	}
	compress(temp);
	return temp;
}

void GameBoard::HandleUp() {
	int temp[4] = { 0,0,0,0 };
	for (int i = 0; i<4; i++) {
		for (int j = 0; j<4; j++) {
			temp[j] = board->element[j][i];
		}
		Combine(temp);
		for (int j = 0; j<4; j++) {
			board->element[j][i] = temp[j];
		}
	}
}

void GameBoard::HandleDown() {
	int temp[4] = { 0,0,0,0 };
	for (int i = 0; i<4; i++) {
		for (int j = 0; j<4; j++) {
			temp[j] = board->element[3 - j][i];
		}
		Combine(temp);
		for (int j = 0; j<4; j++) {
			board->element[3 - j][i] = temp[j];
		}
	}
}

void GameBoard::HandleLeft() {
	int temp[4] = { 0,0,0,0 };
	for (int i = 0; i<4; i++) {
		for (int j = 0; j<4; j++) {
			temp[j] = board->element[i][j];
		}
		Combine(temp);
		for (int j = 0; j<4; j++) {
			board->element[i][j] = temp[j];
		}
	}
}

void GameBoard::HandleRight() {
	int temp[4] = { 0,0,0,0 };
	for (int i = 0; i<4; i++) {
		for (int j = 0; j<4; j++) {
			temp[j] = board->element[i][3 - j];
		}
		Combine(temp);
		for (int j = 0; j<4; j++) {
			board->element[i][3 - j] = temp[j];
		}
	}
}

void GameBoard::AddNumber() {
	GetEmpty();
	if (empty_grids.size() == 0) {
		if (isLose()) {
			isPlayerLose = true;
			return;
		}
	}
	int new_pos = this->getRandomEmptyPos();
	if (new_pos == -1) return;
	int row = new_pos / 10;
	int col = new_pos % 10;
	this->board->element[row][col] = Rand(1, 100) > 90 ? 4 : 2;
	//number[lastRow][lastLine] = (GetRandom(10) == 9) ? 4 : 2;

	isPlayerTurn = true;
	if (empty_grids.size() == 1) {
		if (isLose()) {
			isPlayerLose = true;
		}
	}
}

void GameBoard::GetEmpty() {
	empty_grids.clear();
	for (int i = 0; i<4; i++) {
		for (int j = 0; j<4; j++) {
			if (this->board->get(i, j) == 0) {
				empty_grids.push_back(i * 10 + j);
			}
		}
	}
}




