CXX = g++
CXXFLAGS = -Wall -g -fPIC -std=c++20

LDFLAGS = -shared
LIBS = -leikon

OBJ_DIR = build
OUT_DIR = bin

SRC = $(wildcard *.cpp)
OBJ = $(SRC:%.cpp=$(OBJ_DIR)/%.o)

OUTPUT = $(OUT_DIR)/libeikon_wrap.so

all: $(OUTPUT)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

$(OBJ_DIR)/%.o: %.cpp | $(OUT_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OUTPUT): $(OBJ)
	mkdir -p $(OBJ_DIR)
	$(CXX) $(LDFLAGS) $(OBJ) -o $(OUTPUT) $(LIBS)

clean:
	rm -rf $(OBJ_DIR)/*.o $(OUTPUT)

install: $(OUTPUT)
	sudo cp $(OUTPUT) /usr/lib

.PHONY: all clean install
