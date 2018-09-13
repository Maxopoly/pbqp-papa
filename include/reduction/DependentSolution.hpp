#ifndef REDUCTION_DependentSolution_HPP_
#define REDUCTION_DependentSolution_HPP_

#include <vector>

namespace pbqppapa {

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
class DependentSolution {
private:
	std::vector<unsigned long int> dependencyIndices;
	std::vector<unsigned long int> solutionIndices;
	std::vector<unsigned short int> dependencyDegrees;
	unsigned short int* solutions;

public:
	DependentSolution(const std::vector<PBQPNode<T>*>& dependencyNodes,
			const std::vector<PBQPNode<T>*>& solutionNodes) :
			dependencyIndices(std::vector<unsigned long int>(dependencyNodes.size())), solutionIndices(
					std::vector<unsigned long int>(solutionNodes.size())), dependencyDegrees(
					std::vector<unsigned short int>(dependencyNodes.size())) {
		unsigned long int degreeProduct = 1;
		const unsigned long int length = dependencyNodes.size();
		for (unsigned long int i = 0; i < length; i++) {
			dependencyIndices[i] = (dependencyNodes[i])->getIndex();
			unsigned short int degree = (dependencyNodes[i])->getVectorDegree();
			dependencyDegrees[i] = degree;
			degreeProduct *= degree;
		}
		for (unsigned int i = 0; i < solutionNodes.size(); i++) {
			solutionIndices[i] = (solutionNodes[i])->getIndex();
		}
		solutions = new unsigned short int[dependencyIndices.size()
				* solutionIndices.size() * degreeProduct];
	}

	~DependentSolution() {
		delete[] solutions;
	}

	void setSolution(
			const std::vector<unsigned short int>& dependencySelections,
			const std::vector<unsigned short int>& solutionSelection) {
		unsigned long int index = resolveIndex(dependencySelections);
		std::copy(solutionSelection.begin(), solutionSelection.end(),
				solutions + index);
	}

	void solve(PBQPSolution<T>& solution) const {
		std::vector<unsigned short int> dependencySolution = std::vector<
				unsigned short int>(dependencyIndices.size());
		for (unsigned long int dependencyId : dependencyIndices) {
			dependencySolution.push_back(solution.getSolution(dependencyId));
		}
		unsigned long int index = resolveIndex(dependencySolution);
		for (unsigned long int i = 0; i < solutionIndices.size(); i++) {
			solution.setSolution(solutionIndices[i], solutions[index + i]);
		}
	}

private:

	unsigned long int resolveIndex(
			const std::vector<unsigned short int>& dependencySelections) const {
		unsigned long int index = 0;
		unsigned long int offset = 1;
		for (unsigned long int i = 0; i < dependencyDegrees.size(); i++) {
			index += dependencyDegrees[i] * offset * dependencySelections[i];
			offset *= dependencyDegrees[i];
		}
		return index;
	}
};

}

#endif /* REDUCTION_DependentSolution_HPP_ */
