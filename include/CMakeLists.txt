project(pbqp-papa_includes LANGUAGES CXX)

add_library(core_pbqp_representation STATIC graph/Matrix.hpp graph/Vector.hpp graph/PBQPGraph.hpp graph/PBQPNode.hpp graph/PBQPSolution.hpp graph/PBQPEdge.hpp)
set_target_properties(core_pbqp_representation PROPERTIES LINKER_LANGUAGE CXX)