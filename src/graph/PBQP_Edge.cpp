/*
 * PBQPEdge.cpp
 *
 *  Created on: 3 Aug 2018
 *      Author: Max
 */

#include "graph/PBQP_Edge.hpp"

PBQP_Edge::PBQP_Edge(PBQP_Node* source, PBQP_Node* target, Matrix* matrix) {
	this->source = source;
	this->target = target;
	this->matrix = matrix;
}

PBQP_Edge::~PBQP_Edge() {
	// TODO Auto-generated destructor stub
}


bool PBQP_Edge::isSource(PBQP_Node* node) {
	return node == source;
}

PBQP_Node* PBQP_Edge::getOtherEnd(PBQP_Node* node) {
	if (node == source) {
		return target;
	}
	return source;
}

PBQP_Node* PBQP_Edge::getSource() {
	return source;
}

PBQP_Node* PBQP_Edge::getTarget() {
	return target;
}

Matrix* PBQP_Edge::getMatrix() {
	return matrix;
}
