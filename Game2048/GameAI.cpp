#include "stdafx.h"
#include "GameAI.h"
#include "GameBoard.h"
using namespace std;

// 各个评估标准所占权重
const double GameAI::smoothWeight = 1.0;
const double GameAI::monoWeight = 1.0;
const double GameAI::emptyWeight = 2.7;
const double GameAI::maxWeight = 1.0;

int GameAI::count = 40;

// 构造
GameAI::GameAI(GameBoard *theRule)
{
	this->rule = theRule;
}
// 析构
GameAI::~GameAI() {}
// 评估函数
double GameAI::Evaluate()
{
	/*double temp1 = rule->Smoothness() * smoothWeight;
	double temp2 = rule->Monotonicity() * monoWeight;
	double temp3 = log(rule->GetEmptyNumber()) * emptyWeight;
	double temp4 = rule->MaxValue() * maxWeight;*/

	return rule->Smoothness() * smoothWeight
		+ rule->Monotonicity() * monoWeight
		+ log(rule->GetEmptyNumber()*1.0) * emptyWeight
		+ rule->MaxValue() * maxWeight;
}
// 获取最优值
pair<int, double> GameAI::GetBest(int depth, double alpha, double beta)
{
	// 随机上右 20 次操作
	if (count > 0) {
		if (count % 2) {
			return make_pair(GameBoard::UP, 999);
		}
		return make_pair(GameBoard::RIGHT, 999);
	}
	int move = 0;
	double score = 0;

	std::pair<int, int> result;
	if (rule->isPlayerTurn) {//turn to get move
		score = alpha;// 初始评估值无穷小
		for (int dir = 1; dir <= 4; ++dir) // 遍历四个方向
		{
			GameBoard temp(*rule);
			if (temp.MoveTo(dir)) { // 模拟下一步操作
				temp.SwitchPlayer();
				// 获胜为最高权重
				if (temp.isWin()) {
					return std::make_pair(dir, 100000);
				}
				// 开始为下一步行动评估分数
				if (depth == 0) {
					result = std::make_pair(dir, this->Evaluate());
				}
				else {
					GameAI newai(&temp);
					result = newai.GetBest(depth - 1, score, beta);
				}
				// 取更高的评估分数
				if (result.second > score) {
					score = result.second;
					move = result.first;
					move = dir;
				}
				if (score > beta) {
					return std::make_pair(move, beta);
				}
			}
		}
	}
	else {  //turn to add number
		std::list< std::pair<int, int> > candidate;
		std::list< std::pair<int, int> > position;
		score = beta;
		std::list<int>::iterator itor = (rule->getEmptyGrids()).begin();
		int length = rule->GetEmptyNumber();
		//fill every empty grid with 2 and 4,just cut some branch
		while (length != 0) {
			for (int i = 2; i <= 4; i += 2) {// 2或4
				int value = rule->EvaluateInsert(*itor, i);
				candidate.push_back(std::make_pair((*itor + (100 * value)), i));
			}
			length--;
			itor++;
		}
		int tempValue = 0;
		// 遍历一次候选链表
		std::list< std::pair<int, int> >::iterator it = candidate.begin();
		for (; it != candidate.end(); it++) {
			if ((it->first) / 100 > tempValue) {
				tempValue = (it->first) / 100;
			}
		}
		// 再遍历一次候选链表
		for (it = candidate.begin(); it != candidate.end(); it++) {
			if ((it->first) / 100 == tempValue) {
				position.push_back(std::make_pair((it->first) % 100, it->second));
			}
		}
		//for every left branch
		std::list< std::pair<int, int> >::iterator positionItor = position.begin();
		for (; positionItor != position.end(); positionItor++) {
			GameBoard temp2(*rule);

			temp2.InsertTile(positionItor->first, positionItor->second);// 插入最坏选择
			temp2.SwitchPlayer();
			GameAI newAi2(&temp2);
			result = newAi2.GetBest(depth, alpha, score);
			if (result.second < score) {
				score = result.second;
				// move = result.first;
			}
			if (result.second < alpha) {
				return make_pair(5, alpha);
			}
		}

	}
	return make_pair(move, score);

}