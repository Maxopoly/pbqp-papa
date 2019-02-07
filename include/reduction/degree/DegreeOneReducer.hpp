#ifndef REDUCTION_DEGREEONEREDUCTOR_HPP_
#define REDUCTION_DEGREEONEREDUCTOR_HPP_

#include <vector>
#include <assert.h>

#include "reduction/PBQPReduction.hpp"
#include "reduction/solutions/OnetoOneDependentSolution.hpp"
#include "reduction/solutions/NtoNDependentSolution.hpp"
#include "math/InfinityWrapper.hpp"

namespace pbqppapa {

template<typename T>
class PBQPGraph;
template<typename T>
class NtoNDependentSolution;
template<typename T>
class PBQPSolution;
template<typename T>
class PBQPNode;

/**
 * Removes nodes of degree one by locally optimizing them and adding their cost to the one other
 * node they are connected to
 */
template<typename T>
class DegreeOneReducer: public PBQP_Reduction<T> {
private:
	std::vector<NtoNDependentSolution<T>*> solutions;

public:
	DegreeOneReducer(PBQPGraph<T>* graph) :
			PBQP_Reduction<T>(graph) {
	}

	~DegreeOneReducer() {
		for (NtoNDependentSolution<T>* sol : solutions) {
			delete sol;
		}
	}

	std::vector<PBQPGraph<T>*>& reduce() override {
		auto iter = this->graph->getNodeBegin();
		while (iter != this->graph->getNodeEnd()) {
			PBQPNode<T>* node = *iter++;
			if (node->getDegree() == 1) {
				NtoNDependentSolution<T>* sol = reduceDegreeOne(node,
						this->graph);
				solutions.push_back(sol);
			}
		}
		this->result.push_back(this->graph);
		return this->result;
	}

	void solve(PBQPSolution<T>& solution) {
		auto iter = solutions.rbegin();
		while (iter != solutions.rend()) {
			NtoNDependentSolution<T>* sol = *iter++;
			sol->solve(&solution);
		}
	}

	static NtoNDependentSolution<T>* reduceDegreeOne(PBQPNode<T>* node,
			PBQPGraph<T>* graph) {
		//will explode if node doesnt have an edge
		assert(node->getDegree() == 1);
		PBQPEdge<T>* edge = node->getAdjacentEdges().at(0);
		PBQPNode<T>* otherEnd = edge->getOtherEnd(node);
		std::vector<PBQPNode<T>*> dependencyNodes;
		std::vector<PBQPNode<T>*> solutionNodes;
		dependencyNodes.push_back(otherEnd);
		solutionNodes.push_back(node);
		NtoNDependentSolution<T>* solution = new NtoNDependentSolution<T>(
				dependencyNodes, solutionNodes);
		const bool isSource = edge->isSource(node);
		const unsigned short otherEndDegree = otherEnd->getVectorDegree();
		const unsigned short nodeDegree = node->getVectorDegree();
		for (unsigned short i = 0; i < otherEndDegree; i++) {
			//find minimum for this selection
			T otherEndCost = otherEnd->getVector().get(i);
			unsigned short minSelection = 0;
			T minimum = otherEndCost;
			minimum += node->getVector().get(0);
			if (isSource) {
				minimum += edge->getMatrix().get(0, i);
			} else {
				minimum += edge->getMatrix().get(i, 0);
			}
			for (unsigned short k = 1; k < nodeDegree; k++) {
				T compSum = otherEndCost;
				compSum += node->getVector().get(k);
				if (isSource) {
					compSum += edge->getMatrix().get(k, i);
				} else {
					compSum += edge->getMatrix().get(i, k);
				}
				if (compSum < minimum) {
					minimum = compSum;
					minSelection = k;
				}
			}
			std::vector<unsigned short> dependencySelections;
			std::vector<unsigned short> solutionSelections;
			dependencySelections.push_back(i);
			solutionSelections.push_back(minSelection);
			solution->setSolution(dependencySelections, solutionSelections);
			otherEnd->getVector().get(i) = minimum;
		}
		graph->removeNode(node);
		//TODO chain effect?
		return solution;
	}

	static OnetoOneDependentSolution<InfinityWrapper<T>>* reduceDegreeOneInf(
			PBQPNode<InfinityWrapper<T>>* node,
			PBQPGraph<InfinityWrapper<T>>* graph) {
		//ensure edge exists
		assert(node->getDegree() == 1);
		PBQPEdge<InfinityWrapper<T>>* edge = node->getAdjacentEdges().at(0);
		PBQPNode<InfinityWrapper<T>>* otherEnd = edge->getOtherEnd(node);
		//ensure edge isnt a cycle
		assert(otherEnd != node);
		OnetoOneDependentSolution<InfinityWrapper<T>>* solution =
				new OnetoOneDependentSolution<InfinityWrapper<T>>(node,
						otherEnd);
		const bool isSource = edge->isSource(node);
		const unsigned short otherEndDegree = otherEnd->getVectorDegree();
		const unsigned short nodeDegree = node->getVectorDegree();
		for (unsigned short i = 0; i < otherEndDegree; i++) {
			//find minimum for this selection
			InfinityWrapper<T> otherEndCost = otherEnd->getVector().get(i);
			if (otherEndCost.isInfinite()) {
				continue;
			}
			unsigned short minSelection = 0;
			InfinityWrapper<T> minimum = otherEndCost;
			minimum += node->getVector().get(0);
			if (isSource) {
				minimum += edge->getMatrix().get(0, i);
			} else {
				minimum += edge->getMatrix().get(i, 0);
			}
			for (unsigned short k = 1; k < nodeDegree; k++) {
				InfinityWrapper<T> compSum = otherEndCost;
				compSum += node->getVector().get(k);
				if (isSource) {
					compSum += edge->getMatrix().get(k, i);
				} else {
					compSum += edge->getMatrix().get(i, k);
				}
				if (compSum < minimum) {
					minimum = compSum;
					minSelection = k;
				}
			}
			solution->setSolutionSelection(i, minSelection);
			otherEnd->getVector().get(i) = minimum;
		}
		graph->removeNode(node);
		bool found = false;
		for (unsigned short i = 0; i < otherEndDegree; i++) {
			if (!otherEnd->getVector().get(i).isInfinite()) {
				found = true;
				break;
			}
		}
		if (!found) {
			return NULL;
		}
		return solution;
	}
};

}

#endif /* REDUCTION_DEGREEONEREDUCTOR_HPP_ */
