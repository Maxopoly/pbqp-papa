/*
 * PBQPEdge.h
 *
 *  Created on: 3 Aug 2018
 *      Author: Max
 */

#ifndef PBQP_EDGE_H_
#define PBQP_EDGE_H_

class PBQP_Node;
class Matrix;

class PBQP_Edge {

private:
	PBQP_Node* source;
	PBQP_Node* target;
	Matrix* matrix;

public:
	PBQP_Edge(PBQP_Node*, PBQP_Node*, Matrix*);
	virtual ~PBQP_Edge();
	bool isSource(PBQP_Node*);
	PBQP_Node* getOtherEnd(PBQP_Node*);
	Matrix* getMatrix();
	PBQP_Node* getSource();
	PBQP_Node* getTarget();
};

#endif /* PBQP_EDGE_H_ */
