#ifndef IO_TYPESERIALIZERS_HPP_
#define IO_TYPESERIALIZERS_HPP_

#include <string>

#include "math/InfinityWrapper.hpp"

namespace pbqppapa {

template<typename T>
std::string serializeElement(T t);

template<typename T>
T deserializeElement(std::string);

template<typename T>
std::string getTypeName();


//unsigned long

template<> std::string serializeElement<unsigned long>(unsigned long element) {
	return std::to_string(element);
}

template<> unsigned long deserializeElement<unsigned long>(std::string serialized) {
	return std::strtoul(serialized.c_str(), NULL, 0);
}

template<> std::string getTypeName<unsigned long>() {
	return "unsigned long";
}

//unsigned short

template<> std::string serializeElement<unsigned short>(unsigned short number) {
	return std::to_string(number);
}

template<> unsigned short deserializeElement<unsigned short>(std::string serialized) {
	return (unsigned short) std::strtoul(serialized.c_str(), NULL, 0);
}

template<> std::string getTypeName<unsigned short>() {
	return "unsigned short";
}

//InfinityWrapper

template<typename K>
std::string serializeElement<InfinityWrapper<K>>(InfinityWrapper<K> number) {
	if (number.isInfinite()) {
		return "INF";
	}
	return serializeElement(number.getValue);
}

template<typename T>
InfinityWrapper<T> deserializeElement(std::string serialized) {
	if (serialized == "INF") {
		return InfinityWrapper<T>::getInfinite();
	}
	return InfinityWrapper<T>(deserializeElement<T>(serialized));
}

template<typename T> std::string getTypeName() {
	return "INF " + getTypeName<T>();
}

}



#endif /* IO_TYPESERIALIZERS_HPP_ */
