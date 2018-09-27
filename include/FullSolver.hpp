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
		//standard r0-r2
		while (iter != this->graph->getNodeEnd()) {
			unsigned short degree = (*iter)->getDegree();
			if (degree == 2) {
				iter++;
				localSolutions.push_back(
						DegreeTwoReducer<T>::reduceDegreeTwo(*iter,
								this->graph));
			} else if (degree == 0) {
				iter++;
				localSolutions.push_back(
						DegreeZeroReducer<T>::reduceDegreeZero(*iter,
								this->graph));
			} else {
				PBQPNode<T>* node = *iter;
				if (node->getDegree() != 1) {
					iter++;
					continue;
				}
				PBQPNode<T>* other;
				//make sure R1 cascades properly
				while (node->getDegree() == 1) {
					other = node->getAdjacentEdges().at(0)->getOtherEnd(node);
					iter++;
					localSolutions.push_back(
							DegreeOneReducer<T>::reduceDegreeOne(node,
									this->graph));
					node = other;
				}
			}
		}
		PBQPSolution<T>* solution = new PBQPSolution<T>(
				this->graph->getNodeIndexCounter());
		return solution;
	}

};

}

#endif /* FULLSOLVER_HPP_ */
