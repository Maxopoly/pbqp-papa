#ifndef GRAPH_PBQP_SOLUTION_HPP_
#define GRAPH_PBQP_SOLUTION_HPP_

#include <vector>

template<typename T>
class PBQP_Graph;

template<typename T>
class PBQP_Solution {
private:
	//TODO How do we represent positions that are not decided yet?
	std::vector<int>* selection;

public:
	PBQP_Solution(int length) {
		selection = new std::vector<int>(length);
	}

	virtual ~PBQP_Solution();
	double calculateKnownCost() = 0;  //TODO
	bool isFinished() = 0; //TODO
	void setSolution(int nodeIndex, int solution) {
		(*selection) [nodeIndex] = solution;
	}
	int getSolution(int nodeIndex) {
		return (*selection) [nodeIndex];
	}
};

#endif /* GRAPH_PBQP_SOLUTION_HPP_ */
