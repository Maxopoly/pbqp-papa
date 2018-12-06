#ifndef REDUCTION_SOLUTIONS_DEPENDENTSOLUTION_HPP_
#define REDUCTION_SOLUTIONS_DEPENDENTSOLUTION_HPP_

namespace pbqppapa {

template<typename T>
class PBQPSolution;
template<typename T>
class PBQPNode;
template<typename T>
class PBQPGraph;

template<typename T>
class DependentSolution {

public:

	virtual ~DependentSolution() {};


	virtual void solve(PBQPSolution<T>* solution) {}

	virtual void revertChange(PBQPGraph<T>* graph) {}

	virtual PBQPNode<T>* const getReducedNode() const {return 0;}
};

}



#endif /* REDUCTION_SOLUTIONS_DEPENDENTSOLUTION_HPP_ */
