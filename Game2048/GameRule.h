#include "stdafx.h"
#ifndef RULE_H
#define RULE_H
#include <vector>
#include <list>
#include "FontFactory.h"
#include "SimpleMatrix.cpp"
using namespace std;

class GameRule {
public:
	int score;
	bool isGameRunning;
	bool isPlayerTurn;
	list<int> empty_grids;
	enum Direction { UP=1, DOWN=2, LEFT=3, RIGHT=4 };

private:
	int offset_width;
	CPoint pos_start;
	SimpleMatrix * board;
	//SimpleMatrix * last_board;
	CBitmap bitMap[12];

public:
	GameRule();
	// ���ƹ���
	GameRule(GameRule &rule);
	~GameRule();
	//GameRule& operator=(GameRule& rule);
	// ��ʼ��Ϸ
	void GameStart();
	// �������
	void fillBoardWith(int element);
	// ������Ϸ
	void GameOver();
	// �����ж�
	void SwitchPlayer();
	// ������Ϸ
	void GameRestart();
	// �ж���Ϸ�Ƿ�ʤ��
	bool isWin();
	// �ж���Ϸ�Ƿ����
	bool isDead();
	// ��ȡ���пո������
	list<int>& getEmptyGrids();
	// ��������2��4
	int EvaluateInsert(int position, int value);
	// ����ֵ
	void InsertTile(int position, int value);
	// ��ȡBoard
	SimpleMatrix& GetBoard();
	// ��ȡ��һ��Board
	//SimpleMatrix GetLastBoard();
	// ��������µķ��飬2��4
	void generateNewBlock();
	//  �����������µķ���
	void generateNewBlock(int element);
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
	
private:
	// ���������
	inline double Rand(double L, double R);
	// log2��N
	inline int log2(int n);
	// �������һ����λ
	int getRandomBlankPos();
	// Ԫ���ƶ�&�ں�
	void Action2048(int direction);
};
#endif



