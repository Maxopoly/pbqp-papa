/*
 * PBQPNode.h
 *
 *  Created on: 3 Aug 2018
 *      Author: Max
 */

#ifndef PBQPNODE_H_
#define PBQPNODE_H_

#include <vector>

class PBQP_Edge;
class Matrix;

class PBQP_Node {
private:
	//TODO make this generic
	Matrix* values;
	std::vector<PBQP_Edge*> adjacentEdges;
	int index;

public:
	PBQP_Node(int index, Matrix* values);
	virtual ~PBQP_Node();
	//Should only be used by PBQP_Graph
	void addEdge(PBQP_Edge* edge);
	void removeEdge(PBQP_Edge* edge);

	//Can be used by anyone
	std::vector<int>* getValue();
	std::vector<PBQP_Edge*>* getAdjacentEdges(bool respectDirection);
	std::vector<PBQP_Node*>* getAdjacentNodes(bool respectDirection);
	int getIndex();
	int getVectorDegree();
	int getDegree();
	Matrix* getMatrix();
};

#endif /* PBQPNODE_H_ */
