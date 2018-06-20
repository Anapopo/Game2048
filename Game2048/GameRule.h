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
	// 复制构造
	GameRule(GameRule &rule);
	~GameRule();
	//GameRule& operator=(GameRule& rule);
	// 开始游戏
	void GameStart();
	// 填充盘面
	void fillBoardWith(int element);
	// 结束游戏
	void GameOver();
	// 交换行动
	void SwitchPlayer();
	// 重启游戏
	void GameRestart();
	// 判断游戏是否胜利
	bool isWin();
	// 判断游戏是否结束
	bool isDead();
	// 获取所有空格的链表
	list<int>& getEmptyGrids();
	// 评估插入2或4
	int EvaluateInsert(int position, int value);
	// 插入值
	void InsertTile(int position, int value);
	// 获取Board
	SimpleMatrix& GetBoard();
	// 获取上一个Board
	//SimpleMatrix GetLastBoard();
	// 随机生成新的方块，2或4
	void generateNewBlock();
	//  按需求生成新的方块
	void generateNewBlock(int element);
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
	
private:
	// 生成随机数
	inline double Rand(double L, double R);
	// log2底N
	inline int log2(int n);
	// 随机返回一个空位
	int getRandomBlankPos();
	// 元素移动&融合
	void Action2048(int direction);
};
#endif



