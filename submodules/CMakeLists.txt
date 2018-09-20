project(pbqp-papa_submodules LANGUAGES CXX)

add_library(json STATIC json/single_include/nlohmann/json.hpp)
set_target_properties(json PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(json PUBLIC json/single_include)
