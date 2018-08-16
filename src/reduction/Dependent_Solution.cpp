/*
 * Dependent_Solution.cpp
 *
 *  Created on: 5 Aug 2018
 *      Author: Max
 */

#include "reduction/Dependent_Solution.hpp"

#include "graph/PBQP_Graph.hpp"
#include "graph/PBQP_Node.hpp"
#include "graph/PBQP_Solution.hpp"

#include <algorithm>
#include <iterator>

Dependent_Solution::Dependent_Solution(std::vector<PBQP_Node*>* dependencyNodes,
		std::vector<PBQP_Node*>* solutionNodes) {
	dependencyIndices = new std::vector<int>(dependencyNodes->size());
	dependencyDegrees = new std::vector<int>(dependencyNodes->size());
	int degreeProduct = 1;
	for (unsigned int i = 0; i < dependencyNodes->size(); i++) {
		(*dependencyIndices)[i] = (*dependencyNodes)[i]->getIndex();
		int degree = (*dependencyNodes)[i]->getVektorDegree();
		(*dependencyDegrees)[i] = degree;
		degreeProduct *= degree;
	}
	solutionIndices = new std::vector<int>(solutionNodes->size());
	for (unsigned int i = 0; i < solutionNodes->size(); i++) {
		(*solutionIndices)[i] = (*solutionNodes)[i]->getIndex();
	}
	solutions = new int[dependencyIndices->size() * solutionIndices->size()
			* degreeProduct];
}

Dependent_Solution::~Dependent_Solution() {
	delete dependencyDegrees;
	delete dependencyIndices;
	delete solutionIndices;
	delete[] solutions;
}

void Dependent_Solution::setSolution(std::vector<int>* dependencySelections,
		std::vector<int>* solutionSelection) {
	int index = resolveIndex(dependencySelections);
	std::copy(solutionSelection->begin(), solutionSelection->end(), solutions + index);

}

void Dependent_Solution::solve(PBQP_Solution* solution) {
	std::vector<int>* dependencySolution = new std::vector<int>(
			dependencyIndices->size());
	for (int dependencyId : *dependencyIndices) {
		dependencySolution->push_back(solution->getSolution(dependencyId));
	}
	int index = resolveIndex(dependencySolution);
	for (unsigned int i = 0; i < solutionIndices->size(); i++) {
		solution->setSolution((*solutionIndices)[i], solutions[index + i]);
	}
}

int Dependent_Solution::resolveIndex(std::vector<int>* dependencySelections) {
	int index = 0;
	int offset = 1;
	for (unsigned int i = 0; i < dependencyDegrees->size(); i++) {
		index += (*dependencyDegrees)[i] * offset * ((*dependencySelections)[i]);
		offset *= (*dependencyDegrees)[i];
	}
	return index;
}
