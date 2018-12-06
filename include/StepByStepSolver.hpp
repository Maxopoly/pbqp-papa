#ifndef STEPBYSTEPSOLVER_HPP_
#define STEPBYSTEPSOLVER_HPP_

#include <vector>
#include <queue>

#include "graph/PBQPGraph.hpp"
#include "reduction/solutions/NtoNDependentSolution.hpp"
#include "graph/PBQPSolution.hpp"
#include "reduction/degree/DegreeOneReducer.hpp"
#include "reduction/degree/DegreeTwoReducer.hpp"
#include "reduction/degree/DegreeZeroReducer.hpp"
#include "reduction/heuristic/DegreeNReductor.hpp"
#include "debug/StatKeeper.hpp"
#include "debug/DebugTimer.hpp"
#include "math/InfinityWrapper.hpp"

namespace pbqppapa {

template<typename T>
class PBQPGraph;
template<typename T>
class PBQPSolution;
template<typename T>
class PBQPNode;
template<typename T>
class VektorDegreeOneReducer;
template<typename T>
class VectorDegreeZeroReducer;
template<typename T>
class VectorDegreeZeroReducer;
template<typename T>
class DegreeOneReducer;
template<typename T>
class DegreeZeroReducer;
template<typename T>
class DegreeTwoReducer;
template<typename T>
class InfinityWrapper;

template<typename T>
class StepByStepSolver {
private:

	PBQPGraph<InfinityWrapper<T>>* graph;
	PBQPGraph<InfinityWrapper<T>>* originalGraph;
	std::vector<DependentSolution<InfinityWrapper<T>>*> localSolutions;
	bool useRnAlready;
	std::queue<PBQPNode<InfinityWrapper<T>>*> nodeQueue;
	std::vector<PBQPNode<InfinityWrapper<T>>*> nodeVector;
	typename std::vector<PBQPNode<InfinityWrapper<T>>*>::iterator nodeIterator;
	typename std::vector<PBQPNode<InfinityWrapper<T>>*>::iterator peoIterator;
	//gets default initialized and thus const

public:

	StepByStepSolver(PBQPGraph<InfinityWrapper<T>>* graph) :
			graph(graph), originalGraph(graph), useRnAlready(false) {
		assert(graph->getPEO().size() == graph->getNodeCount());
		peoIterator = graph->getPEO().begin();
		localSolutions.reserve(graph->getNodeCount());
		//copy all nodes in a vector to allow going back on reduction steps
		for (auto nodeIter = graph->getNodeBegin();
				nodeIter != graph->getNodeEnd(); ++nodeIter) {
			nodeVector.push_back(*nodeIter);
		}
		nodeIterator = nodeVector.begin();
	}

	~StepByStepSolver() {
		for (DependentSolution<InfinityWrapper<T>>* solution : localSolutions) {
			delete solution;
		}
	}

private:
	StepByStepSolver<T>* operator=(const StepByStepSolver<T>& other) {
		return 0;
	}

public:
	PBQPNode<InfinityWrapper<T>>* stepForward() {
		while (true) {
			if (nodeQueue.empty()) {
				if (useRnAlready) {
					while (peoIterator != graph->getPEO().end()) {
						PBQPNode<InfinityWrapper<T>>* peoNode = *peoIterator++;
						if (!peoNode->isDeleted()) {
							applyRN(peoNode);
							return peoNode;
						}
						//entire peo has been solved
						return 0;
					}
				} else {
					if (nodeIterator == nodeVector.end()) {
						useRnAlready = true;
						continue;
					}
					PBQPNode<InfinityWrapper<T>>* node = *nodeIterator++;
					nodeQueue.push(node);
				}
			}
			PBQPNode<InfinityWrapper<T>>* node = nodeQueue.front();
			nodeQueue.pop();
			if (node->isDeleted()) {
				continue;
			}
			if (applyR0R1R2(node)) {
				return node;
			}
		}
	}

	bool applyR0R1R2(PBQPNode<InfinityWrapper<T>>* node) {
		unsigned short degree = node->getDegree();
		switch (degree) {
		case 2: {
			std::vector<PBQPNode<InfinityWrapper<T>>*> neighbors =
					node->getAdjacentNodes(false);
			unsigned short oldDegree = neighbors.at(0)->getDegree();
			localSolutions.push_back(
					DegreeTwoReducer<T>::reduceDegreeTwoInf(node, this->graph));
			if (neighbors.at(0)->getDegree() != oldDegree) {
				//this happens if the edge created by the R2 reduction is merged into an existing edge
				nodeQueue.push(neighbors.at(0));
				nodeQueue.push(neighbors.at(1));
			}
			return true;
		}
		case 0:
			localSolutions.push_back(
					DegreeZeroReducer<T>::reduceDegreeZeroInf(node,
							this->graph));
			return true;
		case 1: {
			PBQPNode<InfinityWrapper<T>>* other = node->getAdjacentNodes().at(
					0);
			localSolutions.push_back(
					DegreeOneReducer<T>::reduceDegreeOneInf(node, this->graph));
			nodeQueue.push(other);
			return true;
		}
		default:
			return false;
		}
	}

	void applyRN(PBQPNode<InfinityWrapper<T>>* node) {
		for (PBQPNode<InfinityWrapper<T>>* neighbor : node->getAdjacentNodes()) {
			nodeQueue.push(neighbor);
		}
		localSolutions.push_back(
				DegreeNReducer<T>::reduceRNEarlyDecisionInf(node, this->graph));
	}

	PBQPSolution<InfinityWrapper<T>>* solveFully() {
		while(stepForward()) {
		}
		return retrieveSolution();
	}

	std::vector<PBQPNode<InfinityWrapper<T>>*> stepForward(int howMany) {
		std::vector<PBQPNode<InfinityWrapper<T>>*> result;
		for (unsigned int i = 0; i < howMany; i++) {
			result.push_back(stepForward());
		}
		return result;
	}

	PBQPNode<InfinityWrapper<T>>* stepBackward() {
		if (localSolutions.size() == 0) {
			return 0;
		}
		unsigned int index = localSolutions.size() - 1;
		DependentSolution<InfinityWrapper<T>>* sol = localSolutions.at(index);
		localSolutions.pop_back();
		sol->revertChange(graph);
		PBQPNode<InfinityWrapper<T>>* node = sol->getReducedNode();
		if (useRnAlready) {
			//revert peo
			peoIterator = revertIterator(peoIterator, graph->getPEO().begin(),
					node);
		} else {
			//revert general node iterator
			nodeIterator = revertIterator(nodeIterator, nodeVector.begin(),
					node);
		}
		delete sol;
		return node;
	}

	std::vector<PBQPNode<InfinityWrapper<T>>*> stepBackward(
			unsigned int howMany) {
		std::vector<PBQPNode<InfinityWrapper<T>>*> result;
		for (unsigned int i = 0; i < howMany; i++) {
			result.push_back(stepBackward());
		}
		return result;
	}

	typename std::vector<PBQPNode<InfinityWrapper<T>>*>::iterator revertIterator(
			typename std::vector<PBQPNode<InfinityWrapper<T>>*>::iterator currentIter,
			typename std::vector<PBQPNode<InfinityWrapper<T>>*>::iterator begin,
			PBQPNode<InfinityWrapper<T>>* value) {
		typename std::vector<PBQPNode<InfinityWrapper<T>>*>::iterator revertedIter =
				currentIter;
		while (revertedIter != begin) {
			revertedIter--;
			if (*revertedIter == value) {
				return revertedIter;
			}
		}
		//no reverting needed, element wasnt iterated over yet
		return currentIter;
	}

	void dump(std::string path) {
		//TODO
	}

	PBQPSolution<InfinityWrapper<T>>* retrieveSolution() {
		PBQPSolution<InfinityWrapper<T>>* solution = new PBQPSolution<InfinityWrapper<T>>(
				this->graph->getNodeIndexCounter());
		for (auto iter = localSolutions.rbegin(); iter != localSolutions.rend();
				iter++) {
			(*iter)->solve(solution);
		}
		return solution;
	}

	PBQPGraph<InfinityWrapper<T>>* getGraph() {
		return graph;
	}

	PBQPGraph<InfinityWrapper<T>>* getOriginalGraph() {
		return originalGraph;
	}
};

}

#endif /* STEPBYSTEPSOLVER_HPP_ */
