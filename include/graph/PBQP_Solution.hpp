#ifndef GRAPH_PBQP_SOLUTION_HPP_
#define GRAPH_PBQP_SOLUTION_HPP_

#include <vector>

template<typename T>
class PBQP_Graph;

template<typename T>
class PBQP_Solution {
private:
	//TODO How do we represent positions that are not decided yet?
	std::vector<unsigned short int>* selection;

public:
	PBQP_Solution(int length) {
		selection = new std::vector<unsigned short int>(length);
	}

	~PBQP_Solution() {

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

#endif /* GRAPH_PBQP_SOLUTION_HPP_ */
