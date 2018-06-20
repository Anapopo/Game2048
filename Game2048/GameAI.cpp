#include "stdafx.h"
#include "GameAI.h"
#include "GameBoard.h"
using namespace std;

// ����������׼��ռȨ��
const double GameAI::smoothWeight = 1.0;
const double GameAI::monoWeight = 1.0;
const double GameAI::emptyWeight = 2.7;
const double GameAI::maxWeight = 1.0;

int GameAI::count = 40;

// ����
GameAI::GameAI(GameBoard *theRule)
{
	this->rule = theRule;
}
// ����
GameAI::~GameAI() {}
// ��������
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
// ��ȡ����ֵ
pair<int, double> GameAI::GetBest(int depth, double alpha, double beta)
{
	// ������� 20 �β���
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
		score = alpha;// ��ʼ����ֵ����С
		for (int dir = 1; dir <= 4; ++dir) // �����ĸ�����
		{
			GameBoard temp(*rule);
			if (temp.MoveTo(dir)) { // ģ����һ������
				temp.SwitchPlayer();
				// ��ʤΪ���Ȩ��
				if (temp.isWin()) {
					return std::make_pair(dir, 100000);
				}
				// ��ʼΪ��һ���ж���������
				if (depth == 0) {
					result = std::make_pair(dir, this->Evaluate());
				}
				else {
					GameAI newai(&temp);
					result = newai.GetBest(depth - 1, score, beta);
				}
				// ȡ���ߵ���������
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
			for (int i = 2; i <= 4; i += 2) {// 2��4
				int value = rule->EvaluateInsert(*itor, i);
				candidate.push_back(std::make_pair((*itor + (100 * value)), i));
			}
			length--;
			itor++;
		}
		int tempValue = 0;
		// ����һ�κ�ѡ����
		std::list< std::pair<int, int> >::iterator it = candidate.begin();
		for (; it != candidate.end(); it++) {
			if ((it->first) / 100 > tempValue) {
				tempValue = (it->first) / 100;
			}
		}
		// �ٱ���һ�κ�ѡ����
		for (it = candidate.begin(); it != candidate.end(); it++) {
			if ((it->first) / 100 == tempValue) {
				position.push_back(std::make_pair((it->first) % 100, it->second));
			}
		}
		//for every left branch
		std::list< std::pair<int, int> >::iterator positionItor = position.begin();
		for (; positionItor != position.end(); positionItor++) {
			GameBoard temp2(*rule);

			temp2.InsertTile(positionItor->first, positionItor->second);// �����ѡ��
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