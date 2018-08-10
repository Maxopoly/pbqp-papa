/*
 * Dependent_Solution.h
 *
 *  Created on: 5 Aug 2018
 *      Author: Max
 */

#ifndef REDUCTION_DEPENDENT_SOLUTION_HPP_
#define REDUCTION_DEPENDENT_SOLUTION_HPP_

#include <vector>

class PBQP_Solution;
class PBQP_Node;

class Dependent_Solution {
private:
	std::vector<int>* dependencyIndices;
	std::vector<int>* solutionIndices;
	std::vector<int>* dependencyDegrees;
	int* solutions;

	int resolveIndex(std::vector<int>* dependencySelections);

public:
	Dependent_Solution(std::vector<PBQP_Node*>* dependencyNodes, std::vector<PBQP_Node*>* solutionNodes);
	virtual ~Dependent_Solution();
	void setSolution(std::vector<int>* dependencySelections, std::vector<int>* solutionSelection);
	void solve(PBQP_Solution*);
};

#endif /* REDUCTION_DEPENDENT_SOLUTION_HPP_ */
