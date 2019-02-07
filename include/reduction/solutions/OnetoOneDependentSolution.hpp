#ifndef REDUCTION_SOLUTIONS_ONETOONEDEPENDENTSOLUTION_HPP_
#define REDUCTION_SOLUTIONS_ONETOONEDEPENDENTSOLUTION_HPP_

#include <vector>
#include <assert.h>

#include "reduction/solutions/DependentSolution.hpp"

namespace pbqppapa {

template<typename T>
class PBQPSolution;
template<typename T>
class PBQPNode;
template<typename T>
class DependentSolution;
template<typename T>
class DependentGraph;

template<typename T>
class OnetoOneDependentSolution: public DependentSolution<T> {

private:
	PBQPNode<T>* toSolve;
	PBQPNode<T>* dependencyNode;
	Vector<T> dependencyVector;
	Matrix<T> edgeMatrix;
	bool dependencyIsSource;
	std::vector<unsigned short> selection;

public:
	OnetoOneDependentSolution(PBQPNode<T>* toSolve, PBQPNode<T>* dependency) :
			toSolve(toSolve), dependencyNode(dependency), dependencyVector(
					dependency->getVector()) {
		assert(toSolve->getDegree() == 1);
		selection.resize(dependency->getVectorDegree());
		PBQPEdge<T>* edge = toSolve->getAdjacentEdges().at(0);
		edgeMatrix = edge->getMatrix();
		dependencyIsSource = edge->getSource() == dependency;
	}

	~OnetoOneDependentSolution() {

	}

	void setSolutionSelection(unsigned short dependencySelection,
			unsigned short solutionSelection) {
		assert(dependencySelection < dependencyVector.getRowCount());
		assert(dependencySelection >= 0);
		assert(solutionSelection < toSolve->getVectorDegree());
		assert(solutionSelection >= 0);
		selection.at(dependencySelection) = solutionSelection;
	}

	void solve(PBQPSolution<T>* solution)  override {
		unsigned short dependencySelection = solution->getSolution(dependencyNode);
		solution->setSolution(toSolve->getIndex(), selection.at(dependencySelection));
	}

	void revertChange(PBQPGraph<T>* graph) override {
		dependencyNode->getVector() = dependencyVector;
		graph->addNode(toSolve);
		if (dependencyIsSource) {
			graph->addEdge(dependencyNode, toSolve, edgeMatrix);
		}
		else {
			graph->addEdge(toSolve, dependencyNode,  edgeMatrix);
		}
	}

	PBQPNode<T>* const getReducedNode() const override {return toSolve;}

};
}

#endif /* REDUCTION_SOLUTIONS_ONETOONEDEPENDENTSOLUTION_HPP_ */
