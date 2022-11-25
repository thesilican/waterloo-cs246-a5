CXX := g++
ifeq ($(uname), Darwin)
CXXFLAGS := -std=c++14 -Wall -I /opt/homebrew/include -L /opt/homebrew/lib
else
CXXFLAGS := -std=c++14 -Wall
endif

BIN_PATH := bin
OBJ_PATH := obj
SRC_PATH := src

TARGET_NAME := chess
TARGET := ${BIN_PATH}/${TARGET_NAME}

SRC := ${foreach x, ${SRC_PATH}, ${wildcard ${addprefix ${x}/*,.cc}}}
OBJ := ${addprefix ${OBJ_PATH}/, ${addsuffix .o, ${notdir ${basename ${SRC}}}}}

.PHONY: all
all: ${TARGET}

.PHONY: run
run: all
	./${TARGET}

.PHONY: clean
clean:
	rm -f ${TARGET} ${BIN_PATH}/* ${OBJ_PATH}/*

${TARGET}: ${OBJ}
	${CXX} ${CXXFLAGS} -o ${TARGET} ${OBJ}

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<
