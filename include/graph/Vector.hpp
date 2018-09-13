#ifndef INCLUDE_GRAPH_Vector_HPP_
#define INCLUDE_GRAPH_Vector_HPP_

#include "graph/Matrix.hpp"

namespace pbqppapa {

template<class T>
class Matrix;

/**
 * A matrix with only a single column.
 */
template<class T>
class Vector: public Matrix<T> {
public:
	/**
	 * Creates a new Vector (matrix with one column) with the given amount of rows. The given data
	 * must be an array with matching length
	 */
	Vector<T>(unsigned short int length, T* data) :
			Matrix<T>(length, 1, data) {
		//super constructor does everything for us
	}

	/**
	 * Creates a new Vector with uninitialized content
	 */
	Vector<T>(unsigned short int length) :
			Matrix<T>(length, 1) {
	}

	Vector<T>(const Vector<T>& vek) :
			Matrix<T>(vek) {
	}

	Vector<T>* operator=(const Vector<T>& vek) {
		Matrix<T>::operator=(vek);
		return this;
	}

	/**
	 * Gets the index (row number) of the smallest element within the Vector.
	 * Will encounter undefined behavior if the Vector is of length 0.
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
		return this->content[index];
	}
};

}

#endif /* INCLUDE_GRAPH_Vector_HPP_ */
