#include "graph/PBQPGraph.hpp"
#include "graph/Vector.hpp"
#include "graph/PBQPNode.hpp"
#include "graph/PBQPEdge.hpp"
#include "io/PBQP_Serializer.hpp"
#include "CInterface.h"

#include <vector>

namespace pbqppapa {

PBQPGraph<unsigned long> graph;
std::vector<PBQPNode<unsigned long>*> nodes;


extern "C" int addNode(unsigned long* data, int length) {
	Vector<unsigned long> vek ((unsigned short int) length, data);
	PBQPNode<unsigned long>* node = graph.addNode(vek);
	nodes.at(node->getIndex()) = node;
	return node->getIndex();
}

extern "C" void addEdge(int sourceIndex, int targetIndex, unsigned long* data) {
	PBQPNode<unsigned long>* source = nodes.at(sourceIndex);
	PBQPNode<unsigned long>* target = nodes.at(targetIndex);
	Matrix<unsigned long> mat (source->getVectorDegree(), target->getVectorDegree(), data);
	graph.addEdge(source, target, mat);
}

extern "C" void dump() {
	PBQP_Serializer<unsigned long> serial;
	serial.saveToFile("output.json", &graph);
}

}
