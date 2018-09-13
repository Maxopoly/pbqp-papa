#ifndef REDUCTION_CONNECTEDCOMPONENTSEPARATOR_HPP_
#define REDUCTION_CONNECTEDCOMPONENTSEPARATOR_HPP_

#include <vector>

namespace pbqppapa {

template<typename T>
class PBQPGraph;
template<typename T>
class DependentSolution;
template<typename T>
class NodeConsistentReduction;
template<typename T>
class PBQPNode;
template<typename T>
class PBQP_Reduction;

/**
 * Splits up a graph into its connected components by putting each of them into their own graph instance
 */
template<typename T>
class ConnectedComponentSeparator: public PBQP_Reduction<T> {

public:
	ConnectedComponentSeparator(PBQPGraph<T>* graph) :
			PBQP_Reduction<T>(graph) {
	}

	~ConnectedComponentSeparator() {
	}

	std::vector<PBQPGraph<T>*>& reduce() override {
		while (true) {
			std::set<PBQPNode<T>*> foundNodes = std::set<PBQPNode<T>*>();
			std::vector<PBQPNode<T>*> todoStack = std::vector<PBQPNode<T>*>();
			if (this->graph->getNodeCount() > 0) {
				auto iter = this->graph->getNodeBegin();
				todoStack.push_back(*iter);
			}
			while (!todoStack.empty()) {
				PBQPNode<T>* node = todoStack.back();
				todoStack.pop_back();
				if (foundNodes.count(node) > 0) {
					continue;
				}

				foundNodes.insert(node);
				for (PBQPNode<T>* adjacentNode : node->getAdjacentNodes()) {
					if (foundNodes.count(adjacentNode) == 0) {
						todoStack.push_back(adjacentNode);
					}
				}
			}
			if (foundNodes.size() == this->graph->getNodeCount()) {
				//only one connected component
				this->result.push_back(this->graph);
				break;
			} else {
				//multiple connected components in this graph, so we remove the one we found and keep going
				//TODO Removing the part of the original graph that is not in this component would make more sense
				//as we're more likely to find the bigger components when randomly picking a node
				//That way we'd have to do another log(n) lookup for every node in the original graph though
				PBQPGraph<T>* replacementGraph = new PBQPGraph<T>();
				for (PBQPNode<T>* node : foundNodes) {
					this->graph->removeNode(node, false);
					replacementGraph->addNode(node);
					for (PBQPEdge<T>* edge : node->getAdjacentEdges(true)) {
						//makes us iterate only over outgoing edges, so we can always insert all of those safely
						//while ensuring that every edge is inserted once. We dont need to remove the edge from the
						//old graph as removing the node already does that
						replacementGraph->addEdge(edge);
					}
				}
				this->result.push_back(replacementGraph);
			}
		}
		return this->result;
	}

	void solve(PBQPSolution<T>& solution) override {
		//don't need to do anything
	}
};

}

#endif /* REDUCTION_CONNECTEDCOMPONENTSEPARATOR_HPP_ */
