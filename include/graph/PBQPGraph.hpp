#ifndef PBQPGRAPH_H_
#define PBQPGRAPH_H_

#include <vector>
#include <set>
#include <iterator>
#include <map>
#include <iostream>

#include "graph/PBQPNode.hpp"
#include "graph/PBQPEdge.hpp"

namespace pbqppapa {

template<typename T>
class PBQPEdge;
template<typename T>
class PBQPSolution;
template<typename T>
class PBQPNode;
template<typename T>
class Matrix;
template<typename T>
class Vector;

/**
 * A graph representing a PBQP. The template type represents the data type of the numbers in the cost vectors
 * and cost matrices. It is consistent throughout the entire graph; all edges and all nodes.
 * Note that deleted nodes are removed from the graph, but not deleted from memory.
 * To ease handling with them the deleted flag on them will be set to true and they will remain in memory until the graph that used
 * to hold them is deleted.
 *
 * You should never manually deleted nodes or edges, leave that entirely to a graph instance
 */
template<typename T>
class PBQPGraph {
private:
	unsigned int indexMaximum = 0;
	std::set<PBQPNode<T>*> nodes;
	std::set<PBQPEdge<T>*> edges;
	std::set<PBQPNode<T>*> deletedNodes;
	std::vector<PBQPNode<T>*> peo;

public:

	/**
	 * Create a new empty graph with no nodes
	 */
	PBQPGraph() {
	}

	/**
	 * Deletes all nodes and edges contained within the graph
	 */
	~PBQPGraph() {
		clear();
	}

	/**
	 * Copy constructor deep copies all nodes, edges and PEO
	 */
	PBQPGraph(const PBQPGraph<T>* graph) :
			indexMaximum(graph->indexMaximum) {
		std::map<PBQPNode<T>*, PBQPNode<T>*> nodeReMapping;
		for (PBQPNode<T>* node : graph->nodes) {
			PBQPNode<T>* createdNode = new PBQPNode<T>(node);
			addNode(createdNode);
			nodeReMapping.insert(
					std::pair<PBQPNode<T>*, PBQPNode<T>*>(node, createdNode));
		}
		for (PBQPEdge<T>* edge : graph->edges) {
			PBQPNode<T>* newSource =
					nodeReMapping.find(edge->getSource())->second;
			PBQPNode<T>* newTarget =
					nodeReMapping.find(edge->getTarget())->second;
			PBQPEdge<T>* createdEdge = new PBQPEdge<T>(newSource, newTarget,
					edge);
			addEdge(createdEdge);
			newSource->addEdge(createdEdge);
			newTarget->addEdge(createdEdge);
		}
		for (PBQPNode<T>* oldNode : graph->peo) {
			PBQPNode<T>* newNode = nodeReMapping.find(oldNode)->second;
			peo.push_back(newNode);
		}
	}

	/**
	 * Completly resets the graph, removes all nodes, edges and peo and deletes them
	 */
	void clear() {
		for (PBQPEdge<T>* edge : edges) {
			delete edge;
		}
		edges.clear();
		for (PBQPNode<T>* node : nodes) {
			delete node;
		}
		nodes.clear();
		for (PBQPNode<T>* node : deletedNodes) {
			delete node;
		}
		peo.clear();
		deletedNodes.clear();
	}

	/**
	 * Creates a new node with the given cost Vector and adds it to the graph.
	 * The new node will not have any edges initially
	 */
	PBQPNode<T>* addNode(Vector<T>& vector) {
		PBQPNode<T>* node = new PBQPNode<T>(indexMaximum++, vector);
		nodes.insert(node);
		return node;
	}

	/**
	 * Directly inserts a preexisting node. No checks are done on the internal state of the node or
	 * its possibly referenced edges. The user must ensure that this is handled properly
	 *
	 * It is fine to add readd nodes which were removed from this graph previously. It is not okay to add nodes
	 * which were removed from another graph
	 */
	void addNode(PBQPNode<T>* node) {
		assert(node);
		nodes.insert(node);
		if (node->isDeleted()) {
			node->setDeleted(false);
			deletedNodes.erase(node);
		}
		if (node->getIndex() >= indexMaximum) {
			indexMaximum = node->getIndex() + 1;
		}
	}

	/**
	 * Directly inserts a preexisting edge. No checks are done on the internal state of the edge or
	 * whether the nodes incident to it are even in the graph. The user must ensure that this is handled properly
	 */
	void addEdge(PBQPEdge<T>* edge) {
		assert(edge);
		edges.insert(edge);
	}

	/**
	 * Creates a new edge and inserts it into the graph. The first node given is the
	 * edges source, the second one its target and the given matrix is the cost matrix that
	 * will be associated with the created edge
	 *
	 * Note that PBQPGraph ensures to never have cycles or multiedges. This means if you try to add an
	 * edge which already exists, the cost of the matrix you gave will be added to the existing edge and
	 * the preexisting edge will be returned.
	 * If the source and target node are identical the diagonal of the given matrix will be added to this
	 * node and 0 will be returned
	 */
	PBQPEdge<T>* addEdge(PBQPNode<T>* source, PBQPNode<T>* target,
			Matrix<T>& matrix) {
		assert(source);
		assert(target);
		assert(source->getVectorDegree() == matrix.getRowCount());
		assert(target->getVectorDegree() == matrix.getColumnCount());
		//add diagonal of matrix to node if it is a cycle
		if (source == target) {
			for(unsigned short i = 0; i < source->getVectorDegree(); i++) {
				source->getVector().get(i) += matrix.get(i, i);
			}
			return 0;
		}
		//add to existing edge if one exists
		for (PBQPEdge<T>* edge : source->getAdjacentEdges(false)) {
			if (edge->getOtherEnd(source) == target) {
				if (edge->getSource() == source) {
					edge->getMatrix() += matrix;
				} else {
					edge->getMatrix() += matrix.transpose();
				}
				return edge;
			}
		}
		PBQPEdge<T>* edge = new PBQPEdge<T>(source, target, matrix);
		edges.insert(edge);
		source->addEdge(edge);
		if (source != target) {
			target->addEdge(edge);
		}
		return edge;
	}

	/**
	 * Removes the given node and all edges connected to it from the graph
	 *
	 * If the cleanUp flag is set, the node and its edges will also be deleted,
	 * if not they're removed from the graph, but left alone otherwise.
	 *
	 * This means that if cleanUp is not set, nodes within the graph may be connected
	 * to nodes that are not in the graph anymore officially, but still known through
	 * the edge references of the nodes. Ensuring consistency and something that makes sense
	 * is up to the user when setting cleanUp to false
	 */
	void removeNode(PBQPNode<T>* node, bool cleanUp = true) {
		for (PBQPEdge<T>* edge : std::vector<PBQPEdge<T>*>(
				node->getAdjacentEdges(false))) {
			edges.erase(edge);
			if (cleanUp) {
				edge->getOtherEnd(node)->removeEdge(edge);
				node->removeEdge(edge);
				delete edge;
			}
		}
		nodes.erase(node);
		if (cleanUp) {
			node->setDeleted(true);
			deletedNodes.insert(node);
		}
	}

	/**
	 * Removes the given edge from the graph and deletes it.
	 * The edge is deleted from adjacent nodes as well, but the adjacent nodes stay in the graph
	 */
	void removeEdge(PBQPEdge<T>* edge) {
		edges.erase(edge);
		edge->getSource()->removeEdge(edge);
		edge->getTarget()->removeEdge(edge);
		delete edge;
	}


	/*
	 * Ok I know that the following is a weird iterator pattern but hear me out:
	 * Implementing our own iterator here was out of scope and I was actively recommended against doing so.
	 * I did not want to return the entire set of edges, because that'd heavily violate the encapsulation I'm
	 * trying to achieve here
	 * I could not return a 'const std::set', because it'd contain 'const PBQPNode*' and we want to modify the cost
	 * vectors/matrices of nodes/edges quite often
	 * Additionally nodes will be actively deleted as part of reductions leading to problems when iterating, which is
	 * made a bit less worse by going this way
	 *
	 * TL;DR C++ is bad. Or maybe its just me
	 */

	/**
	 * Gets an iterator to the begin of the nodes in this graph. Consistent with the behavior of std::set,
	 * this iterator stays valid throughout insert() and only gets invalidated if the element its pointing
	 * to is removed
	 */
	typename std::set<PBQPNode<T>*>::iterator getNodeBegin() const {
		return nodes.begin();
	}

	/**
	 * Gets an iterator to the end of the nodes in this graph. Consistent with the behavior of std::set,
	 * this iterator stays valid throughout insert() and only gets invalidated if the element its pointing
	 * to is removed
	 */
	typename std::set<PBQPNode<T>*>::iterator getNodeEnd() const {
		return nodes.end();
	}

	/**
	 * Gets an iterator to the begin of the nodes in this graph. Consistent with the behavior of std::set,
	 * this iterator stays valid throughout insert() and only gets invalidated if the element its pointing
	 * to is removed
	 */
	typename std::set<PBQPEdge<T>*>::iterator getEdgeBegin() const {
		return edges.begin();
	}

	/**
	 * Gets an iterator to the begin of the nodes in this graph. Consistent with the behavior of std::set,
	 * this iterator stays valid throughout insert() and only gets invalidated if the element its pointing
	 * to is removed
	 */
	typename std::set<PBQPEdge<T>*>::iterator getEdgeEnd() const {
		return edges.end();
	}

	/**
	 * Gets the amount of nodes currently in the graph
	 */
	unsigned int getNodeCount() const {
		return nodes.size();
	}

	/**
	 * Gets the amount of edges currently in the graph
	 */
	unsigned int getEdgeCount() const {
		return edges.size();
	}

	/**
	 * Gets the internal counter for node indices. All nodes that ever existed in any graph so far
	 * will have an index smaller than this counter. To ensure that every node has a unique number, even
	 * if we split up graphs and parallelize work on the smaller pieces, this counter is global
	 */
	unsigned int getNodeIndexCounter() const {
		return indexMaximum;
	}

	std::vector<PBQPNode<T>*>& getPEO() {
		//this should return a const vector, but doesnt due because that'd make its
		//iterators const_iterator which introduced problems in other places and led to
		//code duplication
		return peo;
	}

	/**
	 * Replaces the current PEO with the given one
	 */
	void setPEO(std::vector<PBQPNode<T>*> newPeo) {
		peo = newPeo;
	}
};
}

#endif /* PBQPGRAPH_H_ */
