#ifndef DEBUG_GRAPHVISUALIZER_HPP_
#define DEBUG_GRAPHVISUALIZER_HPP_


#include <map>
#include "io/PBQP_Serializer.hpp"

#if PBQP_USE_GVC
#include <gvc.h>

namespace pbqppapa {

template<typename T>
class PBQPGraph;
template<typename T>
class PBQPNode;
template<typename T>
class PBQPEdge;

bool wasGvcInited = false;
GVC_t* gvcGlobalContext;

void initializeGVC_PBQP() {
	if (!wasGvcInited) {
		wasGvcInited = true;
		/* set up a graphviz context - but only once even for multiple graphs */
		if (!gvcGlobalContext) {
			gvcGlobalContext = gvContext();
		}
	}
}

char* convertStringToC(std::string string) {
	char * writable = new char[string.size() + 1];
	std::copy(string.begin(), string.end(), writable);
	writable[string.size()] = '\0'; // don't forget the terminating 0
	return writable;
}

template<typename T>
class GraphVisualizer {



public:
	static void dump(PBQPGraph<T>* graph, std::string path, bool showVectors = false) {
		initializeGVC_PBQP();
		int arguments = 4;
		char** args = new char*[arguments];
		args[0] = convertStringToC("-Tsvg");
		args[1] = convertStringToC("-o" + path);
		args[2] = convertStringToC("-Kdot");
		args[3] = convertStringToC("-Tsvg");
		gvParseArgs(gvcGlobalContext, arguments, args);
		delete[] args;
		Agraph_t* graphVis = agopen(convertStringToC("g"), Agdirected, 0);
		std::map<PBQPNode<T>*, Agnode_t*> nodeMapping;
		PBQP_Serializer<T> serial;
		agattr(graphVis, AGNODE, convertStringToC("URL"),
				convertStringToC("URLVALUE"));
		agattr(graphVis, AGNODE, convertStringToC("tooltip"),
				convertStringToC("tooltipValue"));
		for (auto iter = graph->getNodeBegin(); iter != graph->getNodeEnd();
				++iter) {
			PBQPNode<T>* node = *iter;
			char* name = convertStringToC(
					"N " + std::to_string(node->getIndex()) + "\n"
							+ serial.matrixToString(node->getVector()));
			Agnode_t* nodeVis;
			if (showVectors) {
				nodeVis = agnode(graphVis, name, 1);
			} else {
				nodeVis = agnode(graphVis,
						convertStringToC(std::to_string(node->getIndex())), 1);
			}
			agset(nodeVis, convertStringToC("URL"), convertStringToC("URLVALUE"));
			agset(nodeVis, convertStringToC("tooltip"), name);
			nodeMapping.insert(
					std::pair<PBQPNode<T>*, Agnode_t*>(node, nodeVis));
		}
		if (showVectors) {
			agattr(graphVis, AGEDGE, convertStringToC("label"), convertStringToC("a"));
		}
		agattr(graphVis, AGEDGE, convertStringToC("URL"),
				convertStringToC("URLVALUE"));
		agattr(graphVis, AGEDGE, convertStringToC("edgetooltip"),
				convertStringToC(""));
		agattr(graphVis, AGEDGE, convertStringToC("penwidth"),
				convertStringToC("1.0"));
		for (auto iter = graph->getEdgeBegin(); iter != graph->getEdgeEnd();
				++iter) {
			PBQPEdge<T>* edge = *iter;
			Agnode_t* sourceVis = nodeMapping.find(edge->getSource())->second;
			Agnode_t* targetVis = nodeMapping.find(edge->getTarget())->second;
			char* name = convertStringToC(
					serial.matrixToString(edge->getMatrix()));
			Agedge_t* edgeVis = agedge(graphVis, sourceVis, targetVis, name, 1);
			if (showVectors) {
				agset(edgeVis, convertStringToC("label"), name);
			}
			agset(edgeVis, convertStringToC("URL"), convertStringToC("URLVALUE"));
			agset(edgeVis, convertStringToC("edgetooltip"), name);
			agattr(graphVis, AGEDGE, convertStringToC("penwidth"),
					convertStringToC("1.0"));
		}
		gvLayoutJobs(gvcGlobalContext, graphVis);
		gvRenderJobs(gvcGlobalContext, graphVis);
		gvFreeLayout(gvcGlobalContext, graphVis);
		agclose(graphVis);
	}
};
}

#endif

#endif /* DEBUG_GRAPHVISUALIZER_HPP_ */
