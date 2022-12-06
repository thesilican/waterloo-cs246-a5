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
ifeq ($(shell uname), Darwin)
FASTFLAGS := -std=c++14 -O3 -Wall -lX11 -I /opt/homebrew/include -L /opt/homebrew/lib
else
FASTFLAGS := -std=c++14 -O3 -Wall -lX11
endif

BIN_PATH := bin
OBJ_PATH := obj
SRC_PATH := src

TARGET_NAME := chess
TARGET := $(BIN_PATH)/$(TARGET_NAME)
TARGET_FAST := $(BIN_PATH)/$(TARGET_NAME)fast

SRC := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.cc)))
HEAD := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.h)))
OBJ := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))

.PHONY: default
default: $(TARGET)
	cp $(TARGET) $(TARGET_NAME)

.PHONY: build
build: $(TARGET)

.PHONY: build-fast
build-fast: $(TARGET_FAST)

.PHONY: run
run: $(TARGET)
	./$(TARGET)

.PHONY: run-fast
run-fast: $(TARGET_FAST)
	./$(TARGET_FAST)

.PHONY: clean
clean:
	rm -rf $(BIN_PATH) $(OBJ_PATH) $(TARGET_NAME)

$(TARGET): $(OBJ)
	mkdir -p $(BIN_PATH)
	$(CXX) $(OBJ) $(LINKFLAGS) -o $(TARGET)

$(TARGET_FAST): $(SRC) $(HEAD)
	mkdir -p $(BIN_PATH)
	$(CXX) $(SRC) $(FASTFLAGS) -o $(TARGET_FAST)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cc $(HEAD)
	mkdir -p $(OBJ_PATH)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

test: $(TARGET)
	test/test.py
