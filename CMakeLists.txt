cmake_minimum_required(VERSION 3.1...3.12)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
    cmake_policy(VERSION 3.12)
endif()

project(pbqp-papa VERSION 0.1
                  DESCRIPTION "Solver and analysing tool for Partitioned Boolean Quadratic Problems "
                  LANGUAGES CXX)
                  

enable_testing()
                  
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(submodules)

target_include_directories(core_pbqp_representation PUBLIC include)


add_library(pbqp_io STATIC include/io/PBQP_Serializer.hpp include/io/TypeSerializer.hpp include/io/TypeSerializerFactory.hpp include/io/types/DoubleSerializer.hpp include/io/types/FloatSerializer.hpp include/io/types/UnsignedCharSerializer.hpp include/io/types/UnsignedLongIntSerializer.hpp include/io/types/UnsignedShortIntSerializer.hpp)
set_target_properties(pbqp_io PROPERTIES LINKER_LANGUAGE CXX)


add_library(pbqp_solving STATIC include/analysis/PBQPHandler.hpp include/reduction/ConnectedComponentSeparator.hpp include/reduction/DependentSolution.hpp include/reduction/EdgeDeduplicator.hpp include/reduction/NodeMerger.hpp include/reduction/PBQPReduction.hpp include/reduction/SelfCycleRemover.hpp include/reduction/degree/DegreeOneReductor.hpp include/reduction/degree/DegreeTwoReductor.hpp include/reduction/degree/DegreeZeroReductor.hpp include/reduction/heuristic/DegreeNReductor.hpp include/reduction/vectordegree/VektorDegreeZeroReductor.hpp include/reduction/vectordegree/VektorDegreeOneReductor.hpp)
set_target_properties(pbqp_solving PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(test)
