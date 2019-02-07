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

//signed long
std::string serializeElement2(dummyType<signed long>, signed long element) {
	return std::to_string(element);
}


unsigned long deserializeElement2(dummyType<signed long>, std::string serialized) {
	return std::strtoul(serialized.c_str(), NULL, 0);
}

std::string getTypeName2(dummyType<signed long>) {
	return "signed long";
}

//signed int
std::string serializeElement2(dummyType<signed int>, signed int element) {
	return std::to_string(element);
}


unsigned int deserializeElement2(dummyType<signed int>, std::string serialized) {
	return (signed int) std::strtoul(serialized.c_str(), NULL, 0);
}

std::string getTypeName2(dummyType<signed int>) {
	return "signed int";
}


//signed short
std::string serializeElement2(dummyType<signed short>, signed short element) {
	return std::to_string(element);
}


unsigned short deserializeElement2(dummyType<signed short>, std::string serialized) {
	return (signed short) std::strtoul(serialized.c_str(), NULL, 0);
}

std::string getTypeName2(dummyType<signed short>) {
	return "signed short";
}


}
