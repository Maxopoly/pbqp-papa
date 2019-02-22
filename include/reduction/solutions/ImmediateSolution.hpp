#ifndef REDUCTION_SOLUTIONS_IMMEDIATESOLUTION_HPP_
#define REDUCTION_SOLUTIONS_IMMEDIATESOLUTION_HPP_

#include <vector>

namespace pbqppapa {

template<typename T>
class PBQPSolution;
template<typename T>
class PBQPNode;
template<typename T>
class DependentSolution;

template<typename T>
class ImmediateSolution: public DependentSolution<T> {

private :
	unsigned short selection;
	PBQPNode<T>* node;
	std::vector<PBQPEdge<T>> edges;

public:
	ImmediateSolution(PBQPNode<T>* node, unsigned short selection) : selection(selection), node(node) {
		for(PBQPEdge<T>* edge : node->getAdjacentEdges()) {
			edges.push_back(*edge);
		}
	}

	~ImmediateSolution() {

	}

	void solve(PBQPSolution<T>* solution) override {
		solution->setSolution(node->getIndex(), selection);
	}

	void revertChange(PBQPGraph<T>* graph) const {
		graph->addNode(node);
		for(PBQPEdge<T> edge : edges) {
			graph->addEdge(edge.getSource(), edge.getTarget(), edge.getMatrix());
		}
	}

	PBQPNode<T>* getReducedNode() override {return node;}


};
}

#endif /* REDUCTION_SOLUTIONS_IMMEDIATESOLUTION_HPP_ */
