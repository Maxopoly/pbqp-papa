#ifndef REDUCTION_CONNECTEDCOMPONENTSEPARATOR_HPP_
#define REDUCTION_CONNECTEDCOMPONENTSEPARATOR_HPP_

#include <vector>

template<typename T>
class PBQP_Graph;
template<typename T>
class Dependent_Solution;
template<typename T>
class NodeConsistentReduction;
template<typename T>
class PBQP_Node;
template<typename T>
class PBQP_Reduction;

/**
 * Splits up a graph into its connected components by putting each of them into their own graph instance
 */
template<typename T>
class ConnectedComponentSeparator: public PBQP_Reduction<T> {

public:
	ConnectedComponentSeparator(PBQP_Graph<T>* graph) :
		PBQP_Reduction<T>(graph) {
	}

	~ConnectedComponentSeparator() {
	}

	std::vector<PBQP_Graph<T>*>* reduce() override {
		while (true) {
			std::set<PBQP_Node<T>*> foundNodes = *new std::set<PBQP_Node<T>*>();
			std::vector<PBQP_Node<T>*> todoStack =
					*new std::vector<PBQP_Node<T>*>();
			if(this->graph->getNodeCount() > 0) {
				auto iter = this->graph->getNodeBegin();
				todoStack.push_back(*iter);
			}
			while (!todoStack.empty()) {
				PBQP_Node<T>* node = todoStack.back();
				todoStack.pop_back();
				if (foundNodes.count(node) > 0) {
					continue;
				}
				foundNodes.insert(node);
				for (PBQP_Node<T>* adjacentNode : *(node->getAdjacentNodes())) {
					if (foundNodes.count(adjacentNode) == 0) {
						todoStack.push_back(adjacentNode);
					}
				}
			}
			if (foundNodes.size() == this->graph->getNodeCount()) {
				//only one connected component
				this->result->push_back(this->graph);
				break;
			} else {
				//multiple connected components in this graph, so we remove the one we found and keep going
				//TODO Removing the part of the original graph that is not in this component would make more sense
				//as we're more likely to find the bigger components when randomly picking a node
				//That way we'd have to do another log(n) lookup for every node in the original graph though
				PBQP_Graph<T>* replacementGraph = new PBQP_Graph<T>();
				for (PBQP_Node<T>* node : foundNodes) {
					this->graph->removeNode(node, false);
					replacementGraph->addNode(node);
					for (PBQP_Edge<T>* edge : *(node->getAdjacentEdges(true))) {
						//makes us iterate only over outgoing edges, so we can always insert all of those safely
						//while ensuring that every edge is inserted once. We dont need to remove the edge from the
						//old graph as removing the node already does that
						replacementGraph->addEdge(edge);
					}
				}
				this->result->push_back(replacementGraph);
			}
		}
		return this->result;
	}

	PBQP_Solution<T>* solve(PBQP_Solution<T>* solution) override {
		//don't need to do anything
		return solution;
	}
};

#endif /* REDUCTION_CONNECTEDCOMPONENTSEPARATOR_HPP_ */
