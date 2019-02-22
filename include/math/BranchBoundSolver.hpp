#ifndef MATH_BRANCHBOUNDSOLVER_HPP_
#define MATH_BRANCHBOUNDSOLVER_HPP_

#include <vector>

#include "graph/PBQPGraph.hpp"
#include "reduction/solutions/NtoNDependentSolution.hpp"
#include "graph/PBQPSolution.hpp"
#include "math/InfinityWrapper.hpp"

namespace pbqppapa {

template<typename T>
class PBQPGraph;
template<typename T>
class PBQPSolution;
template<typename T>
class PBQPNode;
template<typename T>
class InfinityWrapper;

/**
 * Attempts to solve a PBQP instance through a branch and bound approach.
 * Note that this does not work for bigger graph or graphs with very little cost variation
 */
template<typename T>
class BranchBoundSolver {

private:

	PBQPGraph<InfinityWrapper<T>>* graph;
	std::vector<PBQPNode<InfinityWrapper<T>>*> nodes;

public:

	/**
	 * Creates a new instance to solve the given graph
	 */
	BranchBoundSolver(PBQPGraph<InfinityWrapper<T>>* graph) :
			graph(graph) {
		for (auto iter = graph->getNodeBegin(); iter != graph->getNodeEnd();
				iter++) {
			nodes.push_back(*iter);
		}
	}

	~BranchBoundSolver() {

	}

public:
	/**
	 * Attempts to find a solution. May not finish before the heat death of the universe if your graph is big
	 */
	PBQPSolution<InfinityWrapper<T>>* solve() {
		InfinityWrapper<T> localCost = InfinityWrapper<T>(0);
		PBQPSolution<InfinityWrapper<T>>* localSolution = new PBQPSolution<
				InfinityWrapper<T>>(graph->getNodeIndexCounter());
		return recursiveSolve(localCost, localSolution, 0);
	}

private:
	PBQPSolution<InfinityWrapper<T>>* recursiveSolve(
			InfinityWrapper<T> currentCost,
			PBQPSolution<InfinityWrapper<T>>* sol, unsigned int nodeCounter) {
		PBQPSolution<InfinityWrapper<T>>* localSolution = new PBQPSolution<
				InfinityWrapper<T>>(*sol);
		PBQPSolution<InfinityWrapper<T>>* minSolution = 0;
		PBQPNode<InfinityWrapper<T>>* node = nodes.at(nodeCounter);
		InfinityWrapper<T> localCost = InfinityWrapper<T>(0);
		InfinityWrapper<T> localMin = InfinityWrapper<T>::getInfinite();
		unsigned short minSelection = 0;
		for (unsigned short index = 0; index < node->getVectorDegree();
				index++) {
			InfinityWrapper<T> value = node->getVector().get(index);
			if (value.isInfinite()) {
				continue;
			}
			boolean allowedViaEdges = true;
			InfinityWrapper<T> edgeSum(0);
			for (PBQPEdge<InfinityWrapper<T>>* edge : node->getAdjacentEdges()) {
				PBQPNode<InfinityWrapper<T>>* otherEnd = nodes.at(nodeCounter);
				if (sol->hasSolution(otherEnd->getIndex())) {
					InfinityWrapper<T> matrixValue;
					if (edge->getSource() == node) {
						matrixValue = edge->getMatrix().get(index,
								sol->getSolution(otherEnd));
					} else {
						matrixValue = edge->getMatrix().get(
								sol->getSolution(otherEnd), index);
					}
					if (matrixValue.isInfinite()) {
						allowedViaEdges = false;
						break;
					}
					edgeSum += matrixValue;
				}
			}
			if (!allowedViaEdges) {
				continue;
			}
			localSolution->setSolution(node->getIndex(), index);
			localCost += value;
			localCost += edgeSum;
			if (nodeCounter == nodes.size() - 1) {
				return localSolution;
			}
			PBQPSolution<InfinityWrapper<T>>* retSolution = recursiveSolve(
					currentCost + localCost, localSolution, nodeCounter + 1);
			if (retSolution == 0) {
				continue;
			}
			InfinityWrapper<T> possibleMin = retSolution->getCurrentCost(graph);
			if (possibleMin < localMin) {
				localMin = possibleMin;
				minSelection = index;
				if (!minSolution) {
					delete minSolution;
				}
				minSolution = retSolution;
			} else {
				delete retSolution;
			}
		}
		delete localSolution;
		return minSolution;
	}
};

}

#endif /* MATH_BRANCHBOUNDSOLVER_HPP_ */
