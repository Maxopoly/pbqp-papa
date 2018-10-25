#ifndef REDUCTION_SOLUTIONS_TWOTOONEDEPENDENTSOLUTION_HPP_
#define REDUCTION_SOLUTIONS_TWOTOONEDEPENDENTSOLUTION_HPP_

#include <vector>
#include <optional>

#include "reduction/solutions/DependentSolution.hpp"
#include "math/InfinityWrapper.hpp"

namespace pbqppapa {

template<typename T>
class PBQPSolution;
template<typename T>
class PBQPNode;
template<typename T>
class PBQPEdge;
template<typename T>
class DependentSolution;
template<typename T>
class DependentGraph;

template<typename T>
class TwotoOneDependentSolution: public DependentSolution<T> {

private:
	PBQPNode<T>* toSolve;
	PBQPNode<T>* dependencyNode1;
	PBQPNode<T>* dependencyNode2;
	Vector<T> firstDependencyVector;
	Vector<T> secondDependencyVector;
	PBQPEdge<T> firstEdge;
	PBQPEdge<T> secondEdge;
	PBQPEdge<T> originalDep1dep2Edge;
	bool originalEdgeExisted = false;
	std::vector<unsigned short> selection;

public:
	TwotoOneDependentSolution(PBQPNode<T>* toSolve,
			PBQPNode<T>* dependencyNode1, PBQPNode<T>* dependencyNode2) :
			toSolve(toSolve), dependencyNode1(dependencyNode1), dependencyNode2(
					dependencyNode2), firstDependencyVector(
					dependencyNode1->getVector(),
					secondDependencyVector(dependencyNode2->getVector()),
					selection(
							dependencyNode1->getVectorDegree()
									* dependencyNode2->getVectorDegree())) {
		assert(toSolve->getDegree() == 2);
		std::vector<PBQPEdge<T>*> edges = toSolve->getAdjacentEdges();
		if (edges.at(0).getOtherEnd(toSolve) == dependencyNode1) {
			firstEdge = &(edges.at(0));
			secondEdge = &(edges.at(1));
		} else {
			firstEdge = &(edges.at(1));
			secondEdge = &(edges.at(0));
		}
		for (PBQPEdge<T>* edge : dependencyNode1->getAdjacentEdges()) {
			if (edge->getOtherEnd(dependencyNode1) == dependencyNode2) {
				originalDep1dep2Edge = &edge;
				originalEdgeExisted = true;
				break;
			}
		}
	}

	~TwotoOneDependentSolution() {

	}

	void setSolutionSelection(unsigned short dependency1Selection,
			unsigned short dependency2Selection,
			unsigned short solutionSelection) {
		assert(dependency1Selection < dependencyNode1->getVectorDegree());
		assert(dependency1Selection >= 0);
		assert(dependency2Selection < dependencyNode2->getVectorDegree());
		assert(dependency2Selection >= 0);
		assert(solutionSelection < toSolve->getVectorDegree());
		assert(solutionSelection >= 0);
		selection.at(resolveIndex(dependency1Selection, dependency2Selection)) =
				solutionSelection;
	}

	void solve(PBQPSolution<T>* solution) const {
		unsigned short dependency1Selection = solution->getSolution(
				dependencyNode1->getIndex());
		assert(dependency1Selection < dependencyNode1->getVectorDegree());
		assert(dependency1Selection >= 0);
		unsigned short dependency2Selection = solution->getSolution(
				dependencyNode1->getIndex());
		assert(dependency2Selection < dependencyNode2->getVectorDegree());
		assert(dependency2Selection >= 0);
		unsigned long index = resolveIndex(dependency1Selection,
				dependency2Selection);
		unsigned short toSolveSelection = selection.at(index);
		solution->setSolution(toSolve->getIndex(),
				selection.at(toSolveSelection));
	}

	void revertChange(PBQPGraph<T>* graph) const {
		assert(!dependencyNode1->isDeleted());
		assert(!dependencyNode2->isDeleted());
		assert(toSolve->isDeleted());
		dependencyNode1->getVector() = firstDependencyVector;
		dependencyNode2->getVector() = secondDependencyVector;
		graph->addNode(toSolve);
		graph->addEdge(firstEdge.getSource(), firstEdge.getTarget(),
				firstEdge.getMatrix());
		graph->addEdge(secondEdge.getSource(), secondEdge.getTarget(),
				secondEdge.getMatrix());
		if (originalEdgeExisted) {
			for (PBQPEdge<T> edge : dependencyNode1->getAdjacentEdges()) {
				if (edge.getOtherEnd(dependencyNode1) == dependencyNode2) {
					edge.getMatrix() = originalDep1dep2Edge.matrix;
				}
			}
		}
	}

private:
	unsigned long inline resolveIndex(unsigned short first,
			unsigned short second) {
		return first * dependencyNode1->getVectorDegree() + second;
	}

};
}

#endif /* REDUCTION_SOLUTIONS_TWOTOONEDEPENDENTSOLUTION_HPP_ */
