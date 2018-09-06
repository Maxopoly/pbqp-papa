#include <string>
#include <cstdlib>
#include "io/TypeSerializer.hpp"

class UnsignedLongIntSerializer : TypeSerializer<unsigned char> {

	UnsignedLongIntSerializer() {};
	~UnsignedLongIntSerializer() {};


	char const* serialize(unsigned long int number) override {
		std::string s = std::to_string(number);
		return s.c_str();
	}


	unsigned long int deserialize(char* serialized) override {
		return (unsigned long int) std::strtoul(serialized, NULL, 0);
	}

};
