TARGET= asg4
CC= g++
CFLAGS= -ggdb
DEPS = graph.h game.h player.h hexGraph.h
OBJ = main.o graph.o graph_algorithm.o game.o player.o hexGraph.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o
	rm -f $(TARGET)

.PHONY: default all clean

