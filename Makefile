CXX = g++
CXXFLAGS = -W -Wall -Wextra -std=c++17
NAME = main
TEST_READ = test_read_FEN
TEST_WRITE = test_write_FEN

all: $(NAME) $(TEST_READ) $(TEST_WRITE)

$(NAME): main.cpp board.cpp view.cpp mask.cpp
	$(CXX) main.cpp board.cpp view.cpp mask.cpp -o $(NAME) $(CXXFLAGS)

$(TEST_READ): test_read_FEN.cpp board.cpp view.cpp mask.cpp
	$(CXX) test_read_FEN.cpp board.cpp view.cpp mask.cpp -o $(TEST_READ) $(CXXFLAGS)

$(TEST_WRITE): test_write_FEN.cpp board.cpp view.cpp mask.cpp
	$(CXX) test_write_FEN.cpp board.cpp view.cpp mask.cpp -o $(TEST_WRITE) $(CXXFLAGS)

clean:
	del /Q *.o $(NAME).exe $(TEST_READ).exe $(TEST_WRITE).exe 2>NUL || exit 0

re:
	make clean
	make all

.PHONY: all clean re
