/*
 * Vektor.cpp
 *
 *  Created on: 7 Aug 2018
 *      Author: Max
 */

#include "Vektor.hpp"

#include "Matrix.hpp"

Vektor::Vektor(int length, double* data) : Matrix(1, length, data){
}

int Vektor::getIndexOfSmallestElement() {
	//we assume that the vektor has at least one element and that whoever is using this function knows what he's doing
	int minimumIndex = 0;
	double minimumValue = content [0];
	for(int i = 0; i < rows; i++) {
		if (content [i] < minimumValue) {
			minimumValue = content [i];
			minimumIndex = i;
		}
	}
	return minimumIndex;
}

