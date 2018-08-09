/*
 * PBQPReduction.cpp
 *
 *  Created on: 5 Aug 2018
 *      Author: Max
 */

#include "PBQPReduction.hpp"

PBQP_Reduction::PBQP_Reduction(PBQP_Graph* graph) {
	this->graph = graph;
	result = new std::vector<PBQP_Graph*>(1);
}
