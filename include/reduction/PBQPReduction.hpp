/*
 * PBQPReduction.h
 *
 *  Created on: 5 Aug 2018
 *      Author: Max
 */

#ifndef REDUCTION_PBQPREDUCTION_HPP_
#define REDUCTION_PBQPREDUCTION_HPP_

#include <vector>

class PBQP_Graph;

class PBQP_Reduction {
protected:
	PBQP_Graph* graph;

public:
	PBQP_Reduction(PBQP_Graph*);
	virtual ~PBQP_Reduction();

	virtual std::vector<PBQP_Graph*>* reduce();
};

#endif /* REDUCTION_PBQPREDUCTION_HPP_ */
