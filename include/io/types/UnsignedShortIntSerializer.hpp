#ifndef IO_TYPES_UNSIGNEDSHORTINTSERIALIZER_HPP_
#define IO_TYPES_UNSIGNEDSHORTINTSERIALIZER_HPP_

#include <string>
#include <cstdlib>
#include "io/TypeSerializer.hpp"

namespace pbqppapa {

class UnsignedShortIntSerializer: public TypeSerializer<unsigned short> {

public:
	UnsignedShortIntSerializer() :
			TypeSerializer("unsigned short") {

	}

	std::string serialize(unsigned short number) override {
		return std::to_string(number);
	}

	unsigned short deserialize(std::string serialized) override {
		return (unsigned short) std::strtoul(serialized.c_str(), NULL, 0);
	}

};

}

#endif /* IO_TYPES_UNSIGNEDSHORTINTSERIALIZER_HPP_ */
