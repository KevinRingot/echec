CXX = g++
CXXFLAGS = -W -Wall -Wextra -std=c++17
NAME = main

all: $(NAME)

$(NAME): main.cpp board.cpp view.cpp mask.cpp
	$(CXX) main.cpp board.cpp view.cpp mask.cpp -o $(NAME) $(CXXFLAGS)

clean:
	del /Q *.o $(NAME).exe fen 2>NUL || exit 0

re:
	make clean
	make all

.PHONY: all clean re
