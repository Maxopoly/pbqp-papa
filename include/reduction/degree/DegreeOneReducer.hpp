#ifndef REDUCTION_DEGREEONEREDUCTOR_HPP_
#define REDUCTION_DEGREEONEREDUCTOR_HPP_

#include <vector>
#include <reduction/PBQPReduction.hpp>
#include <reduction/DependentSolution.hpp>

namespace pbqppapa {

template<typename T>
class PBQPGraph;
template<typename T>
class DependentSolution;
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
	std::vector<DependentSolution<T>*> solutions;

public:
	DegreeOneReducer(PBQPGraph<T>* graph) :
			PBQP_Reduction<T>(graph) {
	}

	~DegreeOneReducer() {
		for (DependentSolution<T>* sol : solutions) {
			delete sol;
		}
	}

	std::vector<PBQPGraph<T>*>& reduce() override {
		auto iter = this->graph->getNodeBegin();
		while (iter != this->graph->getNodeEnd()) {
			PBQPNode<T>* node = *iter++;
			if (node->getDegree() == 1) {
				DependentSolution<T>* sol = reduceDegreeOne(node, this->graph);
				solutions.push_back(sol);
			}
		}
		this->result.push_back(this->graph);
		return this->result;
	}

	static DependentSolution<T>* reduceDegreeOne(PBQPNode<T>* node,
			PBQPGraph<T>* graph) {
		//will explode if node doesnt have an edge
		PBQPEdge<T>* edge = node->getAdjacentEdges().at(0);
		PBQPNode<T>* otherEnd = edge->getOtherEnd(node);
		std::vector<PBQPNode<T>*> dependencyNodes;
		std::vector<PBQPNode<T>*> solutionNodes;
		dependencyNodes.push_back(otherEnd);
		solutionNodes.push_back(node);
		DependentSolution<T>* solution = new DependentSolution<T>(
				dependencyNodes, solutionNodes);
		const bool isSource = edge->isSource(node);
		const unsigned short otherEndDegree = otherEnd->getVectorDegree();
		const unsigned short nodeDegree = node->getVectorDegree();
		const T oneElement = T(1);
		for (unsigned short i = 0; i < otherEndDegree; i++) {
			//find minimum for this selection
			T otherEndCost = otherEnd->getVector().get(i);
			unsigned short minSelection = 0;
			T minimum = otherEndCost;
			if (otherEndCost + oneElement != otherEndCost) { //not infinite!
				minimum += node->getVector().get(0);
				if (isSource) {
					minimum += edge->getMatrix().get(0, i);
				} else {
					minimum += edge->getMatrix().get(i, 0);
				}
				for (unsigned short k = 1; k < nodeDegree; k++) {
					T compSum = otherEndCost;
					minimum += node->getVector().get(k);
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

	void solve(PBQPSolution<T>& solution) {
		auto iter = solutions.rbegin();
		while (iter != solutions.rend()) {
			DependentSolution<T>* sol = *iter++;
			sol->solve(&solution);
		}
	}

private:
	static inline T calcSum(unsigned short sourceSelection,
			unsigned short targetSelection, PBQPEdge<T>* edge) {
		T sum = T();
		sum += edge->getSource()->getVector().get(sourceSelection);
		sum += edge->getTarget()->getVector().get(targetSelection);
		sum += edge->getMatrix().get(sourceSelection, targetSelection);
		return sum;
	}
};

}

#endif /* REDUCTION_DEGREEONEREDUCTOR_HPP_ */
