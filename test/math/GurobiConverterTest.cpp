#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE GurobiConverterTests
#include <boost/test/unit_test.hpp>

#include "math/GurobiConverter.hpp"
#include "io/PBQP_Serializer.hpp"

namespace pbqppapa {


BOOST_AUTO_TEST_CASE(empty) {
	PBQP_Serializer<InfinityWrapper<unsigned int>> serial;
	PBQPGraph<InfinityWrapper<unsigned int>>* ogGraph = serial.loadFromFile("test/testData/normalLibfirmGraph.json");
	GurobiConverter<unsigned int> gConv;
	gConv.convert(ogGraph);
}
}

