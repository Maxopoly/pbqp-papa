#ifndef REDUCTION_NODEMERGER_HPP_
#define REDUCTION_NODEMERGER_HPP_

#include "graph/Vector.hpp"
#include "graph/Matrix.hpp"

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

template<typename T>
class NodeMerger: public PBQP_Reduction<T> {
public:
	NodeMerger(PBQPGraph<T>* graph) :
			PBQP_Reduction<T>(graph) {

	}

	~NodeMerger() {

	}

	static DependentSolution<T>* mergeNodes(PBQPNode<T>* remaining,
			PBQPNode<T>* toRemove, PBQPGraph<T>* graph) {
		Vector<T> resultVector(
				remaining->getVectorDegree() * toRemove->getVectorDegree());
		for (unsigned short int i = 0; i < remaining.getRowCount(); i++) {
			for (unsigned short int k = 0; k < toRemove.getRowCount(); k++) {
				resultVector.get(k + i * toRemove.getRowCount()) =
						remaining->getVector().get(i)
								+ toRemove->getVector().get(k);
			}
		}
		//TODO
	}

};
}

#endif /* REDUCTION_NODEMERGER_HPP_ */
