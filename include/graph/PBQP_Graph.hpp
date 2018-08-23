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
	static unsigned int nodeIndexCounter;
	unsigned int localIndexStart;
	std::set<PBQP_Node<T>*>* const nodes = new std::set<PBQP_Node<T>*>();
	std::set<PBQP_Edge<T>*>* const edges = new std::set<PBQP_Edge<T>*>();

public:

	/**
	 * Create a new empty graph with no nodes
	 */
	PBQP_Graph() {
		localIndexStart = PBQP_Graph::nodeIndexCounter;
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
		PBQP_Node<T>* node = new PBQP_Node<T>(PBQP_Graph::nodeIndexCounter++, vektor);
		nodes->insert(node);
		return node;
	}

	/**
	 * Directly inserts a preexisting node. No checks are done on the internal state of the node or
	 * its possibly referenced edges. The user must ensure that this is handled properly
	 */
	void addNode(PBQP_Node<T>* node) {
		nodes->insert(node);
	}

	/**
	 * Directly inserts a preexisting edge. No checks are done on the internal state of the edge or
	 * whether the nodes incident to it are even in the graph. The user must ensure that this is handled properly
	 */
	void addEdge(PBQP_Edge<T>* edge) {
		edges->insert(edge);
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
	void removeNode(PBQP_Node<T>* node, bool cleanUp = true) {
		for (PBQP_Edge<T>* edge : *(node->getAdjacentEdges(false))) {
			edges->erase(edge);
			if (cleanUp) {
				edge->getOtherEnd(node)->removeEdge(edge);
				delete edge;
			}
		}
		nodes->erase(node);
		if(cleanUp) {
			delete node;
		}
	}

	/**
	 * Removes the given edge from the graph and deletes it.
	 * Does not influence any nodes adjacent to the edge.
	 */
	void removeEdge(PBQP_Edge<T>* edge) {
		edges->erase(edge);
		edge->getSource()->removeEdge(edge);
		edge->getTarget()->removeEdge(edge);
		delete edge;
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
unsigned int PBQP_Graph<T>::nodeIndexCounter = 0;

#endif /* PBQPGRAPH_H_ */
