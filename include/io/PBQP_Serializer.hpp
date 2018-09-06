#ifndef IO_PBQP_SERIALIZER_HPP_
#define IO_PBQP_SERIALIZER_HPP_

#include <stdio.h>
#include <string>
#include <nlohmann/json.hpp>

template<typename T>
class TypeSerializer;
template<typename T>
class PBQP_Graph;

template<typename T>
class PBQP_Serializer {

public:

	void saveToFile(char* path, PBQP_Graph<T>* graph,
			TypeSerializer<T> serializer) {
		FILE* file = fopen(path, "wb");
		nlohmann::json json = graphToJson(graph, serializer);
	}

private:

	nlohmann::json graphToJson(PBQP_Graph<T>* graph,
			TypeSerializer<T> serializer) {
		nlohmann::json json;
		json["meta"] = serializeMeta(graph);
		nlohmann::json costVectors;
		for (auto iter = graph->getNodeBegin(); iter != graph->getNodeEnd();
				iter++) {
			PBQP_Node<T>* node = *iter;
			nlohmann::json costVector;
			for (int i = 0; i < node->getVektorDegree(); i++) {
				costVector.push_back(
						serializer.serialize(node->getVektor()->get(i)));
			}
			costVectors.push_back(costVector);
		}
		json["nodeCosts"] = costVectors;
		nlohmann::json edgeJsons;
		for (auto iter = graph->getEdgeBegin(); iter != graph->getEdgeEnd();
				iter++) {
			PBQP_Edge<T>* edge = *iter;
			nlohmann::json edgeJson;
			edgeJson["columns"] = edge->getMatrix()->getColumnCount();
			edgeJson["rows"] = edge->getMatrix()->getRowCount();
			nlohmann::json matrixValues;
			unsigned int length = edge->getMatrix()->getElementCount();
			for (int i = 0; i < length; i++) {
				matrixValues.push_back(
						serializer.serialize(edge->getMatrix()->getRaw(i)));
			}
			edgeJson["matrix"] = matrixValues;
			edgeJsons.push_back(edgeJson);
		}
		json["edges"] = edgeJsons;
		return json;
	}

	nlohmann::json serializeMeta(PBQP_Graph<T>* graph) {
		nlohmann::json json;
		json["version"] = 1;
		json["nodeCount"] = graph->getNodeCount();
		json["edgeCount"] = graph->getEdgeCount();
		return json;
	}

};

#endif /* IO_PBQP_SERIALIZER_HPP_ */
