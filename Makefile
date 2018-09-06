CXX ?= g++

#Based on a template from https://gist.github.com/mauriciopoppe/de8908f67923091982c8c8136a063ea6

# path #
SRC_PATH = src
BUILD_PATH = build
TEST_PATH = test
TEST_BUILD_PATH = test_build

# extensions #
SRC_EXT = cpp
TEST_EXEC = runtest

# code lists #
# Find all source files in the source directory, sorted by
# most recently modified
SOURCES = $(shell find $(SRC_PATH) -name '*.$(SRC_EXT)' | sort -k 1nr | cut -f2-)
#Find all test source files
TESTS = $(shell find $(TEST_PATH) -name '*.$(SRC_EXT)')
# Set the object file names, with the source directory stripped
# from the path, and the build path prepended in its place
OBJECTS = $(SOURCES:$(SRC_PATH)/%.$(SRC_EXT)=$(BUILD_PATH)/%.o)
# Set the object file names, with the source directory stripped
# from the path, and the build path prepended in its place
TEST_OBJECTS = $(TESTS:$(TEST_PATH)/%.$(SRC_EXT)=$(TEST_BUILD_PATH)/%.o)
# Set the dependency files that will be used to add header dependencies
DEPS = $(OBJECTS:.o=.d)

# flags #
COMPILE_FLAGS = -std=c++11 -Wall -Wextra -g
INCLUDES = -I include/ -I submodules/json/single_include/nlohmann/json.hpp
# Space-separated pkg-config libraries used by this project
LIBS =

.PHONY: default_target
default_target: release

.PHONY: release
release: export CXXFLAGS := $(CXXFLAGS) $(COMPILE_FLAGS)
release: dirs
	@$(MAKE) test

.PHONY: dirs
dirs:
	@echo "Creating directories"
	@mkdir -p $(dir $(TEST_OBJECTS))
	@mkdir -p $(TEST_BUILD_PATH)

.PHONY: clean
clean:
	@echo "Deleting directories"
	@$(RM) -r $(BUILD_PATH)
	@$(RM) -r $(TEST_BUILD_PATH)

#Builds and runs tests
.PHONY: test
test: dirs
test: export CXXFLAGS := $(CXXFLAGS) $(COMPILE_FLAGS)
test: $(TEST_OBJECTS) $(addsuffix $(TEST_EXEC),$(TEST_OBJECTS))

#Run tests
$(TEST_BUILD_PATH)/%.o$(TEST_EXEC): $(TEST_BUILD_PATH)/%.o
	@echo "Running test: $^"
	@./$^ --output_format=XML --log_level=message > $(^)-report.xml
	
#Compile tests
$(TEST_BUILD_PATH)/%.o: $(TEST_PATH)/%.$(SRC_EXT) $(OBJECTS)
	@echo "Compiling: $< -> $@"
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(INCLUDES) -o $@ $^ -lboost_unit_test_framework
	
# Source file rules
# After the first compilation they will be joined with the rules from the
# dependency files to provide header dependencies
$(BUILD_PATH)/%.o: $(SRC_PATH)/%.$(SRC_EXT)
	@echo "Compiling: $< -> $@"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MP -MMD -c $< -o $@
	
# Add dependency files, if they exist
-include $(DEPS)
