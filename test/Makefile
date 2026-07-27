BINARY_NAME := eikon_test

CXX := g++

CXXFLAGS := -std=c++23
CXXFLAGS += -Iinclude

LDFLAGS := -leikon

SRC_DIR := src
OBJ_DIR := build
BIN_DIR := bin

TARGET := $(BIN_DIR)/$(BINARY_NAME)

SRCS := $(wildcard $(SRC_DIR)/*.cpp) test.cpp
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: CXXFLAGS += -O3
all: $(TARGET)

debug: CXXFLAGS += -O0 -g -pg
debug: clean $(TARGET)

$(TARGET): $(OBJS)
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all debug clean
