#ifndef IO_TYPES_UNSIGNEDLONGINTSERIALIZER_HPP_
#define IO_TYPES_UNSIGNEDLONGINTSERIALIZER_HPP_

#include "io/TypeSerializer.hpp"
#include <string>
#include <cstdlib>

namespace pbqppapa {

class UnsignedLongIntSerializer: public TypeSerializer<unsigned long> {

public:
	UnsignedLongIntSerializer() :
			TypeSerializer("unsigned long") {

	}

	std::string serialize(unsigned long number) override {
		std::string s = std::to_string(number);
		return s;
	}

	unsigned long deserialize(std::string serialized) override {
		return (unsigned long) std::strtoul(serialized.c_str(), NULL, 0);
	}
};

}

#endif /* IO_TYPES_UNSIGNEDLONGINTSERIALIZER_HPP_ */
