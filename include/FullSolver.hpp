#ifndef FULLSOLVER_HPP_
#define FULLSOLVER_HPP_

#include <vector>

#include "analysis/PBQPHandler.hpp"
#include "reduction/vektordegree/VektorDegreeOneReductor.hpp"
#include "reduction/vektordegree/VektorDegreeZeroReductor.hpp"

namespace pbqppapa {

template<typename T>
class FullSolver;

template<typename T>
class FullSolver: PBQPHandler<T> {

private:
	bool hasVektorDegreeZeroOrOne;

public:

	FullSolver(PBQPGraph<T>* graph) :
			PBQPHandler<T>(graph) {
	}

	~FullSolver() {

	}

	PBQPSolution<T>* solve() {
		auto iter = this->graph->getNodeBegin();
		std::vector<DependentSolution<T>*> localSolutions;
		//always filter out common nonsense like degree 0/1 out, which would break our advanced reductions
		if (hasVektorDegreeZeroOrOne) {
			while (iter != this->graph->getNodeEnd()) {
				unsigned short degree = (*iter)->getVectorDegree();
				if (degree == 1) {
					localSolutions.push_back(
							VektorDegreeOneReductor::reduceDegreeOne(*iter,
									graph));
				} else if (degree == 0) {
					localSolutions.push_back(
							VektorDegreeZeroReductor::reduceDegreeZero(*iter,
									graph));
				}
				iter++;
			}
		}
		iter = this->graph->getNodeBegin();
		//standard r0-r2
		while (iter != this->graph->getNodeEnd()) {
			unsigned short degree = (*iter)->getDegree();
			if (degree == 2) {
				localSolutions.push_back(
						DegreeTwoReductor::reduceDegreeTwo(*iter, graph));
			} else if (degree == 0) {
				localSolutions.push_back(
						DegreeZeroReductor::reduceDegreeZero(*iter, graph));
			} else {
				PBQPNode<T>* node = *iter;
				PBQPNode<T>* other;
				//make sure R1 cascades properly
				while (node->getDegree() == 1) {
					other = node->getAdjacentEdges().at(0).getOtherEnd(node);
					localSolutions.push_back(
							DegreeOneReductor::reduceDegreeOne(node, graph));
					node = other;
					if (node == *iter && node->getDegree() == 1) {
						iter++;
					}
				}
			}
			iter++;
		}
		PBQPSolution<T>* solution = new PBQPSolution<T>(
				graph->getNodeIndexCounter());
	}

};

}

#endif /* FULLSOLVER_HPP_ */
