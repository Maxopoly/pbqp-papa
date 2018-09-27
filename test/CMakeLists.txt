project(pbqp-papa_tests LANGUAGES CXX)

find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

SET(GCC_COVERAGE_COMPILE_FLAGS "-std=c++11 -Wall -Wextra -g")

add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

add_executable(graphTests graph/GraphTests.cpp)
add_test(NAME graphTests COMMAND $<TARGET_FILE:graphTests> --output_format=HRF --log_level=message --catch_system_error=yes --detect_memory_leaks=1)
target_link_libraries(graphTests ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} core_pbqp_representation)

add_executable(matrixTest graph/MatrixTests.cpp)
add_test(NAME matrixTest COMMAND $<TARGET_FILE:matrixTest> --output_format=HRF --log_level=message --catch_system_error=yes --detect_memory_leaks=1)
target_link_libraries(matrixTest ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} core_pbqp_representation)

add_executable(pbqpSolutionTest graph/PBQPSolutionTests.cpp)
add_test(NAME pbqpSolutionTest COMMAND $<TARGET_FILE:pbqpSolutionTest> --output_format=HRF --log_level=message --catch_system_error=yes --detect_memory_leaks=1)
target_link_libraries(pbqpSolutionTest ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} core_pbqp_representation)

add_executable(connectedComponentSeparatorTest reduction/ConnectedComponentSeparatorTests.cpp)
add_test(NAME connectedComponentSeparatorTest COMMAND $<TARGET_FILE:connectedComponentSeparatorTest> --output_format=HRF --log_level=message --catch_system_error=yes --detect_memory_leaks=1)
target_link_libraries(connectedComponentSeparatorTest ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} core_pbqp_representation)

add_executable(degreeOneReducerTest reduction/DegreeOneReducerTests.cpp)
add_test(NAME degreeOneReducerTest COMMAND $<TARGET_FILE:degreeOneReducerTest> --output_format=HRF --log_level=message --catch_system_error=yes --detect_memory_leaks=1)
target_link_libraries(degreeOneReducerTest ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} core_pbqp_representation)

add_executable(degreeTwoReducerTest reduction/DegreeTwoReducerTests.cpp)
add_test(NAME degreeTwoReducerTest COMMAND $<TARGET_FILE:degreeTwoReducerTest> --output_format=HRF --log_level=message --catch_system_error=yes --detect_memory_leaks=1)
target_link_libraries(degreeTwoReducerTest ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} core_pbqp_representation)

add_executable(degreeZeroReducerTest reduction/DegreeZeroReducerTests.cpp)
add_test(NAME degreeZeroReducerTest COMMAND $<TARGET_FILE:degreeZeroReducerTest> --output_format=HRF --log_level=message --catch_system_error=yes --detect_memory_leaks=1)
target_link_libraries(degreeZeroReducerTest ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} core_pbqp_representation)

add_executable(bruteForceSolverTest solve/BruteForceSolverTests.cpp)
add_test(NAME bruteForceSolverTest COMMAND $<TARGET_FILE:bruteForceSolverTest> --output_format=HRF --log_level=message --catch_system_error=yes --detect_memory_leaks=1)
target_link_libraries(bruteForceSolverTest ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} core_pbqp_representation)

add_executable(serializerTests io/PBQP_SerializerTests.cpp)
add_test(NAME serializerTests COMMAND $<TARGET_FILE:serializerTests> --output_format=HRF --log_level=message --catch_system_error=yes --detect_memory_leaks=1)
target_link_libraries(serializerTests ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} core_pbqp_representation json pbqp-io)

