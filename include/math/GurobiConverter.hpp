#ifndef MATH_GUROBICONVERTER_HPP_
#define MATH_GUROBICONVERTER_HPP_

#include <gurobi_c++.h>
#include "math/InfinityWrapper.hpp"
#include "graph/PBQPGraph.hpp"
#include "graph/PBQPSolution.hpp"
#include "graph/PBQPNode.hpp"
#include "graph/PBQPEdge.hpp"

#include <map>

namespace pbqppapa {

template<typename T>
class PBQPGraph;
template<typename T>
class PBQPNode;
template<typename T>
class PBQPEdge;
template<typename T>
class InfinityWrapper;
template<typename T>
class Vector;
template<typename T>
class PBQPSolution;

template<typename T>
class GurobiConverter {

	GRBEnv env;
	GRBModel model = GRBModel(env);
	GRBVar** nodeSelections;
	unsigned short* nodeVectorLengths;

public:
	GurobiConverter() {

	}

	PBQPSolution<InfinityWrapper<T>>* convert(PBQPGraph<InfinityWrapper<T>>* graph) {
		std::map<PBQPNode<InfinityWrapper<T>>*, unsigned int> nodeToGrbVarMap;
		const unsigned int nodeCount = graph->getNodeCount();
		nodeSelections = new GRBVar*[nodeCount];
		nodeVectorLengths = new unsigned short[nodeCount];

		//create array of selections. Every contained array represents a node and the variables
		//in it represent the possible selections
		unsigned int counter = 0;
		for (auto iter = graph->getNodeBegin(); iter != graph->getNodeEnd();
				++iter) {
			PBQPNode<InfinityWrapper<T>>* node = *iter;
			nodeToGrbVarMap.insert(
					std::pair<PBQPNode<InfinityWrapper<T>>*, unsigned int>(node,
							counter));
			const unsigned short vecLength = node->getVectorDegree();
			nodeSelections[counter] = new GRBVar[vecLength];
			nodeVectorLengths[counter] = vecLength;
			for (int i = 0; i < vecLength; i++) {
				nodeSelections[counter][i] = model.addVar(0.0, 1.0, 0.0,
						GRB_BINARY);
			}
			counter++;
		}

		//Constraints

		//Only select exactly one solution per node
		for (unsigned int i = 0; i < nodeCount; ++i) {
			GRBLinExpr expr = 0;
			for (int k = 0; k < nodeVectorLengths[i]; ++k) {
				expr += nodeSelections[i][k];
			}
			model.addConstr(expr, GRB_EQUAL, 1.0);
		}

		//Add the node cost vectors to the goal to minimize

		GRBQuadExpr totalCost = 0;

		counter = 0;
		for (auto iter = graph->getNodeBegin(); iter != graph->getNodeEnd();
				++iter) {
			PBQPNode<InfinityWrapper<T>>* node = *iter;
			for (int i = 0; i < nodeVectorLengths[counter]; i++) {
				InfinityWrapper<T> value = node->getVector().get(i);
				if (value.isInfinite()) {
					GRBLinExpr expr = 0;
					//disallow infinite values by forcing its selection to be 0
					expr += nodeSelections[counter][i];
					model.addConstr(expr, GRB_EQUAL, 0.0);
				} else {
					// adding specific selection of a cost vector to the total cost
					totalCost += value.getValue() * nodeSelections[counter][i];
				}
			}
			counter++;
		}

		//Add the edge costs to the total goal
		for (auto iter = graph->getEdgeBegin(); iter != graph->getEdgeEnd();
				++iter) {
			PBQPEdge<InfinityWrapper<T>>* edge = *iter;
			PBQPNode<InfinityWrapper<T>>* source = edge->getSource();
			PBQPNode<InfinityWrapper<T>>* target = edge->getTarget();
			unsigned int sourceGrbIndex = nodeToGrbVarMap.find(source)->second;
			unsigned int targetGrbIndex = nodeToGrbVarMap.find(target)->second;
			for (unsigned short row = 0; row < edge->getMatrix().getRowCount();
					++row) {
				for (unsigned short col = 0;
						col < edge->getMatrix().getColumnCount(); ++col) {
					InfinityWrapper<T> value = edge->getMatrix().get(row, col);
					/*if (value.isInfinite()) {
						//disallow infinite values in matrix by forcing the sum of the selections to be 0
						GRBLinExpr expr = 0;
						expr += nodeSelections[sourceGrbIndex][row];
						expr += nodeSelections[targetGrbIndex][col];
						model.addConstr(expr, GRB_EQUAL, 0.0);
					} else { */
						//add specific selection for this edge to the total cost
						totalCost += value.getValue()
								* nodeSelections[sourceGrbIndex][row]
								* nodeSelections[targetGrbIndex][col];
					//}
				}
			}

		}
		model.setObjective(totalCost, GRB_MINIMIZE);
		model.optimize();

		//retrieve solution
		PBQPSolution<InfinityWrapper<T>>* solution = new PBQPSolution<InfinityWrapper<T>> (graph->getNodeIndexCounter());
		for (auto iter = graph->getNodeBegin(); iter != graph->getNodeEnd();
						++iter) {
			PBQPNode<InfinityWrapper<T>>* node = *iter;
			unsigned int nodeIndex = nodeToGrbVarMap.find(node)->second;
			for (int i = 0; i < nodeVectorLengths[nodeIndex]; i++) {
				if (nodeSelections[nodeIndex] [i].get(GRB_DoubleAttr_X) == 1.0) {
					solution->setSolution(nodeIndex, i);
				}
			}
		}


		//free memory
		for(unsigned int i = 0; i < nodeCount; ++i) {
			delete [] nodeSelections [i];
		}
		delete [] nodeVectorLengths;

		return solution;
	}

};

}

#endif /* MATH_GUROBICONVERTER_HPP_ */
