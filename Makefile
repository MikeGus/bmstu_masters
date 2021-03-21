CXX = g++
CXX_VER = -std=c++2a
CXXFLAGS = -Wall -Werror -pedantic -g0 -O3 -Wno-c++11-long-long $(CXX_VER)
LFLAGS = -lconfig++

SOURCES = src/main.c src/arguments_parser.cpp
HEADERS = $(SOURCES:.c=.h)
OBJ = $(SOURCES:.c=.o)

PROG = classificator

all: $(PROG)

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

classificator: $(OBJ)
	$(CXX) $^ -o $@ $(LFLAGS)

clean:
	rm -r src/*.o
