#ifndef GRAPH_PBQPSolution_HPP_
#define GRAPH_PBQPSolution_HPP_

#include <vector>
#include <assert.h>

#include "graph/PBQPGraph.hpp"

namespace pbqppapa {

template<typename T>
class PBQPGraph;
template<typename T>
class PBQPNode;
template<typename T>
class PBQPEdge;

template<typename T>
class PBQPSolution {
private:
	//TODO How do we represent positions that are not decided yet?
	std::vector<unsigned short> selection;
#ifndef NDEBUG
	std::vector<bool> selectionsConfirmed;
#endif

public:
	PBQPSolution(unsigned long length) :
			selection(std::vector<unsigned short>(length))
#ifndef NDEBUG
					, selectionsConfirmed(std::vector<bool>(length))
#endif

	{
	}

	void setSolution(unsigned long nodeIndex, unsigned short solution) {
#ifndef NDEBUG
		selectionsConfirmed.at(nodeIndex) = true;
#endif
		selection.at(nodeIndex) = solution;
	}
	unsigned short getSolution(unsigned long nodeIndex) {
		assert(selectionsConfirmed.at(nodeIndex));
		return selection.at(nodeIndex);
	}

	T getTotalCost(PBQPGraph<T>* graph) {
		T result = T();
		for (auto iter = graph->getNodeBegin(); iter != graph->getNodeEnd();
				iter++) {
			PBQPNode<T>* node = *iter;
			assert(selectionsConfirmed.at(node->getIndex()));
			unsigned short chosenSelection = selection.at(node->getIndex());
			result += node->getVector().get(chosenSelection);
		}
		for (auto iter = graph->getEdgeBegin(); iter != graph->getEdgeEnd();
				iter++) {
			PBQPEdge<T>* edge = *iter;
			unsigned short sourceSelection = selection.at(
					edge->getSource()->getIndex());
			unsigned short targetSelection = selection.at(
					edge->getTarget()->getIndex());
			result += edge->getMatrix().get(sourceSelection, targetSelection);
		}
		return result;
	}
};

}

#endif /* GRAPH_PBQPSolution_HPP_ */
