#ifndef REDUCTION_DEPENDENT_SOLUTION_HPP_
#define REDUCTION_DEPENDENT_SOLUTION_HPP_

#include <vector>

template<typename T>
class PBQP_Solution;
template<typename T>
class PBQP_Node;

template<typename T>
class Dependent_Solution {
private:
	std::vector<int>* dependencyIndices;
	std::vector<int>* solutionIndices;
	std::vector<int>* dependencyDegrees;
	int* solutions;

	int resolveIndex(std::vector<int>* dependencySelections);

public:
	Dependent_Solution(std::vector<PBQP_Node<T>*>* dependencyNodes, std::vector<PBQP_Node<T>*>* solutionNodes) {
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

	virtual ~Dependent_Solution() {
		delete dependencyDegrees;
		delete dependencyIndices;
		delete solutionIndices;
		delete[] solutions;
	}

	void setSolution(std::vector<int>* dependencySelections, std::vector<int>* solutionSelection) {
		int index = resolveIndex(dependencySelections);
		std::copy(solutionSelection->begin(), solutionSelection->end(), solutions + index);
	}

	void solve(PBQP_Solution<T>* solution) {
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

private:

	int resolveIndex(std::vector<int>* dependencySelections) {
		int index = 0;
		int offset = 1;
		for (unsigned int i = 0; i < dependencyDegrees->size(); i++) {
			index += (*dependencyDegrees)[i] * offset * ((*dependencySelections)[i]);
			offset *= (*dependencyDegrees)[i];
		}
		return index;
	}
};

#endif /* REDUCTION_DEPENDENT_SOLUTION_HPP_ */
