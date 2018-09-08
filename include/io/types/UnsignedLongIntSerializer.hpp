#ifndef IO_TYPES_UNSIGNEDLONGINTSERIALIZER_HPP_
#define IO_TYPES_UNSIGNEDLONGINTSERIALIZER_HPP_

#include "io/TypeSerializer.hpp"
#include <string>
#include <cstdlib>

class UnsignedLongIntSerializer: public TypeSerializer<unsigned long int> {

public:
	UnsignedLongIntSerializer() :
			TypeSerializer("unsigned long int") {

	}

	std::string serialize(unsigned long int number) override {
		std::string s = std::to_string(number);
		return s;
	}

	unsigned long int deserialize(std::string serialized) override {
		return (unsigned long int) std::strtoul(serialized.c_str(), NULL, 0);
	}
};

#endif /* IO_TYPES_UNSIGNEDLONGINTSERIALIZER_HPP_ */
