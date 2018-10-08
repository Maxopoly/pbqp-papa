#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE InfinityWrapperTests
#include <boost/test/unit_test.hpp>

#include "math/InfinityWrapper.hpp"

namespace pbqppapa {


BOOST_AUTO_TEST_CASE(infinityLimitTest) {

	InfinityWrapper<unsigned int> inful = InfinityWrapper<unsigned int>::getInfinite();
	InfinityWrapper<unsigned int> inful2 (4294967295);
	BOOST_CHECK(inful == inful2);
	BOOST_CHECK(inful.isInfinite());
	BOOST_CHECK(inful2.isInfinite());
	BOOST_TEST_MESSAGE(std::to_string(inful.getValue()));
	BOOST_TEST_MESSAGE(std::to_string(inful2.getValue()));
}


}
