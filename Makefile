CXX = g++
CXXFLAGS = -Wall -Wextra -O2 -fPIC
LDFLAGS = -shared

BIN_DIR = bin
BUILD_DIR = build
INCLUDE_DIR = include
SRC_DIR = src
CLI_DIR = cli
TEST_DIR = test

LIBNAME = eikon
LIBRARY = $(BUILD_DIR)/lib$(LIBNAME).so
PREFIX = /usr

INCLUDES := $(wildcard $(INCLUDE_DIR)/*.hpp)
SRCS := $(patsubst $(INCLUDE_DIR)/%.hpp, $(SRC_DIR)/%.cpp, $(INCLUDES))
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

CLI_TARGET := $(BIN_DIR)/eikon_cli

.PHONY: clean cli install test

all: $(LIBRARY)

$(LIBRARY): $(OBJS)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDE_DIR)/%.hpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

install: $(LIBRARY)
	mkdir -p $(PREFIX)/lib
	mv $(LIBRARY)  $(PREFIX)/lib

	mkdir -p $(PREFIX)/include/$(LIBNAME)
	cp $(INCLUDES) $(PREFIX)/include/$(LIBNAME)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

cli:
	echo "CLI build not implemented yet."

test:
	echo "Test build not implemented yet."
