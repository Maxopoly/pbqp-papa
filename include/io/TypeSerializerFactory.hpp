#ifndef IO_TYPESERIALIZERFACTORY_HPP_
#define IO_TYPESERIALIZERFACTORY_HPP_

#include <set>
#include <map>

#include "io/types/UnsignedLongIntSerializer.hpp"
#include "io/types/UnsignedShortIntSerializer.hpp"
#include "io/types/UnsignedCharSerializer.hpp"
#include "io/types/FloatSerializer.hpp"
#include "io/types/DoubleSerializer.hpp"

namespace pbqppapa {

class TypeSerializerFactory {

private:
	UnsignedLongIntSerializer* unsignedLongIntSerializer =
			new UnsignedLongIntSerializer();
	UnsignedShortIntSerializer* unsignedShortIntSerializer =
			new UnsignedShortIntSerializer();
	UnsignedCharSerializer* unsignedCharSerializer =
			new UnsignedCharSerializer();
	FloatSerializer* floatSerializer = new FloatSerializer();
	DoubleSerializer* doubleSerializer = new DoubleSerializer();

public:
	UnsignedLongIntSerializer* get(unsigned long);
	UnsignedShortIntSerializer* get(unsigned short);
	UnsignedCharSerializer* get(unsigned char);
	FloatSerializer* get(float);
	DoubleSerializer* get(double);

};

}

#endif /* IO_TYPESERIALIZERFACTORY_HPP_ */
