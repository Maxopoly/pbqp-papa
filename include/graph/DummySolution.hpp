#ifndef GRAPH_DUMMYSOLUTION_HPP_
#define GRAPH_DUMMYSOLUTION_HPP_

template<typename T>
class PBQPSolution;


template<typename T>
class DummySolution : public PBQPSolution<T>{

	T totalCost;

public:

	DummySolution(T cost) : PBQPSolution<T>(0), totalCost(cost) {
	}

	T getTotalCost(const PBQPGraph<T>* graph) override {
		return totalCost;
	}

};



#endif /* GRAPH_DUMMYSOLUTION_HPP_ */
