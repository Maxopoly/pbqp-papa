#ifndef IO_TYPES_FLOATSERIALIZER_HPP_
#define IO_TYPES_FLOATSERIALIZER_HPP_

#include <string>
#include <cstdlib>
#include "io/TypeSerializer.hpp"

namespace pbqppapa {

class FloatSerializer: public TypeSerializer<float> {

public:
	FloatSerializer() :
			TypeSerializer("float") {
	}

	std::string serialize(float number) override {
		return std::to_string(number);
	}

	float deserialize(std::string serialized) override {
		return std::stof(serialized);
	}

};

}

#endif /* IO_TYPES_FLOATSERIALIZER_HPP_ */
