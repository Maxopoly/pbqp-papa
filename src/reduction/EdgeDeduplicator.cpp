/*
 * EdgeDeduplicator.cpp
 *
 *  Created on: 5 Aug 2018
 *      Author: Max
 */

#include "EdgeDeduplicator.hpp"

#include "PBQP_Graph.hpp";
#include "PBQP_Node.hpp";
#include "PBQP_Edge.hpp";
#include "Matrix.hpp";
#include <map>
#include <vector>
#include "PBQPReduction.hpp"

EdgeDeduplicator::EdgeDeduplicator(PBQP_Graph* graph) :
		PBQP_Reduction(graph) {
}

EdgeDeduplicator::~EdgeDeduplicator() {
	// TODO Auto-generated destructor stub
}

PBQP_Graph* EdgeDeduplicator::solve() {
	for (PBQP_Node* node : *graph->getNodes()) {
		//sort edges into a map where the adjacent node is the key. If we find an edge which already has entry for its node, we found a duplicate
		std::map<PBQP_Node*, PBQP_Edge*> adjacencyMap = new std::map<PBQP_Node*,
				PBQP_Edge*>();
		std::vector<PBQP_Edge*>* edges = node->getAdjacentEdges(false);
		for (int i = 0; i < node->getDegree(); i++) {
			PBQP_Edge* edge = (*edges)[i];
			PBQP_Node* otherEnd = edge->getOtherEnd(node);
			auto search = adjacencyMap.find(otherEnd);
			if (search == adjacencyMap.end()) {
				//entry for this node already exists, meaning we already found an edge connecting to it before
				PBQP_Edge* existingEdge = (PBQP_Edge*) (search->second());
				//check direction to determine addition method
				if (existingEdge->getSource() == edge->getSource()) {
					//facing same direction
					*(existingEdge->getMatrix()) += edge->getMatrix();
				} else {
					//facing opposite direction
					*(existingEdge->getMatrix()) += edge->getMatrix()->transpose();
				}
				graph->removeEdge(edge);
				//list of nodes just got one element shorter, so we need to make sure we dont skip an element
				i--;
			} else {
				//node not previously known, so insert it
				adjacencyMap.insert(std::pair<PBQP_Node*, PBQP_Edge*> (otherEnd, edge));
			}
		}
	}
	return graph;
}
