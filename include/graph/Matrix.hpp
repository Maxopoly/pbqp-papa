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

template <class T>
class Matrix {

protected:
	int rows;
	int columns;
	T* content;
	int contentLength;

public:
	Matrix(int, int, T*);
	virtual ~Matrix();

	//modifiers
	Matrix<T>* operator+=(const Matrix<T>&);
	Matrix<T>* operator-=(const Matrix<T>&);
	Matrix<T>* operator*=(T);
	Matrix<T>* operator/=(T);

	//calculations producing a new matrix
	Matrix<T>* transpose();
	//single vector containing the diagonal elements (0,0 ; 1,1 etc.)
	Vektor* getDiagonal();

	//getter
	double get(int row, int column);
	int getRowCount();
	int getColumnCount();


};

#endif /* GRAPH_MATRIX_H_ */
