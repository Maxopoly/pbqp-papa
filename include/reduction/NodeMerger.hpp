#ifndef REDUCTION_NODEMERGER_HPP_
#define REDUCTION_NODEMERGER_HPP_

#include "graph/Vector.hpp"
#include "graph/Matrix.hpp"
#include "graph/PBQPNode.hpp"

namespace pbqppapa {

template<typename T>
class PBQPGraph;
template<typename T>
class NtoNDependentSolution;
template<typename T>
class NodeConsistentReduction;
template<typename T>
class PBQPNode;
template<typename T>
class PBQP_Reduction;

template<typename T>
class NodeMerger: public PBQP_Reduction<T> {
public:
	NodeMerger(PBQPGraph<T>* graph) :
			PBQP_Reduction<T>(graph) {

	}

	~NodeMerger() {

	}

	static NtoNDependentSolution<T>* mergeNodes(PBQPNode<T>* remaining,
			PBQPNode<T>* toRemove, PBQPGraph<T>* graph) {
		Vector<T> resultVector(
				remaining->getVectorDegree() * toRemove->getVectorDegree());
		for (unsigned short i = 0; i < toRemove->getVectorDegree(); i++) {
			for (unsigned short k = 0; k < remaining->getVectorDegree();
					k++) {
				resultVector.get(k + i * remaining->getVectorDegree()) =
						remaining->getVector().get(k)
								+ toRemove->getVector().get(i);
			}
		}
		for (PBQPEdge<T>* edge : remaining->getAdjacentEdges()) {
			if (edge->getOtherEnd(remaining) == toRemove) {
				Matrix<T> refMat;
				if (edge->isSource(remaining)) {
					refMat = edge->getMatrix().transpose();
				} else {
					refMat = edge->getMatrix();
				}
				for (unsigned long i = 0; i < refMat.getElementCount(); i++) {
					resultVector.get(i) += refMat.getRaw(i);
				}
				graph->removeEdge(edge);
			} else {
				if (edge->isSource(remaining)) {
					edge->getMatrix() = edge->getMatrix().multiplyRows();
				} else {
					edge->getMatrix() = edge->getMatrix().multiplyColumns();
				}
			}
		}
		for (PBQPEdge<T>* edge : toRemove->getAdjacentEdges()) {
			Matrix<T> edgeMat =
					edge->isSource(toRemove) ?
							edge->getMatrix().multiplyRowsIndividually() :
							edge->getMatrix().multiplyColumnsIndividually();
			PBQPNode<T>* otherEnd = edge->getOtherEnd(toRemove);
			graph->removeEdge(edge);
			graph->addEdge(remaining, otherEnd, edgeMat);
		}
		std::vector<PBQPNode<T>*> dependencies;
		dependencies.push_back(remaining);
		std::vector<PBQPNode<T>*> solutions;
		solutions.push_back(remaining);
		solutions.push_back(toRemove);
		NtoNDependentSolution<T>* sol = new NtoNDependentSolution<T>(dependencies,
				solutions);
		for (unsigned short i = 0; i < toRemove->getVectorDegree(); i++) {
			for (unsigned short k = 0; k < remaining->getVectorDegree();
					k++) {
				std::vector<PBQPNode<T>*> dependencySelection;
				dependencySelection.push_back(
						k + i * remaining->getVectorDegree());
				std::vector<PBQPNode<T>*> solutionSelection;
				solutionSelection.push_back(k);
				solutionSelection.push_back(i);
			}
		}
		graph->removeNode(toRemove);
		return sol;
	}

};
}

#endif /* REDUCTION_NODEMERGER_HPP_ */
