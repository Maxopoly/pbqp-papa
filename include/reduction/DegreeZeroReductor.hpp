/*
 * DegreeZero.hpp
 *
 *  Created on: 6 Aug 2018
 *      Author: Max
 */

#ifndef VALIDATION_DEGREEZEROREDUCTOR_HPP_
#define VALIDATION_DEGREEZEROREDUCTOR_HPP_

#include <vector>
#include <reduction/PBQPReduction.hpp>

class PBQP_Graph;
class Dependent_Solution;
class PBQP_Solution;

class DegreeZeroReductor : public PBQP_Reduction {
private:
	Dependent_Solution* solution;

public:
	DegreeZeroReductor(PBQP_Graph*);
	~DegreeZeroReductor();

	std::vector<PBQP_Graph*>* reduce();
	PBQP_Solution* solve(PBQP_Solution*);
};

#endif /* VALIDATION_DEGREEZEROREDUCTOR_HPP_ */
