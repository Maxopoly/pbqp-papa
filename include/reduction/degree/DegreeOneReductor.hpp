#ifndef REDUCTION_DEGREEONEREDUCTOR_HPP_
#define REDUCTION_DEGREEONEREDUCTOR_HPP_

#include <vector>
#include <reduction/PBQPReduction.hpp>

namespace pbqppapa {

template<typename T>
class PBQPGraph;
template<typename T>
class Dependent_Solution;
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
	std::vector<Dependent_Solution<T>*> solutions;

public:
	DegreeOneReductor(PBQPGraph<T>*) :
			PBQP_Reduction<T>(graph) {
		solutions = *new std::vector<Dependent_Solution<T>*>();
	}

	~DegreeOneReductor() {
		for (Dependent_Solution<T>* sol : solutions) {
			delete sol;
		}
	}

	std::vector<PBQPGraph*>* reduce() {
		for (PBQPNode<T>* node : *(graph->getNodes())) {
			if (node->getDegree() == 1) {
				Dependent_Solution<T>* sol = reduceDegreeOne(node);
				solutions->push_back(sol);
			}
		}
		Dependent_Solution<T>* solution = new Dependent_Solution<T>(
				new std::vector<PBQPNode*>(0), targetNodes);
		solution->setSolution(new std::vector<int>(0), nodeSolution);
		result->push_back(graph);
		return result;
	}

	static Dependent_Solution<T>* reduceDegreeOne(PBQPNode<T>* node,
			PBQPGraph<T>* graph) {
		//will explode if node doesnt have an edge
		PBQPEdge<T>* edge = (*node->getAdjacentEdges())[0];
		PBQPNode<T>* otherEnd = edge->getOtherEnd(node);
		std::vector<PBQPNode*> dependencyNodes = *new std::vector<PBQPNode*>();
		std::vector<PBQPNode*> solutionNodes = *new std::vector<PBQPNode*>();
		dependencyNodes.push_back(otherEnd);
		solutionNodes.push_back(node);
		Dependent_Solution<T>* solution = new Dependent_Solution<T>(
				dependencyNodes, solutionNodes);
		//TODO proof against vektor of length 0
		for (unsigned short int i = 0; i < otherEnd->getVektorDegree(); i++) {
			//find minimum for this selection
			T maximum;
			if (edge->isSource(node)) {
				maximum += calcSum(0, i, edge);
			} else {
				maximum += calcSum(i, 0, edge);
			}
			unsigned short int maxSelection = 0;
			for (unsigned short int k = 1; k < node->getVektorDegree(); k++) {
				T compSum;
				if (edge->isSource(node)) {
					compSum += calcSum(k, i, edge);
				} else {
					compSum += calcSum(i, k, edge);
				}
				if (compSum > maximum) {
					maximum = compSum;
					maxSelection = k;
				}
			}
			std::vector<unsigned short int> dependencySelections =
					*new std::vector<unsigned short int>();
			std::vector<unsigned short int> solutionSelections =
					*new std::vector<unsigned short int>();
			dependencySelections.push_back(i);
			solutionSelections.push_back(maxSelection);
			solution->setSolution(dependencySelections, solutionSelections);
			otherEnd->getVektor()->get(i) = maximum;
		}
		graph->removeNode(node);
		//TODO chain effect?
		return solution;
	}

	PBQPSolution<T>* solve(PBQPSolution<T>* solution) {
		this->solution->solve(solution);
		return solution;
	}

private:
	static inline T calcSum(unsigned short int sourceSelection,
			unsigned short int targetSelection, PBQPEdge<T>* edge) {
		T sum = new T();
		sum += edge->getSource()->getVektor()->get(sourceSelection);
		sum += edge->getTarget()->getVektor()->get(targetSelection);
		sum += edge->getMatrix()->get(sourceSelection, targetSelection);
		return sum;
	}
};

}

#endif /* REDUCTION_DEGREEONEREDUCTOR_HPP_ */
