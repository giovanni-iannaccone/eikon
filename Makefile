CXX = g++

BIN_DIR = bin
BUILD_DIR = build
INCLUDE_DIR = include

INCLUDES = $(INCLUDE_DIR)/raccoon.hpp raccoon_cli.cpp
OBJS = $(INCLUDES:%.cpp=$(BUILD_DIR)/%.o)

TARGET = $(BIN_DIR)/raccoon_cli

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $^ -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean

