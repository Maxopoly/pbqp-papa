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


}
