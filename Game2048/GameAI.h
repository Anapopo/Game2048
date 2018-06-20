#include "stdafx.h"
#ifndef AI_H
#define AI_H
#include <list>
#include <utility>
#include "GameRule.h"
using namespace std;

class GameAI {
public:
	GameAI(GameRule *rule);
	static int count;
	pair<int, double> GetBest(int depth, double alpha, double beta);
	~GameAI();

private:
	GameRule * rule;
	double Evaluate();
	static const double smoothWeight;
	static const double monoWeight;
	static const double emptyWeight;
	static const double maxWeight;
};
#endif