CXX = g++

BIN_DIR = bin
BUILD_DIR = build
INCLUDE_DIR = include
LIB_DIR = lib
TEST_DIR = test

LIBS = $(LIB_DIR)/raccoon.cpp $(LIB_DIR)/matrix_utils.cpp $(TEST_DIR)/test.cpp
OBJS = $(LIBS:%.cpp=$(BUILD_DIR)/%.o)

TARGET = $(BIN_DIR)/test_raccoon

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

