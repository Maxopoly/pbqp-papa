#include "graph/PBQPGraph.hpp"
#include "graph/Vector.hpp"
#include "graph/PBQPNode.hpp"
#include "graph/PBQPEdge.hpp"
#include "CInterface.h"
#include "FullSolver.hpp"
#include "math/InfinityWrapper.hpp"

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


PBQPGraph<InfinityWrapper<unsigned long>>* graph;
std::map<int, PBQPNode<InfinityWrapper<unsigned long>>*> nodes;
bool remapIndices = false;
static unsigned long counter = 0;



extern "C" int addNode(unsigned long* data, int length, int index) {
	InfinityWrapper<unsigned long> infData [length];
	for(int i = 0; i < length; i++) {
		infData [i] = InfinityWrapper<unsigned long>(data [i]);
	}
	Vector<InfinityWrapper<unsigned long>> vek ((unsigned short) length, infData);
	PBQPNode<InfinityWrapper<unsigned long>>* node = graph->addNode(vek);
	int internalIndex = node->getIndex();
	if (remapIndices) {
		nodes.insert(std::pair<int, PBQPNode<InfinityWrapper<unsigned long>>*>(index, node));
		return index;
	}
	else {
		nodes.insert(std::pair<int, PBQPNode<InfinityWrapper<unsigned long>>*>(internalIndex, node));
		return internalIndex;
	}
}

extern "C" void addEdge(int sourceIndex, int targetIndex, unsigned long* data) {
	PBQPNode<InfinityWrapper<unsigned long>>* source;
	PBQPNode<InfinityWrapper<unsigned long>>* target;
	if (remapIndices) {
		auto iterSource = nodes.find(sourceIndex);
		if (iterSource == nodes.end()) {
			std::cout << "Could not find index while remapping, skipped egde";
			//TODO Exception?
			return;
		}
		source = iterSource->second;
		auto iterTarget = nodes.find(targetIndex);
		if (iterTarget == nodes.end()) {
			std::cout << "Could not find index while remapping, skipped egde";
			//TODO Exception?
			return;
		}
		target = iterTarget->second;
	}
	else {
		source = nodes.at(sourceIndex);
		target = nodes.at(targetIndex);
	}
	int length = source->getVectorDegree() * target->getVectorDegree();
	InfinityWrapper<unsigned long> infData [length];
	for(int i = 0; i < length; i++) {
		infData [i] = InfinityWrapper<unsigned long>(data [i]);
	}
	Matrix<InfinityWrapper<unsigned long>> mat (source->getVectorDegree(), target->getVectorDegree(), infData);
	graph->addEdge(source, target, mat);
}

extern "C" void solve() {
	if (graph->getNodeCount() == 0) {
		return;
	}
	printf("\n Solving graph with size: %d; Counter: %d\n", graph->getNodeCount(), counter);
	DebugTimer timer;
	timer.startTimer();
	FullSolver<InfinityWrapper<unsigned long>> solver (graph);
	PBQPSolution<InfinityWrapper<unsigned long>>* sol = solver.solve();
	timer.stopTimer();
	delete sol;
	std::cout << timer.getOutput();

}

extern "C" void enableNodeRemapping() {
	remapIndices = true;
}

extern "C" void setNodeAmount(unsigned int amount) {
	nodes.clear();
	if (graph != NULL) {
		delete graph;
	}
	graph = new PBQPGraph<InfinityWrapper<unsigned long>>();
}



}
