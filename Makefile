TARGET= hex109
CC= g++
CFLAGS= -std=c++0x -O3 -Wall -Wextra
DEPS = game.h player.h hexBoard.h hexGraph.h
OBJ = main.o game.o player.o hexGraph.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o
	rm -f $(TARGET)

.PHONY: default all clean

