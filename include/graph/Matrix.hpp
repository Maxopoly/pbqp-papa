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
		delete [] content;
		content = new T[rows * columns];
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
	inline T& get(unsigned short int row, unsigned short int column) const {
		return content[(row * columns) + column];
	}

	/**
	 * Retrieves an element by its raw index in the content array
	 */
	inline T& getRaw(unsigned int index) const {
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
	inline unsigned int getElementCount() const {
		return columns * rows;
	}

	/**
	 * Multiplies a matrix by placing multiple instances of it below each other, for example:
	 *
	 * A B
	 * C D
	 *
	 * (with multiplier 2) --->
	 *
	 * A B
	 * C D
	 * A B
	 * C D
	 *
	 *
	 */
	Matrix<T> multiplyRows(const unsigned short int multiplier) const {
		Matrix<T> result(rows * multiplier, columns);
		const unsigned long sectorSize = sizeof(T) * rows * columns;
		for (unsigned short int i = 0; i < multiplier; i++) {
			std::memcpy(result.content + i * sectorSize, content, sectorSize);
		}
		return result;
	}

	/**
	 * Multiplies a matrix by placing multiple instances of it below each other by multiplying
	 * rows individually, for example:
	 *
	 * A B
	 * C D
	 *
	 * (with multiplier 2) --->
	 *
	 * A B
	 * A B
	 * C D
	 * C D
	 *
	 *
	 */
	Matrix<T> multiplyRowsIndividually(
			const unsigned short int multiplier) const {
		Matrix<T> result(rows * multiplier, columns);
		const unsigned long rowLength = sizeof(T) * columns;
		const unsigned long sectionLength = rowLength * multiplier;
		for (unsigned short int i = 0; i < rows; i++) {
			for (unsigned short int offset = 0; offset < multiplier; offset++) {
				std::memcpy(
						result.content + (rowLength * offset) + (sectionLength * i),
						content + (rowLength * i), rowLength);
			}
		}
		return result;
	}

	/**
	 * Multiplies a matrix by placing multiple instances of it bext to each other by multiplying
	 * columns individually, for example:
	 *
	 * A B
	 * C D
	 *
	 * (with multiplier 2) --->
	 *
	 * A A B B
	 * C C D D
	 *
	 *
	 */
	Matrix<T> multiplyColumnsIndividually(
			const unsigned short int multiplier) const {
		Matrix<T> result(rows, columns * multiplier);
		const unsigned long sectorLength = rows * columns;
		for (unsigned long i = 0; i < rows * columns; i++) {
			for (int column = 0; column < multiplier; column++) {
				result.getRaw(i + (column * sectorLength)) = getRaw(i);
			}
		}
		return result;
	}

	/**
	 * Multiplies a matrix by placing multiple instances of it next to each other, for example:
	 *
	 * A B
	 * C D
	 *
	 * (with multiplier 2) --->
	 *
	 * A B A B
	 * C D C D
	 *
	 *
	 */
	Matrix<T> multiplyColumns(const unsigned short int multiplier) const {
		Matrix<T> result(rows, columns * multiplier);
		const unsigned long rowLength = columns * sizeof(T);
		const unsigned long rowDataLength = rowLength * multiplier;
		for (unsigned short i = 0; i < rows; i++) {
			for (int column = 0; column < multiplier; column++) {
				std::memcpy(
						result.content + (rowDataLength * i) + (column * rowLength),
						content + (rowLength * i), rowLength);
			}
		}
		return result;
	}

};

}

#endif /* GRAPH_MATRIX_H_ */
