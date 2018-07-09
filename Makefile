PWD=$(CURDIR)
BUILD_DIR=$(PWD)/build
SRCDIR=$(PWD)/src
DIRS=$(SRCDIR) $(SRCDIR)/disposable
INCLUDE_DIR=$(PWD)/include
INCLUDE_DIRS=$(INCLUDE_DIR) $(INCLUDE_DIR)/ej
DIRS_BUILT=$(BUILD_DIR)/dirs_built

INCLUDES=$(foreach d,$(INCLUDE_DIRS),$(wildcard $(d)/*.h))
SOURCES=$(foreach d,$(DIRS),$(wildcard $(d)/*.c))
STRIPPED_SOURCES=$(patsubst $(SRCDIR)/%,%,$(SOURCES))
TESTDIR=$(PWD)/test
TESTDIRS=$(TESTDIR) $(TESTDIR)/disposable
TEST_BUILD_DIR=$(BUILD_DIR)/test
TEST_DIRS=$(filter-out $(TESTDIR), \
    $(patsubst $(TESTDIR)/%,$(TEST_BUILD_DIR)/%,$(TESTDIRS)))
TEST_SOURCES=$(foreach d,$(TESTDIRS),$(wildcard $(d)/*.cpp))
STRIPPED_TEST_SOURCES=$(patsubst $(TESTDIR)/%,%,$(TEST_SOURCES))
TEST_OBJECTS=$(patsubst %.cpp,$(TEST_BUILD_DIR)/%.o,$(STRIPPED_TEST_SOURCES))
TESTBIN=$(TEST_BUILD_DIR)/testji

CHECKED_BUILD_DIR=$(BUILD_DIR)/checked
CHECKED_DIRS=$(filter-out $(SRCDIR), \
	$(patsubst $(SRCDIR)/%,$(CHECKED_BUILD_DIR)/%,$(DIRS)))
CHECKED_OBJECTS= \
    $(patsubst %.c,$(CHECKED_BUILD_DIR)/%.o,$(STRIPPED_SOURCES))
DEBUG_BUILD_DIR=$(BUILD_DIR)/debug
DEBUG_DIRS=$(filter-out $(SRCDIR), \
	$(patsubst $(SRCDIR)/%,$(DEBUG_BUILD_DIR)/%,$(DIRS)))
DEBUG_OBJECTS= \
    $(patsubst %.c,$(DEBUG_BUILD_DIR)/%.o,$(STRIPPED_SOURCES))
RELEASE_BUILD_DIR=$(BUILD_DIR)/release
RELEASE_DIRS=$(filter-out $(SRCDIR), \
	$(patsubst $(SRCDIR)/%,$(RELEASE_BUILD_DIR)/%,$(DIRS)))
RELEASE_OBJECTS= \
    $(patsubst %.c,$(RELEASE_BUILD_DIR)/%.o,$(STRIPPED_SOURCES))
LIB_NAME=libej.a
CHECKED_LIB=$(CHECKED_BUILD_DIR)/$(LIB_NAME)
DEBUG_LIB=$(DEBUG_BUILD_DIR)/$(LIB_NAME)
RELEASE_LIB=$(RELEASE_BUILD_DIR)/$(LIB_NAME)

#Dependencies
GTEST_DIR=$(PWD)/contrib/googletest/googletest
GTEST_OBJ=$(TEST_BUILD_DIR)/gtest-all.o

COMMON_INCLUDES=-I $(PWD)/include
COMMON_CFLAGS=-std=c11 $(COMMON_INCLUDES)
COMMON_CXXFLAGS=-std=c++14 $(COMMON_INCLUDES)
CHECKED_CFLAGS=$(COMMON_CFLAGS) -O0 -fprofile-arcs -ftest-coverage
DEBUG_CFLAGS=  $(COMMON_CFLAGS) -O0 -g
RELEASE_CFLAGS=$(COMMON_CFLAGS) -O2
TEST_CXXFLAGS=$(COMMON_CXXFLAGS) -I $(GTEST_DIR) \
    -I $(GTEST_DIR)/include -O2

.PHONY: pre-build build-dirs all clean test
.SECONDARY: all

pre-build: build-dirs
	$(MAKE) all

build-dirs: $(DIRS_BUILT)

all: $(CHECKED_LIB) $(DEBUG_LIB) $(RELEASE_LIB)

clean:
	rm -rf $(BUILD_DIR)

$(CHECKED_LIB): $(CHECKED_OBJECTS)
	$(AR) rcs  $@ $(CHECKED_OBJECTS)

$(DEBUG_LIB): $(DEBUG_OBJECTS)
	$(AR) rcs  $@ $(DEBUG_OBJECTS)

$(RELEASE_LIB): $(RELEASE_OBJECTS)
	$(AR) rcs $@ $(RELEASE_OBJECTS)

$(CHECKED_BUILD_DIR)/%.o: $(INCLUDES) $(SRCDIR)/%.c
	$(CC) $(CHECKED_CFLAGS) -c -o $@ $(SRCDIR)/$*.c

$(DEBUG_BUILD_DIR)/%.o: $(INCLUDES) $(SRCDIR)/%.c
	$(CC) $(DEBUG_CFLAGS) -c -o $@ $(SRCDIR)/$*.c

$(RELEASE_BUILD_DIR)/%.o: $(INCLUDES) $(SRCDIR)/%.c
	$(CC) $(RELEASE_CFLAGS) -c -o $@ $(SRCDIR)/$*.c

$(TEST_BUILD_DIR)/%.o: $(INCLUDES) $(TESTDIR)/%.cpp
	$(CXX) $(TEST_CXXFLAGS) -c -o $@ $(TESTDIR)/$*.cpp

test: pre-build $(TESTBIN)
	$(TESTBIN)

$(GTEST_OBJ): $(GTEST_DIR)/src/gtest-all.cc
	$(CXX) $(TEST_CXXFLAGS) -c -o $@ $<

$(TESTBIN): $(CHECKED_OBJECTS) $(TEST_OBJECTS) $(GTEST_OBJ)
	find $(TEST_BUILD_DIR) -name "*.gcda" -exec rm {} \; -print
	rm -f gtest-all.gcda
	$(CXX) $(TEST_CXXFLAGS) -fprofile-arcs -o $@ $(TEST_OBJECTS) \
	    $(CHECKED_OBJECTS) $(GTEST_OBJ) -lpthread -lstdc++

$(DIRS_BUILT):
	mkdir -p $(BUILD_DIR) $(CHECKED_BUILD_DIR) $(DEBUG_BUILD_DIR) \
             $(RELEASE_BUILD_DIR) $(TEST_BUILD_DIR) $(CHECKED_DIRS) \
             $(DEBUG_DIRS) $(RELEASE_DIRS) $(TEST_DIRS)
	touch $(DIRS_BUILT)
