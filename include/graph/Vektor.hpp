/*
 * Vektor.hpp
 *
 *  Created on: 7 Aug 2018
 *      Author: Max
 */

#ifndef GRAPH_VEKTOR_HPP_
#define GRAPH_VEKTOR_HPP_

#include "Matrix.hpp"

class Vektor : public Matrix {
public:
	Vektor(int, double*);

	int getIndexOfSmallestElement();
};

#endif /* GRAPH_VEKTOR_HPP_ */
