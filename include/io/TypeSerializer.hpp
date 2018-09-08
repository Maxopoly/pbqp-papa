#ifndef IO_TYPESERIALIZER_HPP_
#define IO_TYPESERIALIZER_HPP_

template<typename T>
class TypeSerializer {

private:
	const std::string identifier;

public:
	TypeSerializer(const std::string identifier) :
			identifier(identifier) {
	}
	;
	virtual ~TypeSerializer() {
	}
	;
	virtual std::string serialize(T t) = 0;
	virtual T deserialize(std::string) = 0;

	const std::string getIdentifier() {
		return identifier;
	}
};

#endif /* IO_TYPESERIALIZER_HPP_ */
