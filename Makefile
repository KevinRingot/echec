CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
TARGET = main
OBJS = main.o board.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

main.o: main.cpp board.hpp types.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

board.o: board.cpp board.hpp types.hpp
	$(CXX) $(CXXFLAGS) -c board.cpp

clean:
	del /Q $(OBJS) $(TARGET).exe 2>NUL || exit 0

.PHONY: all clean
