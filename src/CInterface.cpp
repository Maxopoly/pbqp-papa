#include "graph/PBQPGraph.hpp"
#include "graph/PBQPSolution.hpp"
#include "graph/Vector.hpp"
#include "graph/PBQPNode.hpp"
#include "graph/PBQPEdge.hpp"
#include "CInterface.h"
#include "solve/StepByStepSolver.hpp"
#include "math/InfinityWrapper.hpp"
#include "io/PBQP_Serializer.hpp"
#if PBQP_USE_GUROBI
	#include "math/GurobiConverter.hpp"
#endif

#include <stdio.h>
#include <vector>
#include <time.h>
#include <iostream>
#include <algorithm>

namespace pbqppapa {

#define CINTERFACEIMPL(TYPENAME,SHORTNAME) \
struct pbqp_ ## SHORTNAME ## _parsing { \
PBQPGraph<InfinityWrapper< TYPENAME >>* graph; \
std::vector<PBQPNode<InfinityWrapper< TYPENAME >>*> nodes; \
std::vector<PBQPNode<InfinityWrapper< TYPENAME >>*> peo; \
unsigned long maximumIndex = 0; \
}; \
extern "C" void pbqp_ ## SHORTNAME ## _addNode(struct  pbqp_ ## SHORTNAME ## _parsing* pbqpparsing, \
		TYPENAME* data, unsigned short length, unsigned int index) { \
	InfinityWrapper<TYPENAME> infData[length]; \
	for (unsigned short i = 0; i < length; i++) { \
		infData[i] = InfinityWrapper<TYPENAME>(data[i]); \
	} \
	Vector<InfinityWrapper<TYPENAME>> vek(length, infData); \
	PBQPNode<InfinityWrapper<TYPENAME>>* node = new PBQPNode<InfinityWrapper<TYPENAME>>(index, vek); \
	pbqpparsing->graph->addNode(node); \
	pbqpparsing->nodes.at(index) = node; \
} \
extern "C" void pbqp_ ## SHORTNAME ## _addEdge(struct  pbqp_ ## SHORTNAME ## _parsing* pbqpparsing, \
	unsigned int sourceIndex, unsigned int targetIndex, TYPENAME* data) { \
	PBQPNode<InfinityWrapper<TYPENAME>>* source = pbqpparsing->nodes.at(sourceIndex); \
	PBQPNode<InfinityWrapper<TYPENAME>>* target = pbqpparsing->nodes.at(targetIndex); \
	const unsigned short length = source->getVectorDegree() * target->getVectorDegree(); \
	InfinityWrapper<TYPENAME> infData[length]; \
	for (unsigned short i = 0; i < length; i++) { \
		infData[i] = InfinityWrapper<TYPENAME>(data[i]); \
	} \
	Matrix<InfinityWrapper<TYPENAME>> mat(source->getVectorDegree(), \
			target->getVectorDegree(), infData); \
	pbqpparsing->graph->addEdge(source, target, mat); \
} \
struct pbqp_ ## SHORTNAME ## _solution* pbqp_ ## SHORTNAME ## _convertSolution(struct  pbqp_ ## SHORTNAME ## _parsing* \
		pbqpparsing, PBQPSolution<InfinityWrapper<TYPENAME>>* cppSol) { \
	pbqp_ ## SHORTNAME ## _solution* cSol = new pbqp_ ## SHORTNAME ## _solution(); \
	cSol->selections = new unsigned short [pbqpparsing->maximumIndex]; \
	for(unsigned int i = 0; i < pbqpparsing->maximumIndex; i++) { \
		if (!pbqpparsing->nodes.at(i)) { \
			continue; \
		} \
		cSol->selections[i] = cppSol->getSolution(i); \
	} \
	cSol->length = pbqpparsing->maximumIndex; \
	return cSol; \
} \
extern "C" struct pbqp_ ## SHORTNAME ## _solution* pbqp_ ## SHORTNAME ## _solve(struct pbqp_ ## SHORTNAME ## _parsing* \
		pbqpparsing) { \
	if (pbqpparsing->graph->getNodeCount() == 0) { \
		return new pbqp_ ## SHORTNAME ## _solution(); \
	} \
	StepByStepSolver<TYPENAME> solver(pbqpparsing->graph); \
	PBQPSolution<InfinityWrapper<TYPENAME>>* cppLevelSol = solver.solveFully(); \
	pbqp_ ## SHORTNAME ## _solution* cLevelSol = pbqp_ ## SHORTNAME ## _convertSolution(pbqpparsing, cppLevelSol); \
	delete cppLevelSol; \
	return cLevelSol; \
} \
extern "C" struct pbqp_ ## SHORTNAME ## _parsing* pbqp_ ## SHORTNAME ## _createInstance( \
		unsigned int maximumIndex) { \
	pbqp_ ## SHORTNAME ## _parsing* result = new pbqp_ ## SHORTNAME ## _parsing (); \
	result->graph = new PBQPGraph<InfinityWrapper<TYPENAME>>(); \
	result->nodes.resize(maximumIndex); \
	return result; \
} \
extern "C" void pbqp_ ## SHORTNAME ## _addToPEO(struct pbqp_ ## SHORTNAME ## _parsing* pbqpparsing, \
		unsigned int index) { \
		pbqpparsing->peo.push_back(pbqpparsing->nodes.at(index)); \
} \
extern "C" void pbqp_ ## SHORTNAME ## _dump(struct pbqp_ ## SHORTNAME ## _parsing* pbqpparsing, char* path) { \
	if (pbqpparsing->graph->getNodeCount() == 0) { \
		std::cout << "Graph requested to dump to flat file was empty, did not dump"; \
		return; \
	} \
	if (pbqpparsing->graph->getEdgeCount() == 0) { \
		std::cout << "NO EDGES"; \
		return; \
	} \
	std::string stringPath; \
	if(!path) { \
		stringPath = std::to_string(std::chrono::system_clock::now().time_since_epoch().count()) + ".json"; \
	} \
	else { \
		stringPath = std::string(path);\
	} \
	std::cout << "Dumping to " << stringPath << "\n"; \
	pbqpparsing->graph->setPEO(pbqpparsing->peo); \
	PBQP_Serializer<InfinityWrapper<TYPENAME>>::saveToFile(stringPath, pbqpparsing->graph); \
} \
extern "C" void pbqp_ ## SHORTNAME ## _free(struct pbqp_ ## SHORTNAME ## _parsing* pbqpparsing) { \
	delete pbqpparsing->graph; \
	delete pbqpparsing; \
} \
extern "C" void pbqp_ ## SHORTNAME ## _finishPEO(struct pbqp_ ## SHORTNAME ## _parsing* pbqpparsing) { \
	std::reverse(pbqpparsing->peo.begin(), pbqpparsing->peo.end()); \
} \
extern "C" TYPENAME pbqp_ ## SHORTNAME ## _getNodeVektorValue(struct pbqp_ ## SHORTNAME ## _parsing* pbqpparsing, \
		unsigned int nodeIndex, unsigned short entryIndex) { \
	PBQPNode<InfinityWrapper<TYPENAME>>* node = pbqpparsing->nodes.at(nodeIndex); \
	return node->getVector().get(entryIndex).getValue(); \
} \
extern "C" unsigned int pbqp_ ## SHORTNAME ## _getAdjacentEdgeCount(struct pbqp_ ## SHORTNAME ## _parsing* pbqpparsing, \
		unsigned int nodeIndex) { \
	PBQPNode<InfinityWrapper<TYPENAME>>* node = pbqpparsing->nodes.at(nodeIndex); \
	return node->getDegree(); \
} \
extern "C" unsigned short pbqp_ ## SHORTNAME ## _getVectorLength(struct pbqp_ ## SHORTNAME ## _parsing* pbqpparsing, \
		unsigned int nodeIndex) { \
	PBQPNode<InfinityWrapper<TYPENAME>>* node = pbqpparsing->nodes.at(nodeIndex); \
	return node->getVectorDegree(); \
} \
extern "C" TYPENAME pbqp_ ## SHORTNAME ## _getMatrixEntry(struct pbqp_ ## SHORTNAME ## _parsing* pbqpparsing, \
		unsigned int nodeIndex, unsigned int adjacencyIndex, unsigned short row, unsigned short column) { \
	PBQPNode<InfinityWrapper<TYPENAME>>* node = pbqpparsing->nodes.at(nodeIndex); \
	return node->getAdjacentEdges().at(adjacencyIndex)->getMatrix().get(row, column).getValue(); \
} \
extern "C" unsigned int pbqp_ ## SHORTNAME ## _getAdjacentNodeIndex(struct pbqp_ ## SHORTNAME ## _parsing* pbqpparsing, \
		unsigned int nodeIndex, unsigned int adjacencyIndex) { \
	PBQPNode<InfinityWrapper<TYPENAME>>* node = pbqpparsing->nodes.at(nodeIndex); \
	return node->getAdjacentNodes().at(adjacencyIndex)->getIndex(); \
}

#if PBQP_USE_GUROBI
#define CGUROBI(TYPENAME,SHORTNAME) \
extern "C" struct pbqp_ ## SHORTNAME ## _solution* pbqp_ ## SHORTNAME ## _solveGurobi(struct pbqp_ ## SHORTNAME ## _parsing* pbqpparsing) { \
	if (pbqpparsing->graph->getNodeCount() == 0) { \
		return new pbqp_ ## SHORTNAME ## _solution(); \
	} \
	GurobiConverter<TYPENAME> gurobi (pbqpparsing->graph); \
	PBQPSolution<InfinityWrapper<TYPENAME>>* cppLevelSol = gurobi.solveGurobiQuadratic(); \
	pbqp_ ## SHORTNAME ## _solution* cLevelSol = pbqp_ ## SHORTNAME ## _convertSolution(pbqpparsing, cppLevelSol); \
	delete cppLevelSol; \
	return cLevelSol; \
}
#endif

CINTERFACEIMPL(unsigned int, uint)

CINTERFACEIMPL(unsigned short, ushort)

CINTERFACEIMPL(unsigned long, ulong)

#if PBQP_USE_GUROBI
CGUROBI(unsigned int, uint)
CGUROBI(unsigned short, ushort)
CGUROBI(unsigned long, ulong)
#endif

}
