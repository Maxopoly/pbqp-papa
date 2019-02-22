#ifndef INCLUDE_GRAPH_Vector_HPP_
#define INCLUDE_GRAPH_Vector_HPP_

#include "graph/Matrix.hpp"

namespace pbqppapa {

template<typename T>
class InfinityWrapper;

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
	Vector<T>(unsigned short length, T* data) :
			Matrix<T>(length, 1, data) {
		//super constructor does everything for us
	}

	Vector<T>() : Matrix<T>() {}

	/**
	 * Creates a new Vector with uninitialized content
	 */
	Vector<T>(unsigned short length) :
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
	unsigned short getIndexOfSmallestElement() const {
		unsigned short minimumIndex = 0;
		T minimumValue = this->content[0];
		for (unsigned short i = 0; i < this->rows; i++) {
			if (this->content[i] < minimumValue) {
				minimumValue = this->content[i];
				minimumIndex = i;
			}
		}
		return minimumIndex;
	}

	/**
	 * Gets an element in the vector by its index/row
	 */
	T& get(unsigned short index) const {
		assert(index < this->rows);
		return this->content[index];
	}
};

}

#endif /* INCLUDE_GRAPH_Vector_HPP_ */
