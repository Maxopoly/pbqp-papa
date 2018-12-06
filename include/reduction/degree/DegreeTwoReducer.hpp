#ifndef REDUCTION_DEGREETWOREDUCTOR_HPP_
#define REDUCTION_DEGREETWOREDUCTOR_HPP_

#include <vector>
#include <set>
#include <iterator>

#include "reduction/solutions/TwoToOneDependentSolution.hpp"
#include "reduction/solutions/NtoNDependentSolution.hpp"
#include "reduction/PBQPReduction.hpp"
#include "math/InfinityWrapper.hpp"

namespace pbqppapa {

template<typename T>
class PBQPGraph;
template<typename T>
class PBQPEdge;
template<typename T>
class NtoNDependentSolution;
template<typename T>
class PBQPSolution;
template<typename T>
class PBQPNode;
template<typename T>
class PBQP_Reduction;

template<typename T>
class DegreeTwoReducer: public PBQP_Reduction<T> {
private:
	std::vector<NtoNDependentSolution<T>*> solutions;

public:
	DegreeTwoReducer(PBQPGraph<T>* graph) :
			PBQP_Reduction<T>(graph) {
	}

	~DegreeTwoReducer() {
		for (NtoNDependentSolution<T>* sol : solutions) {
			delete sol;
		}
	}

	std::vector<PBQPGraph<T>*>& reduce() {
		auto iter = this->graph->getNodeBegin();
		while (iter != this->graph->getNodeEnd()) {
			PBQPNode<T>* node = *iter++;
			if (node->getDegree() == 2) {
				NtoNDependentSolution<T>* sol = reduceDegreeTwo(node,
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

	/**
	 * Reduces a given node of degree 2. Based on the selection of the adjacent nodes,
	 * an ideal selection in the given node is calculated, which is then transformed into
	 * a single edge connecting the nodes adjacent to the given node
	 */
	static NtoNDependentSolution<T>* reduceDegreeTwo(PBQPNode<T>* node,
			PBQPGraph<T>* graph) {
		//TODO exception if vector degree of any is 0
		std::vector<PBQPNode<T>*> dependencyNodes;
		auto it = graph->getEdgeBegin();
		PBQPEdge<T>* firstEdge = *it++;
		PBQPNode<T>* firstNode = firstEdge->getOtherEnd(node);
		PBQPEdge<T>* secondEdge = *it;
		PBQPNode<T>* secondNode = secondEdge->getOtherEnd(node);
		dependencyNodes.push_back(firstNode);
		dependencyNodes.push_back(secondNode);
		bool isFirstSource = firstEdge->isSource(firstNode);
		bool isSecondSource = secondEdge->isSource(secondNode);
		std::vector<PBQPNode<T>*> solutionNodes;
		solutionNodes.push_back(node);
		NtoNDependentSolution<T>* solution = new NtoNDependentSolution<T>(
				dependencyNodes, solutionNodes);
		//edge will go from first to second node
		Matrix<T> resultMatrix(firstNode->getVectorDegree(),
				secondNode->getVectorDegree());
		for (unsigned short firstSelection = 0;
				firstSelection < firstNode->getVectorDegree();
				firstSelection++) {
			for (unsigned short secondSelection = 0;
					secondSelection < secondNode->getVectorDegree();
					secondSelection++) {
				T minimum = node->getVector().get(0);
				unsigned short minimalNodeSelection = 0;
				if (isFirstSource) {
					minimum += firstEdge->getMatrix().get(firstSelection, 0);
				} else {
					minimum += firstEdge->getMatrix().get(0, firstSelection);
				}
				if (isSecondSource) {
					minimum += secondEdge->getMatrix().get(secondSelection, 0);
				} else {
					minimum += secondEdge->getMatrix().get(0, secondSelection);
				}
				for (unsigned short nodeSelection = 1;
						nodeSelection < node->getVectorDegree();
						nodeSelection++) {
					T sum = node->getVector().get(nodeSelection);
					if (isFirstSource) {
						sum += firstEdge->getMatrix().get(firstSelection,
								nodeSelection);
					} else {
						sum += firstEdge->getMatrix().get(nodeSelection,
								firstSelection);
					}
					if (isSecondSource) {
						sum += secondEdge->getMatrix().get(secondSelection,
								nodeSelection);
					} else {
						sum += secondEdge->getMatrix().get(nodeSelection,
								secondSelection);
					}
					if (sum < minimum) {
						minimum = sum;
						minimalNodeSelection = nodeSelection;
					}
				}
				std::vector<unsigned short> solutionSelection;
				solutionSelection.push_back(minimalNodeSelection);
				std::vector<unsigned short> dependencySelection;
				dependencySelection.push_back(firstSelection);
				dependencySelection.push_back(secondSelection);
				resultMatrix.get(firstSelection, secondSelection) = minimum;
				solution->setSolution(dependencySelection, solutionSelection);
			}
		}
		graph->removeNode(node);
		graph->addEdge(firstNode, secondNode, resultMatrix);
		return solution;
	}

	static TwotoOneDependentSolution<InfinityWrapper<T>>* reduceDegreeTwoInf(
			PBQPNode<InfinityWrapper<T>>* node,
			PBQPGraph<InfinityWrapper<T>>* graph) {
		assert(node->getVectorDegree() > 0);
		assert(node->getDegree() == 2);
		auto it = graph->getEdgeBegin();
		PBQPEdge<InfinityWrapper<T>>* const firstEdge = *it++;
		PBQPNode<InfinityWrapper<T>>* const firstNode = firstEdge->getOtherEnd(
				node);
		PBQPEdge<InfinityWrapper<T>>* const secondEdge = *it;
		PBQPNode<InfinityWrapper<T>>* const secondNode = secondEdge->getOtherEnd(
				node);
		const bool isFirstSource = firstEdge->isSource(firstNode);
		const bool isSecondSource = secondEdge->isSource(secondNode);
		TwotoOneDependentSolution<InfinityWrapper<T>>* solution =
				new TwotoOneDependentSolution<InfinityWrapper<T>>(node,
						firstNode, secondNode);
		Matrix<InfinityWrapper<T>> resultMatrix(firstNode->getVectorDegree(),
				secondNode->getVectorDegree());
		bool foundSolution = false;
		for (unsigned short firstSelection = 0;
				firstSelection < firstNode->getVectorDegree();
				firstSelection++) {
			if (firstNode->getVector().get(firstSelection).isInfinite()) {
				//we just leave these values uninitialized, they will never be chosen (hopefully)
				continue;
			}
			for (unsigned short secondSelection = 0;
					secondSelection < secondNode->getVectorDegree();
					secondSelection++) {
				if (secondNode->getVector().get(secondSelection).isInfinite()) {
					//we just leave these values uninitialized, they will never be chosen (hopefully)
					continue;
				}
				InfinityWrapper<T> minimum = node->getVector().get(0);
				unsigned short minimalNodeSelection = 0;
				if (isFirstSource) {
					minimum += firstEdge->getMatrix().get(firstSelection, 0);
				} else {
					minimum += firstEdge->getMatrix().get(0, firstSelection);
				}
				if (isSecondSource) {
					minimum += secondEdge->getMatrix().get(secondSelection, 0);
				} else {
					minimum += secondEdge->getMatrix().get(0, secondSelection);
				}
				for (unsigned short nodeSelection = 1;
						nodeSelection < node->getVectorDegree();
						nodeSelection++) {
					InfinityWrapper<T> sum = node->getVector().get(
							nodeSelection);
					if (isFirstSource) {
						sum += firstEdge->getMatrix().get(firstSelection,
								nodeSelection);
					} else {
						sum += firstEdge->getMatrix().get(nodeSelection,
								firstSelection);
					}
					if (isSecondSource) {
						sum += secondEdge->getMatrix().get(secondSelection,
								nodeSelection);
					} else {
						sum += secondEdge->getMatrix().get(nodeSelection,
								secondSelection);
					}
					if (sum < minimum) {
						minimum = sum;
						minimalNodeSelection = nodeSelection;
					}
				}
				if (!minimum.isInfinite()) {
					foundSolution = true;
				}
				solution->setSolutionSelection(firstSelection, secondSelection,
						minimalNodeSelection);
			}
		}
		if (!foundSolution) {
			return NULL;
		}
		graph->removeNode(node);
		graph->addEdge(firstNode, secondNode, resultMatrix);
		return solution;
	}
};

}

#endif /* REDUCTION_DEGREETWOREDUCTOR_HPP_ */
