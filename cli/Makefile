BINARY_NAME := eikoncli

CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++23 -fPIC

LDFLAGS := -leikon

TARGET := $(BINARY_NAME)

SRC_DIR := src
BUILD_DIR := build

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

all: $(TARGET)

debug: CXXFLAGS += -O0 -g -pg
debug: clean all

release: CXXFLAGS += -03
release: clean all

$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< $(LDFLAGS) -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

install: $(TARGET)
	mkdir -p $(INSTALL_LIBDIR)
	mkdir -p $(INSTALL_INCLUDEDIR)/$(BINARY_NAME)

	cp $(TARGET) $(INSTALL_LIBDIR)/
	cp include/*.hpp include/*.h $(INSTALL_INCLUDEDIR)/$(BINARY_NAME)/

.PHONY: all clean install debug
