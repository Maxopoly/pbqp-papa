/*
 * PBQPSolution.cpp
 *
 *  Created on: 5 Aug 2018
 *      Author: Max
 */

#include "graph/PBQP_Solution.hpp"

PBQP_Solution::PBQP_Solution(int length) {
	selection = new std::vector<int>(length);

}

PBQP_Solution::~PBQP_Solution() {
	// TODO Auto-generated destructor stub
}

double PBQP_Solution::calculateKnownCost() {

}

bool PBQP_Solution::isFinished() {
	//TODO
}

void PBQP_Solution::setSolution(int nodeIndex, int solution) {
	(*selection) [nodeIndex] = solution;
}

int PBQP_Solution::getSolution(int nodeIndex) {
	return (*selection) [nodeIndex];
}

