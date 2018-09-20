project(pbqp-papa_src LANGUAGES CXX)

add_library(pbqp-io STATIC io/TypeSerializerFactory.cpp)
target_link_libraries(pbqp-io core_pbqp_representation)
set_target_properties(pbqp-io PROPERTIES LINKER_LANGUAGE CXX)
