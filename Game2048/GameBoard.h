#include "stdafx.h"
#ifndef BOARD_H
#define BOARD_H
#include <vector>
#include <list>
#include "FontFactory.h"
#include "SimpleMatrix.cpp"
using namespace std;

class GameBoard {
public:
	int score;
	bool isGameRunning;
	bool isPlayerTurn;
	bool isPlayerWin;
	bool isPlayerLose;
	list<int> empty_grids;
	int empty_count;
	enum Direction { UP=1, DOWN=2, LEFT=3, RIGHT=4 };

private:
	// int offset_width;
	//CPoint pos_start;
	SimpleMatrix * board;
	CBitmap bitMap[12];

public:
	GameBoard();
	GameBoard(GameBoard &rule);
	~GameBoard();
	// ��ʼ��Ϸ
	void GameStart();
	// �������
	void fillBoardWith(int element);
	// ������Ϸ
	void GameOver();
	// �����غ�
	void SwitchPlayer();
	// ������Ϸ
	void GameRestart();
	// �ж���Ϸ�Ƿ�ʤ��
	bool isWin();
	// �ж���Ϸ�Ƿ����
	bool isLose();
	// ��ȡ���пո������
	list<int>& getEmptyGrids();

	void GetEmpty();
	// ��������2��4
	int EvaluateInsert(int position, int value);
	// ����ֵ
	void InsertTile(int position, int value);
	// ��ȡBoard
	SimpleMatrix& GetBoard();
	// ��������µķ��飬2��4
	void generateNewBlock();
	//  �����������µķ���
	void generateNewBlock(int element);

	void AddNumber();
	// �����ƶ�Ԫ�ط���
	bool MoveTo(int d);
	void LoadBitMap();
	// ���Ʒ���
	void DrawScore(CDC &dc);
	void Draw(CDC &dc);
	// ������ǰƽ����
	int Smoothness();
	// ����������
	int Monotonicity();
	// ��ȡ���ֵ
	int MaxValue();
	// ��ȡ��ǰ��λ��Ŀ
	int GetEmptyNumber();
	
	int* Combine(int *temp);
	void HandleUp();
	void HandleDown();
	void HandleLeft();
	void HandleRight();
private:
	// ���������
	inline double Rand(double L, double R);
	// log2��N
	inline int log2(int n);
	// �������һ����λ
	int getRandomEmptyPos();
	// Ԫ���ƶ�&�ں�
	void Action2048(int direction);

	/* https://gitlab.com/Anapopo/game2048withAI/blob/master/grid.cpp */
	/*search input array and compress them in fornt several dimension*/
	int* compress(int *t);

};
#endif



