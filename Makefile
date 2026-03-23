CXX = g++
CXXFLAGS = -W -Wall -Wextra -std=c++17
NAME = main
OBJS = main.cpp board.cpp

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(OBJS) -o $(NAME) $(CXXFLAGS)

clean:
	del /Q *.o $(NAME).exe 2>NUL || exit 0

.PHONY: all clean
