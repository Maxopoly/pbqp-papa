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
	std::vector<unsigned short int> solutions;

public:
	DependentSolution(const std::vector<PBQPNode<T>*>& dependencyNodes,
			const std::vector<PBQPNode<T>*>& solutionNodes) :
			dependencyIndices(
					std::vector<unsigned long int>(dependencyNodes.size())), solutionIndices(
					std::vector<unsigned long int>(solutionNodes.size())), dependencyDegrees(
					std::vector<unsigned short int>(dependencyNodes.size())) {
		const unsigned long int length = dependencyNodes.size();
		unsigned long solutionAmount = 1;
		for (unsigned long int i = 0; i < length; i++) {
			dependencyIndices.at(i) = (dependencyNodes.at(i))->getIndex();
			unsigned short int degree = (dependencyNodes.at(i))->getVectorDegree();
			dependencyDegrees.at(i) = degree;
			solutionAmount *= degree;
		}
		for (unsigned int i = 0; i < solutionNodes.size(); i++) {
			solutionIndices.at(i) = (solutionNodes.at(i))->getIndex();
		}
		solutions = std::vector<unsigned short int> (solutionAmount * solutionIndices.size());
	}

	void setSolution(
			const std::vector<unsigned short int>& dependencySelections,
			const std::vector<unsigned short int>& solutionSelection) {
		unsigned long int index = resolveIndex(dependencySelections);
		std::copy(solutionSelection.begin(), solutionSelection.end(),
				solutions.data() + index);
	}

	void solve(PBQPSolution<T>& solution) const {
		std::vector<unsigned short int> dependencySolution (dependencyIndices.size());
		for(unsigned long int i =0 ; i < dependencyIndices.size(); i++) {
			dependencySolution.at(i) = solution.getSolution(dependencyIndices.at(i));
		}
		unsigned long int index = resolveIndex(dependencySolution);
		for (unsigned long int i = 0; i < solutionIndices.size(); i++) {
			solution.setSolution(solutionIndices.at(i), solutions.at(index + i));
		}
	}

private:

	unsigned long int resolveIndex(
			const std::vector<unsigned short int>& dependencySelections) const {
		unsigned long int index = 0;
		unsigned long int offset = 1;
		for (unsigned long int i = 0; i < dependencyDegrees.size(); i++) {
			index += offset * dependencySelections.at(i);
			offset *= dependencyDegrees.at(i);
		}
		return index;
	}
};

}

#endif /* REDUCTION_DependentSolution_HPP_ */
