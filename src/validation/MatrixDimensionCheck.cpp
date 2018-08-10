/*
 * MatrixDimensionCheck.cpp
 *
 *  Created on: 6 Aug 2018
 *      Author: Max
 */

#include "validation/MatrixDimensionCheck.hpp"
#include "graph/PBQP_Graph.hpp"
#include "graph/PBQP_Edge.hpp"
#include "graph/PBQP_Node.hpp"
#include "graph/Matrix.hpp"

MatrixDimensionCheck::MatrixDimensionCheck(PBQP_Graph* graph) :
		PBQPValidator(graph) {
}

MatrixDimensionCheck::~MatrixDimensionCheck() {

}

bool MatrixDimensionCheck::check() {
	for (PBQP_Edge* edge : *graph->getEdges()) {
		//TODO Design proper exceptions that are thrown here?
		if (edge->getSource()->getVectorDegree()
				!= edge->getMatrix()->getColumnCount()) {
			return false;
		}
		if (edge->getTarget()->getVectorDegree()
				!= edge->getMatrix()->getRowCount()) {
			return false;
		}
	}
	return true;
}

