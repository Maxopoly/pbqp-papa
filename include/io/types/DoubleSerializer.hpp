#ifndef IO_TYPES_DOUBLESERIALIZER_HPP_
#define IO_TYPES_DOUBLESERIALIZER_HPP_

#include <string>
#include <cstdlib>
#include "io/TypeSerializer.hpp"

namespace pbqppapa {

class DoubleSerializer: public TypeSerializer<double> {

public:
	DoubleSerializer() :
			TypeSerializer("double") {
	}

	std::string serialize(double number) override {
		return std::to_string(number);
	}

	double deserialize(std::string serialized) override {
		return std::stod(serialized);
	}

};

}

#endif /* IO_TYPES_DOUBLESERIALIZER_HPP_ */
