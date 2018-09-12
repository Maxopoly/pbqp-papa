#ifndef PBQPGRAPH_H_
#define PBQPGRAPH_H_

#include <vector>
#include <set>
#include <iterator>

namespace pbqppapa {

template<typename T>
class PBQPEdge;
template<typename T>
class PBQPNode;
template<typename T>
class Matrix;
template<typename T>
class Vektor;

/**
 * A graph representing a PBQP. The template represents the data type of the numbers in the cost vektors
 * and cost matrices. It is consistent throughout the entire graph; all edges and all nodes.
 */
template<typename T>
class PBQPGraph {
private:
	static unsigned int nodeIndexCounter;
	unsigned int localIndexStart;
	std::set<PBQPNode<T>> nodes = std::set<PBQPNode<T>>();
	std::set<PBQPEdge<T>> edges = std::set<PBQPEdge<T>>();

public:

	/**
	 * Create a new empty graph with no nodes
	 */
	PBQPGraph() {
		localIndexStart = PBQPGraph::nodeIndexCounter;
	}

	/**
	 * Creates a new node with the given cost vektor and adds it to the graph.
	 * The new node will not have any edges initially
	 */
	PBQPNode<T>* addNode(Vektor<T>& vektor) {
		PBQPNode<T> node = PBQPNode<T>(PBQPGraph::nodeIndexCounter++,
				vektor);
		nodes.insert(node);
		return &node;
	}

	/**
	 * Directly inserts a preexisting node. No checks are done on the internal state of the node or
	 * its possibly referenced edges. The user must ensure that this is handled properly
	 */
	void addNode(PBQPNode<T>& node) {
		nodes.insert(node);
	}

	/**
	 * Directly inserts a preexisting edge. No checks are done on the internal state of the edge or
	 * whether the nodes incident to it are even in the graph. The user must ensure that this is handled properly
	 */
	void addEdge(PBQPEdge<T>& edge) {
		edges.insert(edge);
	}

	/**
	 * Creates a new edge and inserts it into the graph. The first node given is the
	 * edges source, the second one its target and the given matrix is the cost matrix that
	 * will be associated with the created edge
	 */
	PBQPEdge<T>* addEdge(PBQPNode<T>* source, PBQPNode<T>* target,
			Matrix<T>& matrix) {
		PBQPEdge<T> edge = PBQPEdge<T>(source, target, matrix);
		edges.insert(edge);
		source->addEdge(&edge);
		if (source != target) {
			target->addEdge(&edge);
		}
		return &edge;
	}

	/**
	 * Removes the given node and all edges connected to it from the graph
	 *
	 */
	void removeNode(PBQPNode<T>* node) {
		for (PBQPEdge<T>* edge : node->getAdjacentEdges(false)) {
			edge->getOtherEnd(node)->removeEdge(edge);
			edges.erase(*edge);
		}
		nodes.erase(*node);
	}

	/**
	 * Removes the given edge from the graph and deletes it.
	 * The edge is deleted from adjacent nodes as well, but the adjacent nodes stay in the graph
	 */
	void removeEdge(PBQPEdge<T>* edge)  {
		edge->getSource()->removeEdge(edge);
		edge->getTarget()->removeEdge(edge);
		edges.erase(*edge);
	}

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
	 * The index counter at which this graph was created, meaning all nodes in the graph will have
	 * an index equal or bigger
	 */
	unsigned int getLocalIndexStart() const {
		return localIndexStart;
	}

	/**
	 * Gets the internal counter for node indices. All nodes that ever existed in any graph so far
	 * will have an index smaller than this counter. To ensure that every node has a unique number, even
	 * if we split up graphs and parallelize work on the smaller pieces, this counter is global
	 */
	static unsigned int getNodeIndexCounter() {
		return nodeIndexCounter;
	}

};

//dark magic to initialize static members of a template
template<typename T>
unsigned int PBQPGraph<T>::nodeIndexCounter = 0;

}

#endif /* PBQPGRAPH_H_ */
