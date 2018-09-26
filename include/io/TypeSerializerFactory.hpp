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

public:
	static UnsignedLongIntSerializer get(unsigned long int);
	static UnsignedShortIntSerializer get(unsigned short int);
	static UnsignedCharSerializer get(unsigned char);
	static FloatSerializer get(float);
	static DoubleSerializer get(double);

};

}

#endif /* IO_TYPESERIALIZERFACTORY_HPP_ */
