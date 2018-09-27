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
	std::vector<unsigned short> selection;

public:
	PBQPSolution(unsigned long length) : selection (std::vector<unsigned short>(length)) {
	}

	void setSolution(unsigned long nodeIndex, unsigned short solution) {
		selection.at(nodeIndex) = solution;
	}
	unsigned short getSolution(unsigned long nodeIndex) {
		return selection.at(nodeIndex);
	}
};

}

#endif /* GRAPH_PBQPSolution_HPP_ */
