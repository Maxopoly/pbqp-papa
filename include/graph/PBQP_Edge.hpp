#ifndef PBQP_EDGE_H_
#define PBQP_EDGE_H_

template<typename T>
class PBQP_Node;
template<typename T>
class Matrix;

/**
 * Represents a directed edge in a PBQP, leading from one node to another with an associated cost matrix.
 * The template corresponds to the data type used in the cost matrix */
template<typename T>
class PBQP_Edge {

private:
	PBQP_Node<T>* const source;
	PBQP_Node<T>* const target;
	Matrix<T>* const matrix;

public:

	/**
	 * Creates a new directed edge from the given source node to the given target node.
	 * The matrix associated with this edge will be the one given.
	 *
	 * For a valid PBQP, the amount of rows in the matrix must match the length
	 * of the vektor associated with the source node and the amount of columns
	 * in the matrix must match the length of the vektor associated with the target node
	 */
	PBQP_Edge(PBQP_Node<T>* source, PBQP_Node<T>* target, Matrix<T>* matrix) : source(source), target(target), matrix(matrix) {
	}

	/**
	 * Deletes only the content matrix, but not any related nodes
	 */
	~PBQP_Edge() {
		delete matrix;
	}

	/**
	 * Checks whether the given node is the source of this edge and returns true in that case
	 */
	bool isSource(const PBQP_Node<T>* node) const {
		return node == source;
	}

	/**
	 * Given one end of this edge, this function will return the other end (disregarding the
	 * 	direction of the edge). For a cycle, the same node will be returned.
	 *
	 * If the given node is not part of this edge, the edges source will be returned
	 */
	PBQP_Node<T>* getOtherEnd(const PBQP_Node<T>* node) const{
		if (node == source) {
			return target;
		}
		return source;
	}

	/**
	 * Gets the cost matrix associated with this edge
	 */
	Matrix<T>* getMatrix() const{
		return matrix;
	}

	/**
	 * Gets the source node of this edge. May be identical to the target in case of a cycle
	 */
	PBQP_Node<T>* getSource() const{
		return source;
	}

	/**
	 * Gets the target node of this edge. May be identical to the source in case of a cycle
	 */
	PBQP_Node<T>* getTarget() const{
		return target;
	}

	/**
	 * Gets whether this edge is a cycle, meaning source and target are identical
	 */
	bool isCycle() const{
		return source == target;
	}

	bool operator==(const PBQP_Node<T>& e) const {
		return this == &e;
	}
};

#endif /* PBQP_EDGE_H_ */
