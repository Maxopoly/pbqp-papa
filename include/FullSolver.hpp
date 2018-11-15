#ifndef FULLSOLVER_HPP_
#define FULLSOLVER_HPP_

#include <vector>
#include <queue>

#include "analysis/PBQPHandler.hpp"
#include "graph/PBQPGraph.hpp"
#include "graph/PBQPSolution.hpp"
#include "reduction/degree/DegreeOneReducer.hpp"
#include "reduction/degree/DegreeTwoReducer.hpp"
#include "reduction/degree/DegreeZeroReducer.hpp"
#include "reduction/vectordegree/VectorDegreeOneReducer.hpp"
#include "reduction/vectordegree/VectorDegreeZeroReducer.hpp"
#include "reduction/SelfCycleRemover.hpp"
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
class FullSolver;

template<typename T>
class FullSolver: PBQPHandler<T> {

private:
	bool hasVektorDegreeZeroOrOne;
	std::vector<NtoNDependentSolution<T>*> localSolutions;

public:

	FullSolver(PBQPGraph<T>* graph) :
			PBQPHandler<T>(graph), hasVektorDegreeZeroOrOne(true) {
	}

	~FullSolver() {
		for(NtoNDependentSolution<T>* sol : localSolutions) {
			delete sol;
		}
	}

	PBQPSolution<T>* solve() {
		auto iter = this->graph->getNodeBegin();
		//always filter out common nonsense like degree 0/1, which would break our advanced reductions
		if (hasVektorDegreeZeroOrOne) {
			while (iter != this->graph->getNodeEnd()) {

				unsigned short degree = (*iter++)->getVectorDegree();
				if (degree == 1) {
					localSolutions.push_back(
							VectorDegreeOneReducer<T>::reduceVectorDegreeOne(
									*iter, this->graph));
				} else if (degree == 0) {
					VectorDegreeZeroReducer<T>::reduceVectorDegreeZero(*iter,
							this->graph);
				}
			}
		}
		iter = this->graph->getNodeBegin();
		//standard r0-r2 as much as possible
		std::queue<PBQPNode<T>*> queue;
		while (iter != this->graph->getNodeEnd()) {
			PBQPNode<T>* node = *iter;
			queue.push(node);
			while (!queue.empty()) {
				node = queue.front();
				queue.pop();
				if (node->isDeleted()) {
					continue;
				}
				if (node == *iter) {
					iter++;
				}
				unsigned short degree = node->getDegree();
				switch (degree) {
				case 2: {
					std::vector<PBQPNode<T>*> neighbors =
							node->getAdjacentNodes(false);
					unsigned short oldDegree = neighbors.at(0)->getDegree();
					localSolutions.push_back(
							DegreeTwoReducer<T>::reduceDegreeTwo(node,
									this->graph));
					if (neighbors.at(0)->getDegree() != oldDegree) {
						//this happens if the edge created by the R2 reduction is merged into an existing edge
						queue.push(neighbors.at(0));
						queue.push(neighbors.at(1));
					}
					break;
				}
				case 0:
					localSolutions.push_back(
							DegreeZeroReducer<T>::reduceDegreeZero(node,
									this->graph));
					break;
				case 1: {
					PBQPNode<T>* other = node->getAdjacentNodes().at(0);
					localSolutions.push_back(
							DegreeOneReducer<T>::reduceDegreeOne(node,
									this->graph));
					queue.push(other);
					break;
				}
				default:
					continue;
				}
			}
		}
		//start applying RN
		iter = this->graph->getNodeBegin();
		while (iter != this->graph->getNodeEnd()) {
			PBQPNode<T>* node = *iter++;
			if (node->isDeleted()) {
				continue;
			}
			for (PBQPNode<T>* neighborNode : node->getAdjacentNodes()) {
				queue.push(neighborNode);
			}
			localSolutions.push_back(
					DegreeNReducer<T>::reduceRNEarlyDecision(node,
							this->graph));
			while (!queue.empty()) {
				node = queue.front();
				queue.pop();
				if (node->isDeleted()) {
					continue;
				}
				if (node == *iter) {
					iter++;
				}
				unsigned short degree = node->getDegree();
				switch (degree) {
				case 2: {
					std::vector<PBQPNode<T>*> neighbors =
							node->getAdjacentNodes(false);
					unsigned short oldDegree = neighbors.at(0)->getDegree();
					localSolutions.push_back(
							DegreeTwoReducer<T>::reduceDegreeTwo(node,
									this->graph));
					if (neighbors.at(0)->getDegree() != oldDegree) {
						//this happens if the edge created by the R2 reduction is merged into an existing edge
						queue.push(neighbors.at(0));
						queue.push(neighbors.at(1));
					}
					break;
				}
				case 0:
					localSolutions.push_back(
							DegreeZeroReducer<T>::reduceDegreeZero(node,
									this->graph));
					break;
				case 1: {
					PBQPNode<T>* other = node->getAdjacentNodes().at(0);
					localSolutions.push_back(
							DegreeOneReducer<T>::reduceDegreeOne(node,
									this->graph));
					queue.push(other);
					break;
				}
				}

			}
		}
		PBQPSolution<T>* solution = new PBQPSolution<T>(
				this->graph->getNodeIndexCounter());
		for (auto iter = localSolutions.rbegin(); iter != localSolutions.rend();
				iter++) {
			(*iter)->solve(solution);
		}
		return solution;
	}


	PBQPSolution<T>* solveInf() {
		auto iter = this->graph->getNodeBegin();
		//standard r0-r2 as much as possible
		std::queue<PBQPNode<T>*> queue;
		while (iter != this->graph->getNodeEnd()) {
			PBQPNode<T>* node = *iter;
			queue.push(node);
			while (!queue.empty()) {
				node = queue.front();
				queue.pop();
				if (node->isDeleted()) {
					continue;
				}
				if (node == *iter) {
					iter++;
				}
				unsigned short degree = node->getDegree();
				switch (degree) {
				case 2: {
					std::vector<PBQPNode<T>*> neighbors =
							node->getAdjacentNodes(false);
					unsigned short oldDegree = neighbors.at(0)->getDegree();
					localSolutions.push_back(
							DegreeTwoReducer<T>::reduceDegreeTwoInf(node,
									this->graph));
					if (neighbors.at(0)->getDegree() != oldDegree) {
						//this happens if the edge created by the R2 reduction is merged into an existing edge
						queue.push(neighbors.at(0));
						queue.push(neighbors.at(1));
					}
					break;
				}
				case 0:
					localSolutions.push_back(
							DegreeZeroReducer<T>::reduceDegreeZeroInf(node,
									this->graph));
					break;
				case 1: {
					PBQPNode<T>* other = node->getAdjacentNodes().at(0);
					localSolutions.push_back(
							DegreeOneReducer<T>::reduceDegreeOneInf(node,
									this->graph));
					queue.push(other);
					break;
				}
				default:
					continue;
				}
			}
		}
		//start applying RN
		//TODO priorize high degree nodes, follow PEO
		iter = this->graph->getNodeBegin();
		while (iter != this->graph->getNodeEnd()) {
			PBQPNode<T>* node = *iter++;
			if (node->isDeleted()) {
				continue;
			}
			for (PBQPNode<T>* neighborNode : node->getAdjacentNodes()) {
				queue.push(neighborNode);
			}
			localSolutions.push_back(
					DegreeNReducer<T>::reduceRNEarlyDecision(node,
							this->graph));
			while (!queue.empty()) {
				node = queue.front();
				queue.pop();
				if (node->isDeleted()) {
					continue;
				}
				if (node == *iter) {
					iter++;
				}
				unsigned short degree = node->getDegree();
				switch (degree) {
				case 2: {
					std::vector<PBQPNode<T>*> neighbors =
							node->getAdjacentNodes(false);
					unsigned short oldDegree = neighbors.at(0)->getDegree();
					localSolutions.push_back(
							DegreeTwoReducer<T>::reduceDegreeTwo(node,
									this->graph));
					if (neighbors.at(0)->getDegree() != oldDegree) {
						//this happens if the edge created by the R2 reduction is merged into an existing edge
						queue.push(neighbors.at(0));
						queue.push(neighbors.at(1));
					}
					break;
				}
				case 0:
					localSolutions.push_back(
							DegreeZeroReducer<T>::reduceDegreeZero(node,
									this->graph));
					break;
				case 1: {
					PBQPNode<T>* other = node->getAdjacentNodes().at(0);
					localSolutions.push_back(
							DegreeOneReducer<T>::reduceDegreeOne(node,
									this->graph));
					queue.push(other);
					break;
				}
				}

			}
		}
		PBQPSolution<T>* solution = new PBQPSolution<T>(
				this->graph->getNodeIndexCounter());
		for (auto iter = localSolutions.rbegin(); iter != localSolutions.rend();
				iter++) {
			(*iter)->solve(solution);
		}
		return solution;
	}

};

}

#endif /* FULLSOLVER_HPP_ */
