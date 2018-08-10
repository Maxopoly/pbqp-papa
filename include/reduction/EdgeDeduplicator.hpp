/*
 * EdgeDeduplicator.h
 *
 *  Created on: 5 Aug 2018
 *      Author: Max
 */

#ifndef REDUCTION_EDGEDEDUPLICATOR_HPP_
#define REDUCTION_EDGEDEDUPLICATOR_HPP_

#include "reduction/PBQPReduction.hpp"

class PBQP_Reduction;
class PBQP_Graph;
class PBQP_Solution;

class EdgeDeduplicator : public PBQP_Reduction {
public:
	EdgeDeduplicator(PBQP_Graph*);
	~EdgeDeduplicator();

	std::vector<PBQP_Graph*>* reduce();

	PBQP_Solution* solve(PBQP_Solution*);
};

#endif /* REDUCTION_EDGEDEDUPLICATOR_HPP_ */
