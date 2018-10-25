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

/*
 bool remapIndices = false;

 #define cInterfaceImplementation(X)   \
PBQPGraph<X>* graphX; \
std::map<unsigned int, PBQPNode<X>*> nodesX; \
extern "C" int addNode(X* data, unsigned short length, unsigned int index) { \
	Vector<X> vek (length, data); \
	PBQPNode<X>* node = graphX->addNode(vek); \
	unsigned int internalIndex = node->getIndex(); \
	if (remapIndices) { \
		nodesX.insert(std::pair<unsigned int, PBQPNode<X>*>(index, node)); \
		return index; \
	} \
	else { \
		nodesX.insert(std::pair<unsigned int, PBQPNode<X>*>(internalIndex, node)); \
		return internalIndex; \
	} \
} \
extern "C" void addEdge(unsigned int sourceIndex, unsigned int targetIndex, X* data) { \
	PBQPNode<X>* source; \
	PBQPNode<X>* target; \
	if (remapIndices) { \
		auto iterSource = nodesX.find(sourceIndex); \
		if (iterSource == nodesX.end()) { \
			std::cout << "Could not find index while remapping, skipped egde"; \
			return; \
		} \
		source = iterSource->second; \
		auto iterTarget = nodesX.find(targetIndex); \
		if (iterTarget == nodesX.end()) { \
			std::cout << "Could not find index while remapping, skipped egde"; \
			return; \
		} \
		target = iterTarget->second; \
	} \
	else { \
		source = nodesX.at(sourceIndex); \
		target = nodesX.at(targetIndex); \
	} \
	Matrix<X> mat (source->getVectorDegree(), target->getVectorDegree(), data); \
	graphX->addEdge(source, target, mat); \
} \
extern "C" void dump(char* path, X type) { \
	if (graphX->getNodeCount() == 0) { \
		std::cout << "Graph requested to dump to flat file was empty, did not dump"; \
		return; \
	} \
	PBQP_Serializer<X> serial; \
	serial.saveToFile(path, graphX); \
} \
extern "C" void resetAndPrepare(unsigned int nodeAmount, bool useNodeRemapping, X type) { \
	nodesX.clear(); \
	if (graphX != NULL) { \
		delete graphX; \
	} \
	remapIndices = useNodeRemapping; \
	graphX = new PBQPGraph<X>(); \
}


 cInterfaceImplementation(unsigned int)

 cInterfaceImplementation(unsigned short)

 cInterfaceImplementation(unsigned long)

 cInterfaceImplementation(unsigned char)

 cInterfaceImplementation(float)

 cInterfaceImplementation(double)
 */

extern "C" int addNodePBQP(pbqpparsing* pbqpparsing, unsigned long* data,
		int length, int index) {
	InfinityWrapper<unsigned long> infData[length];
	for (int i = 0; i < length; i++) {
		infData[i] = InfinityWrapper<unsigned long>(data[i]);
	}
	Vector<InfinityWrapper<unsigned long>> vek((unsigned short) length,
			infData);
	PBQPNode<InfinityWrapper<unsigned long>>* node =
			pbqpparsing->graph->addNode(vek);
	int internalIndex = node->getIndex();
	if (pbqpparsing->remapIndices) {
		pbqpparsing->nodes.insert(
				std::pair<int, PBQPNode<InfinityWrapper<unsigned long>>*>(index,
						node));
		return index;
	} else {
		pbqpparsing->nodes.insert(
				std::pair<int, PBQPNode<InfinityWrapper<unsigned long>>*>(
						internalIndex, node));
		return internalIndex;
	}
}

extern "C" void addEdgePBQP(pbqpparsing* pbqpparsing, int sourceIndex,
		int targetIndex, unsigned long* data) {
	PBQPNode<InfinityWrapper<unsigned long>>* source;
	PBQPNode<InfinityWrapper<unsigned long>>* target;
	if (pbqpparsing->remapIndices) {
		auto iterSource = pbqpparsing->nodes.find(sourceIndex);
		if (iterSource == pbqpparsing->nodes.end()) {
			std::cout << "Could not find index while remapping, skipped egde";
			//TODO Exception?
			return;
		}
		source = iterSource->second;
		auto iterTarget = pbqpparsing->nodes.find(targetIndex);
		if (iterTarget == pbqpparsing->nodes.end()) {
			std::cout << "Could not find index while remapping, skipped egde";
			//TODO Exception?
			return;
		}
		target = iterTarget->second;
	} else {
		source = pbqpparsing->nodes.at(sourceIndex);
		target = pbqpparsing->nodes.at(targetIndex);
	}
	int length = source->getVectorDegree() * target->getVectorDegree();
	InfinityWrapper<unsigned long> infData[length];
	for (int i = 0; i < length; i++) {
		infData[i] = InfinityWrapper<unsigned long>(data[i]);
	}
	Matrix<InfinityWrapper<unsigned long>> mat(source->getVectorDegree(),
			target->getVectorDegree(), infData);
	pbqpparsing->graph->addEdge(source, target, mat);
}

extern "C" void solvePBQP(pbqpparsing* pbqpparsing) {
	if (pbqpparsing->graph->getNodeCount() == 0) {
		return;
	}
	printf("\n Solving graph with size: %d; Counter: %d\n",
			pbqpparsing->graph->getNodeCount());
	DebugTimer timer;
	timer.startTimer();
	FullSolver<InfinityWrapper<unsigned long>> solver(pbqpparsing->graph);
	PBQPSolution<InfinityWrapper<unsigned long>>* sol = solver.solve();
	timer.stopTimer();
	delete sol;
	std::cout << timer.getOutput();
}

extern "C" pbqpparsing* createInstancePBQP(bool useNodeRemapping, int nodeAmount) {
	pbqpparsing* result = new pbqpparsing;
	result->remapIndices = useNodeRemapping;
	result->graph = new PBQPGraph<InfinityWrapper<unsigned long>>();
	return result;
}

extern "C" void addToPEOPBQP(pbqpparsing* pbqpparsing, int index) {
	pbqpparsing->peo.push_back(getNode(pbqpparsing, index));
}

extern "C" void dumpPBQP(pbqpparsing* pbqpparsing, char* path) {
	if (pbqpparsing->graph->getNodeCount() == 0) {
		std::cout << "Graph requested to dump to flat file was empty, did not dump";
		return;
	}
	PBQP_Serializer<InfinityWrapper<unsigned long>> serial;
	serial.saveToFile(path, pbqpparsing->graph);
}

PBQPNode<InfinityWrapper<unsigned long>>* getNode(pbqpparsing* pbqpparsing, int index) {
	//TODO what happens if a bad index is given?
	return pbqpparsing->nodes.find(index)->second;
}

}
