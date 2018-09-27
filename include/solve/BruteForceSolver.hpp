#ifndef SOLVE_BRUTEFORCESOLVER_HPP_
#define SOLVE_BRUTEFORCESOLVER_HPP_

#include "solve/PBQPSolver.hpp"

namespace pbqppapa {

template<typename T>
class PBQPGraph;
template<typename T>
class PBQPSolution;
template<typename T>
class PBQPSolver;
template<typename T>
class PBQPNode;

/**
 * Tries every possible combination to calculate a perfect solution
 *
 * We attempt to do so with the minimal amount of work, which we ideally do by only
 * recalculating the part of the graph that changed compared to the previous iteration.
 *
 * This leads us to a counter where every digit refers to a node in the graph. Each of these digits/nodes
 * can have a possible value within [0,VectorDegree) where VectorDegree is the length of the cost Vector
 * associated with the node. For example for 4 nodes, each with a Vector length of 3, we would count:
 * [0 0 0 0], [0 0 0 1], [0 0 0 2], [0 0 1 0], .... , [2 2 2 2]
 *
 * The problem with this are the cascading changes when higher indexed digits change, for example here:
 * [0 1 1 1] --> [1 0 0 0]. We need to recalculate the cost for the entire graph here, because the entire
 * selection changed!
 *
 * To sidestep this problem, we are not just going to count up the way we know it from binary, but instead
 * we use an adapted version of the Gray code (https://en.wikipedia.org/wiki/Gray_code)
 *
 *
 * By remembering the previous total cost and knowing which node we just changed the selection for in our gray code,
 * we only need to recalculate one node and all of its incident edges per iteration/solution.
 */
template<typename T>
class BruteForceSolver: public PBQPSolver<T> {

private:
	/**
	 * Selection arrays equal a possible solution for the PBQP.
	 * The value at index [i] is the index of the solution in the cost Vector of the node with internal index i
	 *
	 * Example: selection [5] = 3
	 * means that the third element in the cost Vector of the node with index 5 was chosen for the solution.
	 *
	 * Not all indices in these arrays are actually used. Due to reduction steps some nodes might have been/removed added,
	 * but to not waste time converting indices when calculating the cost of a solution, the index in this array is the index
	 * of the node
	 */
	std::vector<unsigned short> currentSelection;
	std::vector<unsigned short> minimalSelection;
	/**
	 * Node index counter of the graph and the length of any selection arrays
	 */
	unsigned int size;
	/**
	 * For our custom gray code we need a consistent array without the gaps which the selections possibly have.
	 * This array contains all nodes in the graph and the position within this array
	 * implicitly gives the node associated with it a number in [0, sizeofGraph)
	 */
	std::vector<PBQPNode<T>*> nodes;
	/**
	 * The Vector degree of each node, subtracted by 1. Indexing is the same as in the nodes array
	 */
	std::vector<unsigned short> limits;
	/**
	 * Trend of each digit, where true means ascending and false means descending. We start with every digit ascending
	 */
	std::vector<bool> trend;

	/**
	 * Cost of the selection we last checked
	 */
	T previousCost;
	/**
	 * Minimal cost we found so far
	 */
	T minimalCost;
	unsigned short previousSelectionOfNodeLastUpdated;
	long nodeLastUpdated;

public:
	BruteForceSolver(PBQPGraph<T>* graph) :
			PBQPSolver<T>(graph), currentSelection(
					std::vector<unsigned short>(
							graph->getNodeIndexCounter(), 0)), minimalSelection(
					std::vector<unsigned short>(
							graph->getNodeIndexCounter(), 0)), size(
					graph->getNodeIndexCounter()), nodes(
					std::vector<PBQPNode<T>*>(graph->getNodeCount())), limits(
					std::vector<unsigned short>(graph->getNodeCount())), trend(
					std::vector<bool>(graph->getNodeCount(), true)) {
		unsigned long index = 0;
		//TODO Speed this up by completly copying the nodes to reduce the amount of reference lookup neccessary later on?
		for (auto iter = graph->getNodeBegin(); iter != graph->getNodeEnd();
				iter++) {
			PBQPNode<T>* node = *iter;
			nodes[index] = node;
			limits[index++] = node->getVectorDegree() - 1;
		}
	}

	~BruteForceSolver() {
	}

	void solve() override {
		minimalCost = calculateNewSolution();
		previousCost = minimalCost;
		while ((nodeLastUpdated = incrementSolution()) != -1) {
			T currValue = calculateDiffSolution();
			if (currValue < minimalCost) {
				minimalSelection = currentSelection;
				minimalCost = currValue;
			}
			previousCost = currValue;
		}
		//copy solution
		for(PBQPNode<T>* node : nodes) {
			this->solution->setSolution(node->getIndex(), minimalSelection.at(node->getIndex()));
		}
	}

private:

	/**
	 * Given the cost of the previous selection, the node which was changed and the previous selection
	 * of the node changed, this method calculates the total cost of the current selection with
	 * minimal effort
	 */
	T calculateDiffSolution() {
		T sum = T();
		PBQPNode<T>* nodeChanged = nodes[nodeLastUpdated];
		unsigned short currentNodeSelection =
				currentSelection[nodeChanged->getIndex()];
		for (PBQPEdge<T>* edge : nodeChanged->getAdjacentEdges(false)) {
			if (edge->getSource() == nodeChanged) {
				sum += edge->getMatrix().get(currentNodeSelection,
						currentSelection[edge->getTarget()->getIndex()]);
				sum -= edge->getMatrix().get(
						previousSelectionOfNodeLastUpdated,
						currentSelection[edge->getTarget()->getIndex()]);
			} else {
				sum += edge->getMatrix().get(
						currentSelection[edge->getSource()->getIndex()],
						currentNodeSelection);
				sum -= edge->getMatrix().get(
						currentSelection[edge->getSource()->getIndex()],
						previousSelectionOfNodeLastUpdated);
			}
		}
		sum += nodeChanged->getVector().get(currentNodeSelection);
		sum -= nodeChanged->getVector().get(
				previousSelectionOfNodeLastUpdated);
		return previousCost + sum;
	}

	/**
	 * Calculates the total cost for the current solution from scratch
	 */
	T calculateNewSolution() {
		T sum = T();
		auto iter = this->graph->getEdgeBegin();
		while (iter != this->graph->getEdgeEnd()) {
			PBQPEdge<T>* edge = *iter++;
			sum += edge->getMatrix().get(
					currentSelection[edge->getSource()->getIndex()],
					currentSelection[edge->getTarget()->getIndex()]);
		}
		auto nodeIter = this->graph->getNodeBegin();
		while (nodeIter != this->graph->getNodeEnd()) {
			PBQPNode<T>* node = (PBQPNode<T>*) *nodeIter++;
			sum += node->getVector().get(currentSelection[node->getIndex()]);
		}
		return sum;
	}

	/**
	 * Increments the internal gray code selection by one
	 */
	long incrementSolution() {
		for (int i = 0; i < this->graph->getNodeCount(); i++) {
			unsigned int index = (nodes[i])->getIndex();
			if (trend[i]) {
				//ascending
				if (currentSelection[index] == limits[i]) {
					//change trend, but don't change value
					trend[i] = false;
				} else {
					previousSelectionOfNodeLastUpdated =
							currentSelection [index]++;
					return i;
				}
			} else {
				//descending
				if (currentSelection[index] == 0) {
					//change trend, but don't change value
					trend[i] = true;
				} else {
					previousSelectionOfNodeLastUpdated =
							currentSelection[index]--;
					return i;
				}
			}
		}
		//only happens once we did one full cycle
		return -1;
	}
};

}

#endif /* SOLVE_BRUTEFORCESOLVER_HPP_ */
