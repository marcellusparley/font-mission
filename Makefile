SRC = $(wildcard src/*cpp)
OBJ = $(SRC:.cpp=.o)
CC = g++
CXXFLAGS = -w -std=c++14
LINKER_FLAGS = -lncurses
NAME = test.out

$(NAME): $(OBJ)
	$(CC) -o $@ $^ $(LINKER_FLAGS)

.PHONY: clean
clean:
	rm -f $(OBJ) myprog
