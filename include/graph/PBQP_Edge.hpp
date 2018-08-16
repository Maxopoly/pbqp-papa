/*
 * PBQPEdge.h
 *
 *  Created on: 3 Aug 2018
 *      Author: Max
 */

#ifndef PBQP_EDGE_H_
#define PBQP_EDGE_H_

class PBQP_Node;
class Matrix;

/**
 * Represents a directed edge in a PBQP, leading from one node to another with an associated cost matrix.
 * The template corresponds to the data type used in the cost matrix
 */
template <class T>
class PBQP_Edge {

private:
	PBQP_Node* source;
	PBQP_Node* target;
	Matrix<T>* matrix;

public:

	/**
	 * Creates a new directed edge from the given source node to the given target node.
	 * The matrix associated with this edge will be the one given.
	 *
	 * For a valid PBQP, the amount of rows in the matrix must match the length
	 * of the vektor associated with the source node and the amount of columns
	 * in the matrix must match the length of the vektor associated with the target node
	 */
	PBQP_Edge(PBQP_Node*, PBQP_Node*, Matrix<T>*);

	/**
	 * Deletes only the content matrix, but not any related nodes
	 */
	~PBQP_Edge();

	/**
	 * Checks whether the given node is the source of this edge and returns true in that case
	 */
	bool isSource(PBQP_Node*);

	/**
	 * Given one end of this edge, this function will return the other end (disregarding the
	 * 	direction of the edge). For a cycle, the same node will be returned.
	 *
	 * If the given node is not part of this edge, the edges source will be returned
	 */
	PBQP_Node* getOtherEnd(PBQP_Node*);

	/**
	 * Gets the cost matrix associated with this edge
	 */
	Matrix<T>* getMatrix();

	/**
	 * Gets the source node of this edge. May be identical to the target in case of a cycle
	 */
	PBQP_Node* getSource();

	/**
	 * Gets the target node of this edge. May be identical to the source in case of a cycle
	 */
	PBQP_Node* getTarget();
};

#endif /* PBQP_EDGE_H_ */
