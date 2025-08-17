CXX = g++
CXXFLAGS = -Wall -Wextra -O2 -fPIC

BIN_DIR = bin
BUILD_DIR = build

LIBNAME = libraccoon.so
LIBRARY = $(BUILD_DIR)/$(LIBNAME)
PREFIX = /usr

INCLUDE_DIR = include
SRC_DIR = src

CLI_DIR = cli
TEST_DIR = test

INCLUDES := $(wildcard $(INCLUDE_DIR)/*.hpp)
SRCS := $(patsubst $(INCLUDE_DIR)/%.hpp, $(SRC_DIR)/%.cpp, $(INCLUDES))
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

CLI_TARGET := $(BIN_DIR)/raccoon_cli

.PHONY: clean cli install test

$(LIBRARY): $(OBJS)
	$(CXX) $(CXXFLAGS) -shared $^ -o $@

$(OBJS): $(SRCS)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

install: $(LIBRARY)
	mv $(LIBRARY)  $(PREFIX)/lib
	cp $(INCLUDES) $(PREFIX)/include

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

