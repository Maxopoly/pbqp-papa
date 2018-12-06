#include "graph/PBQPGraph.hpp"
#include "graph/PBQPSolution.hpp"
#include "graph/Vector.hpp"
#include "graph/PBQPNode.hpp"
#include "graph/PBQPEdge.hpp"
#include "CInterface.h"
#include "FullSolver.hpp"
#include "math/InfinityWrapper.hpp"
#include "io/PBQP_Serializer.hpp"
//#include "math/GurobiConverter.hpp"

#include <stdio.h>
#include <vector>
#include <time.h>
#include <iostream>

namespace pbqppapa {

#define CINTERFACEIMPL(TYPENAME,SHORTNAME) \
struct pbqp_ ## SHORTNAME ## _parsing { \
PBQPGraph<InfinityWrapper< TYPENAME >>* graph; \
std::map<unsigned int, PBQPNode<InfinityWrapper< TYPENAME >>*> nodes; \
std::vector<PBQPNode<InfinityWrapper< TYPENAME >>*> peo; \
bool remapIndices = false; \
unsigned long maximumIndex = 0; \
}; \
extern "C" unsigned int pbqp_ ## SHORTNAME ## _addNode(struct  pbqp_ ## SHORTNAME ## _parsing* pbqpparsing, \
		TYPENAME* data, unsigned short length, unsigned int index) { \
	InfinityWrapper<TYPENAME> infData[length]; \
	for (unsigned short i = 0; i < length; i++) { \
		infData[i] = InfinityWrapper<TYPENAME>(data[i]); \
	} \
	Vector<InfinityWrapper<TYPENAME>> vek(length, infData); \
	PBQPNode<InfinityWrapper<TYPENAME>>* node = \
			pbqpparsing->graph->addNode(vek); \
	unsigned int internalIndex = node->getIndex(); \
	if (pbqpparsing->remapIndices) { \
		pbqpparsing->nodes.insert( \
				std::pair<unsigned int, PBQPNode<InfinityWrapper<TYPENAME>>*>(index, \
						node)); \
		if (pbqpparsing->maximumIndex < index) { \
			pbqpparsing->maximumIndex = index; \
		} \
		return index; \
	} else { \
		pbqpparsing->nodes.insert( \
				std::pair<unsigned int, PBQPNode<InfinityWrapper<TYPENAME>>*>( \
						internalIndex, node)); \
			if (pbqpparsing->maximumIndex < internalIndex) { \
				pbqpparsing->maximumIndex = internalIndex; \
			} \
		return internalIndex; \
	} \
} \
extern "C" void pbqp_ ## SHORTNAME ## _addEdge(struct  pbqp_ ## SHORTNAME ## _parsing* pbqpparsing, \
	unsigned int sourceIndex, unsigned int targetIndex, TYPENAME* data) { \
	PBQPNode<InfinityWrapper<TYPENAME>>* source; \
	PBQPNode<InfinityWrapper<TYPENAME>>* target; \
	if (pbqpparsing->remapIndices) { \
		auto iterSource = pbqpparsing->nodes.find(sourceIndex); \
		if (iterSource == pbqpparsing->nodes.end()) { \
			std::cout << "Could not find index while remapping, skipped egde"; \
			return; \
		} \
		source = iterSource->second; \
		auto iterTarget = pbqpparsing->nodes.find(targetIndex); \
		if (iterTarget == pbqpparsing->nodes.end()) { \
			std::cout << "Could not find index while remapping, skipped egde"; \
			return; \
		} \
		target = iterTarget->second; \
	} else { \
		source = pbqpparsing->nodes.at(sourceIndex); \
		target = pbqpparsing->nodes.at(targetIndex); \
	} \
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
	for(auto iter : pbqpparsing->nodes) { \
		cSol->selections[iter.first] = cppSol->getSolution(iter.second->getIndex()); \
	} \
	cSol->length = pbqpparsing->maximumIndex; \
	return cSol; \
} \
extern "C" struct pbqp_ ## SHORTNAME ## _solution* pbqp_ ## SHORTNAME ## _solve(struct pbqp_ ## SHORTNAME ## _parsing* \
		pbqpparsing) { \
	if (pbqpparsing->graph->getNodeCount() == 0) { \
		return new pbqp_ ## SHORTNAME ## _solution(); \
	} \
	FullSolver<InfinityWrapper<TYPENAME>> solver(pbqpparsing->graph); \
	PBQPSolution<InfinityWrapper<TYPENAME>>* cppLevelSol = solver.solve(); \
	pbqp_ ## SHORTNAME ## _solution* cLevelSol = pbqp_ ## SHORTNAME ## _convertSolution(pbqpparsing, cppLevelSol); \
	delete cppLevelSol; \
	return cLevelSol; \
} \
/*extern "C" struct pbqp_ ## SHORTNAME ## _solution* pbqp_ ## SHORTNAME ## _solveGurobi(struct pbqp_ ## SHORTNAME ## _parsing* pbqpparsing) { \
	if (pbqpparsing->graph->getNodeCount() == 0) { \
		return new pbqp_ ## SHORTNAME ## _solution(); \
	} \
	GurobiConverter<TYPENAME> gurobi; \
	PBQPSolution<InfinityWrapper<TYPENAME>>* cppLevelSol = gurobi.solveGurobiLinear(pbqpparsing->graph); \
	pbqp_ ## SHORTNAME ## _solution* cLevelSol = pbqp_ ## SHORTNAME ## _convertSolution(pbqpparsing, cppLevelSol); \
	delete cppLevelSol; \
	return cLevelSol; \
} */ \
extern "C" struct pbqp_ ## SHORTNAME ## _parsing* pbqp_ ## SHORTNAME ## _createInstance( \
		bool useNodeRemapping, unsigned int nodeAmount) { \
	pbqp_ ## SHORTNAME ## _parsing* result = new pbqp_ ## SHORTNAME ## _parsing (); \
	result->remapIndices = useNodeRemapping; \
	result->graph = new PBQPGraph<InfinityWrapper<TYPENAME>>(); \
	return result; \
} \
extern "C" void pbqp_ ## SHORTNAME ## _addToPEO(struct pbqp_ ## SHORTNAME ## _parsing* pbqpparsing, \
		unsigned int index) { \
		pbqpparsing->peo.push_back(pbqpparsing->nodes.find(index)->second); \
} \
extern "C" void pbqp_ ## SHORTNAME ## _dump(struct pbqp_ ## SHORTNAME ## _parsing* pbqpparsing, char* path) { \
	if (pbqpparsing->graph->getNodeCount() == 0) { \
		std::cout << "Graph requested to dump to flat file was empty, did not dump"; \
		return; \
	} \
	std::string stringPath; \
	if(!path) { \
		stringPath = std::to_string(std::clock()) + ".json"; \
	} \
	else { \
		stringPath = std::string(path);\
	} \
	pbqpparsing->graph->setPEO(pbqpparsing->peo); \
	PBQP_Serializer<InfinityWrapper<TYPENAME>> serial; \
	serial.saveToFile(stringPath, pbqpparsing->graph); \
} \
extern "C" void pbqp_ ## SHORTNAME ## _free(struct pbqp_ ## SHORTNAME ## _parsing* pbqpparsing) { \
	delete pbqpparsing->graph; \
	delete pbqpparsing; \
}

CINTERFACEIMPL(unsigned int, uint)

CINTERFACEIMPL(unsigned short, ushort)

CINTERFACEIMPL(unsigned long, ulong)

/*
 CINTERFACEIMPL(unsigned char,uchar)

 CINTERFACEIMPL(float,float)

 CINTERFACEIMPL(double,double) */

}
