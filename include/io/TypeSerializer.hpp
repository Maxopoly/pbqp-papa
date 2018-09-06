#ifndef IO_TYPESERIALIZER_HPP_
#define IO_TYPESERIALIZER_HPP_

template<typename T>
class TypeSerializer {

public:
	TypeSerializer() {};
	virtual ~TypeSerializer();
	virtual char const* serialize(T t) = 0;
	virtual T deserialize(char const*) = 0;
};

#endif /* IO_TYPESERIALIZER_HPP_ */
