ROPE_PREFIX=./ropebwt2
HTS_PREFIX=./htslib
ABPOA_PREFIX=./abPOA
RF_PREFIX=./rapidfuzz-cpp
INTTREE_PREFIX=./interval-tree
PARASAIL_PREFIX=./parasail
LIBDEFLATE_PREFIX=./libdeflate
CXX=g++
CXXFLAGS=-std=c++14 -Wall -Wno-comment -Wno-unused-function -Wno-narrowing -g -O0 -I$(ROPE_PREFIX) -I$(RF_PREFIX) -I$(HTS_PREFIX) -I$(ABPOA_PREFIX)/include -I$(PARASAIL_PREFIX) -I$(INTTREE_PREFIX) -fopenmp -Wno-sign-compare -Wno-unused-variable
LIBS=-L$(HTS_PREFIX) -L$(ABPOA_PREFIX)/lib -L$(LIBDEFLATE_PREFIX) -L$(LIBDEFLATE_PREFIX)/lib -ldeflate -L$(PARASAIL_PREFIX)/build -lparasail -lm -lpthread -lz -lhts -labpoa
BIN=$(ROPE_PREFIX)/mrope.o $(ROPE_PREFIX)/rope.o $(ROPE_PREFIX)/rld0.o $(ROPE_PREFIX)/rle.o

HPP = $(wildcard *.hpp)
SRC = $(wildcard *.cpp)
OBJS = $(BIN) $(SRC:.cpp=.o)

LOCAL_BUILD ?= 0
ifneq ($(LOCAL_BUILD), 0)
	HPP += $(wildcard src/*.hpp)
	SRC += $(wildcard src/*.cpp)
	CXX += -Isrc -DLOCAL_BUILD
endif

$(info $(LOCAL_BUILD))

all: SVDSS
debug: CXXFLAGS += -DDEBUG_MODE -g
debug: PingPong

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

SVDSS: $(OBJS) $(HPP)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LIBS)

clean:
	rm -f SVDSS *.o
.PHONY: clean

