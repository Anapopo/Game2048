#include "stdafx.h"
#ifndef AI_H
#define AI_H
#include <list>
#include <utility>
#include "GameBoard.h"
using namespace std;

class GameAI {
public:
	GameAI(GameBoard *rule);
	static int count;
	pair<int, double> GetBest(int depth, double alpha, double beta);
	~GameAI();

private:
	GameBoard * rule;
	double Evaluate();
	static const double smoothWeight;
	static const double monoWeight;
	static const double emptyWeight;
	static const double maxWeight;
};
#endif