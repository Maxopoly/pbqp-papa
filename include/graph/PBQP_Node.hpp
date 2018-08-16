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
class Vektor;

template <class T>
class PBQP_Node {
private:
	//TODO make this generic
	Vektor<T>* values;
	std::vector<PBQP_Edge<T>*> adjacentEdges;
	int index;

public:
	PBQP_Node(int index, Vektor<T>* values);
	~PBQP_Node();
	//Should only be used by PBQP_Graph
	void addEdge(PBQP_Edge<T>* edge);
	void removeEdge(PBQP_Edge*<T> edge);

	//Can be used by anyone
	std::vector<PBQP_Edge<T>*>* getAdjacentEdges(bool respectDirection);
	std::vector<PBQP_Node<T>*>* getAdjacentNodes(bool respectDirection);
	int getIndex();
	int getVektorDegree();
	int getDegree();
	Vektor<T>* getVektor();
	//needed so we can sort nodes into maps
	bool operator< (const PBQP_Node<T>& e) const;
};

#endif /* PBQPNODE_H_ */
