#ifndef SOLVE_BRUTEFORCESOLVER_HPP_
#define SOLVE_BRUTEFORCESOLVER_HPP_

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
class BruteForceSolver: PBQPSolver<T> {

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
	unsigned short int currentSelection[];
	unsigned short int minimalSelection[];
	/**
	 * Node index counter of the graph and the length of any selection arrays
	 */
	unsigned int size;
	/**
	 * For our custom gray code we need a consistent array without the gaps which the selections possibly have.
	 * This array contains all nodes in the graph and the position within this array
	 * implicitly gives the node associated with it a number in [0, sizeofGraph)
	 */
	PBQPNode<T>* nodes[];
	/**
	 * The Vector degree of each node, subtracted by 1. Indexing is the same as in the nodes array
	 */
	unsigned short int limits[];
	/**
	 * Trend of each digit, where true means ascending and false means descending. We start with every digit ascending
	 */
	bool trend[];

	/**
	 * Cost of the selection we last checked
	 */
	T previousCost;
	/**
	 * Minimal cost we found so far
	 */
	T minimalCost;
	unsigned short int previousSelectionOfNodeLastUpdated;
	unsigned int nodeLastUpdated;

public:
	BruteForceSolver(PBQPGraph<T>* graph, PBQPSolution<T>* solution) :
			PBQPSolver<T>(graph, solution), currentSelection(
					new unsigned short int[graph->getNodeIndexCounter()]), minimalSelection(
					new unsigned short int[graph->getNodeIndexCounter()]), size(
					graph->getNodeIndexCounter()), nodes(
					new PBQPNode<T>*[graph->getNodeCount()]), limits(
					new unsigned int[graph->getNodeCount()]), trend(
					new bool[graph->getNodeCount()]) {
		//init both solution and starting point to all 0
		memset(&currentSelection, 0, size * sizeof(unsigned short int));
		memset(&minimalSelection, 0, size * sizeof(unsigned short int));
		int index = 0;
		//TODO Speed this up by completly copying the nodes to reduce the amount of reference lookup neccessary later on?
		for (auto iter = graph->getNodeBegin(); iter != graph->getNodeEnd();
				iter++) {
			PBQPNode<T>* node = *iter;
			nodes[index] = node;
			limits[index++] = node->getVectorDegree() - 1;
		}
		memset(&trend, 1, size * sizeof(bool));
	}

	~BruteForceSolver() {
	}

	PBQPSolution<T> solve() {
		unsigned long long int solutionsCheckedSoFar = 0;
		minimalCost = calculateNewSolution();
		previous = minimalCost;
		while ((nodeLastUpdated = incrementSolution()) != -1) {
			T currValue = calculateDiffSolution();
			if (currValue < minimalCost) {
				memcpy(currentSelection, minimalSelection,
						size * sizeof(unsigned short int));
				minimalCost = currValue;
			}
			previousCost = currValue;
		}
		return solution;
	}

private:

	/**
	 * Given the cost of the previous selection, the node which was changed and the previous selection of the node changed,
	 * this method calculates the total cost of the current selection with minimal effort
	 */
	T calculateDiffSolution() {
		T sum = *new T();
		PBQPNode<T>* nodeChanged = nodes[nodeLastUpdated];
		unsigned short int currentNodeSelection =
				currentSelection[nodeChanged->getIndex()];
		unsigned short int previousNodeSelection = currentNodeSelection - 1;
		if (previousNodeSelection < 0) {
			previousNodeSelection = nodeChanged->getVectorDegree() - 1;
		}
		for (PBQPEdge<T>* edge : nodeChanged->getAdjacentEdges(false)) {
			sum += edge->getMatrix()->get(
					currentSelection[edge->getSource()->getIndex()],
					currentSelection[edge->getTarget()->getIndex()]);
			if (edge->getSource() == nodeChanged) {
				sum += edge->getMatrix()->get(currentNodeSelection,
						currentSelection[edge->getTarget()->getIndex()]);
				sum -= edge->getMatrix()->get(
						previous[edge->getSource()->getIndex()],
						currentSelection[edge->getTarget()->getIndex()]);
			} else {
				sum += edge->getMatrix()->get(
						currentSelection[edge->getSource()->getIndex()],
						currentNodeSelection);
				sum -= edge->getMatrix()->get(
						currentSelection[edge->getSource()->getIndex()],
						previousNodeSelection);
			}
		}
		sum += nodeChanged->getVector()->get(currentNodeSelection);
		sum -= nodeChanged->getVector()->get(previousNodeSelection);
		return previousCost + sum;
	}

	/**
	 * Calculates the total cost for the current solution from scratch
	 */
	T calculateNewSolution() {
		T sum = new T();
		for (PBQPEdge<T>* edge : graph->getEdges()) {
			sum += edge->getMatrix()->get(
					currentSelection[edge->getSource()->getIndex()],
					currentSelection[edge->getTarget()->getIndex()]);
		}
		for (PBQPNode<T>* node : graph->getNodes()) {
			sum += node->getVector()->get(currentSelection[node->getIndex()]);
		}
		return sum;
	}

	/**
	 * Increments the internal gray code selection by one
	 */
	unsigned int incrementSolution() {
		for (int i = 0; i < graph->getNodeCount(); i++) {
			unsigned int index = (nodes[i])->getIndex();
			if (trend[i]) {
				//ascending
				if (currentSelection[index] == limits[i]) {
					//change trend, but don't change value
					trend[i] = false;
				} else {
					previousSelectionOfNodeLastUpdated =
							currentSelection[index]++;
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
