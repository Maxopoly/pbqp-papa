#ifndef GRAPH_PBQPSolution_HPP_
#define GRAPH_PBQPSolution_HPP_

#include <vector>

template<typename T>
class PBQPGraph;

template<typename T>
class PBQPSolution {
private:
	//TODO How do we represent positions that are not decided yet?
	std::vector<unsigned short int>* selection;

public:
	PBQPSolution(int length) {
		selection = new std::vector<unsigned short int>(length);
	}

	~PBQPSolution() {

	}
	double calculateKnownCost() {

	}
	bool isFinished() {

	}
	void setSolution(int nodeIndex, int solution) {
		(*selection) [nodeIndex] = solution;
	}
	int getSolution(int nodeIndex) {
		return (*selection) [nodeIndex];
	}
};

#endif /* GRAPH_PBQPSolution_HPP_ */
