#ifndef IO_TYPES_UNSIGNEDCHARSERIALIZER_HPP_
#define IO_TYPES_UNSIGNEDCHARSERIALIZER_HPP_

#include <string>
#include <cstdlib>
#include "io/TypeSerializer.hpp"

class UnsignedCharSerializer: public TypeSerializer<unsigned char> {

public:
	UnsignedCharSerializer() :
			TypeSerializer("unsigned char") {

	}

	std::string serialize(unsigned char number) override {
		return std::to_string(number);
	}

	unsigned char deserialize(std::string serialized) override {
		return (unsigned char) std::strtoul(serialized.c_str(), NULL, 0);
	}

};

#endif /* IO_TYPES_UNSIGNEDCHARSERIALIZER_HPP_ */
