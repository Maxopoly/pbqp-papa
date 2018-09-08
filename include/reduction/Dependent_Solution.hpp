#ifndef REDUCTION_DEPENDENT_SOLUTION_HPP_
#define REDUCTION_DEPENDENT_SOLUTION_HPP_

#include <vector>

template<typename T>
class PBQPSolution;
template<typename T>
class PBQPNode;

/**
 * Stores the solution of a set of nodes A dependent on the solution of another set of nodes B
 *
 * The exact sets A and B are given to the constructor and then for each possible selection in A,
 * a selection in B must be given.
 *
 * This allows removing B from the graph, solving the remaining (simpler) graph and retrieving
 * a solution for B once A (which is part of the remaining graph) is solved
 */
template<typename T>
class Dependent_Solution {
private:
	std::vector<int>* dependencyIndices;
	std::vector<int>* solutionIndices;
	std::vector<unsigned short int>* dependencyDegrees;
	unsigned short int* solutions;

public:
	Dependent_Solution(std::vector<PBQPNode<T>*>* dependencyNodes,
			std::vector<PBQPNode<T>*>* solutionNodes) {
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
		solutions = new unsigned short int[dependencyIndices->size()
				* solutionIndices->size() * degreeProduct];
	}

	virtual ~Dependent_Solution() {
		delete dependencyDegrees;
		delete dependencyIndices;
		delete solutionIndices;
		delete[] solutions;
	}

	void setSolution(std::vector<unsigned short int>* dependencySelections,
			std::vector<unsigned short int>* solutionSelection) {
		int index = resolveIndex(dependencySelections);
		std::copy(solutionSelection->begin(), solutionSelection->end(),
				solutions + index);
	}

	void solve(PBQPSolution<T>* solution) {
		std::vector<unsigned short int>* dependencySolution = new std::vector<
				unsigned short int>(dependencyIndices->size());
		for (unsigned int dependencyId : *dependencyIndices) {
			dependencySolution->push_back(solution->getSolution(dependencyId));
		}
		int index = resolveIndex(dependencySolution);
		for (unsigned int i = 0; i < solutionIndices->size(); i++) {
			solution->setSolution((*solutionIndices)[i], solutions[index + i]);
		}
	}

private:

	int resolveIndex(std::vector<unsigned short int>* dependencySelections) {
		int index = 0;
		int offset = 1;
		for (unsigned int i = 0; i < dependencyDegrees->size(); i++) {
			index += (*dependencyDegrees)[i] * offset
					* ((*dependencySelections)[i]);
			offset *= (*dependencyDegrees)[i];
		}
		return index;
	}
};

#endif /* REDUCTION_DEPENDENT_SOLUTION_HPP_ */
