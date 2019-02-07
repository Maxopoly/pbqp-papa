#ifndef MATH_GUROBICONVERTER_HPP_
#define MATH_GUROBICONVERTER_HPP_

#include <gurobi_c++.h>
#include "math/InfinityWrapper.hpp"
#include "graph/PBQPGraph.hpp"
#include "graph/PBQPSolution.hpp"
#include "graph/PBQPNode.hpp"
#include "graph/PBQPEdge.hpp"

#include <map>
#include <iostream>

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

private:
	GRBEnv env;
	GRBModel model = GRBModel(env);
	GRBVar** nodeSelections;
	GRBVar*** edgeSelections;
	unsigned short* nodeVectorLengths;
	std::map<PBQPNode<InfinityWrapper<T>>*, unsigned int> nodeToGrbVarMap;
	unsigned int nodeCount;

public:
	GurobiConverter() {

	}

	PBQPSolution<InfinityWrapper<T>>* solveGurobiLinear(
			const PBQPGraph<InfinityWrapper<T>>* graph) {
		setup(graph);
		//create variables
		createNodeSelection(graph);
		//only one selection per node
		limitSelectionPerNode();
		GRBLinExpr totalCost = 0;
		//cost at the nodes
		GRBLinExpr nodeCost = getNodeSelectionCost(graph);
		totalCost += nodeCost;
		//create edge variables
		setupEdgeSelectionLinear(graph);
		//tie edge selection to node selection
		limitEdgeSelectionLinear(graph);
		//add edge cost
		GRBLinExpr edgeCost = getEdgeSelectionLinear(graph);
		totalCost += edgeCost;
		model.setObjective(totalCost, GRB_MINIMIZE);
		model.optimize();
		//retrieve solution
		PBQPSolution<InfinityWrapper<T>>* solution = retrieveSolution(graph);
		tearDown();
		tearDownEdgesLinear(graph);
		return solution;
	}

	PBQPSolution<InfinityWrapper<T>>* solveGurobiQuadratic(
			const PBQPGraph<InfinityWrapper<T>>* graph) {
		setup(graph);
		//create variables
		createNodeSelection(graph);
		//only one selection per node
		limitSelectionPerNode();

		GRBQuadExpr totalCost = 0;
		//cost at the nodes
		GRBLinExpr nodeCost = getNodeSelectionCost(graph);
		totalCost += nodeCost;
		//cost at the edges
		GRBQuadExpr edgeCost = getEdgeSelectionCostQuadratic(graph);
		totalCost += edgeCost;
		model.setObjective(totalCost, GRB_MINIMIZE);
		model.optimize();

		//retrieve solution
		PBQPSolution<InfinityWrapper<T>>* solution = retrieveSolution(graph);
		tearDown();

		return solution;
	}

private:

	void setup(const PBQPGraph<InfinityWrapper<T>>* graph) {
		nodeCount = graph->getNodeCount();
		nodeSelections = new GRBVar*[nodeCount];
		nodeVectorLengths = new unsigned short[nodeCount];
		nodeToGrbVarMap.clear();
	}

	void tearDown() {
		for (unsigned int i = 0; i < nodeCount; ++i) {
			delete[] nodeSelections[i];
		}
		delete[] nodeVectorLengths;
		nodeToGrbVarMap.clear();
	}

	void tearDownEdgesLinear(const PBQPGraph<InfinityWrapper<T>>* graph) {
		unsigned const int edgeCount = graph->getEdgeCount();
		unsigned int counter = 0;
		for (auto iter = graph->getEdgeBegin(); iter != graph->getEdgeEnd();
				++iter) {
			PBQPEdge<InfinityWrapper<T>>* edge = *iter;
			unsigned const short rowCount = edge->getMatrix().getRowCount();
			for (unsigned short row = 0; row < rowCount; ++row) {
				delete[] edgeSelections[counter][row];
			}
			delete[] edgeSelections[counter];
			counter++;
		}
		delete[] edgeSelections;
	}

	void createNodeSelection(const PBQPGraph<InfinityWrapper<T>>* graph) {
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
	}

	void limitSelectionPerNode() {
		//Only select exactly one solution per node
		for (unsigned int i = 0; i < nodeCount; ++i) {
			GRBLinExpr expr = 0;
			for (int k = 0; k < nodeVectorLengths[i]; ++k) {
				expr += nodeSelections[i][k];
			}
			model.addConstr(expr, GRB_EQUAL, 1.0);
		}
	}

	GRBLinExpr getNodeSelectionCost(
			const PBQPGraph<InfinityWrapper<T>>* graph) {
		unsigned int counter = 0;
		GRBLinExpr totalCost = 0;
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
		return totalCost;
	}

	GRBQuadExpr getEdgeSelectionCostQuadratic(
			const PBQPGraph<InfinityWrapper<T>>* graph) {
		GRBQuadExpr totalCost = 0;
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
					if (value.isInfinite()) {
						//disallow infinite values in matrix by forcing the sum of the selections to be 0
						GRBQuadExpr expr = nodeSelections[sourceGrbIndex][row]
								* nodeSelections[targetGrbIndex][col];
						model.addQConstr(expr, GRB_EQUAL, 0.0);
					} else {
						//add specific selection for this edge to the total cost
						totalCost += value.getValue()
								* nodeSelections[sourceGrbIndex][row]
								* nodeSelections[targetGrbIndex][col];
					}
				}
			}
		}
		return totalCost;
	}

	PBQPSolution<InfinityWrapper<T>>* retrieveSolution(
			const PBQPGraph<InfinityWrapper<T>>* graph) {
		PBQPSolution<InfinityWrapper<T>>* solution = new PBQPSolution<
				InfinityWrapper<T>>(graph->getNodeIndexCounter());
		for (auto iter = graph->getNodeBegin(); iter != graph->getNodeEnd();
				++iter) {
			PBQPNode<InfinityWrapper<T>>* node = *iter;
			unsigned int nodeIndex = nodeToGrbVarMap.find(node)->second;
			for (int i = 0; i < nodeVectorLengths[nodeIndex]; i++) {
				if (nodeSelections[nodeIndex][i].get(GRB_DoubleAttr_X) == 1.0) {
					solution->setSolution(node->getIndex(), i);
				}
			}
		}
		return solution;
	}

	void setupEdgeSelectionLinear(const PBQPGraph<InfinityWrapper<T>>* graph) {
		unsigned const int edgeCount = graph->getEdgeCount();
		edgeSelections = new GRBVar**[edgeCount];
		unsigned int counter = 0;
		for (auto iter = graph->getEdgeBegin(); iter != graph->getEdgeEnd();
				++iter) {
			PBQPEdge<InfinityWrapper<T>>* edge = *iter;
			unsigned const short rowCount = edge->getMatrix().getRowCount();
			unsigned const short columnCount =
					edge->getMatrix().getColumnCount();
			edgeSelections[counter] = new GRBVar*[rowCount];
			for (unsigned short row = 0; row < rowCount; ++row) {
				edgeSelections[counter][row] = new GRBVar[columnCount];
				for (unsigned short column = 0; column < columnCount;
						++column) {
					edgeSelections[counter][row][column] = model.addVar(0.0,
							1.0, 0.0, GRB_BINARY);
				}
			}
			++counter;
		}
	}

	GRBLinExpr getEdgeSelectionLinear(
			const PBQPGraph<InfinityWrapper<T>>* graph) {
		unsigned const int edgeCount = graph->getEdgeCount();
		GRBLinExpr totalCost = 0;
		unsigned int counter = 0;
		for (auto iter = graph->getEdgeBegin(); iter != graph->getEdgeEnd();
				++iter) {
			PBQPEdge<InfinityWrapper<T>>* edge = *iter;
			unsigned const short rowCount = edge->getMatrix().getRowCount();
			unsigned const short columnCount =
					edge->getMatrix().getColumnCount();
			for (unsigned short row = 0; row < rowCount; ++row) {
				for (unsigned short column = 0; column < columnCount;
						++column) {
					InfinityWrapper<T> selectionCost = edge->getMatrix().get(
							row, column);
					if (selectionCost.isInfinite()) {
						GRBLinExpr tempSum = 0;
						tempSum += edgeSelections[counter][row][column];
						model.addConstr(tempSum, GRB_EQUAL, 0.0);
					} else {
						totalCost += edgeSelections[counter][row][column]
								* selectionCost.getValue();
					}
				}
			}
			++counter;
		}
		return totalCost;
	}

	void limitEdgeSelectionLinear(const PBQPGraph<InfinityWrapper<T>>* graph) {
		unsigned const int edgeCount = graph->getEdgeCount();
		unsigned int counter = 0;
		for (auto iter = graph->getEdgeBegin(); iter != graph->getEdgeEnd();
				++iter) {
			PBQPEdge<InfinityWrapper<T>>* edge = *iter;
			PBQPNode<InfinityWrapper<T>>* source = edge->getSource();
			PBQPNode<InfinityWrapper<T>>* target = edge->getTarget();
			unsigned int sourceGrbIndex = nodeToGrbVarMap.find(source)->second;
			unsigned int targetGrbIndex = nodeToGrbVarMap.find(target)->second;
			unsigned const short rowCount = edge->getMatrix().getRowCount();
			unsigned const short columnCount =
					edge->getMatrix().getColumnCount();
			//sum of row selections equals selection in that row in the source node
			for (unsigned short row = 0; row < rowCount; ++row) {
				GRBLinExpr cost = 0;
				for (unsigned short column = 0; column < columnCount;
						++column) {
					cost += edgeSelections[counter][row][column];
				}
				cost -= nodeSelections[sourceGrbIndex][row];
				model.addConstr(cost, GRB_EQUAL, 0.0);
			}
			//sum of column selections equals selection in that row in the target node
			for (unsigned short column = 0; column < columnCount; ++column) {
				GRBLinExpr cost = 0;
				for (unsigned short row = 0; row < rowCount; ++row) {
					cost += edgeSelections[counter][row][column];
				}
				cost -= nodeSelections[targetGrbIndex][column];
				model.addConstr(cost, GRB_EQUAL, 0.0);
			}
			++counter;
		}
	}

};

}

#endif /* MATH_GUROBICONVERTER_HPP_ */
