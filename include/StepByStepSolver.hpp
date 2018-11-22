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
class StepByStepSolver {
private:

	PBQPGraph<T>* graph;
	std::vector<PBQPNode<T>*> peo;
	std::vector<NtoNDependentSolution<T>*> localSolutions;
	bool useRnAlready;
	std::queue<PBQPNode<T>*> nodeQueue;
	typename std::set<PBQPNode<T>*>::iterator nodeIterator;
	typename std::vector<PBQPNode<T>*>::iterator peoIterator;

public:

	StepByStepSolver(PBQPGraph<T>* graph, std::vector<PBQPNode<T>*> peo) :
			graph(graph), peo(peo), useRnAlready(false), nodeIterator(graph->getNodeBegin()) {
	}

	~StepByStepSolver() {
	}

	PBQPNode<T>* stepForward() {
		while (true) {
			if (nodeQueue.empty()) {
				if (useRnAlready) {
					while(peoIterator != peo.end()) {
						PBQPNode<T>* peoNode = *peoIterator++;
						if (!peoNode->isDeleted()) {
							applyRN(peoNode);
							return peoNode;
						}
						//entire peo has been solved
						return 0;
					}
				}
				else {
				if (nodeIterator == graph->getNodeEnd()) {
						useRnAlready = true;
						continue;
				}
				PBQPNode<T>* node = *nodeIterator++;
				nodeQueue.push(node);
				}
			}
			PBQPNode<T>* node = nodeQueue.front();
			nodeQueue.pop();
			if (node->isDeleted()) {
				continue;
			}
			if (applyR0R1R2(node)) {
				return node;
			}
		}
	}

	bool applyR0R1R2(PBQPNode<T>* node) {
		unsigned short degree = node->getDegree();
		switch (degree) {
		case 2: {
			std::vector<PBQPNode<T>*> neighbors = node->getAdjacentNodes(false);
			unsigned short oldDegree = neighbors.at(0)->getDegree();
			localSolutions.push_back(
					DegreeTwoReducer<T>::reduceDegreeTwo(node, this->graph));
			if (neighbors.at(0)->getDegree() != oldDegree) {
				//this happens if the edge created by the R2 reduction is merged into an existing edge
				nodeQueue.push(neighbors.at(0));
				nodeQueue.push(neighbors.at(1));
			}
			return true;
		}
		case 0:
			localSolutions.push_back(
					DegreeZeroReducer<T>::reduceDegreeZero(node, this->graph));
			return true;
		case 1: {
			PBQPNode<T>* other = node->getAdjacentNodes().at(0);
			localSolutions.push_back(
					DegreeOneReducer<T>::reduceDegreeOne(node, this->graph));
			nodeQueue.push(other);
			return true;
		}
		default:
			return false;
		}
	}

	void applyRN(PBQPNode<T>* node) {
		for(PBQPNode <T>* neighbor : node->getAdjacentNodes()) {
			nodeQueue.push(neighbor);
		}
		localSolutions.push_back(
				DegreeNReducer<T>::reduceRNEarlyDecision(node,
						this->graph));
	}

	std::vector<PBQPNode<T>*> stepForward(int howMany) {
		std::vector<PBQPNode<T>*> result;
		for (unsigned int i = 0; i < howMany; i++) {
			result.push_back(stepForward());
		}
		return result;
	}

	PBQPNode<T>* stepBackward() {
		if (localSolutions.size() == 0) {
			return 0;
		}
		unsigned int index = localSolutions.size() - 1;
		NtoNDependentSolution<T> sol = localSolutions.at(index);
		localSolutions.erase(index);
		sol.revertChange(graph);
	}

	std::vector<PBQPNode<T>*> stepBackward(unsigned int howMany) {
		std::vector<PBQPNode<T>*> result;
		for (unsigned int i = 0; i < howMany; i++) {
			result.push_back(stepBackward());
		}
		return result;
	}

	void dump(std::string path) {
		//TODO
	}

	const PBQPGraph<T>* getGraph() {
		return graph;
	}
};

}

#endif /* STEPBYSTEPSOLVER_HPP_ */
