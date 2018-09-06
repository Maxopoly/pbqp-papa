#include <string>
#include <cstdlib>
#include "io/types/UnsignedLongIntSerializer.hpp"

char const* UnsignedLongIntSerializer::serialize(unsigned long int number) {
	std::string s = std::to_string(number);
	return s.c_str();
}

unsigned long int UnsignedLongIntSerializer::deserialize(char const* serialized) {
	return (unsigned long int) std::strtoul(serialized, NULL, 0);
}

