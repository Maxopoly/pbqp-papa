#include "math/InfinityWrapper.hpp"
#include <limits.h>

namespace pbqppapa {

template<> unsigned long InfinityWrapper<unsigned long>::magicInfiniteNumber = ULONG_MAX;
template<> unsigned int InfinityWrapper<unsigned int>::magicInfiniteNumber = UINT_MAX;
template<> unsigned short InfinityWrapper<unsigned short>::magicInfiniteNumber = USHRT_MAX;

}
