/*
 * MatrixDimensionCheck.h
 *
 *  Created on: 6 Aug 2018
 *      Author: Max
 */

#ifndef VALIDATION_MATRIXDIMENSIONCHECK_HPP_
#define VALIDATION_MATRIXDIMENSIONCHECK_HPP_

template<typename T>
class PBQPValidator;
template<typename T>
class PBQP_Graph;
template<typename T>
class PBQP_Edge;

#include "graph/PBQP_Edge.hpp"

template<typename T>
class MatrixDimensionCheck: public PBQPValidator<T> {
public:
	MatrixDimensionCheck(PBQP_Graph<T>* graph) :
			PBQPValidator<T>(graph) {

	}

	~MatrixDimensionCheck();

	bool check() {
		for (PBQP_Edge<T>* edge : *(graph->getEdges())) {
			//TODO Design proper exceptions that are thrown here?
			if (edge->getSource()->getVektorDegree()
					!= edge->getMatrix()->getColumnCount()) {
				return false;
			}
			if (edge->getTarget()->getVektorDegree()
					!= edge->getMatrix()->getRowCount()) {
				return false;
			}
		}
		return true;
	}
};

#endif /* VALIDATION_MATRIXDIMENSIONCHECK_HPP_ */
