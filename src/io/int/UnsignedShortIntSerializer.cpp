#include <string>
#include <cstdlib>
#include "io/TypeSerializer.hpp"

class UnsignedShortIntSerializer : TypeSerializer<unsigned short int> {

	UnsignedShortIntSerializer() {};
	~UnsignedShortIntSerializer() {};


	char const* serialize(unsigned short int number) override {
		std::string s = std::to_string(number);
		return s.c_str();
	}


	unsigned short int deserialize(char* serialized) override {
		return (unsigned short int) std::strtoul(serialized, NULL, 0);
	}

};



