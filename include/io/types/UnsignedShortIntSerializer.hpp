#ifndef IO_TYPES_UNSIGNEDSHORTINTSERIALIZER_HPP_
#define IO_TYPES_UNSIGNEDSHORTINTSERIALIZER_HPP_

#include <string>
#include <cstdlib>
#include "io/TypeSerializer.hpp"

class UnsignedShortIntSerializer : public TypeSerializer<unsigned short int> {

public:
	UnsignedShortIntSerializer() : TypeSerializer("unsigned short int") {

	}

	std::string serialize(unsigned short int number) override {
		return std::to_string(number);
	}


	unsigned short int deserialize(std::string serialized) override {
		return (unsigned short int) std::strtoul(serialized.c_str(), NULL, 0);
	}

};



#endif /* IO_TYPES_UNSIGNEDSHORTINTSERIALIZER_HPP_ */
