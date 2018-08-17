/*
 * PBQPValidator.h
 *
 *  Created on: 6 Aug 2018
 *      Author: Max
 */

#ifndef VALIDATION_PBQPVALIDATOR_HPP_
#define VALIDATION_PBQPVALIDATOR_HPP_

template<typename T>
class PBQP_Graph;

/**
 * Checks a certain property of a PBQP to ensure that the PBQP is valid. Being valid doesn't indicate anything about
 * the (existance of a) solution, but only whether the problem is formulated properly.
 *
 * Running these validations separately removes the need for a lot of safety checks further into PBQP processing
 * and even allows entirely skipping validation for PBQP that are known to be valid.
 *
 * Instances may not delete the graph given to them
 */
template<typename T>
class PBQPValidator {
protected:
	const PBQP_Graph<T>* graph;

public:
	PBQPValidator(PBQP_Graph<T>*) {
		this->graph = graph;
	}
	virtual ~PBQPValidator();

	/**
	 * Check the graph given in the constructor and return whether it's okay.
	 * The given graph may not be modified
	 */
	virtual bool check() = 0;
};

#endif /* VALIDATION_PBQPVALIDATOR_HPP_ */
