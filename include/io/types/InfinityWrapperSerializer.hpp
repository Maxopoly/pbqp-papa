#ifndef IO_TYPES_INFINITYWRAPPERSERIALIZER_HPP_
#define IO_TYPES_INFINITYWRAPPERSERIALIZER_HPP_

#include <string>
#include <cstdlib>
#include "io/TypeSerializer.hpp"
#include "io/TypeSerializerFactory.hpp"
#include "math/InfinityWrapper.hpp"

namespace pbqppapa {

template<typename T>
class InfinityWrapperSerializer: public TypeSerializer<InfinityWrapper<T>> {

	TypeSerializer<T> internalSerial = TypeSerializerFactory::get(T ());

public:
	InfinityWrapperSerializer() :
			TypeSerializer<InfinityWrapper<T>>("INF " + internalSerial.getIdentifier()) {
	}

	std::string serialize(InfinityWrapper<T> number) const override {
		if (number.isInfinite()) {
				return "INF";
		}
		return internalSerial.serialize(number.getValue());
	}

	float deserialize(std::string serialized) const override {
		if (serialized == "INF") {
			return InfinityWrapper<T>::getInfinite();
		}
		return internalSerial.deserialize(serialized);
	}

};

}

#endif /* IO_TYPES_INFINITYWRAPPERSERIALIZER_HPP_ */
