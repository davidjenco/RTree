CXXFLAGS=-Wall -pedantic -std=c++2a -g

HEADERS = $(wildcard src/*.h)
SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:src/%.cpp=build/%.o)
PROGRAM = rtree

.PHONY: all
all: compile

.PHONY: run
run: compile
	./$(PROGRAM)

.PHONY: compile
compile: $(PROGRAM)

$(PROGRAM): build/main
	cp $< $@

build/main: $(OBJECTS)
	mkdir -p $(@D)
	g++ $^ -o $@

build/%.o: src/%.cpp
	mkdir -p $(@D)
	g++ $(CXXFLAGS) $< -c -o $@

.PHONY: clean
clean:
	rm -rf $(PROGRAM) build/ res/
