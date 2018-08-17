#ifndef REDUCTION_EDGEDEDUPLICATOR_HPP_
#define REDUCTION_EDGEDEDUPLICATOR_HPP_

#include "reduction/PBQPReduction.hpp"

template<typename T>
class NodeConsistentReduction;
template<typename T>
class PBQP_Graph;
template<typename T>
class PBQP_Solution;

template<typename T>
class EdgeDeduplicator: public NodeConsistentReduction<T> {
public:

	EdgeDeduplicator(PBQP_Graph<T>* graph) :
			NodeConsistentReduction<T>(graph) {
	}

	~EdgeDeduplicator() {
	}

	std::vector<PBQP_Graph<T>*>* reduce() {
		for (PBQP_Node<T>* node : *graph->getNodes()) {
			//sort edges into a map where the adjacent node is the key. If we find an edge which already has entry for its node, we found a duplicate
			std::map<PBQP_Node<T>*, PBQP_Edge<T>*>* adjacencyMap = new std::map<
					PBQP_Node<T>*, PBQP_Edge<T>*>();
			std::vector<PBQP_Edge<T>*>* edges = node->getAdjacentEdges(false);
			for (int i = 0; i < node->getDegree(); i++) {
				PBQP_Edge<T>* edge = (*edges)[i];
				PBQP_Node<T>* otherEnd = edge->getOtherEnd(node);
				auto search = adjacencyMap->find(otherEnd);
				if (search != adjacencyMap->end()) {
					//entry for this node already exists, meaning we already found an edge connecting to it before
					PBQP_Edge<T>* existingEdge =
							(PBQP_Edge<T>*) (search->second);
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
							std::pair<PBQP_Node<T>*, PBQP_Edge<T>*>(otherEnd,
									edge));
				}
			}
			delete adjacencyMap;
		}
		result->push_back(graph);
		return result;
	}
};

#endif /* REDUCTION_EDGEDEDUPLICATOR_HPP_ */
