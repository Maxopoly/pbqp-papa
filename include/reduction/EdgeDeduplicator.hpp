#ifndef REDUCTION_EDGEDEDUPLICATOR_HPP_
#define REDUCTION_EDGEDEDUPLICATOR_HPP_

#include "reduction/PBQPReduction.hpp"

namespace pbqppapa {

template<typename T>
class NodeConsistentReduction;
template<typename T>
class PBQPGraph;
template<typename T>
class PBQPSolution;

template<typename T>
class EdgeDeduplicator: public NodeConsistentReduction<T> {
public:

	EdgeDeduplicator(PBQPGraph<T>* graph) :
			NodeConsistentReduction<T>(graph) {
	}

	~EdgeDeduplicator() {
	}

	std::vector<PBQPGraph<T>*>& reduce() {
		for (PBQPNode<T>* node : *graph->getNodes()) {
			//sort edges into a map where the adjacent node is the key. If we find an edge which already has entry for its node, we found a duplicate
			std::map<PBQPNode<T>*, PBQPEdge<T>*>* adjacencyMap = new std::map<
					PBQPNode<T>*, PBQPEdge<T>*>();
			std::vector<PBQPEdge<T>*>* edges = node->getAdjacentEdges(false);
			for (int i = 0; i < node->getDegree(); i++) {
				PBQPEdge<T>* edge = (*edges)[i];
				PBQPNode<T>* otherEnd = edge->getOtherEnd(node);
				auto search = adjacencyMap->find(otherEnd);
				if (search != adjacencyMap->end()) {
					//entry for this node already exists, meaning we already found an edge connecting to it before
					PBQPEdge<T>* existingEdge = (PBQPEdge<T>*) (search->second);
					//check direction to determine addition method
					if (existingEdge->getSource() == edge->getSource()) {
						//facing same direction
						*(existingEdge->getMatrix()) += *(edge->getMatrix());
					} else {
						//facing opposite direction
						*(existingEdge->getMatrix()) +=
								*(edge->getMatrix()->transpose());
					}
					graph->removeEdge(edge);
					//list of nodes just got one element shorter, so we need to make sure we dont skip an element
					i--;
				} else {
					//node not previously known, so insert it
					adjacencyMap->insert(
							std::pair<PBQPNode<T>*, PBQPEdge<T>*>(otherEnd,
									edge));
				}
			}
			delete adjacencyMap;
		}
		result->push_back(graph);
		return result;
	}
};

}

#endif /* REDUCTION_EDGEDEDUPLICATOR_HPP_ */
