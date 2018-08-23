/*
 * Vektor.hpp
 *
 *  Created on: Aug 17, 2018
 *      Author: max
 */

#ifndef INCLUDE_GRAPH_VEKTOR_HPP_
#define INCLUDE_GRAPH_VEKTOR_HPP_

#include "graph/Matrix.hpp"

template<class T>
class Matrix;

/**
 * A matrix with only a single column.
 * To avoid confusion with the commonly used std::vector, this one is spelled with a k
 */
template<class T>
class Vektor: public Matrix<T> {
public:
	/**
	 * Creates a new vektor (matrix with one column) with the given amount of rows. The given data
	 * must be an array with matching length
	 */
	Vektor<T>(unsigned short int length, T* data) :
			Matrix<T>(1, length, data) {
		//super constructor does everything for us
	}

	/**
	 * Gets the index (row number) of the smallest element within the vektor.
	 * Will encounter undefined behavior if the vektor is of length 0.
	 */
	unsigned short int getIndexOfSmallestElement() const {
		int minimumIndex = 0;
		double minimumValue = this->content[0];
		for (int i = 0; i < this->rows; i++) {
			if (this->content[i] < minimumValue) {
				minimumValue = this->content[i];
				minimumIndex = i;
			}
		}
		return minimumIndex;
	}

	T& get(unsigned short int index) const {
		return this->content [index];
	}
};

#endif /* INCLUDE_GRAPH_VEKTOR_HPP_ */
