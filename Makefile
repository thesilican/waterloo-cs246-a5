CXX := g++
ifeq ($(shell uname), Darwin)
CXXFLAGS := -std=c++14 -Wall -I /opt/homebrew/include
else
CXXFLAGS := -std=c++14 -Wall
endif
ifeq ($(shell uname), Darwin)
LINKFLAGS := -lX11 -L /opt/homebrew/lib
else
LINKFLAGS := -lX11
endif

BIN_PATH := bin
OBJ_PATH := obj
SRC_PATH := src

TARGET_NAME := chess
TARGET := $(BIN_PATH)/$(TARGET_NAME)

SRC := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.cc)))
HEAD := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.h)))
OBJ := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))

.PHONY: all
all: $(TARGET)

.PHONY: run
run: all
	./$(TARGET)

.PHONY: clean
clean:
	rm -rf $(TARGET) $(BIN_PATH) $(OBJ_PATH)

$(TARGET): $(OBJ)
	mkdir -p $(BIN_PATH)
	$(CXX) $(OBJ) $(LINKFLAGS) -o $(TARGET)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cc $(HEAD)
	mkdir -p $(OBJ_PATH)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: debug-makefile
debug-makefile:
	@echo $(CXXFLAGS)

test: all
	test/test.py
