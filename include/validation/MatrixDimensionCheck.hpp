/*
 * MatrixDimensionCheck.h
 *
 *  Created on: 6 Aug 2018
 *      Author: Max
 */

#ifndef VALIDATION_MATRIXDIMENSIONCHECK_HPP_
#define VALIDATION_MATRIXDIMENSIONCHECK_HPP_

class PBQPValidator;
class PBQP_Graph;

class MatrixDimensionCheck : public PBQPValidator {
public:
	MatrixDimensionCheck(PBQP_Graph* graph);

	bool check();
};

#endif /* VALIDATION_MATRIXDIMENSIONCHECK_HPP_ */
