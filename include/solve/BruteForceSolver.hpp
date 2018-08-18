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
 */
template<typename T>
class BruteForceSolver: PBQPSolver<T> {

private:
	const unsigned int recalcThreshhold = 2;
	unsigned short int* previousSelection;
	unsigned short int* currentSelection;
	unsigned short int* minimalSelection;
	unsigned int size;
	T previousCost;
	T minimalCost;
	unsigned int* nodeIndiceRemapping;
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
		nodeIndiceRemapping = new unsigned int*[graph->getNodeCount()];
		vektorDegreesPreRemapping = new unsigned int*[graph->getNodeCount()];
		int index = 0;
		//TODO Speed this up by completly copying the nodes to reduce the amount of reference lookup neccessary later on?
		for (PBQP_Node<T>* node : graph->getNodes()) {
			nodes[index] = node->getIndex();
			vektorDegreesPreRemapping [index] = node->getVektorDegree();
			index++;
		}
		totalSolutionsToCheck =
				(new SolutionAmountChecker<T>(graph))->getSolutionAmount();
	}

	~BruteForceSolver() {
		delete [] previousSelection;
		delete [] currentSelection;
		delete [] minimalSelection;
		delete [] nodeIndiceRemapping;
	}

	PBQP_Solution<T> solve() {
		unsigned long long int solutionsCheckedSoFar = 0;
		minimalCost = calculateNewSolution();
		previous = minimalCost;
		while (solutionsCheckedSoFar++ < totalSolutionsToCheck) {
			unsigned int nodesChanged = incrementSolution();
			T currValue;
			if (size / nodesChanged > recalcThreshhold) {
				currValue = calculateNewSolution();
			}
			else {
				currValue = calculateDiffSolution();
			}
			if (currValue < minimalCost) {
				memcpy(currentSelection, minimalSelection, size * sizeof(unsigned short int));
				minimalCost = currValue;
			}
			previousCost = currValue;
		}
		return solution;
	}

private:

	T calculateDiffSolution() {
		//TODO
		return new T();
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
		for(PBQP_Node<T>* node : graph->getNodes()) {
			sum += node->getVektor()->get(currentSelection[node->getIndex()]);
		}
		return sum;
	}

	unsigned int incrementSolution() {
		unsigned int index = 0;
		memcpy(currentSelection, previousSelection, size * sizeof(unsigned short int));
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
