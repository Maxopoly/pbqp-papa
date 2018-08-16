/*
 * Vektor.hpp
 *
 *  Created on: 7 Aug 2018
 *      Author: Max
 */

#ifndef GRAPH_VEKTOR_HPP_
#define GRAPH_VEKTOR_HPP_

#include "Matrix.hpp"

template <class T>
class Vektor : public Matrix <T> {
public:
	Vektor<T>(int, T*);

	int getIndexOfSmallestElement();
};

#endif /* GRAPH_VEKTOR_HPP_ */
