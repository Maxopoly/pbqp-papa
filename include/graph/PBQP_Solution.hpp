/*
 * PBQPSolution.h
 *
 *  Created on: 5 Aug 2018
 *      Author: Max
 */

#ifndef GRAPH_PBQP_SOLUTION_HPP_
#define GRAPH_PBQP_SOLUTION_HPP_

#include <vector>

class PBQP_Graph;

class PBQP_Solution {
private:
	//TODO How do we represent positions that are not decided yet?
	std::vector<int>* selection;

public:
	PBQP_Solution(int);
	virtual ~PBQP_Solution();
	double calculateKnownCost();
	bool isFinished();
	void setSolution(int nodeIndex, int solution);
	int getSolution(int);
};

#endif /* GRAPH_PBQP_SOLUTION_HPP_ */
