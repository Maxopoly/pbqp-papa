/*
 * PBQPValidator.h
 *
 *  Created on: 6 Aug 2018
 *      Author: Max
 */

#ifndef VALIDATION_PBQPVALIDATOR_HPP_
#define VALIDATION_PBQPVALIDATOR_HPP_

class PBQP_Graph;

class PBQPValidator {
protected:
	PBQP_Graph* graph;

public:
	PBQPValidator(PBQP_Graph*);
	virtual ~PBQPValidator();

	virtual bool check() = 0;
};

#endif /* VALIDATION_PBQPVALIDATOR_HPP_ */
