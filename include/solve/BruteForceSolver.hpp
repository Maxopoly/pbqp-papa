#ifndef SOLVE_BRUTEFORCESOLVER_HPP_
#define SOLVE_BRUTEFORCESOLVER_HPP_

template<typename T>
class PBQP_Graph;
template<typename T>
class PBQP_Solution;
template<typename T>
class PBQPSolver;
template<typename T>
class PBQP_Node;

/**
 * Tries every possible combination to calculate a perfect solution
 *
 * We attempt to do so with the minimal amount of work, which we ideally do by only
 * recalculating the part of the graph that changed compared to the previous iteration.
 *
 * This leads us to a counter where every digit refers to a node in the graph. Each of these digits/nodes
 * can have a possible value within [0,vektorDegree) where vektorDegree is the length of the cost vektor
 * associated with the node. For example for 4 nodes, each with a vektor length of 3, we would count:
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
	 * The value at index [i] is the index of the solution in the cost vektor of the node with internal index i
	 *
	 * Example: selection [5] = 3
	 * means that the third element in the cost vektor of the node with index 5 was chosen for the solution.
	 *
	 * Not all indices in these arrays are actually used. Due to reduction steps some nodes might have been/removed added,
	 * but to not waste time converting indices when calculating the cost of a solution, the index in this array is the index
	 * of the node
	 */
	unsigned short int* currentSelection;
	unsigned short int* minimalSelection;
	/**
	 * Node index counter of the graph and the length of any selection arrays
	 */
	unsigned int size;

	/**
	 * Cost of the selection we last checked
	 */
	T previousCost;
	/**
	 * Minimal cost we found so far
	 */
	T minimalCost;
	unsigned int nodeLastUpdated;
	/**
	 * For our custom gray code we need a consistent array without the gaps which the selections possibly have.
	 * This array contains all nodes in the graph and the position within this array
	 * implicitly gives the node associated with it a number in [0, sizeofGraph)
	 */
	PBQP_Node<T>** nodes;
	unsigned short int* vektorDegreesPreRemapping;
	unsigned long long int totalSolutionsToCheck;

public:
	BruteForceSolver(PBQP_Graph<T>* graph, PBQP_Solution<T>* solution) :
			PBQPSolver<T>(graph, solution) {
		size = graph->getNodeIndexCounter();
		currentSelection = new unsigned short int[size];
		minimalSelection = new unsigned short int[size];
		previousSelection = new unsigned short int[size];
		for (unsigned int i = 0; i < size; i++) {
			currentSelection[i] = 0;
			minimalSelection[i] = 0;
		}
		nodes = new PBQP_Node<T>*[graph->getNodeCount()];
		vektorDegreesPreRemapping = new unsigned int*[graph->getNodeCount()];
		int index = 0;
		//TODO Speed this up by completly copying the nodes to reduce the amount of reference lookup neccessary later on?
		for (PBQP_Node<T>* node : graph->getNodes()) {
			nodes[index] = node;
			vektorDegreesPreRemapping[index] = node->getVektorDegree();
			index++;
		}
		totalSolutionsToCheck =
				(new SolutionAmountChecker<T>(graph))->getSolutionAmount();
	}

	~BruteForceSolver() {
		delete[] currentSelection;
		delete[] minimalSelection;
		delete[] nodeIndiceRemapping;
		delete [] vektorDegreesPreRemapping;
	}

	PBQP_Solution<T> solve() {
		unsigned long long int solutionsCheckedSoFar = 0;
		minimalCost = calculateNewSolution();
		previous = minimalCost;
		while (solutionsCheckedSoFar++ < totalSolutionsToCheck) {
			nodeLastUpdated = incrementSolution();
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

	T calculateDiffSolution() {
		T sum = new T();
		for (PBQP_Edge<T>* edge : graph->getEdges()) {
			sum += edge->getMatrix()->get(
					edge->getTarget()->getVektor()->get(
							currentSelection[edge->getTarget()->getIndex()]),
					edge->getSource()->getVektor()->get(
							currentSelection[edge->getSource()->getIndex()]));
		}
		sum +=
	}

	T calculateNewSolution() {
		T sum = new T();
		for (PBQP_Edge<T>* edge : graph->getEdges()) {
			sum += edge->getMatrix()->get(
					edge->getTarget()->getVektor()->get(
							currentSelection[edge->getTarget()->getIndex()]),
					edge->getSource()->getVektor()->get(
							currentSelection[edge->getSource()->getIndex()]));
		}
		for (PBQP_Node<T>* node : graph->getNodes()) {
			sum += node->getVektor()->get(currentSelection[node->getIndex()]);
		}
		return sum;
	}

	unsigned int incrementSolution() {
		unsigned int index = 0;
		memcpy(currentSelection, previousSelection,
				size * sizeof(unsigned short int));
		while (true) {
			currentSelection[nodeIndiceRemapping[index]]++;
			if (currentSelection[index] >= vektorDegreesPreRemapping[index]) {
				currentSelection[index++] = 0;
			} else {
				break;
			}
		}
	}
};

#endif /* SOLVE_BRUTEFORCESOLVER_HPP_ */
