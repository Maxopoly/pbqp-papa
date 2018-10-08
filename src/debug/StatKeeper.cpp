#include "debug/StatKeeper.hpp"

namespace pbqppapa {

std::string StatKeeper::getSumUp() {
	std::string result;
	result += "Nodes: ";
	result += std::to_string(nodeCount);
	result += ", Edges: ";
	result += ", E/N ratio: ";
	result += std::to_string((float) edgeCount / (float) nodeCount);
	result += '\n';
	result += "Avg. vector degree: ";
	result += std::to_string(averageVectorDegree);
	result += '\n';
	result += "R0 applied: ";
	result += std::to_string(r0Applied);
	result += '\n';
	result += "R1 applied: ";
	result += std::to_string(r1Applied);
	result += '\n';
	result += "R2 applied: ";
	result += std::to_string(r2Applied);
	result += '\n';
	result += "R2 applied: ";
	result += std::to_string(rNEarlyApplied);
	result += '\n';
	return result;
}


void StatKeeper::applyR0() {
	++r0Applied;
}

void StatKeeper::applyR1() {
	++r1Applied;
}

void StatKeeper::applyR2() {
	++r2Applied;
}

void StatKeeper::applyRNEarly() {
	++rNEarlyApplied;
}

}
