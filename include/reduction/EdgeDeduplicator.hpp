#ifndef REDUCTION_EDGEDEDUPLICATOR_HPP_
#define REDUCTION_EDGEDEDUPLICATOR_HPP_

#include "reduction/PBQPReduction.hpp"
#include "graph/PBQPGraph.hpp"

namespace pbqppapa {

template<typename T>
class PBQPReduction;
template<typename T>
class PBQPGraph;
template<typename T>
class PBQPSolution;

template<typename T>
class EdgeDeduplicator: public PBQPReduction<T> {
public:

	EdgeDeduplicator(PBQPGraph<T>* graph) :
		PBQPReduction<T>(graph) {
	}

	~EdgeDeduplicator() {
	}

	std::vector<PBQPGraph<T>*>& reduce() {
		auto iter = this->graph->getNodeBegin();
		while (iter != this->graph->getNodeEnd()) {
			//sort edges into a map where the adjacent node is the key.
			//If we find an edge which already has entry for its node, we found a duplicate
			std::map<PBQPNode<T>*, PBQPEdge<T>*> adjacencyMap;
			PBQPNode<T>* node = *iter;
			for(PBQPEdge<T>* edge : node->getAdjacentEdges()) {
				PBQPNode<T>* otherEnd = edge->getOtherEnd(node);
				auto search = adjacencyMap->find(otherEnd);
				if (search != adjacencyMap->end()) {
					//entry for this node already exists, meaning we already found an edge
					//connecting to it before
					PBQPEdge<T>* existingEdge = (PBQPEdge<T>*) (search->second);
					//check direction to determine addition method
					if (existingEdge->getSource() == edge->getSource()) {
						//facing same direction
						existingEdge->getMatrix() += edge->getMatrix();
					} else {
						//facing opposite direction
						existingEdge->getMatrix() += edge->getMatrix().transpose();
					}
					//TODO iterator problems?
					graph->removeEdge(edge);
				} else {
					//node not previously known, so insert it
					adjacencyMap->insert(
							std::pair<PBQPNode<T>*, PBQPEdge<T>*>(otherEnd,
									edge));
				}
			}
		}
		result->push_back(graph);
		return result;
	}
};

}

#endif /* REDUCTION_EDGEDEDUPLICATOR_HPP_ */
