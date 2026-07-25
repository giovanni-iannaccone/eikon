BINARY_NAME := eikon_test

CXX := g++
CXXFLAGS := -std=c++23

LDFLAGS := -leikon

SRC_DIR := .
BIN_DIR := bin

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
TARGET := $(BIN_DIR)/$(BINARY_NAME)

all: $(TARGET)

debug: CXXFLAGS += -O0 -g -pg
debug: clean all

$(TARGET): $(SRCS)
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean debug
