#ifndef GRAPH_MATRIX_H_
#define GRAPH_MATRIX_H_

#include <vector>
#include <algorithm>
#include <iterator>

template<typename T>
class Matrix {

protected:
	const unsigned short int rows;
	const unsigned short int columns;
	T* const content;

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
	Matrix(unsigned short int columns, unsigned short int rows, T* data) :
			rows(rows), columns(columns), content(data) {
	}

	/**
	 * Creates a new matrix with uninitialized content
	 */
	Matrix(unsigned short int columns, unsigned short int rows) :
				rows(rows), columns(columns), content(new T [rows * columns]) {
	}

	Matrix(const Matrix<T>& matrix) :
			rows(matrix.rows), columns(matrix.columns), content(
					new T[rows * columns]) {
		memcpy(matrix.content, content, rows * columns * sizeof(T));
	}

	~Matrix() {
		delete[] content;
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
		for (int i = 0; i < length; i++) {
			content[i] -= other.content[i];
		}
		return this;
	}

	/**
	 * Multiplies each value in the entire matrix by a given factor
	 */
	Matrix<T>* operator*=(T factor) {
		int length = rows * columns;
		for (int i = 0; i < length; i++) {
			content[i] *= factor;
		}
		return this;
	}

	/**
	 * Divides each value in the entire matrix by a given divisor
	 */
	Matrix<T>* operator/=(T quotient) {
		int length = rows * columns;
		for (int i = 0; i < length; i++) {
			content[i] /= quotient;
		}
		return this;
	}

	/**
	 *  Creates a transposed version of this matrix.
	 */
	Matrix<T>* transpose() const {
		const int size = columns * rows;
		T* resultData = new T[size];
		for (int n = 0; n < size; n++) {
			int i = n / rows;
			int j = n % rows;
			resultData[n] = content[columns * j + i];
		}
		return new Matrix<T>(rows, columns, resultData);
	}

	/**
	 * Retrieves a single element by position
	 */
	T& get(unsigned short int row, unsigned short int column) const {
		return content[(row * columns) + column];
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

};

#endif /* GRAPH_MATRIX_H_ */
