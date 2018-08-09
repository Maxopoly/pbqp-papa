/*
 * PBQPGraph.cpp
 *
 *  Created on: 3 Aug 2018
 *      Author: Max
 */

#include "graph/PBQP_Graph.hpp"
#include "graph/PBQP_Edge.hpp"
#include "graph/PBQP_Node.hpp"
#include "graph/Vektor.hpp"

#include <algorithm>

PBQP_Graph::PBQP_Graph() {
	nodes = new std::vector<PBQP_Node*>();
	edges = new std::vector<PBQP_Edge*>();
	nodeIndexCounter = 0;
}

PBQP_Graph::~PBQP_Graph() {
	// TODO Auto-generated destructor stub
}

PBQP_Node* PBQP_Graph::addNode(Vektor* matrix) {
	PBQP_Node* node = new PBQP_Node(nodeIndexCounter++, matrix);
	nodes->push_back(node);
	return node;
}

void PBQP_Graph::addEdge(PBQP_Node* source, PBQP_Node* target, Matrix* matrix){
	PBQP_Edge* edge = new PBQP_Edge(source, target, matrix);
	edges->push_back(edge);
	source->addEdge(edge);
	target->addEdge(edge);
}

void PBQP_Graph::removeNode(PBQP_Node* node) {
	for(PBQP_Edge* edge : *(node->getAdjacentEdges(false))) {
		internalEdgeCleanUp(edge);
		edge->getOtherEnd(node)->removeEdge(edge);
	}
	nodes->erase(
				std::remove(nodes->begin(), nodes->end(), node),
				nodes->end());
}


void PBQP_Graph::internalEdgeCleanUp(PBQP_Edge* edge) {
	edges->erase(
				std::remove(edges->begin(), edges->end(), edge),
				edges->end());
}

void PBQP_Graph::removeEdge(PBQP_Edge* edge) {
	internalEdgeCleanUp(edge);
	edge->getSource()->removeEdge(edge);
	edge->getTarget()->removeEdge(edge);
}

std::vector<PBQP_Node*>* PBQP_Graph::getNodes() {
	return nodes;
}

std::vector<PBQP_Edge*>* PBQP_Graph::getEdges() {
	return edges;
}

int PBQP_Graph::getNodeCount() {
	return nodes->size();
}

int PBQP_Graph::getEdgeCount() {
	return edges->size();
}
