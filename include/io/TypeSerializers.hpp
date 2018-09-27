#ifndef IO_TYPESERIALIZERS_HPP_
#define IO_TYPESERIALIZERS_HPP_

#include <string>

#include "math/InfinityWrapper.hpp"

namespace pbqppapa {

template <typename T>
struct dummyType{};



//1

template<typename T>
std::string serializeElement(T t) {
	return serializeElement2(dummyType<T>{}, t);
}

template<typename T>
T deserializeElement(std::string serialized) {
	return deserializeElement2(dummyType<T>{}, serialized);
}

template<typename T>
std::string getTypeName() {
	return getTypeName2(dummyType<T>{});
}


//2
std::string serializeElement2(dummyType<unsigned long>, unsigned long element);

unsigned long deserializeElement2(dummyType<unsigned long>, std::string serialized);

std::string getTypeName2(dummyType<unsigned long>);


//InfinityWrapper

template<typename T>
std::string serializeElement2(dummyType<InfinityWrapper<T>>, InfinityWrapper<T> number) {
	if (number.isInfinite()) {
		return "INF";
	}
	return serializeElement<T>(number.getValue);
}

template<typename T>
InfinityWrapper<T> deserializeElement2(dummyType<InfinityWrapper<T>>, std::string serialized) {
	if (serialized == "INF") {
		return InfinityWrapper<T>::getInfinite();
	}
	return InfinityWrapper<T>(deserializeElement<T>(serialized));
}

template<typename T> std::string getTypeName2(dummyType<InfinityWrapper<T>>) {
	return "INF " + getTypeName<T>();
}




/*
//unsigned short

std::string serializeElement(dummyType<unsigned short>, unsigned short number) {
	return std::to_string(number);
}

unsigned short deserializeElement(dummyType<unsigned short>, std::string serialized) {
	return (unsigned short) std::strtoul(serialized.c_str(), NULL, 0);
}

std::string getTypeName(dummyType<unsigned short>) {
	return "unsigned short";
} */

}



#endif /* IO_TYPESERIALIZERS_HPP_ */
