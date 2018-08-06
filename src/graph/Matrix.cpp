/*
 * Matrix.cpp
 *
 *  Created on: 3 Aug 2018
 *      Author: Max
 */

#include "graph/Matrix.hpp"

Matrix::Matrix(int rows, int columns, double* data) {
	this->rows = rows;
	this->columns = columns;
	content = data;
	contentLength = sizeof(double);
}

Matrix::~Matrix() {
	// TODO Auto-generated destructor stub
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

double Matrix::get(int row, int column) {
	return content [(row * columns) + column];
}

int Matrix::getRowCount() {
	return rows;
}

int Matrix::getColumnCount() {
	return columns;
}

Matrix* Matrix::transpose() {
	double resultData = new double [columns * rows];
	const int size = columns * rows;
	for(int n = 0; n < size; n++) {
		int i = n / columns;
		int j = n % columns;
		resultData [n] = content [rows * j + i];
	}
	return new Matrix(columns, rows, resultData);
}

