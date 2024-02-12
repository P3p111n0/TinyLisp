CXX = g++
CFLAGS = -std=c++20 -Wall -pedantic -g -O2
LD = g++
LDFLAGS =
TARGET = TinyLisp

HEADERS = $(wildcard src/*.h src/*/*.h src/*/*/*.h src/*/*/*/*.h)
IMPLS = $(wildcard src/*.cpp src/*/*.cpp src/*/*/*.cpp src/*/*/*/*.cpp)
OBJS = $(patsubst %.cpp, build/%.o, $(IMPLS))
BUILD_DIR = $(dir $(OBJS))

.PHONY: all compile debug clean run

default: all

all: $(TARGET)

debug: CFLAGS += -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -ftrapv
debug: LDFLAGS += -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -ftrapv
debug: compile

compile: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

run: compile
	./$(TARGET)

build/%.o:
	$(CXX) $(CFLAGS) -c $< -o $@

build:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p build

ifeq (, $(filter $(MAKECMDGOALS), clean doc))
-include build/Makefile.d
endif

build/Makefile.d:  $(IMPLS) $(HEADERS) build
		$(foreach f, $(IMPLS), ${CXX} -MM  $(f) -MT $(patsubst %.cpp, build/%.o, $(f)) >> $@ ;)

clean:
	@rm -rf doc
	@rm -rf ${TARGET}
	@rm -rf build