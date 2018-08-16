/*
 * PBQPNode.cpp
 *
 *  Created on: 3 Aug 2018
 *      Author: Max
 */

#include "graph/PBQP_Node.hpp"
#include <algorithm>
#include <set>
#include "graph/PBQP_Edge.hpp"
#include "graph/Matrix.hpp"
#include "graph/Vektor.hpp"

PBQP_Node::PBQP_Node(int index, Vektor* matrix) {
	this->index = index;
	this->values = matrix;
}

PBQP_Node::~PBQP_Node() {
	delete [] values;
}

void PBQP_Node::addEdge(PBQP_Edge* edge) {
	adjacentEdges.push_back(edge);
}

void PBQP_Node::removeEdge(PBQP_Edge* edge) {
	//Erase�remove idiom
	adjacentEdges.erase(
			std::remove(adjacentEdges.begin(), adjacentEdges.end(), edge),
			adjacentEdges.end());
}

std::vector<PBQP_Edge*>* PBQP_Node::getAdjacentEdges(bool respectDirection) {
	if (!respectDirection) {
		return &adjacentEdges;
	}
	std::vector<PBQP_Edge*>* outgoingEdges = new std::vector<PBQP_Edge*>();
	//TODO Should we maybe just store this explicitly? Check back later how often we actually have to recompute this vector
	for (PBQP_Edge* edge : adjacentEdges) {
		if (edge->isSource(this)) {
			outgoingEdges->push_back(edge);
		}
	}
	return outgoingEdges;
}

std::vector<PBQP_Node*>* PBQP_Node::getAdjacentNodes(bool respectDirection) {
	//TODO Same as in the adjacent edge function, maybe we should just store all of this explictly to save computation time?
	//separate loops so we only check respectDirection once, instead of during every loop iteration
	std::set <PBQP_Node*>* resultSet = new std::set <PBQP_Node*>();
	std::vector <PBQP_Node*>* nodes = new std::vector <PBQP_Node*>();
	if (respectDirection) {
		for (PBQP_Edge* edge : adjacentEdges) {
			PBQP_Node* other = edge->getOtherEnd(this);
			if (edge->isSource(this) && resultSet->insert(other).second) {
				nodes->push_back(other);
			}
		}
	} else {
		for (PBQP_Edge* edge : adjacentEdges) {
			PBQP_Node* other = edge->getOtherEnd(this);
			if(resultSet->insert(other).second) {
				nodes->push_back(other);
			}
		}
	}
	return nodes;
}

int PBQP_Node::getIndex() {
	return index;
}

int PBQP_Node::getVektorDegree() {
	return values->getRowCount();
}

int PBQP_Node::getDegree() {
	return adjacentEdges.size();
}

Vektor* PBQP_Node::getVektor() {
	return values;
}

bool PBQP_Node::operator< (const PBQP_Node& e) const {
	return this->index < e.index;
}
