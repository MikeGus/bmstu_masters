CXX = clang++
CXX_VER = -std=c++2a
CXXFLAGS = $(CXX_VER) -Wall -Werror -g0 -O3 -I/usr/local/include
LFLAGS = -lconfig++ -L/usr/local/lib
SOURCES = src/main.cpp src/arguments_parser.cpp src/classificator.cpp src/feature.cpp src/rule_list.cpp src/rule_tree.cpp src/sample.cpp
HEADERS = $(SOURCES:.cpp=.h)
OBJ = $(SOURCES:.cpp=.o)

PROG = classificator

all: $(PROG)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

classificator: $(OBJ)
	$(CXX) $^ -o $@ $(LFLAGS)

clean:
	rm -r src/*.o
