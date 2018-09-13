#ifndef VALIDATION_MATRIXDIMENSIONCHECK_HPP_
#define VALIDATION_MATRIXDIMENSIONCHECK_HPP_

namespace pbqppapa {

template<typename T>
class PBQPValidator;
template<typename T>
class PBQPGraph;
template<typename T>
class PBQPEdge;

#include "graph/PBQPEdge.hpp"

template<typename T>
class MatrixDimensionCheck: public PBQPValidator<T> {
public:
	MatrixDimensionCheck(PBQPGraph<T>* graph) :
			PBQPValidator<T>(graph) {

	}

	~MatrixDimensionCheck();

	bool check() {
		for (PBQPEdge<T>* edge : *(graph->getEdges())) {
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
};

}

#endif /* VALIDATION_MATRIXDIMENSIONCHECK_HPP_ */
