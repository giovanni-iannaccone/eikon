BINARY_NAME := eikon

CXX := g++

CXXFLAGS := -std=c++23 -fPIC -shared -O3 -Wall -Wextra

SRC_DIR := src
BUILD_DIR := build

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

SHARED_EXT := so
LIB_PREFIX := lib

INSTALL_LIBDIR := /usr/lib
INSTALL_INCLUDEDIR := /usr/include

TARGET := $(BUILD_DIR)/$(LIB_PREFIX)$(BINARY_NAME).$(SHARED_EXT)

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

debug: CXXFLAGS := -std=c++23 -fPIC -shared -O0 -g -pg -Wall -Wextra 
debug: clean all

clean:
	rm -rf $(BUILD_DIR)

install: CXXFLAGS += -O3
install: $(TARGET)
	mkdir -p "$(INSTALL_LIBDIR)" "$(INSTALL_INCLUDEDIR)/$(BINARY_NAME)"
	install -m 755 "$(TARGET)" "$(INSTALL_LIBDIR)/"
	install -m 644 include/*.hpp "$(INSTALL_INCLUDEDIR)/$(BINARY_NAME)/"

.PHONY: all clean install debug
