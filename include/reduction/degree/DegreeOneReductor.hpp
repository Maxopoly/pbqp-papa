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
class DegreeOneReductor: public PBQP_Reduction<T> {
private:
	std::vector<DependentSolution<T>*> solutions;

public:
	DegreeOneReductor(PBQPGraph<T>* graph) :
			PBQP_Reduction<T>(graph) {
	}

	~DegreeOneReductor() {
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
		bool isSource = edge->isSource(node);
		for (unsigned short int i = 0; i < otherEnd->getVectorDegree(); i++) {
			//find minimum for this selection
			T minimum = T();
			if (isSource) {
				minimum += calcSum(0, i, edge);
			} else {
				minimum += calcSum(i, 0, edge);
			}
			unsigned short int minSelection = 0;
			for (unsigned short int k = 1; k < node->getVectorDegree(); k++) {
				T compSum = T ();
				if (isSource) {
					compSum += calcSum(k, i, edge);
				} else {
					compSum += calcSum(i, k, edge);
				}
				if (compSum < minimum) {
					minimum = compSum;
					minSelection = k;
				}
			}
			std::vector<unsigned short int> dependencySelections;
			std::vector<unsigned short int> solutionSelections;
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
		while(iter != solutions.rend()) {
			DependentSolution<T>* sol = *iter++;
			sol->solve(solution);
		}
	}

private:
	static inline T calcSum(unsigned short int sourceSelection,
			unsigned short int targetSelection, PBQPEdge<T>* edge) {
		T sum = T();
		sum += edge->getSource()->getVector().get(sourceSelection);
		sum += edge->getTarget()->getVector().get(targetSelection);
		sum += edge->getMatrix().get(sourceSelection, targetSelection);
		return sum;
	}
};

}

#endif /* REDUCTION_DEGREEONEREDUCTOR_HPP_ */
