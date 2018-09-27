#include "graph/PBQPGraph.hpp"
#include "graph/Vector.hpp"
#include "graph/PBQPNode.hpp"
#include "graph/PBQPEdge.hpp"
#include "io/PBQP_Serializer.hpp"
#include "io/TypeSerializers.hpp"
#include "CInterface.h"

#include <stdio.h>
#include <vector>
#include <time.h>
#include <chrono>

namespace pbqppapa {

PBQPGraph<unsigned long>* graph;
std::map<int, PBQPNode<unsigned long>*> nodes;
bool remapIndices = false;
static unsigned long counter = 0;



extern "C" int addNode(unsigned long* data, int length, int index) {
	Vector<unsigned long> vek ((unsigned short) length, data);
	PBQPNode<unsigned long>* node = graph->addNode(vek);
	int internalIndex = node->getIndex();
	if (remapIndices) {
		nodes.insert(std::pair<int, PBQPNode<unsigned long>*>(index, node));
		return index;
	}
	else {
		nodes.insert(std::pair<int, PBQPNode<unsigned long>*>(internalIndex, node));
		return internalIndex;
	}
}

extern "C" void addEdge(int sourceIndex, int targetIndex, unsigned long* data) {
	PBQPNode<unsigned long>* source;
	PBQPNode<unsigned long>* target;
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
	Matrix<unsigned long> mat (source->getVectorDegree(), target->getVectorDegree(), data);
	graph->addEdge(source, target, mat);
}

extern "C" void dump() {
	if (graph->getNodeCount() == 0) {
		return;
	}
	printf("\n Printing graph with size: %d; Counter: %d\n", graph->getNodeCount(), counter);
	std::clock_t time = clock();
	PBQP_Serializer<unsigned long> serial;
	std::string name = "jsonOutputs2/output";
	name += std::to_string(counter++);
	name += "____";
	name += std::to_string(long(time));
	name += ".json";
	serial.saveToFile(name, graph);
}

extern "C" void enableNodeRemapping() {
	remapIndices = true;
}

extern "C" void setNodeAmount(unsigned int amount) {
	nodes.clear();
	if (graph != NULL) {
		delete graph;
	}
	graph = new PBQPGraph<unsigned long>();
}

}
