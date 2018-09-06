#ifndef IO_TYPES_UNSIGNEDLONGINTSERIALIZER_HPP_
#define IO_TYPES_UNSIGNEDLONGINTSERIALIZER_HPP_

#include "io/TypeSerializer.hpp"

class UnsignedLongIntSerializer : public TypeSerializer<unsigned long int> {

public:

	char const* serialize(unsigned long int number) override;

	unsigned long int deserialize(char const* serialized) override;
};



#endif /* IO_TYPES_UNSIGNEDLONGINTSERIALIZER_HPP_ */
