/*
 * Matrix.cpp
 *
 *  Created on: 3 Aug 2018
 *      Author: Max
 */

#include "graph/Vektor.hpp"
#include <algorithm>

Matrix::Matrix(int columns, int rows, double* data) {
	this->rows = rows;
	this->columns = columns;
	content = data;
	contentLength = sizeof(double);
}

Matrix::~Matrix() {
	delete [] content;
}

Matrix* Matrix::operator+=(const Matrix& other) {
	int length = rows * columns;
	for(int i = 0; i < length; i++) {
		content[i] += other.content [i];
	}
	return this;
}

Matrix* Matrix::operator-=(const Matrix& other) {
	int length = rows * columns;
	for(int i = 0; i < length; i++) {
		content[i] -= other.content [i];
	}
	return this;
}

Matrix* Matrix::operator*=(double multiplier) {
	int length = rows * columns;
	for(int i = 0; i < length; i++) {
		content[i] *= multiplier;
	}
	return this;
}
Matrix* Matrix::operator/=(double quotient) {
	int length = rows * columns;
	for(int i = 0; i < length; i++) {
		content[i] *= quotient;
	}
	return this;
}

double Matrix::get(int column, int row) {
	return content [(row * columns) + column];
}

int Matrix::getRowCount() {
	return rows;
}

int Matrix::getColumnCount() {
	return columns;
}

Matrix* Matrix::transpose() {
	double* resultData = new double [columns * rows];
	const int size = columns * rows;
	for(int n = 0; n < size; n++) {
		int i = n / columns;
		int j = n % columns;
		resultData [n] = content [rows * j + i];
	}
	return new Matrix(rows, columns, resultData);
}

Vektor* Matrix::getDiagonal() {
	int diagonalLength = std::min(rows,columns);
	double* resultData = new double [diagonalLength];
	for(int i = 0; i < diagonalLength; i++) {
		resultData [i] = get(i,i);
	}
	return new Vektor(diagonalLength, resultData);
}

