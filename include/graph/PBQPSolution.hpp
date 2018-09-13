#ifndef GRAPH_PBQPSolution_HPP_
#define GRAPH_PBQPSolution_HPP_

#include <vector>

namespace pbqppapa {

template<typename T>
class PBQPGraph;

template<typename T>
class PBQPSolution {
private:
	//TODO How do we represent positions that are not decided yet?
	std::vector<unsigned short int> selection;

public:
	PBQPSolution(unsigned long int length) : selection (std::vector<unsigned short int>(length)) {
	}

	void setSolution(unsigned long int nodeIndex, unsigned short int solution) {
		selection[nodeIndex] = solution;
	}
	unsigned short int getSolution(unsigned long int nodeIndex) {
		return selection[nodeIndex];
	}
};

}

#endif /* GRAPH_PBQPSolution_HPP_ */
