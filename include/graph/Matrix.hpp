#ifndef GRAPH_MATRIX_H_
#define GRAPH_MATRIX_H_

#include <vector>
#include <algorithm>
#include <iterator>
#include <cstring>

namespace pbqppapa {

template<typename T>
class Matrix {

protected:
	const unsigned short int rows;
	const unsigned short int columns;
	T* const content;

public:
	/**
	 * Creates a new n x m matrix with n rows and m columns.
	 * The given data is expected to be an array of length n * m,
	 * row by row, for example:
	 *
	 *  0 1
	 *  2 3
	 *
	 */
	Matrix(unsigned short int rows, unsigned short int columns, T* data) :
			rows(rows), columns(columns), content(new T[rows * columns]) {
		std::memcpy(content, data, sizeof(T) * rows * columns);
	}

	/**
	 * Creates a new matrix with uninitialized content
	 */
	Matrix(unsigned short int rows, unsigned short int columns) :
			rows(rows), columns(columns), content(new T[rows * columns]) {
	}

	Matrix(const Matrix<T>& matrix) :
			rows(matrix.rows), columns(matrix.columns), content(
					new T[rows * columns]) {
		std::memcpy(content, matrix.content, rows * columns * sizeof(T));
	}

	~Matrix() {
		delete[] content;
	}

	Matrix<T>* operator=(const Matrix<T>& other) {
		rows = other.rows;
		columns = other.columns;
		content = new T [rows*columns];
		memcpy(content, other.content, rows * columns * sizeof(T));
		return this;
	}

	/**
	 * Adds the given matrix to this one. Dimensions aren't explicitly checked and expected to be identical.
	 * If they're not, you're at fault
	 */
	Matrix<T>* operator+=(const Matrix<T>& other) {
		int length = rows * columns;
		for (int i = 0; i < length; i++) {
			content[i] += other.content[i];
		}
		return this;
	}

	/**
	 * Subtracts the given matrix from this one. Dimensions aren't explicitly checked and expected to be identical.
	 * If they're not, you're at fault
	 */
	Matrix<T>* operator-=(const Matrix<T>& other) {
		const unsigned long int length = rows * columns;
		for (unsigned long int i = 0; i < length; i++) {
			content[i] -= other.content[i];
		}
		return this;
	}

	bool operator==(const Matrix<T>& other) const {
		if (other.getRowCount() != this->getRowCount()
				|| other.getColumnCount() != this->getColumnCount()) {
			return false;
		}
		const unsigned long int length = rows * columns;
		for (unsigned long int i = 0; i < length; i++) {
			if (content[i] != other.content[i]) {
				return false;
			}
		}
		return true;
	}

	/**
	 * Multiplies each value in the entire matrix by a given factor
	 */
	Matrix<T>* operator*=(const T& factor) {
		const unsigned long int length = rows * columns;
		for (unsigned long int i = 0; i < length; i++) {
			content[i] *= factor;
		}
		return this;
	}

	/**
	 * Divides each value in the entire matrix by a given divisor
	 */
	Matrix<T>* operator/=(const T& quotient) {
		const unsigned long int length = rows * columns;
		for (unsigned long int i = 0; i < length; i++) {
			content[i] /= quotient;
		}
		return this;
	}

	/**
	 *  Creates a transposed version of this matrix.
	 */
	Matrix<T> transpose() const {
		const unsigned long int size = columns * rows;
		Matrix<T> result = Matrix<T>(columns, rows);
		for (unsigned long int n = 0; n < size; n++) {
			int i = n / rows;
			int j = n % rows;
			result.getRaw(n) = content[columns * j + i];
		}
		return result;
	}

	/**
	 * Retrieves a single element by position
	 */
	T& get(unsigned short int row, unsigned short int column) const {
		return content[(row * columns) + column];
	}

	/**
	 * Retrieves an element by its raw index in the content array
	 */
	T& getRaw(unsigned int index) const {
		return content[index];
	}

	/**
	 * Gets the amount of rows
	 */
	const unsigned short int& getRowCount() const {
		return rows;
	}

	/**
	 * Gets the amount of columns
	 */
	const unsigned short int& getColumnCount() const {
		return columns;
	}

	/**
	 * Gets the amount of elements
	 */
	unsigned int getElementCount() const {
		return columns * rows;
	}

};

}

#endif /* GRAPH_MATRIX_H_ */
