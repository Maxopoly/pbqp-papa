#ifndef IO_PBQP_SERIALIZER_HPP_
#define IO_PBQP_SERIALIZER_HPP_

#include <cstdio>
#include <fstream>
#include <string>
#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
#include <io/TypeSerializer.hpp>
#include <io/TypeSerializerFactory.hpp>

template<typename T>
class TypeSerializer;
template<typename T>
class PBQPGraph;

template<typename T>
class PBQP_Serializer {

private:
	TypeSerializerFactory serializerFactory;

public:

	void saveToFile(std::string path, PBQPGraph<T>* graph) {
		std::ofstream out(path);
		TypeSerializer<T>* serializer = serializerFactory.get(T());
		nlohmann::json json = graphToJson(graph, serializer);
		out << json << std::endl;
	}

	PBQPGraph<T>* loadFromFile(std::string path) {
		std::ifstream in(path);
		TypeSerializer<T>* serializer = serializerFactory.get(T());
		nlohmann::json json;
		in >> json;
		return jsonToGraph(json, serializer);
	}

private:

	PBQPGraph<T>* jsonToGraph(nlohmann::json json, TypeSerializer<T>* serializer) {
		nlohmann::json metaJson = json ["meta"];
		std::string parsedType = metaJson["type"];
		if (parsedType.compare(std::string(serializer->getIdentifier())) != 0) {
			//TODO exception
			std::cout << "Invalid type loading";
			return NULL;
		}
		PBQPGraph<T>* graph = new PBQPGraph<T>();
		std::map<unsigned int, PBQPNode<T>*> nodeByIndex = std::map<unsigned int, PBQPNode<T>*>();
		nlohmann::json nodeJson = json ["nodes"];
		for(nlohmann::json singleNodeJson : nodeJson) {
			unsigned int index = singleNodeJson ["index"];
			Vektor<T>* vek = parseVektor(singleNodeJson ["cost"], serializer);
			PBQPNode<T>* node = new PBQPNode<T>(index, vek);
			graph->addNode(node);
			nodeByIndex.insert(std::make_pair(index, node));
		}
		nlohmann::json edgeJson = json ["edges"];
		for(nlohmann::json singleEdgeJson : edgeJson) {
			Matrix<T>* mat = parseMatrix(singleEdgeJson, serializer);
			PBQPNode<T>* source = nodeByIndex.find(singleEdgeJson ["source"])->second ;
			PBQPNode<T>* target = nodeByIndex.find(singleEdgeJson ["target"])->second ;
			graph->addEdge(source, target, mat);
		}
		return graph;
	}

	Vektor<T>* parseVektor(nlohmann::json json, TypeSerializer<T>* serializer) {
		Vektor<T>* vek = new Vektor<T>((unsigned short int)json.size());
		for(unsigned short int i = 0; i < json.size(); i++) {
			vek->get(i) = serializer->deserialize(json [i]);
		}
		return vek;
	}

	Matrix<T>* parseMatrix(nlohmann::json json, TypeSerializer<T>* serializer) {
		unsigned short int rows = json ["rows"];
		unsigned short int columns = json ["columns"];
		nlohmann::json valueJson = json ["cost"];
		Matrix<T>* mat = new Matrix<T>(rows, columns);
		for(unsigned int i = 0; i < valueJson.size(); i++) {
			mat->getRaw(i) = serializer->deserialize(valueJson [i]);
		}
		return mat;
	}

	nlohmann::json graphToJson(PBQPGraph<T>* graph,
			TypeSerializer<T>* serializer) {
		nlohmann::json json;
		json["meta"] = serializeMeta(graph, serializer);
		nlohmann::json nodeJsons = nlohmann::json::array();
		for (auto iter = graph->getNodeBegin(); iter != graph->getNodeEnd();
				iter++) {
			nlohmann::json nodeJson;
			PBQPNode<T>* node = *iter;
			nodeJson ["index"] = node->getIndex();
			nlohmann::json costVector = nlohmann::json::array();
			for (unsigned short int i = 0; i < node->getVektorDegree(); i++) {
				costVector.push_back(
						serializer->serialize(node->getVektor()->get(i)));
			}
			nodeJson ["cost"] = costVector;
			nodeJsons.push_back(nodeJson);
		}
		json["nodes"] = nodeJsons;
		nlohmann::json edgeJsons = nlohmann::json::array();
		for (auto iter = graph->getEdgeBegin(); iter != graph->getEdgeEnd();
				iter++) {
			PBQPEdge<T>* edge = *iter;
			nlohmann::json edgeJson;
			edgeJson["source"] = edge->getSource()->getIndex();
			edgeJson["target"] = edge->getTarget()->getIndex();
			edgeJson["columns"] = edge->getMatrix()->getColumnCount();
			edgeJson["rows"] = edge->getMatrix()->getRowCount();
			nlohmann::json matrixValues;
			unsigned int length = edge->getMatrix()->getElementCount();
			for (unsigned int i = 0; i < length; i++) {
				matrixValues.push_back(
						serializer->serialize(edge->getMatrix()->getRaw(i)));
			}
			edgeJson["cost"] = matrixValues;
			edgeJsons.push_back(edgeJson);
		}
		json["edges"] = edgeJsons;
		return json;
	}

	nlohmann::json serializeMeta(PBQPGraph<T>* graph, TypeSerializer<T>* serializer) {
		nlohmann::json json;
		json["version"] = 1;
		json["nodeCount"] = graph->getNodeCount();
		json["edgeCount"] = graph->getEdgeCount();
		json ["type"] = serializer->getIdentifier();
		return json;
	}

};

#endif /* IO_PBQP_SERIALIZER_HPP_ */
