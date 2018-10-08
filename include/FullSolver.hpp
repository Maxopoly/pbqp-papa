#ifndef FULLSOLVER_HPP_
#define FULLSOLVER_HPP_

#include <vector>

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

public:

	FullSolver(PBQPGraph<T>* graph) :
			PBQPHandler<T>(graph), hasVektorDegreeZeroOrOne(true) {
	}

	~FullSolver() {

	}

	PBQPSolution<T>* solve() {
		StatKeeper stats;
		auto iter = this->graph->getNodeBegin();
		std::vector<DependentSolution<T>*> localSolutions;
		//always filter out common nonsense like degree 0/1, which would break our advanced reductions
		if (hasVektorDegreeZeroOrOne) {
			while (iter != this->graph->getNodeEnd()) {

				unsigned short degree = (*iter)->getVectorDegree();
				if (degree == 1) {
					localSolutions.push_back(
							VectorDegreeOneReducer<T>::reduceVectorDegreeOne(
									*iter, this->graph));
				} else if (degree == 0) {
					VectorDegreeZeroReducer<T>::reduceVectorDegreeZero(*iter,
							this->graph);
				}
				iter++;
			}
		}
		iter = this->graph->getNodeBegin();
		//standard r0-r2 as much as possible
		DebugTimer timerr02 ("R0, R1, R2 Reductions initially");
		timerr02.startTimer();
		while (iter != this->graph->getNodeEnd()) {
			PBQPNode<T>* node = *iter;
			unsigned short degree = node->getDegree();

			switch (degree) {
			case 2:
				iter++;
				localSolutions.push_back(
						DegreeTwoReducer<T>::reduceDegreeTwo(node,
								this->graph));
				stats.applyR2();
				break;
			case 0:
				iter++;
				localSolutions.push_back(
						DegreeZeroReducer<T>::reduceDegreeZero(node,
								this->graph));
				stats.applyR0();
				break;
			case 1:
				PBQPNode<T>* other;
				other = node->getAdjacentEdges().at(0)->getOtherEnd(node);
				iter++;
				localSolutions.push_back(
						DegreeOneReducer<T>::reduceDegreeOne(node,
								this->graph));
				stats.applyR1();
				if (other->getDegree() == 2) {
					if (*iter == other) {
						iter++;
					}
					//only need to handle this case, in all others the degree was already
					//low enough before the reduction
					localSolutions.push_back(
							DegreeTwoReducer<T>::reduceDegreeTwo(other,
									this->graph));
					stats.applyR2();
				}
				break;
			default:
				iter++;

			}
		}
		timerr02.stopTimer();
		//start applying RN
		//TODO priorize high degree nodes, follow PEO
		iter = this->graph->getNodeBegin();
		DebugTimer rNTimer("RN,R2 combined full reduction");
		rNTimer.startTimer();
		while (iter != this->graph->getNodeEnd()) {
			PBQPNode<T>* node = *iter++;
			std::vector<PBQPNode<T>*> neighbors = node->getAdjacentNodes();
			localSolutions.push_back(
					DegreeNReducer<T>::reduceRNEarlyDecision(node,
							this->graph));
			stats.applyRNEarly();
			auto neighborIter = neighbors.begin();
			while (neighborIter != neighbors.end()) {
				PBQPNode<T>* neighbor = *neighborIter++;
				if (neighbor->getDegree() == 2) {
					if (neighbor == *iter) {
						iter++;
					}
					localSolutions.push_back(
							DegreeTwoReducer<T>::reduceDegreeTwo(neighbor,
									this->graph));
					stats.applyR2();
				}
			}
		}
		rNTimer.stopTimer();
		DebugTimer backTrackTimer("Backtracking");
		backTrackTimer.startTimer();
		PBQPSolution<T>* solution = new PBQPSolution<T>(
				this->graph->getNodeIndexCounter());
		for(auto iter = localSolutions.rbegin(); iter != localSolutions.rend(); iter++) {
			(*iter)->solve(solution);
		}
		backTrackTimer.stopTimer();
		std::cout << timerr02.getOutput();
		std::cout << rNTimer.getOutput();
		std::cout << backTrackTimer.getOutput();
		std::cout << stats.getSumUp();
		return solution;
	}

};

}

#endif /* FULLSOLVER_HPP_ */
