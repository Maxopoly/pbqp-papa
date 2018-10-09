#ifndef DEBUG_STATKEEPER_HPP_
#define DEBUG_STATKEEPER_HPP_

#include <string>

#include "graph/PBQPGraph.hpp"

namespace pbqppapa {

class StatKeeper {

private:
	unsigned int r0Applied = 0;
	unsigned int r1Applied = 0;
	unsigned int r2Applied = 0;
	unsigned int rNEarlyApplied = 0;
	unsigned int nodeCount;
	unsigned int edgeCount;
	float averageVectorDegree;

public:

	template<typename T>
	void submitGraphBefore(PBQPGraph<T>* graph) {
		nodeCount = graph->getNodeCount();
		edgeCount = graph->getEdgeCount();
		float vectorDegreeSum = 0;
		for(auto iter = graph->getNodeBegin(); iter != graph->getNodeEnd(); iter++) {
			vectorDegreeSum += (float) ((*iter)->getVectorDegree());
		}
		averageVectorDegree = vectorDegreeSum / (float) nodeCount;
	}

	void applyR0();

	void applyR1();

	void applyR2();

	void applyRNEarly();

	std::string getSumUp();

};

}



#endif /* DEBUG_STATKEEPER_HPP_ */
