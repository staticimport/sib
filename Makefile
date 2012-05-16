GLOBAL_DEPENDENCIES = Makefile

# Compiler
CXX := $(shell which ccache 2>/dev/null) g++

# C++ Main Source Files
MAIN_CXX_SOURCE_ROOT_DIRS = src/main
MAIN_CXX_HEADER_FILES := $(foreach dir,$(MAIN_CXX_SOURCE_ROOT_DIRS),$(shell find $(dir) -name "*.hpp"))
MAIN_CXX_ALL_COMPILABLE_FILES := $(foreach dir,$(MAIN_CXX_SOURCE_ROOT_DIRS),$(shell find $(dir) -name "*.cpp"))
#MAIN_CXX_IMPLEMENTATION_FILES := $(wildcard $(MAIN_CXX_HEADER_FILES:.hpp=.cpp))
MAIN_CXX_IMPLEMENTATION_FILES = $(MAIN_CXX_ALL_COMPILABLE_FILES)

# C++ Test Source Files
TEST_CXX_SOURCE_ROOT_DIRS = src/test
TEST_CXX_HEADER_FILES := $(foreach dir,$(TEST_CXX_SOURCE_ROOT_DIRS),$(shell find $(dir) -name "*.hpp"))
TEST_CXX_ALL_COMPILABLE_FILES := $(foreach dir,$(TEST_CXX_SOURCE_ROOT_DIRS),$(shell find $(dir) -name "*.cpp"))
TEST_CXX_IMPLEMENTATION_FILES := $(wildcard $(TEST_CXX_HEADER_FILES:.hpp=.cpp))

# Build Destination Common
OBJECTS_ROOT_DIR = build

# Main Build Destination
MAIN_ALL_OBJECTS := $(addprefix $(OBJECTS_ROOT_DIR)/,$(subst src/,,$(MAIN_CXX_ALL_COMPILABLE_FILES:.cpp=.o)))
MAIN_IMPLEMENTATION_OBJECTS := $(addprefix $(OBJECTS_ROOT_DIR)/,$(subst src/,,$(MAIN_CXX_IMPLEMENTATION_FILES:.cpp=.o)))
MAIN_OBJECT_DIRS := $(sort $(dir $(MAIN_ALL_OBJECTS)))
MAIN_TARGET_LIBRARY = install/lib/libsib.a

# Main Build Destination
TEST_ALL_OBJECTS := $(addprefix $(OBJECTS_ROOT_DIR)/,$(subst src/,,$(TEST_CXX_ALL_COMPILABLE_FILES:.cpp=.o)))
TEST_IMPLEMENTATION_OBJECTS := $(addprefix $(OBJECTS_ROOT_DIR)/,$(subst src/,,$(TEST_CXX_IMPLEMENTATION_FILES:.cpp=.o)))
#TEST_IMPLEMENTATION_OBJECTS := $(addprefix $(OBJECTS_ROOT_DIR)/,$(subst src/,,$(TEST_CXX_IMPLEMENTATION_FILES:.cpp=.o)))
TEST_OBJECT_DIRS := $(sort $(dir $(TEST_ALL_OBJECTS)))

# Common Compilation Flags
DEBUGGING_FLAGS = -ggdb3
ENVIRONMENT_FLAGS = -pthread -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64
MISCELLANEOUS_FLAGS = -pipe 
OPTIMIZATION_FLAGS = -O3 -fno-strict-aliasing
WARNING_FLAGS = -Wall -Werror

# Main Compilation Flags
MAIN_INCLUDE_PATH_FLAGS = -I$(BOOST_ROOT)/include -Isrc/main #$(foreach dir,$(MAIN_CXX_IMPLEMENTATION_FILES),-I$(dir))
MAIN_CXX_FLAGS = $(WARNING_FLAGS) $(OPTIMIZATION_FLAGS) $(DEBUGGING_FLAGS) \
                $(ENVIRONMENT_FLAGS) $(MISCELLANEOUS_FLAGS) \
                $(MAIN_INCLUDE_PATH_FLAGS)
main_cxx_flags_with_deps = $(MAIN_CXX_FLAGS) -MMD -MF $(1:.o=.d) -MT $1

# Test Compilation Flags
TEST_INCLUDE_PATH_FLAGS = -I$(GTEST_ROOT)/include -Iinstall/include $(foreach dir,$(TEST_CXX_SOURCE_ROOT_DIRS),-I$(dir))
TEST_CXX_FLAGS = $(WARNING_FLAGS) $(OPTIMIZATION_FLAGS) $(DEBUGGING_FLAGS) \
                 $(ENVIRONMENT_FLAGS) $(MISCELLANEOUS_FLAGS) \
                 $(TEST_INCLUDE_PATH_FLAGS)
test_cxx_flags_with_deps = $(TEST_CXX_FLAGS) -MMD -MF $(1:.o=.d) -MT $1

# Test Linking Flags
TEST_LIBRARY_PATH_FLAGS = -L$(GTEST_ROOT)/lib -Linstall/lib
TEST_LIBRARY_LINK_FLAGS = -lgtest -lsib

all: main test_run

main: main_init main_compile $(MAIN_TARGET_LIBRARY) main_install

test: main test_init test_compile $(TEST_TARGET_LIBRARY) test_install test_run

clean:
	rm -rf build/* install/*

main_init: | check_main_env
	mkdir -p $(MAIN_OBJECT_DIRS) install/include/internal install/lib

check_main_env:
	@if test "$(BOOST_ROOT)" = ""; then \
	  echo "Environment variable BOOST_ROOT not set!"; \
	  exit 1; \
	fi

main_compile: $(MAIN_ALL_OBJECTS) | main_init

$(OBJECTS_ROOT_DIR)/main/%.o: $(GLOBAL_DEPENDENCIES)
	$(CXX) $(call main_cxx_flags_with_deps,$@) -c src/main/$*.cpp -o $@

$(MAIN_TARGET_LIBRARY): $(MAIN_IMPLEMENTATION_OBJECTS)
	ar rcs $(MAIN_TARGET_LIBRARY) $(MAIN_IMPLEMENTATION_OBJECTS)

main_install: | main_compile
	cp src/main/*.hpp install/include
	find src/main/internal -name "*.hpp" | xargs -I {} cp {} install/include/internal
	find src/main/internal -name "*.inl" | xargs -I {} cp {} install/include/internal

test_init: main_install check_test_env
	echo $(TEST_IMPLEMENTATION_OBJECTS)
	mkdir -p $(TEST_OBJECT_DIRS) bin

check_test_env:
	@if test "$(GMOCK_ROOT)" = ""; then \
	  echo "Environment variable GMOCK_ROOT not set!"; \
	  exit 1; \
	fi
	@if test "$(GTEST_ROOT)" = ""; then \
	  echo "Environment variable GTEST_ROOT not set!"; \
	  exit 1; \
	fi

test_compile: $(TEST_ALL_OBJECTS) | test_init

$(OBJECTS_ROOT_DIR)/test/%.o: test_init
	$(CXX) $(call test_cxx_flags_with_deps,$@) -c src/test/$*.cpp -o $@

$(TEST_TARGET_LIBRARY): $(TEST_ALL_OBJECTS)
	ar rcs $(TEST_TARGET_LIBRARY) $(TEST_IMPLEMENTATION_OBJECTS)

bin/test-all: test_compile
	$(CXX) $(TEST_CXX_FLAGS) $(TEST_LIBRARY_PATH_FLAGS) $(TEST_LIBRARY_LINK_FLAGS) $(TEST_ALL_OBJECTS) -o bin/test-all

test_run: bin/test-all
	./bin/test-all

%.d:

.PHONY: all main test clean main_init main_compile main_install test_init test_compile test_install test_run check_test_env check_main_env

define depends_include_template
-include $(1)

endef

$(eval $(foreach dep,$(wildcard $(ALL_OBJECTS:.o=.d)),$(call depends_include_template,$(dep))))


# DO NOT DELETE
