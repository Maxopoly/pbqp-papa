#ifndef GRAPH_MATRIX_H_
#define GRAPH_MATRIX_H_

#include <vector>

template<typename T>
class Matrix {

protected:
	unsigned short int rows;
	unsigned short int columns;
	T* content;

public:
	/**
	 * Creates a new n x m matrix with n columns and m rows.
	 * The given data is expected to be an array of length n * m,
	 * row by row, for example:
	 *
	 *  0 1
	 *  2 3
	 *
	 */
	Matrix(unsigned short int columns, unsigned short int rows, T* data) {
		this->rows = rows;
		this->columns = columns;
		content = data;
	}

	~Matrix() {
		delete [] content;
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
		int length = rows * columns;
		for(int i = 0; i < length; i++) {
			content[i] -= other.content [i];
		}
		return this;
	}

	/**
	 * Multiplies each value in the entire matrix by a given factor
	 */
	Matrix<T>* operator*=(T factor) {
		int length = rows * columns;
		for(int i = 0; i < length; i++) {
			content[i] *= factor;
		}
		return this;
	}

	/**
	 * Divides each value in the entire matrix by a given divisor
	 */
	Matrix<T>* operator/=(T quotient) {
		int length = rows * columns;
		for(int i = 0; i < length; i++) {
			content[i] *= quotient;
		}
		return this;
	}

	/**
	 *  Creates a transposed version of this matrix.
	 */
	Matrix<T>* transpose() {
		T* resultData = new T [columns * rows];
		const int size = columns * rows;
		for(int n = 0; n < size; n++) {
			int i = n / columns;
			int j = n % columns;
			resultData [n] = content [rows * j + i];
		}
		return new Matrix<T>(rows, columns, resultData);
	}

	/**
	 * Retrieves a single element by position
	 */
	T get(int row, int column) {
		return content [(row * columns) + column];
	}

	/**
	 * Gets the amount of rows
	 */
	unsigned short int getRowCount() {
		return rows;
	}

	/**
	 * Gets the amount of columns
	 */
	unsigned short int getColumnCount() {
		return columns;
	}

};

#endif /* GRAPH_MATRIX_H_ */
