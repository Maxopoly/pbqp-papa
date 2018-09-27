#include "io/TypeSerializerFactory.hpp"

namespace pbqppapa {

UnsignedLongIntSerializer* TypeSerializerFactory::get(unsigned long) {
	return unsignedLongIntSerializer;
}

UnsignedShortIntSerializer* TypeSerializerFactory::get(unsigned short) {
	return unsignedShortIntSerializer;
}

UnsignedCharSerializer* TypeSerializerFactory::get(unsigned char) {
	return unsignedCharSerializer;
}

FloatSerializer* TypeSerializerFactory::get(float) {
	return floatSerializer;
}

DoubleSerializer* TypeSerializerFactory::get(double) {
	return doubleSerializer;
}

}
