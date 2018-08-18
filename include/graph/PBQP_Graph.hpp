#ifndef PBQPGRAPH_H_
#define PBQPGRAPH_H_

#include <vector>
#include <set>

template<typename T>
class PBQP_Edge;
template<typename T>
class PBQP_Node;
template<typename T>
class Matrix;
template<typename T>
class Vektor;

/**
 * A graph representing a PBQP. The template represents the data type of the numbers in the cost vektors
 * and cost matrices. It is consistent throughout the entire graph; all edges and all nodes.
 */
template<typename T>
class PBQP_Graph {
private:
	unsigned int nodeIndexCounter;
	std::set<PBQP_Node<T>*>* const nodes = new std::set<PBQP_Node<T>*>();
	std::set<PBQP_Edge<T>*>* const edges = new std::set<PBQP_Edge<T>*>();

public:

	/**
	 * Create a new empty graph with no nodes
	 */
	PBQP_Graph() {
		nodeIndexCounter = 0;
	}

	/**
	 * Deletes all nodes and edges contained within the graph
	 */
	~PBQP_Graph() {
		for (PBQP_Edge<T>* edge : *edges) {
			delete edge;
		}
		delete edges;
		for (PBQP_Node<T>* node : *nodes) {
			delete node;
		}
		delete nodes;
	}

	/**
	 * Creates a new node with the given cost vektor and adds it to the graph.
	 * The new node will not have any edges initially
	 */
	PBQP_Node<T>* addNode(Vektor<T>* vektor) {
		PBQP_Node<T>* node = new PBQP_Node<T>(nodeIndexCounter++, vektor);
		nodes->insert(node);
		return node;
	}

	/**
	 * Creates a new edge and inserts it into the graph. The first node given is the
	 * edges source, the second one its target and the given matrix is the cost matrix that
	 * will be associated with the created edge
	 */
	PBQP_Edge<T>* addEdge(PBQP_Node<T>* source, PBQP_Node<T>* target,
			Matrix<T>* matrix) {
		PBQP_Edge<T>* edge = new PBQP_Edge<T>(source, target, matrix);
		edges->insert(edge);
		source->addEdge(edge);
		if (source != target) {
			target->addEdge(edge);
		}
		return edge;
	}

	/**
	 * Removes the given node from the graph and deletes it.
	 * All edges connecting to this node will also be removed and deleted
	 */
	void removeNode(PBQP_Node<T>* node) {
		for (PBQP_Edge<T>* edge : *(node->getAdjacentEdges(false))) {
			edges->erase(edge);
			edge->getOtherEnd(node)->removeEdge(edge);
		}
		nodes->erase(node);
	}

	/**
	 * Removes the given edge from the graph and deletes it.
	 * Does not influence any nodes adjacent to the edge.
	 */
	void removeEdge(PBQP_Edge<T>* edge) {
		edges->erase(edge);
		edge->getSource()->removeEdge(edge);
		edge->getTarget()->removeEdge(edge);
	}

	/**
	 * Gets all nodes currently part of the graph
	 */
	const std::set<PBQP_Node<T>*>* getNodes() const {
		return nodes;
	}

	/**
	 * Gets all edges currently part of the graph.
	 */
	const std::set<PBQP_Edge<T>*>* getEdges() const {
		return edges;
	}

	/**
	 * Gets the amount of nodes currently in the graph
	 */
	unsigned int getNodeCount() const {
		return nodes->size();
	}

	/**
	 * Gets the amount of edges currently in the graph
	 */
	unsigned int getEdgeCount() const {
		return edges->size();
	}

	/**
	 * Gets the internal counter for node indices. All nodes that ever existed in the graph so far
	 * will have an index smaller than this counter
	 */
	unsigned int getNodeIndexCounter() const {
		return nodeIndexCounter;
	}

};

#endif /* PBQPGRAPH_H_ */
