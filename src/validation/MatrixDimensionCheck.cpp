/*
 * MatrixDimensionCheck.cpp
 *
 *  Created on: 6 Aug 2018
 *      Author: Max
 */

#include "MatrixDimensionCheck.hpp"
#include "PBQP_Graph.hpp"
#include "PBQP_Edge.hpp"
#include "PBQP_Node.hpp"
#include "PBQPValidator.hpp"
#include "Matrix.hpp"

MatrixDimensionCheck::MatrixDimensionCheck(PBQP_Graph* graph) :
		PBQPValidator(graph) {
	// TODO Auto-generated constructor stub

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

