#include "io/TypeSerializers.hpp"

namespace pbqppapa {

//unsigned long
std::string serializeElement2(dummyType<unsigned long>, unsigned long element) {
	return std::to_string(element);
}


unsigned long deserializeElement2(dummyType<unsigned long>, std::string serialized) {
	return std::strtoul(serialized.c_str(), NULL, 0);
}

std::string getTypeName2(dummyType<unsigned long>) {
	return "unsigned long";
}

//unsigned int
std::string serializeElement2(dummyType<unsigned int>, unsigned int element) {
	return std::to_string(element);
}


unsigned int deserializeElement2(dummyType<unsigned int>, std::string serialized) {
	return (unsigned int) std::strtoul(serialized.c_str(), NULL, 0);
}

std::string getTypeName2(dummyType<unsigned int>) {
	return "unsigned int";
}


//unsigned short
std::string serializeElement2(dummyType<unsigned short>, unsigned short element) {
	return std::to_string(element);
}


unsigned short deserializeElement2(dummyType<unsigned short>, std::string serialized) {
	return (unsigned short) std::strtoul(serialized.c_str(), NULL, 0);
}

std::string getTypeName2(dummyType<unsigned short>) {
	return "unsigned short";
}


}
