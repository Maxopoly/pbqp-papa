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

add_executable(degreeOneReductorTest reduction/DegreeOneReductorTests.cpp)
add_test(NAME degreeOneReductorTest COMMAND $<TARGET_FILE:degreeOneReductorTest> --output_format=HRF --log_level=message --catch_system_error=yes --detect_memory_leaks=1)
target_link_libraries(degreeOneReductorTest ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} core_pbqp_representation)

add_executable(degreeTwoReductorTest reduction/DegreeTwoReductorTests.cpp)
add_test(NAME degreeTwoReductorTest COMMAND $<TARGET_FILE:degreeTwoReductorTest> --output_format=HRF --log_level=message --catch_system_error=yes --detect_memory_leaks=1)
target_link_libraries(degreeTwoReductorTest ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} core_pbqp_representation)

add_executable(degreeZeroReductorTest reduction/DegreeZeroReductorTests.cpp)
add_test(NAME degreeZeroReductorTest COMMAND $<TARGET_FILE:degreeZeroReductorTest> --output_format=HRF --log_level=message --catch_system_error=yes --detect_memory_leaks=1)
target_link_libraries(degreeZeroReductorTest ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} core_pbqp_representation)

add_executable(bruteForceSolverTest solve/BruteForceSolverTests.cpp)
add_test(NAME bruteForceSolverTest COMMAND $<TARGET_FILE:bruteForceSolverTest> --output_format=HRF --log_level=message --catch_system_error=yes --detect_memory_leaks=1)
target_link_libraries(bruteForceSolverTest ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} core_pbqp_representation)


