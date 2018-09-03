#ifndef REDUCTION_DEGREETWOREDUCTOR_HPP_
#define REDUCTION_DEGREETWOREDUCTOR_HPP_

#include <vector>
#include <set>
#include <iterator>

template<typename T>
class PBQP_Graph;
template<typename T>
class PBQP_Edge;
template<typename T>
class Dependent_Solution;
template<typename T>
class PBQP_Solution;
template<typename T>
class PBQP_Node;
template<typename T>
class PBQP_Reduction;

template<typename T>
class DegreeTwoReductor: public PBQP_Reduction<T> {
private:
	std::vector<Dependent_Solution<T>*> solutions;

public:
	DegreeTwoReductor(PBQP_Graph<T>* graph) :
			PBQP_Reduction<T>(graph) {
		solutions = *new std::vector<Dependent_Solution<T>*>();
	}

	~DegreeTwoReductor() {
		for (Dependent_Solution<T>* sol : solutions) {
			delete sol;
		}
	}

	std::vector<PBQP_Graph*>* reduce() {
		for (PBQP_Node<T>* node : *(graph->getNodes())) {
			if (node->getDegree() == 2) {
				Dependent_Solution<T>* sol = reduceDegreeTwo(node);
				solutions->push_back(sol);
			}
		}
		Dependent_Solution<T>* solution = new Dependent_Solution<T>(new std::vector<PBQP_Node*>(0),
				targetNodes);
		solution->setSolution(new std::vector<int>(0), nodeSolution);
		result->push_back(graph);
		return result;
	}

	PBQP_Solution<T>* solve(PBQP_Solution<T>* solution) {
		this->solution->solve(solution);
		return solution;
	}

	/**
	 * Reduces a given node of degree 2. Based on the selection of the adjacent nodes,
	 * an ideal selection in the given node is calculated, which is then transformed into
	 * a single edge connecting the nodes adjacent to the given node
	 */
	static Dependent_Solution<T>* reduceDegreeTwo(PBQP_Node<T>* node,
			PBQP_Graph<T>* graph) {
		std::vector<PBQP_Node*> dependencyNodes =
				*new std::vector<PBQP_Node*>();
		std::set<PBQP_Edge*>::iterator it = graph->getEdges()->begin();
		PBQP_Edge<T>* firstEdge = *it;
		PBQP_Node<T>* firstNode = firstEdge->getOtherEnd(node);
		PBQP_Edge<T>* secondEdge = *it;
		PBQP_Node<T>* secondNode = firstEdge->getOtherEnd(node);
		dependencyNodes.push_back(firstNode);
		dependencyNodes.push_back(secondNode);
		bool isFirstSource = firstEdge->isSource(firstNode);
		bool isSecondSource = secondEdge->isSource(secondNode);
		std::vector<PBQP_Node*> solutionNodes = *new std::vector<PBQP_Node*>();
		solutionNodes.push_back(node);
		Dependent_Solution<T>* solution = new Dependent_Solution<T>(
				dependencyNodes, solutionNodes);
		Matrix<T>* resultMatrix = new Matrix<T>(secondNode->getVektorDegree(),
				firstNode->getVektorDegree());
		for (unsigned short int firstSelection = 0;
				firstSelection < firstNode->getVektorDegree();
				firstSelection++) {
			for (unsigned short int secondSelection = 0;
					secondSelection < secondNode->getVektorDegree();
					secondSelection++) {
				T maximum = node->getVektor()->get(0);
				unsigned short int maximumNodeSelection = 0;
				if (isFirstSource) {
					maximum += firstEdge->getMatrix()->get(firstSelection, 0);
				} else {
					maximum += firstEdge->getMatrix()->get(0, firstSelection);
				}
				if (isSecondSource) {
					maximum += secondEdge->getMatrix()->get(secondSelection, 0);
				} else {
					maximum += secondEdge->getMatrix()->get(0, secondSelection);
				}
				for (unsigned short nodeSelection = 0;
						nodeSelection < node->getVektorDegree();
						nodeSelection++) {
					T sum = node->getVektor()->get(nodeSelection);
					if (isFirstSource) {
						sum += firstEdge->getMatrix()->get(firstSelection,
								nodeSelection);
					} else {
						sum += firstEdge->getMatrix()->get(nodeSelection,
								firstSelection);
					}
					if (isSecondSource) {
						sum += secondEdge->getMatrix()->get(secondSelection,
								nodeSelection);
					} else {
						sum += secondEdge->getMatrix()->get(nodeSelection,
								secondSelection);
					}
					if (sum > maximum) {
						maximum = sum;
						maximumNodeSelection = nodeSelection;
					}
				}
				std::vector<unsigned short int> solutionSelection =
						*new std::vector<unsigned short int>();
				solutionSelection.push_back(maximumNodeSelection);
				std::vector<unsigned short int> dependencySelection =
						*new std::vector<unsigned short int>();
				dependencySelection.push_back(firstSelection);
				dependencySelection.push_back(secondSelection);
				resultMatrix->get(firstSelection, secondSelection) = maximum;
				solution->setSolution(dependencySelection, solutionSelection);
			}
		}
		graph->removeNode(node);
		graph->addEdge(firstNode, secondNode, resultMatrix);
		return solution;
	}
};

#endif /* REDUCTION_DEGREETWOREDUCTOR_HPP_ */
