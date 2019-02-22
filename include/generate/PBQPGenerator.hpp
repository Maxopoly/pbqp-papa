#ifndef GENERATE_PBQPGENERATOR_HPP_
#define GENERATE_PBQPGENERATOR_HPP_

#include <stdlib.h>

#include "graph/PBQPGraph.hpp"

namespace pbqppapa {

template<typename T>
class PBQPGraph;

/**
 * Utility class for testing purposes which can generated randomized PBQP instances
 */
template<typename T>
class PBQPGenerator {

private:
	unsigned long nodeCount;
	float nodeDegree;
	int upperValueBound;
	unsigned short upperVectorLengthBound;
	unsigned short lowerVectorLengthBound;

public:

	PBQPGenerator(unsigned long nodeCount, float nodeDegree,
			int upperValueBound, unsigned short upperVectorLengthBound,
			unsigned short lowerVectorLengthBound) :
			nodeCount(nodeCount), nodeDegree(nodeDegree), upperValueBound(
					upperValueBound), upperVectorLengthBound(
					upperVectorLengthBound), lowerVectorLengthBound(
					lowerVectorLengthBound) {
		srand(time(NULL));
	}

	PBQPGenerator() :
			PBQPGenerator(20, 2.5f, 100, 6, 2) {

	}

	~PBQPGenerator() {

	}

	PBQPGraph<T>* generate() const {
		PBQPGraph<T>* graph = new PBQPGraph<T>();
		for (unsigned long i = 0; i < nodeCount; i++) {
			addRandomNode(graph);
		}
		const long edgeCount = nodeCount * static_cast<long>(nodeDegree);
		for (int i = 0; i < edgeCount; i++) {
			genRandomEdge(graph);
		}
		return graph;
	}

	void addRandomNode(PBQPGraph<T>* graph) const {
		int length = rand() % (upperVectorLengthBound - lowerVectorLengthBound);
		length += lowerVectorLengthBound;
		Vector<T> vec(length);
		for (unsigned short i = 0; i < length; i++) {
			vec.get(i) = genRandomNumber();
		}
		graph->addNode(vec);
	}

	void genRandomEdge(PBQPGraph<T>* graph) const {
		if (graph->getNodeCount() < 2) {
			return;
		}
		int firstIndex = rand() % (graph->getNodeCount());
		int secondIndex = rand() % (graph->getNodeCount());
		PBQPNode<T>* sourceNode = NULL;
		PBQPNode<T>* targetNode = NULL;
		int counter = 0;
		//TODO O(n) sucks
		for (auto iter = graph->getNodeBegin(); iter != graph->getNodeEnd();
				iter++) {
			PBQPNode<T>* node = *iter;
			if (counter == firstIndex) {
				sourceNode = node;
			}
			if (counter == secondIndex) {
				targetNode = node;
			}
			if (sourceNode != NULL && targetNode != NULL) {
				break;
			}
			counter++;
		}
		if (sourceNode == NULL || targetNode == NULL) {
			return;
		}
		Matrix<T> mat(sourceNode->getVectorDegree(),
				targetNode->getVectorDegree());
		for (unsigned short i = 0; i < sourceNode->getVectorDegree(); i++) {
			for (unsigned short k = 0; k < targetNode->getVectorDegree(); k++) {
				mat.get(i, k) = genRandomNumber();
			}
		}
		graph->addEdge(sourceNode, targetNode, mat);
	}

	T genRandomNumber() const {
		int randomNum = rand() % upperValueBound;
		return static_cast<T>(randomNum);
	}

	unsigned long& getNodeCount() const {
		return nodeCount;
	}

	float& getNodeDegree() const {
		return nodeDegree;
	}
};

}

#endif /* GENERATE_PBQPGENERATOR_HPP_ */
