#ifndef DEBUG_GRAPHVISUALIZER_HPP_
#define DEBUG_GRAPHVISUALIZER_HPP_

#include <gvc.h>
#include <map>
#include "io/PBQP_Serializer.hpp"

namespace pbqppapa {

template<typename T>
class PBQPGraph;
template<typename T>
class PBQPNode;
template<typename T>
class PBQPEdge;

bool wasGvcInited = false;
static GVC_t* gvc;

void init() {
	if (!wasGvcInited) {
		wasGvcInited = true;
		/* set up a graphviz context - but only once even for multiple graphs */
		if (!gvc) {
			gvc = gvContext();
		}
	}
}

char* convertString(std::string string) {
	char * writable = new char[string.size() + 1];
	std::copy(string.begin(), string.end(), writable);
	writable[string.size()] = '\0'; // don't forget the terminating 0
	return writable;
}

template<typename T>
void dump(PBQPGraph<T>* graph, std::string path) {
	init();
	int arguments = 4;
	char** args = new char*[arguments];
	args [0] = convertString("-Tsvg");
	args [1] = convertString("-o" + path);
	args [2] = convertString("-Kdot");
	args [3] = convertString("-Tsvg");
	gvParseArgs(gvc, arguments, args);
	delete [] args;
	Agraph_t* graphVis = agopen("g", Agdirected, 0);
	std::map<PBQPNode<T>*, Agnode_t*> nodeMapping;
	PBQP_Serializer<T> serial;
	agattr(graphVis, AGNODE, "URL", "TESTEST");
	agattr(graphVis, AGNODE, "tooltip", "TESTEST");
	for(auto iter = graph->getNodeBegin(); iter != graph->getNodeEnd(); ++iter) {
		PBQPNode<T>* node = *iter;
		char* name = convertString("N" + std::to_string(node->getIndex()) + "\n" + serial.matrixToString(node->getVector()));
		Agnode_t* nodeVis = agnode(graphVis, convertString(std::to_string(node->getIndex())), 1);
		agset(nodeVis, "URL", "TESTTEST");
		agset(nodeVis, "tooltip", name);
		//delete [] name;
		nodeMapping.insert(std::pair<PBQPNode<T>*, Agnode_t*>(node,nodeVis));
	}
	//agattr(graphVis, AGEDGE, "label", "a");
	agattr(graphVis, AGEDGE, "URL", "TESTEST");
	agattr(graphVis, AGEDGE, "edgetooltip", "");
	agattr(graphVis, AGEDGE, "penwidth", "5.0");
	for(auto iter = graph->getEdgeBegin(); iter != graph->getEdgeEnd(); ++iter) {
			PBQPEdge<T>* edge = *iter;
			Agnode_t* sourceVis = nodeMapping.find(edge->getSource())->second;
			Agnode_t* targetVis = nodeMapping.find(edge->getTarget())->second;
			char* name = convertString(serial.matrixToString(edge->getMatrix()));
			Agedge_t* edgeVis = agedge(graphVis, sourceVis, targetVis, name, 1);
			//agset(edgeVis, "label", name);
			agset(edgeVis, "URL", "TESTEST");
			agset(edgeVis, "edgetooltip", name);
			agattr(graphVis, AGEDGE, "penwidth", "5.0");


	}
	gvLayoutJobs(gvc, graphVis);
	gvRenderJobs(gvc, graphVis);
	gvFreeLayout(gvc, graphVis);
	agclose(graphVis);
}



}

#endif /* DEBUG_GRAPHVISUALIZER_HPP_ */
