/*
 * SelfCycleRemover.h
 *
 *  Created on: 6 Aug 2018
 *      Author: Max
 */

#ifndef REDUCTION_SELFCYCLEREMOVER_HPP_
#define REDUCTION_SELFCYCLEREMOVER_HPP_

class PBQP_Reduction;
class PBQP_Graph;
class PBQP_Solution;

class SelfCycleRemover : public PBQP_Reduction {
public:
	SelfCycleRemover(PBQP_Graph*);

	PBQP_Graph* reduce();

	PBQP_Solution* solve(PBQP_Solution*);
};

#endif /* REDUCTION_SELFCYCLEREMOVER_HPP_ */
