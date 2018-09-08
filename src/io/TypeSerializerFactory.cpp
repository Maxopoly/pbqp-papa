#include "io/TypeSerializerFactory.hpp"

namespace pbqppapa {

UnsignedLongIntSerializer* TypeSerializerFactory::get(unsigned long int) {
	return unsignedLongIntSerializer;
}

UnsignedShortIntSerializer* TypeSerializerFactory::get(unsigned short int) {
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
