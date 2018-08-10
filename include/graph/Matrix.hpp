/*
 * Matrix.h
 *
 *  Created on: 3 Aug 2018
 *      Author: Max
 */

#ifndef GRAPH_MATRIX_H_
#define GRAPH_MATRIX_H_

#include <vector>

class Vektor;

class Matrix {

protected:
	int rows;
	int columns;
	double* content;
	int contentLength;

	void allocDataSpace();

public:
	Matrix(int, int, double*);
	virtual ~Matrix();

	//modifiers
	Matrix* operator+=(const Matrix&);
	Matrix* operator-=(const Matrix&);
	Matrix* operator*=(double);
	Matrix* operator/=(double);

	//calculations producing a new matrix
	Matrix* transpose();
	//single vector containing the diagonal elements (0,0 ; 1,1 etc.)
	Vektor* getDiagonal();

	//getter
	double get(int row, int column);
	int getRowCount();
	int getColumnCount();


};

#endif /* GRAPH_MATRIX_H_ */
