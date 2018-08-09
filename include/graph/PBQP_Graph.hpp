/*
 * PBQPGraph.h
 *
 *  Created on: 3 Aug 2018
 *      Author: Max
 */

#ifndef PBQPGRAPH_H_
#define PBQPGRAPH_H_

#include <vector>

class PBQP_Edge;
class PBQP_Node;
class Matrix;
class Vektor;

class PBQP_Graph {
private:
	int nodeIndexCounter;
	std::vector<PBQP_Node*>* nodes;
	std::vector<PBQP_Edge*>* edges;
	//TODO Both of these should be maps instead of lists
	void internalEdgeCleanUp(PBQP_Edge*);

public:
	PBQP_Graph();
	virtual ~PBQP_Graph();
	PBQP_Node* addNode(Vektor*);
	void addEdge(PBQP_Node*, PBQP_Node*, Matrix*);
	void removeNode(PBQP_Node*);
	void removeEdge(PBQP_Edge*);
	std::vector<PBQP_Node*>* getNodes();
	std::vector<PBQP_Edge*>* getEdges();
	int getNodeCount();
	int getEdgeCount();

};

#endif /* PBQPGRAPH_H_ */
