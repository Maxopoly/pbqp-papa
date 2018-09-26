#ifndef MATH_INFINITYWRAPPER_HPP_
#define MATH_INFINITYWRAPPER_HPP_

namespace pbqppapa {

/**
 * Decorates an arithmetic type by representing infinity as the biggest number displayable in the type
 * and implementing comparison and addition accordingly
 */
template<typename T>
class InfinityWrapper {
private:
	T wrappedValue;
	//we assume that this is always the biggest displayable number in the template type
	//TODO what if we reach infinite through convential ways, but dont overflow?
	static T magicInfiniteNumber;

	InfinityWrapper<T>() {
		//this is needed, because if T is for example int, its default constructor would not be called
		wrappedValue = T();
	}

	InfinityWrapper<T>(T value) {
		wrappedValue = value;
	}

	static InfinityWrapper<T> getInfinite() {
		InfinityWrapper<T> result;
		result.wrappedValue = magicInfiniteNumber;
		return result;
	}


	inline T& getValue() {
		return wrappedValue;
	}

	inline bool operator==(InfinityWrapper<T> rhs) {
		return rhs.wrappedValue == this->wrappedValue;
	}

	inline InfinityWrapper<T>& operator+=(InfinityWrapper<T> rhs) {
		if (this->isInfinite()) {
			return *this;
		}
		if (rhs.isInfinite()) {
			wrappedValue = magicInfiniteNumber;
			return *this;
		}
		this->wrappedValue += rhs.wrappedValue;
		return *this;
	}

	inline InfinityWrapper<T>& operator-=(InfinityWrapper<T> rhs) {
		//TODO: this is kinda sketchy, rethink maybe?
		if (this->isInfinite()) {
			return *this;
		}
		if (rhs.isInfinite()) {
			//do nothing for now
			return *this;
		}
		this->wrappedValue -= rhs.wrappedValue;
		return *this;
	}

	inline bool isInfinite() {
		return wrappedValue == magicInfiniteNumber;
	}

};

template<typename T>
inline InfinityWrapper<T>& operator+(InfinityWrapper<T> lhs,
		const InfinityWrapper<T>& rhs) {
	lhs += rhs;
	return lhs;
}

template<typename T>
inline InfinityWrapper<T>& operator-(InfinityWrapper<T> lhs,
		const InfinityWrapper<T>& rhs) {
	lhs -= rhs;
	return lhs;
}

template<typename T>
inline bool operator==(const InfinityWrapper<T>& lhs,
		const InfinityWrapper<T>& rhs) {
	return lhs.wrappedValue == rhs.wrappedValue;
}

template<typename T>
inline bool operator!=(const InfinityWrapper<T>& lhs, const InfinityWrapper<T>& rhs) {
	return lhs.wrappedValue != rhs.wrappedValue;
}

template<typename T>
inline bool operator<(const InfinityWrapper<T>& lhs, const InfinityWrapper<T>& rhs) {
	return lhs < rhs;
}

template<typename T>
inline bool operator>(const InfinityWrapper<T>& lhs, const InfinityWrapper<T>& rhs) {
	return operator<(rhs, lhs);
}

template<typename T>
inline bool operator<=(const InfinityWrapper<T>& lhs, const InfinityWrapper<T>& rhs) {
	return !operator>(lhs, rhs);
}

template<typename T>
inline bool operator>=(const InfinityWrapper<T>& lhs, const InfinityWrapper<T>& rhs) {
	return !operator<(lhs, rhs);
}

}

#endif /* MATH_INFINITYWRAPPER_HPP_ */
