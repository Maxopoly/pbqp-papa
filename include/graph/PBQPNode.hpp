#ifndef PBQPNODE_H_
#define PBQPNODE_H_

#include <vector>
#include <algorithm>
//#include "graph/Matrix.hpp"

template<typename T>
class PBQPEdge;
template<typename T>
class Vektor;

template<typename T>
class PBQPNode {
private:
	Vektor<T>* const values;
	std::vector<PBQPEdge<T>*>* const incidentEdges = new std::vector<PBQPEdge<T>*>();
	unsigned const int index;

public:
	/**
	 * Should only be used by PBQPGraph internally. Index counter is held by PBQPGraph instance
	 */
	PBQPNode(unsigned const int index, Vektor<T>* const values) : values(values), index(index) {
	}

	/**
	 * Deletes only this node and its cost vektor. Cleanup of edges must be done by a higher level (usually PBQPGraph)
	 */
	~PBQPNode() {
		delete values;
		delete incidentEdges;
	}

	//TODO make proper iterators here
	std::vector<PBQPEdge<T>*>* getAdjacentEdges(const bool respectDirection = false) const {
		if (!respectDirection) {
			return incidentEdges;
		}
		std::vector<PBQPEdge<T>*>* outgoingEdges = new std::vector<PBQPEdge<T>*>();
		//TODO Should we maybe just store this explicitly? Check back later how often we actually have to recompute this vector
		for (PBQPEdge<T>* edge : *incidentEdges) {
			if (edge->isSource(this)) {
				outgoingEdges->push_back(edge);
			}
		}
		return outgoingEdges;
	}

	std::vector<PBQPNode<T>*>* getAdjacentNodes(const bool respectDirection = false) const {
		//TODO Same as in the adjacent edge function, maybe we should just store all of this explictly to save computation time?
		//separate loops so we only check respectDirection once, instead of during every loop iteration
		std::set <PBQPNode<T>*>* resultSet = new std::set <PBQPNode<T>*>();
		std::vector <PBQPNode<T>*>* nodes = new std::vector <PBQPNode<T>*>();
		if (respectDirection) {
			for (PBQPEdge<T>* edge : *incidentEdges) {
				PBQPNode<T>* other = edge->getOtherEnd(this);
				if (edge->isSource(this) && resultSet->insert(other).second) {
					nodes->push_back(other);
				}
			}
		} else {
			for (PBQPEdge<T>* edge : *incidentEdges) {
				PBQPNode<T>* other = edge->getOtherEnd(this);
				if(resultSet->insert(other).second) {
					nodes->push_back(other);
				}
			}
		}
		return nodes;
	}

	/**
	 * Index identifies this node uniquely in the graph it was created in, even after the node is deleted
	 */
	unsigned int getIndex() const {
		return index;
	}

	/**
	 * Gets the length (amount of rows) of the cost vektor
	 */
	unsigned short int getVektorDegree() const {
		return values->getRowCount();
	}

	/**
	 * Gets the amount of edges connected to this node
	 */
	unsigned int getDegree() const {
		return incidentEdges->size();
	}

	/**
	 * Gets the cost vektor associated with this node
	 */
	Vektor<T>* getVektor() const {
		return values;
	}

	/**
	 * Compares based on index, needed so we can sort nodes into maps
	 */
	bool operator< (const PBQPNode<T>& e) const {
		return this->index < e.index;
	}

	/**
	 * Compares based on index,
	 */
	bool operator==(const PBQPNode<T>& e) const {
		return this->index == e.index;
	}

	/**
	 * Should only be used by PBQPGraph internally.
	 */
	void addEdge(PBQPEdge<T>* edge) {
		incidentEdges->push_back(edge);
	}

	/**
	 * Should only be used by PBQPGraph internally.
	 */
	void removeEdge(PBQPEdge<T>* edge) {
		//Erase�remove idiom
		incidentEdges->erase(
				std::remove(incidentEdges->begin(), incidentEdges->end(), edge),
				incidentEdges->end());
	}
};

#endif /* PBQPNODE_H_ */
