#ifndef IO_TYPESERIALIZERS_HPP_
#define IO_TYPESERIALIZERS_HPP_

#include <string>

#include "math/InfinityWrapper.hpp"

namespace pbqppapa {

template <typename T>
struct dummyType{};

template<typename T>
std::string serializeElement(T t) {
	return serializeElement(dummyType<T>{}, t);
}

template<typename T>
T deserializeElement(std::string serialized) {
	return deserializeElement(dummyType<T>{}, serialized);
}

template<typename T>
std::string getTypeName() {
	return getTypeName(dummyType<T>{});
}


//unsigned long

std::string serializeElement(dummyType<unsigned long>, unsigned long element) {
	return std::to_string(element);
}

unsigned long deserializeElement(dummyType<unsigned long>, std::string serialized) {
	return std::strtoul(serialized.c_str(), NULL, 0);
}

std::string getTypeName(dummyType<unsigned long>) {
	return "unsigned long";
}

//unsigned short

std::string serializeElement(dummyType<unsigned short>, unsigned short number) {
	return std::to_string(number);
}

unsigned short deserializeElement(dummyType<unsigned short>, std::string serialized) {
	return (unsigned short) std::strtoul(serialized.c_str(), NULL, 0);
}

std::string getTypeName(dummyType<unsigned short>) {
	return "unsigned short";
}

//InfinityWrapper

template<typename T>
std::string serializeElement(dummyType<InfinityWrapper<T>>, InfinityWrapper<T> number) {
	if (number.isInfinite()) {
		return "INF";
	}
	return serializeElement<T>(number.getValue);
}

template<typename T>
InfinityWrapper<T> deserializeElement(dummyType<InfinityWrapper<T>>, std::string serialized) {
	if (serialized == "INF") {
		return InfinityWrapper<T>::getInfinite();
	}
	return InfinityWrapper<T>(deserializeElement<T>(serialized));
}

template<typename T> std::string getTypeName(dummyType<InfinityWrapper<T>>) {
	return "INF " + getTypeName<T>();
}

}



#endif /* IO_TYPESERIALIZERS_HPP_ */
