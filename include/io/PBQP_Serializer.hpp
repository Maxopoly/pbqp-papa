#ifndef IO_PBQP_SERIALIZER_HPP_
#define IO_PBQP_SERIALIZER_HPP_

#include <cstdio>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
#include <iterator>
#include <map>

#include "io/TypeSerializers.hpp"
#include "graph/PBQPGraph.hpp"

namespace pbqppapa {

template<typename T>
class TypeSerializer;
template<typename T>
class PBQPGraph;
template<typename T>
class InfinityWrapper;

template<typename T>
class PBQP_Serializer {

public:

	void saveToFile(std::string path, PBQPGraph<T>* graph, bool debug = false) {
		std::ofstream out(path);
		nlohmann::json json = graphToJson(graph, debug);
		out << json << std::endl;
		out.close();
	}

	PBQPGraph<T>* loadFromFile(std::string path) const {
		std::ifstream in(path);
		nlohmann::json json;
		in >> json;
		in.close();
		return jsonToGraph(json);
	}

	std::string matrixToString(Matrix<T> matrix,
			std::string separator = " ") const {
		std::string output;
		for (unsigned short row = 0; row < matrix.getRowCount(); row++) {
			for (unsigned short column = 0; column < matrix.getColumnCount();
					column++) {
				output += serializeElement<T>(matrix.get(row, column));
				output += separator;
			}
			output += '\n';
		}
		return output;
	}

private:

	PBQPGraph<T>* jsonToGraph(nlohmann::json json) const {
		nlohmann::json metaJson = json["meta"];
		std::string parsedType = metaJson["type"];
		if (parsedType.compare(getTypeName<T>()) != 0) {
			//TODO exception
			//std::cout << "Invalid type loading";
			//return NULL;
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
		if (json.find("peo") != json.end()) {
			nlohmann::json peoJson = json["peo"];
			std::vector<PBQPNode<T>*> peoVector;
			for (unsigned int i = 0; i < peoJson.size(); i++) {
				PBQPNode<T>* peoNode = nodeByIndex.find(peoJson[i])->second;
				peoVector.push_back(peoNode);
			}
			graph->setPEO(peoVector);
		}
		return graph;
	}

	Vector<T> parseVector(nlohmann::json json) const {
		Vector<T> vek = Vector<T>((unsigned short) json.size());
		for (unsigned short i = 0; i < json.size(); i++) {
			vek.get(i) = deserializeElement<T>(json[i]);
		}
		return vek;
	}

	Matrix<T> parseMatrix(nlohmann::json json) const {
		unsigned short rows = json["rows"];
		unsigned short columns = json["columns"];
		nlohmann::json valueJson = json["cost"];
		Matrix<T> mat = Matrix<T>(rows, columns);
		for (unsigned int i = 0; i < valueJson.size(); i++) {
			mat.getRaw(i) = deserializeElement<T>(valueJson[i]);
		}
		return mat;
	}

	nlohmann::json graphToJson(PBQPGraph<T>* graph, bool debug) const {
		nlohmann::json json;
		json["meta"] = serializeMeta(graph);
		//TODO PEO
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
			if (debug) {
				nodeJson["degree"] = node->getDegree();
				nlohmann::json neighborVector = nlohmann::json::array();
				for (PBQPNode<T>* neighbor : node->getAdjacentNodes()) {
					neighborVector.push_back(
							std::to_string(neighbor->getIndex()));
				}
				nodeJson["neighbours"] = neighborVector;
			}
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
		if (graph->getPEO().size() > 0) {
			json["peo"] = serializePEO(graph->getPEO());
		}
		return json;
	}

	nlohmann::json serializePEO(const std::vector<PBQPNode<T>*>& peo) const {
		nlohmann::json peoVector = nlohmann::json::array();
		for (auto peoIter = peo.begin(); peoIter != peo.end(); ++peoIter) {
			peoVector.push_back((*peoIter)->getIndex());
		}
		return peoVector;
	}

	nlohmann::json serializeMeta(const PBQPGraph<T>* graph) const {
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
