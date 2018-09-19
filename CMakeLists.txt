cmake_minimum_required(VERSION 3.1...3.12)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
    cmake_policy(VERSION 3.12)
endif()

project(pbqp-papa VERSION 0.1
                  DESCRIPTION "Solver and analysing tool for Partitioned Boolean Quadratic Problems "
                  LANGUAGES CXX)
                  
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


add_library(core_pbqp_representation STATIC include/graph/Matrix.hpp include/graph/Vector.hpp include/graph/PBQPGraph.hpp include/graph/PBQPNode.hpp include/graph/PBQPSolution.hpp include/graph/PBQPEdge.hpp)
set_target_properties(core_pbqp_representation PROPERTIES LINKER_LANGUAGE CXX)

add_library(pbqp_io STATIC include/io/PBQP_Serializer.hpp include/io/TypeSerializer.hpp include/io/TypeSerializerFactory.hpp include/io/types/DoubleSerializer.hpp include/io/types/FloatSerializer.hpp include/io/types/UnsignedCharSerializer.hpp include/io/types/UnsignedLongIntSerializer.hpp include/io/types/UnsignedShortIntSerializer.hpp)
set_target_properties(pbqp_io PROPERTIES LINKER_LANGUAGE CXX)


add_library(pbqp_solving STATIC include/analysis/PBQPHandler.hpp include/reduction/ConnectedComponentSeparator.hpp include/reduction/DependentSolution.hpp include/reduction/EdgeDeduplicator.hpp include/reduction/NodeMerger.hpp include/reduction/PBQPReduction.hpp include/reduction/SelfCycleRemover.hpp include/reduction/degree/DegreeOneReductor.hpp include/reduction/degree/DegreeTwoReductor.hpp include/reduction/degree/DegreeZeroReductor.hpp include/reduction/heuristic/DegreeNReductor.hpp include/reduction/vectordegree/VektorDegreeZeroReductor.hpp include/reduction/vectordegree/VektorDegreeOneReductor.hpp)
set_target_properties(pbqp_solving PROPERTIES LINKER_LANGUAGE CXX)


find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

add_executable(graphTests test/graph/GraphTests.cpp)
add_test(graphTests graphTests)


target_link_libraries(graphTests
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
		core_pbqp_representation
                      )

