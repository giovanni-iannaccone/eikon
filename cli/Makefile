BINARY_NAME := eikoncli

CXX := g++
CXXFLAGS := -std=c++23 -fPIC

SRC_DIR := .
TARGET := 

SRCS := $(wildcard $(SRC_DIR)/*.cpp)

all: $(TARGET)

debug: CXXFLAGS += -O0 -g -pg
debug: clean all

release: CXXFLAGS += -03
release: clean all

$(TARGET): $(OBJS)
	$(CXX) $(SHARED_FLAGS) -o $@ $^

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

install: $(TARGET)
	mkdir -p $(INSTALL_LIBDIR)
	mkdir -p $(INSTALL_INCLUDEDIR)/$(BINARY_NAME)

	cp $(TARGET) $(INSTALL_LIBDIR)/
	cp include/*.hpp include/*.h $(INSTALL_INCLUDEDIR)/$(BINARY_NAME)/

.PHONY: all clean install debug
