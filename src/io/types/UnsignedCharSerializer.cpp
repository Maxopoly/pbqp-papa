
#include <string>
#include <cstdlib>
#include "io/TypeSerializer.hpp"

class UnsignedCharSerializer : TypeSerializer<unsigned char> {

	UnsignedCharSerializer() {};
	~UnsignedCharSerializer() {};


	char const* serialize(unsigned char number) override {
		std::string s = std::to_string(number);
		return s.c_str();
	}


	unsigned char deserialize(char const* serialized) override {
		return (unsigned char) std::strtoul(serialized, NULL, 0);
	}

};
