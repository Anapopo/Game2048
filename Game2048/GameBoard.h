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
	// 开始游戏
	void GameStart();
	// 填充盘面
	void fillBoardWith(int element);
	// 结束游戏
	void GameOver();
	// 交换回合
	void SwitchPlayer();
	// 重启游戏
	void GameRestart();
	// 判断游戏是否胜利
	bool isWin();
	// 判断游戏是否结束
	bool isLose();
	// 获取所有空格的链表
	list<int>& getEmptyGrids();

	void GetEmpty();
	// 评估插入2或4
	int EvaluateInsert(int position, int value);
	// 插入值
	void InsertTile(int position, int value);
	// 获取Board
	SimpleMatrix& GetBoard();
	// 随机生成新的方块，2或4
	void generateNewBlock();
	//  按需求生成新的方块
	void generateNewBlock(int element);

	void AddNumber();
	// 公有移动元素方法
	bool MoveTo(int d);
	void LoadBitMap();
	// 绘制分数
	void DrawScore(CDC &dc);
	void Draw(CDC &dc);
	// 评估当前平滑性
	int Smoothness();
	// 评估单调性
	int Monotonicity();
	// 获取最大值
	int MaxValue();
	// 获取当前空位数目
	int GetEmptyNumber();
	
	int* Combine(int *temp);
	void HandleUp();
	void HandleDown();
	void HandleLeft();
	void HandleRight();
private:
	// 生成随机数
	inline double Rand(double L, double R);
	// log2底N
	inline int log2(int n);
	// 随机返回一个空位
	int getRandomEmptyPos();
	// 元素移动&融合
	void Action2048(int direction);

	/* https://gitlab.com/Anapopo/game2048withAI/blob/master/grid.cpp */
	/*search input array and compress them in fornt several dimension*/
	int* compress(int *t);

};
#endif



