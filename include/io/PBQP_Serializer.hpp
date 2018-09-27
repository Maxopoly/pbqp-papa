#ifndef IO_PBQP_SERIALIZER_HPP_
#define IO_PBQP_SERIALIZER_HPP_

#include <cstdio>
#include <fstream>
#include <string>
#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
#include <io/TypeSerializers.hpp>

namespace pbqppapa {

template<typename T>
class TypeSerializer;
template<typename T>
class PBQPGraph;

template<typename T>
class PBQP_Serializer {

public:

	void saveToFile(std::string path, PBQPGraph<T>* graph) {
		std::ofstream out(path);
		nlohmann::json json = graphToJson(graph);
		out << json << std::endl;
		out.close();
	}

	PBQPGraph<T>* loadFromFile(std::string path) {
		std::ifstream in(path);
		nlohmann::json json;
		in >> json;
		in.close();
		return jsonToGraph(json);
	}

private:

	PBQPGraph<T>* jsonToGraph(nlohmann::json json) {
		nlohmann::json metaJson = json["meta"];
		std::string parsedType = metaJson["type"];
		if (parsedType.compare(getTypeName<T>()) != 0) {
			//TODO exception
			std::cout << "Invalid type loading";
			return NULL;
		}
		PBQPGraph<T>* graph = new PBQPGraph<T>();
		std::map<unsigned int, PBQPNode<T>*> nodeByIndex = std::map<
				unsigned int, PBQPNode<T>*>();
		nlohmann::json nodeJson = json["nodes"];
		for (nlohmann::json singleNodeJson : nodeJson) {
			unsigned int index = singleNodeJson["index"];
			Vector<T> vek = parseVector(singleNodeJson["cost"]);
			PBQPNode<T>* node = new PBQPNode<T>(index, vek);
			graph->addNode(node);
			nodeByIndex.insert(std::make_pair(index, node));
		}
		nlohmann::json edgeJson = json["edges"];
		for (nlohmann::json singleEdgeJson : edgeJson) {
			Matrix<T> mat = parseMatrix(singleEdgeJson);
			PBQPNode<T>* source =
					nodeByIndex.find(singleEdgeJson["source"])->second;
			PBQPNode<T>* target =
					nodeByIndex.find(singleEdgeJson["target"])->second;
			graph->addEdge(source, target, mat);
		}
		return graph;
	}

	Vector<T> parseVector(nlohmann::json json) {
		Vector<T> vek = Vector < T > ((unsigned short) json.size());
		for (unsigned short i = 0; i < json.size(); i++) {
			vek.get(i) = deserializeElement<T>(json[i]);
		}
		return vek;
	}

	Matrix<T> parseMatrix(nlohmann::json json) {
		unsigned short rows = json["rows"];
		unsigned short columns = json["columns"];
		nlohmann::json valueJson = json["cost"];
		Matrix<T> mat = Matrix < T > (rows, columns);
		for (unsigned int i = 0; i < valueJson.size(); i++) {
			mat.getRaw(i) = deserializeElement<T>(valueJson[i]);
		}
		return mat;
	}

	nlohmann::json graphToJson(PBQPGraph<T>* graph) {
		nlohmann::json json;
		json["meta"] = serializeMeta(graph);
		nlohmann::json nodeJsons = nlohmann::json::array();
		for (auto iter = graph->getNodeBegin(); iter != graph->getNodeEnd();
				iter++) {
			nlohmann::json nodeJson;
			PBQPNode<T>* node = *iter;
			nodeJson["index"] = node->getIndex();
			nlohmann::json costVector = nlohmann::json::array();
			for (unsigned short i = 0; i < node->getVectorDegree(); i++) {
				costVector.push_back(
						serializeElement<T>(node->getVector().get(i)));
			}
			nodeJson["cost"] = costVector;
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
			edgeJson["columns"] = edge->getMatrix().getColumnCount();
			edgeJson["rows"] = edge->getMatrix().getRowCount();
			nlohmann::json matrixValues;
			unsigned int length = edge->getMatrix().getElementCount();
			for (unsigned int i = 0; i < length; i++) {
				matrixValues.push_back(
						serializeElement<T>(edge->getMatrix().getRaw(i)));
			}
			edgeJson["cost"] = matrixValues;
			edgeJsons.push_back(edgeJson);
		}
		json["edges"] = edgeJsons;
		return json;
	}

	nlohmann::json serializeMeta(PBQPGraph<T>* graph) {
		nlohmann::json json;
		json["version"] = 1;
		json["nodeCount"] = graph->getNodeCount();
		json["edgeCount"] = graph->getEdgeCount();
		json["type"] = getTypeName<T>();
		return json;
	}

};

}

#endif /* IO_PBQP_SERIALIZER_HPP_ */
