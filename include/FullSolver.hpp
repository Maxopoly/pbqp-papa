#ifndef FULLSOLVER_HPP_
#define FULLSOLVER_HPP_

#include <vector>

#include "analysis/PBQPHandler.hpp"
#include "graph/PBQPGraph.hpp"
#include "graph/PBQPSolution.hpp"
#include "reduction/degree/DegreeOneReductor.hpp"
#include "reduction/degree/DegreeTwoReductor.hpp"
#include "reduction/degree/DegreeZeroReductor.hpp"
#include "reduction/vectordegree/VectorDegreeOneReductor.hpp"
#include "reduction/vectordegree/VectorDegreeZeroReductor.hpp"

namespace pbqppapa {

template<typename T>
class PBQPGraph;
template<typename T>
class PBQPSolution;
template<typename T>
class PBQPNode;
template<typename T>
class VektorDegreeOneReductor;
template<typename T>
class VectorDegreeZeroReductor;
template<typename T>
class VectorDegreeZeroReductor;
template<typename T>
class DegreeOneReductor;
template<typename T>
class DegreeZeroReductor;
template<typename T>
class DegreeTwoReductor;

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
							VectorDegreeOneReductor<T>::reduceVectorDegreeOne(
									*iter, this->graph));
				} else if (degree == 0) {
					VectorDegreeZeroReductor<T>::reduceVectorDegreeZero(*iter,
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
						DegreeTwoReductor<T>::reduceDegreeTwo(*iter,
								this->graph));
			} else if (degree == 0) {
				iter++;
				localSolutions.push_back(
						DegreeZeroReductor<T>::reduceDegreeZero(*iter,
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
							DegreeOneReductor<T>::reduceDegreeOne(node,
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
