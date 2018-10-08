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

std::string serializeElement2(dummyType<unsigned int>, unsigned int element);

unsigned int deserializeElement2(dummyType<unsigned int>, std::string serialized);

std::string getTypeName2(dummyType<unsigned int>);

std::string serializeElement2(dummyType<unsigned short>, unsigned short element);

unsigned short deserializeElement2(dummyType<unsigned short>, std::string serialized);

std::string getTypeName2(dummyType<unsigned short>);


//InfinityWrapper

template<typename T>
std::string serializeElement2(dummyType<InfinityWrapper<T>>, InfinityWrapper<T> number) {
	if (number.isInfinite()) {
		return "INF";
	}
	return serializeElement2(dummyType<T> {}, number.getValue());
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

}


#endif /* IO_TYPESERIALIZERS_HPP_ */
