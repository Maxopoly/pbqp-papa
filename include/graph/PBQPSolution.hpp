#ifndef GRAPH_PBQPSolution_HPP_
#define GRAPH_PBQPSolution_HPP_

#include <vector>
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

/**
 * Represents a solution for a PBQP instances. Must be instantiated with a length higher than the highest index
 * of any node in the belonging graph
 */
template<typename T>
class PBQPSolution {
private:
	std::vector<unsigned short> selection;
	unsigned int solvedCount = 0;
#ifndef NDEBUG
	std::vector<bool> selectionsConfirmed;
#endif

public:
	/**
	 * Creates a new blank solution with no selection assigned. Given length must be bigger than the index
	 * of any node for which a solution will be put in this instance
	 */
	PBQPSolution(unsigned int length) :
			selection(std::vector<unsigned short>(length))
#ifndef NDEBUG
			,selectionsConfirmed(std::vector<bool>(length, false))
#endif

	{
	}

	virtual ~PBQPSolution() {

	}

	/**
	 * Sets the selection for a node with the given index to the given selection
	 */
	void setSolution(unsigned int nodeIndex, unsigned short selectionToSetTo) {
		assert(nodeIndex < selectionsConfirmed.size());
		#ifndef NDEBUG
		if (selectionsConfirmed.at(nodeIndex)) {
			solvedCount--;
		}
		else {
			selectionsConfirmed.at(nodeIndex) = true;
		}
		#endif
		selection.at(nodeIndex) = selectionToSetTo;
		solvedCount++;
	}

	/**
	 * Sets the selection for the given node to the given selection
	 */
	void setSolution(PBQPNode<T>* node, unsigned short selection) {
		assert(node);
		setSolution(node->getIndex(), selection);
	}

	/**
	 * Gets the selection solution for the node with the given index. Use 'hasSolutution()' first if you are not
	 * sure whether this node was solved!
	 */
	unsigned short getSolution(unsigned int nodeIndex) const {
		assert(nodeIndex < selection.size());
		assert(selectionsConfirmed.at(nodeIndex));
		return selection.at(nodeIndex);
	}

	/**
	 * Gets the selection solution for the node given. Use 'hasSolutution()' first if you are not
	 * sure whether this node was solved!
	 */
	unsigned short getSolution(PBQPNode<T>* node) const {
		assert(node);
		return getSolution(node->getIndex());
	}

	/**
	 * Assumes all nodes in the given graph are solved and calculates the total cost created by this solution in the given graph.
	 * Will explode if not all nodes in the given graph have a selection assigned in this instance
	 */
	virtual T getTotalCost(const PBQPGraph<T>* graph) const {
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

#ifndef NDEBUG
	/**
	 * Calculates the total cost created by the nodes which currently have a selection assigned in this instance.
	 * Nodes not assigned a selection will be ignored and edges which dont have both sides solved will as well be ignored
	 */
	virtual T getCurrentCost(const PBQPGraph<T>* graph) const {
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

	/**
	 * Checks whether the node with the given index had a selection assigned yet
	 */
	bool hasSolution(unsigned int nodeIndex) const {
		return selectionsConfirmed.at(nodeIndex);
	}

	/**
	 * Checks whether the given node had a selection assigned yet
	 */
	bool hasSolution(PBQPNode<T>* node) const {
		assert(node);
		return selectionsConfirmed.at(node->getIndex());
	}
#endif

	/**
	 * Gets the total amount of selection assigned so far.
	 * If NDEBUG is not set, this counter will be incremented every time a node has a selection assigned, even if the same node
	 * is assigned multiple times
	 * If NDEBUG is set, the counter will only be incremented when a node not given a selection yet is given a selection
	 */
	unsigned int getNodesSolvedCount() {
		return solvedCount;
	}
};

}

#endif /* GRAPH_PBQPSolution_HPP_ */
