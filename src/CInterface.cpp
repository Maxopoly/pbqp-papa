#include "graph/PBQPGraph.hpp"
#include "graph/Vector.hpp"
#include "graph/PBQPNode.hpp"
#include "graph/PBQPEdge.hpp"
#include "CInterface.h"
#include "FullSolver.hpp"
#include "math/InfinityWrapper.hpp"
#include "io/PBQP_Serializer.hpp"

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
}; \
extern "C" unsigned int pbqp_ ## SHORTNAME ## _addNode(pbqp_ ## SHORTNAME ## _parsing* pbqpparsing, \
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
		return index; \
	} else { \
		pbqpparsing->nodes.insert( \
				std::pair<unsigned int, PBQPNode<InfinityWrapper<TYPENAME>>*>( \
						internalIndex, node)); \
		return internalIndex; \
	} \
} \
extern "C" void pbqp_ ## SHORTNAME ## _addEdge(pbqp_ ## SHORTNAME ## _parsing* pbqpparsing, \
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
extern "C" void pbqp_ ## SHORTNAME ## _solve(pbqp_ ## SHORTNAME ## _parsing* pbqpparsing) { \
	if (pbqpparsing->graph->getNodeCount() == 0) { \
		return; \
	} \
	FullSolver<InfinityWrapper<TYPENAME>> solver(pbqpparsing->graph); \
	PBQPSolution<InfinityWrapper<TYPENAME>>* sol = solver.solve(); \
	delete sol; \
} \
extern "C" pbqp_ ## SHORTNAME ## _parsing* pbqp_ ## SHORTNAME ## _createInstance( \
		bool useNodeRemapping, unsigned int nodeAmount) { \
	pbqp_ ## SHORTNAME ## _parsing* result = new pbqp_ ## SHORTNAME ## _parsing (); \
	result->remapIndices = useNodeRemapping; \
	result->graph = new PBQPGraph<InfinityWrapper<TYPENAME>>(); \
	return result; \
} \
extern "C" void pbqp_ ## SHORTNAME ## _addToPEO(pbqp_ ## SHORTNAME ## _parsing* pbqpparsing, \
		unsigned int index) { \
		pbqpparsing->peo.push_back(pbqpparsing->nodes.find(index)->second); \
} \
extern "C" void pbqp_ ## SHORTNAME ## _dump(pbqp_ ## SHORTNAME ## _parsing* pbqpparsing, char* path) { \
	if (pbqpparsing->graph->getNodeCount() == 0) { \
		std::cout << "Graph requested to dump to flat file was empty, did not dump"; \
		return; \
	} \
	PBQP_Serializer<InfinityWrapper<TYPENAME>> serial; \
	serial.saveToFile(path, pbqpparsing->graph); \
} \
extern "C" void pbqp_ ## SHORTNAME ## _free(pbqp_ ## SHORTNAME ## _parsing* pbqpparsing) { \
	delete pbqpparsing->graph; \
	delete pbqpparsing; \
}

CINTERFACEIMPL(unsigned int,uint)

CINTERFACEIMPL(unsigned short,ushort)

CINTERFACEIMPL(unsigned long,ulong)

/*
CINTERFACEIMPL(unsigned char,uchar)

CINTERFACEIMPL(float,float)

CINTERFACEIMPL(double,double) */




}
