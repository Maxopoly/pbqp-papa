#ifndef IO_PBQP_SERIALIZER_HPP_
#define IO_PBQP_SERIALIZER_HPP_

#include <cstdio>
#include <fstream>
#include <string>
#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
#include <io/TypeSerializer.hpp>

template<typename T>
class TypeSerializer;
template<typename T>
class PBQP_Graph;

template<typename T>
class PBQP_Serializer {

public:

	void saveToFile(const char* path, PBQP_Graph<T>* graph,
			TypeSerializer<T>* serializer) {
		std::ofstream out(path);
		nlohmann::json json = graphToJson(graph, serializer);
		out << json << std::endl;
	}

	void loadFromFile(char* path, TypeSerializer<T>* serializer) {

		//nlohmann::json json = jsonToGraph(graph, serializer);
	}

private:

	PBQP_Graph<T>* jsonToGraph(nlohmann::json json, TypeSerializer<T>* serializer) {
		nlohmann::json nodeJson = json ["nodes"];
	}

	nlohmann::json graphToJson(PBQP_Graph<T>* graph,
			TypeSerializer<T>* serializer) {
		nlohmann::json json;
		json["meta"] = serializeMeta(graph);
		nlohmann::json nodeJsons;
		for (auto iter = graph->getNodeBegin(); iter != graph->getNodeEnd();
				iter++) {
			nlohmann::json nodeJson;
			PBQP_Node<T>* node = *iter;
			nodeJson ["index"] = node->getIndex();
			nlohmann::json costVector;
			for (int i = 0; i < node->getVektorDegree(); i++) {
				costVector.push_back(
						serializer->serialize(node->getVektor()->get(i)));
			}
			nodeJson.push_back(costVector);
			nodeJsons.push_back(nodeJson);
		}
		json["nodes"] = nodeJsons;
		nlohmann::json edgeJsons;
		for (auto iter = graph->getEdgeBegin(); iter != graph->getEdgeEnd();
				iter++) {
			PBQP_Edge<T>* edge = *iter;
			nlohmann::json edgeJson;
			edgeJson["source"] = edge->getSource()->getIndex();
			edgeJson["target"] = edge->getTarget()->getIndex();
			edgeJson["columns"] = edge->getMatrix()->getColumnCount();
			edgeJson["rows"] = edge->getMatrix()->getRowCount();
			nlohmann::json matrixValues;
			unsigned int length = edge->getMatrix()->getElementCount();
			for (int i = 0; i < length; i++) {
				matrixValues.push_back(
						serializer->serialize(edge->getMatrix()->getRaw(i)));
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
