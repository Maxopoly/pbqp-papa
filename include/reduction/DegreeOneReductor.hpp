#ifndef REDUCTION_DEGREEONEREDUCTOR_HPP_
#define REDUCTION_DEGREEONEREDUCTOR_HPP_

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

/**
 * Removes nodes of degree one by locally optimizing them and adding their cost to the one other
 * node they are connected to
 */
template<typename T>
class DegreeOneReductor: public PBQP_Reduction<T> {
private:
	std::vector<Dependent_Solution<T>*> solutions;

public:
	DegreeOneReductor(PBQP_Graph<T>*) :
			PBQP_Reduction<T>(graph) {
		solutions = *new std::vector<Dependent_Solution<T>*>();
	}

	~DegreeOneReductor() {
		for (Dependent_Solution<T>* sol : solutions) {
			delete sol;
		}
	}

	std::vector<PBQP_Graph*>* reduce() {
		std::vector<PBQP_Node<T>*>* nodes = graph->getNodes();
		std::vector<PBQP_Node<T>*>* targetNodes =
				new std::vector<PBQP_Node<T>*>();
		std::vector<int>* nodeSolution = new std::vector<int>();
		for (int i = 0; i < graph->getNodeCount(); i++) {
			if (((*nodes)[i])->getDegree() == 1) {
				PBQP_Node<T>* node = ((*nodes)[i]);
				reduceDegreeOne(node);
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

	Dependent_Solution<T>* reduceDegreeOne(PBQP_Node<T>* node) {
		//will explode if node doesnt have an edge
		PBQP_Edge<T>* edge = (*node->getAdjacentEdges())[0];
		PBQP_Node<T>* otherEnd = edge->getOtherEnd(node);
		std::vector<PBQP_Node*> dependencyNodes =
				*new std::vector<PBQP_Node*>();
		std::vector<PBQP_Node*> solutionNodes = *new std::vector<PBQP_Node*>();
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
		}
		return solution;
	}

	PBQP_Solution<T>* solve(PBQP_Solution<T>* solution) {
		this->solution->solve(solution);
		return solution;
	}

private:
	inline T calcSum(unsigned short int sourceSelection,
			unsigned short int targetSelection, PBQP_Edge<T>* edge) {
		T sum = new T();
		sum += edge->getSource()->getVektor()->get(sourceSelection);
		sum += edge->getTarget()->getVektor()->get(targetSelection);
		sum += edge->getMatrix()->get(sourceSelection, targetSelection);
		return sum;
	}
};

#endif /* REDUCTION_DEGREEONEREDUCTOR_HPP_ */
