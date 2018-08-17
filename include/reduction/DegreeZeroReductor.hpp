/*
 * DegreeZero.hpp
 *
 *  Created on: 6 Aug 2018
 *      Author: Max
 */

#ifndef VALIDATION_DEGREEZEROREDUCTOR_HPP_
#define VALIDATION_DEGREEZEROREDUCTOR_HPP_

#include <vector>
#include <reduction/PBQPReduction.hpp>

template<typename T>
class PBQP_Graph;
template<typename T>
class Dependent_Solution;
template<typename T>
class PBQP_Solution;
template<typename T>
class PBQP_Node;

template<typename T>
class DegreeZeroReductor : public PBQP_Reduction <T> {
private:
	Dependent_Solution<T>* solution;

public:
	DegreeZeroReductor(PBQP_Graph<T>*) : PBQP_Reduction<T>(graph) {
		//solution is initialized by the reduction
	}

	~DegreeZeroReductor() {
		delete solution;
	}

	std::vector<PBQP_Graph*>* reduce() {
		std::vector<PBQP_Node<T>*>* nodes = graph->getNodes();
		std::vector<PBQP_Node<T>*>* targetNodes = new std::vector<PBQP_Node<T>*>();
		std::vector<int>* nodeSolution = new std::vector<int>();
		for (int i = 0; i < graph->getNodeCount(); i++) {
			if (((*nodes)[i])->getDegree() == 0) {
				PBQP_Node<T>* node = ((*nodes)[i]);
				nodeSolution->push_back(
						node->getVektor()->getIndexOfSmallestElement());
				targetNodes->push_back(node);
				graph->removeNode(node);
				i--;
			}
		}
		solution = new Dependent_Solution<T>(new std::vector<PBQP_Node*>(0),
				targetNodes);
		solution->setSolution(new std::vector<int>(0), nodeSolution);
		result->push_back(graph);
		return result;
	}

	PBQP_Solution<T>* solve(PBQP_Solution<T>* solution) {
		this->solution->solve(solution);
		return solution;
	}
};

#endif /* VALIDATION_DEGREEZEROREDUCTOR_HPP_ */
