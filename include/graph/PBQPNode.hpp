#ifndef PBQPNODE_H_
#define PBQPNODE_H_

#include <vector>
#include <algorithm>

namespace pbqppapa {

template<typename T>
class PBQPEdge;
template<typename T>
class Vektor;

template<typename T>
class PBQPNode {
private:
	Vektor<T> values;
	std::vector<PBQPEdge<T>*> incidentEdges = std::vector<PBQPEdge<T>*>();
	std::vector<PBQPEdge<T>*> outgoingEdges = std::vector<PBQPEdge<T>*>();
	unsigned const int index;

public:
	/**
	 * Should only be used by PBQPGraph internally. Index counter is held by PBQPGraph instance
	 */
	PBQPNode(unsigned const int index, Vektor<T>& values) :
			values(values), index(index) {
	}

	//TODO make proper iterators here
	const std::vector<PBQPEdge<T>*>& getAdjacentEdges(
			const bool respectDirection = false) const {
		if (respectDirection) {
			return outgoingEdges;
		} else {
			return incidentEdges;
		}
	}

	std::vector<PBQPNode<T>*> getAdjacentNodes(const bool respectDirection =
			false) {
		std::set<PBQPNode<T>*> resultSet = std::set<PBQPNode<T>*>();
		std::vector<PBQPNode<T>*> nodes = std::vector<PBQPNode<T>*>();
		std::vector<PBQPEdge<T>*>& edgesToLookAt =
				(respectDirection ? outgoingEdges : incidentEdges);
		for (PBQPEdge<T>* edge : edgesToLookAt) {
			PBQPNode<T>* other = edge->getOtherEnd(this);
			if (resultSet.insert(other).second) {
				nodes.push_back(other);
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
		return values.getRowCount();
	}

	/**
	 * Gets the amount of edges connected to this node
	 */
	unsigned int getDegree() const {
		return incidentEdges.size();
	}

	/**
	 * Gets the cost vektor associated with this node
	 */
	Vektor<T>& getVektor() {
		return values;
	}

	/**
	 * Compares based on index, needed so we can sort nodes into maps
	 */
	bool operator<(const PBQPNode<T>& e) const {
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
		incidentEdges.push_back(edge);
		if (edge->getSource() == this) {
			outgoingEdges.push_back(edge);
		}
	}

	/**
	 * Should only be used by PBQPGraph internally.
	 */
	void removeEdge(PBQPEdge<T>* edge) {
		//Erase�remove idiom
		incidentEdges.erase(
				std::remove(incidentEdges.begin(), incidentEdges.end(), edge),
				incidentEdges.end());
	}
};

}

#endif /* PBQPNODE_H_ */
