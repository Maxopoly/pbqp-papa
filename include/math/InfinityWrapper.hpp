#ifndef MATH_INFINITYWRAPPER_HPP_
#define MATH_INFINITYWRAPPER_HPP_

#include "limits.h"

namespace pbqppapa {

/**
 * Decorates an arithmetic type by representing infinity as the biggest number displayable in the type
 * and implementing comparison, addition and subtraction accordingly.
 * For example:
 *
 * ∞ + 5 = ∞
 * ∞ + ∞ = ∞
 * ∞ - 4 = ∞
 * 4 + ∞ = ∞
 * etc.
 *
 * Special is:
 * ∞ > ∞ = true
 * ∞ < ∞ = true
 * ∞ >= ∞ = true
 *
 * DOES NOT IMPLEMENT MULTIPLICATION OR DIVISION. DO NOT USE IT
 */
template<typename T>
class InfinityWrapper {
private:
	T wrappedValue;
	//we assume that this is always the biggest displayable number in the template type
	//TODO what if we reach infinite through convential ways, but dont overflow?
	static T magicInfiniteNumber;

public:

	InfinityWrapper<T>() {
		//this is needed, because if T is for example int, its default constructor would not be called
		wrappedValue = T();
	}

	InfinityWrapper<T>(T value) {
		wrappedValue = value;
	}

	/**
	 * Infinity for this template type
	 */
	static InfinityWrapper<T> getInfinite() {
		InfinityWrapper<T> result;
		result.wrappedValue = magicInfiniteNumber;
		return result;
	}

	/**
	 * True artithemtic value wrapped by this instance
	 */
	inline const T& getValue() const {
		return wrappedValue;
	}

	inline bool operator==(InfinityWrapper<T> rhs) {
		return rhs.wrappedValue == this->wrappedValue;
	}

	inline bool operator==(T rhs) {
		return operator==(InfinityWrapper<T>(rhs));
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

	inline InfinityWrapper<T>& operator+=(T rhs) {
		return operator+=(InfinityWrapper<T>(rhs));
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

	inline InfinityWrapper<T>& operator-=(T rhs) {
		return operator-=(InfinityWrapper<T>(rhs));
	}

	inline bool isInfinite() const {
		return wrappedValue == magicInfiniteNumber;
	}

};

template<typename T>
inline InfinityWrapper<T> operator+(InfinityWrapper<T> lhs,
		const InfinityWrapper<T>& rhs) {
	lhs += rhs;
	return lhs;
}

template<typename T>
inline InfinityWrapper<T> operator+(InfinityWrapper<T>& lhs,
		const T& rhs) {
	return operator+(lhs, InfinityWrapper<T>(rhs));
}

template<typename T>
inline InfinityWrapper<T> operator-(InfinityWrapper<T> lhs,
		const InfinityWrapper<T>& rhs) {
	lhs -= rhs;
	return lhs;
}

template<typename T>
inline InfinityWrapper<T> operator-(InfinityWrapper<T>& lhs,
		const T& rhs) {
	return operator-(lhs, InfinityWrapper<T>(rhs));
}

template<typename T>
inline bool operator==(const InfinityWrapper<T>& lhs,
		const InfinityWrapper<T>& rhs) {
	return lhs.getValue() == rhs.getValue();
}

template<typename T>
inline bool operator==(InfinityWrapper<T>& lhs,
		const T& rhs) {
	return operator==(lhs, InfinityWrapper<T>(rhs));
}

template<typename T>
inline bool operator!=(const InfinityWrapper<T>& lhs, const InfinityWrapper<T>& rhs) {
	return lhs.getValue() != rhs.getValue();
}

template<typename T>
inline bool operator!=(InfinityWrapper<T>& lhs,
		const T& rhs) {
	return operator!=(lhs, InfinityWrapper<T>(rhs));
}

template<typename T>
inline bool operator<(const InfinityWrapper<T>& lhs, const InfinityWrapper<T>& rhs) {
	if (lhs.isInfinite() && rhs.isInfinite()) {
		return true;
	}
	return lhs.getValue() < rhs.getValue();
}

template<typename T>
inline bool operator<(InfinityWrapper<T>& lhs,
		const T& rhs) {
	return operator<(lhs, InfinityWrapper<T>(rhs));
}

template<typename T>
inline bool operator>(const InfinityWrapper<T>& lhs, const InfinityWrapper<T>& rhs) {
	return operator<(rhs, lhs);
}

template<typename T>
inline InfinityWrapper<T> operator>(InfinityWrapper<T>& lhs,
		const T& rhs) {
	return operator>(lhs, InfinityWrapper<T>(rhs));
}

template<typename T>
inline bool operator<=(const InfinityWrapper<T>& lhs, const InfinityWrapper<T>& rhs) {
	//need special handling if both are infinite
	if (lhs.getValue() == rhs.getValue()) {
		return true;
	}
	return !operator>(lhs, rhs);
}

template<typename T>
inline bool operator<=(InfinityWrapper<T>& lhs,
		const T& rhs) {
	return operator<=(lhs, InfinityWrapper<T>(rhs));
}

template<typename T>
inline bool operator>=(const InfinityWrapper<T>& lhs, const InfinityWrapper<T>& rhs) {
	//need special handling if both are infinite
	if (lhs.getValue() == rhs.getValue()) {
		return true;
	}
	return !operator<(lhs, rhs);
}

template<typename T>
inline bool operator>=(InfinityWrapper<T>& lhs,
		const T& rhs) {
	return operator>=(lhs, InfinityWrapper<T>(rhs));
}

}

#endif /* MATH_INFINITYWRAPPER_HPP_ */
