#ifndef GRAPH_PBQPSolution_HPP_
#define GRAPH_PBQPSolution_HPP_

#include <vector>
#include <assert.h>
#include <stdio.h>
#include <iostream>

#include "graph/PBQPGraph.hpp"
#include "io/TypeSerializers.hpp"

namespace pbqppapa {

template<typename T>
class PBQPGraph;
template<typename T>
class PBQPNode;
template<typename T>
class PBQPEdge;
template<typename T>
class TypeSerializers;

template<typename T>
class PBQPSolution {
private:
	std::vector<unsigned short> selection;
#ifndef NDEBUG
	std::vector<bool> selectionsConfirmed;
#endif

public:
	PBQPSolution(unsigned int length) :
			selection(std::vector<unsigned short>(length)), selectionsConfirmed(
					std::vector<bool>(length, false))

	{
	}

	virtual ~PBQPSolution() {

	}

	void setSolution(unsigned int nodeIndex, unsigned short solution) {
		selectionsConfirmed.at(nodeIndex) = true;
		selection.at(nodeIndex) = solution;
	}

	unsigned short getSolution(unsigned int nodeIndex) {
		assert(selectionsConfirmed.at(nodeIndex));
		return selection.at(nodeIndex);
	}

	unsigned short getSolution(PBQPNode<T>* node) {
		assert(node);
		return getSolution(node->getIndex());
	}

	virtual T getTotalCost(const PBQPGraph<T>* graph) {
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

	virtual T getCurrentCost(const PBQPGraph<T>* graph) {
		T result = T();
		for (auto iter = graph->getNodeBegin(); iter != graph->getNodeEnd();
				iter++) {
			PBQPNode<T>* node = *iter;
			if (!selectionsConfirmed.at(node->getIndex())) {
				continue;
			}
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
			if (!selectionsConfirmed.at(edge->getSource()->getIndex())) {
							continue;
			}
			if (!selectionsConfirmed.at(edge->getTarget()->getIndex())) {
							continue;
			}
			result += edge->getMatrix().get(sourceSelection, targetSelection);
		}
		return result;
	}

	bool hasSolution(unsigned int nodeIndex) {
		return selectionsConfirmed.at(nodeIndex);
	}

	unsigned int getNodesSolvedCount() {
		unsigned int result = 0;
		for(const bool& value: selectionsConfirmed) {
			if (value) {
				result++;
			}
		}
		return result;
	}

	bool hasSolution(PBQPNode<T>* node) {
		assert(node);
		return hasSolution(node->getIndex());
	}
};

}

#endif /* GRAPH_PBQPSolution_HPP_ */
