/*
 * PBQPGraph.h
 *
 *  Created on: 3 Aug 2018
 *      Author: Max
 */

#ifndef PBQPGRAPH_H_
#define PBQPGRAPH_H_

#include <vector>

class PBQP_Edge;
class PBQP_Node;
class Matrix;
class Vektor;

/**
 * A graph representing a PBQP. The template represents the data type of the numbers in the cost vektors
 * and cost matrices. It must be consistent throughout the entire graph and is consistent throughout all
 * edges and nodes in the same graph
 */
template <class T>
class PBQP_Graph {
private:
	int nodeIndexCounter;
	std::vector<PBQP_Node<T>*>* nodes;
	std::vector<PBQP_Edge<T>*>* edges;
	//TODO Both of these should be maps instead of lists
	void internalEdgeCleanUp(PBQP_Edge<T>*);

public:

	/**
	 * Create a new empty graph with no nodes
	 */
	PBQP_Graph();

	/**
	 * Deletes all nodes and edges contained within the graph
	 */
	~PBQP_Graph();

	/**
	 * Creates a new node with the given cost vektor and adds it to the graph.
	 * The new node will not have any edges initially
	 */
	PBQP_Node* addNode(Vektor<T>*);

	/**
	 * Creates a new edge and inserts it into the graph. The first node given is the
	 * edges source, the second one its target and the given matrix is the cost matrix that
	 * will be associated with the created edge
	 */
	PBQP_Edge* addEdge(PBQP_Node<T>*, PBQP_Node<T>*, Matrix<T>*);

	/**
	 * Removes the given node from the graph and deletes it.
	 * All edges connecting to this node will also be removed and deleted
	 */
	void removeNode(PBQP_Node<T>*);

	/**
	 * Removes the given edge from the graph and deletes it.
	 * Does not influence any nodes adjacent to the edge.
	 */
	void removeEdge(PBQP_Edge<T>*);

	/**
	 * Gets all nodes currently part of the graph
	 * Points to the actual internal representation, so DO NOT MODIFY DIRECTLY.
	 */
	std::vector<PBQP_Node<T>*>* getNodes();

	/**
	 * Gets all edges currently part of the graph
	 * Points to the actual internal representation, so DO NOT MODIFY DIRECTLY.
	 */
	std::vector<PBQP_Edge<T>*>* getEdges();

	/**
	 * Gets the amount of nodes currently in the graph
	 */
	int getNodeCount();

	/**
	 * Gets the amount of edges currently in the graph
	 */
	int getEdgeCount();

};

#endif /* PBQPGRAPH_H_ */
