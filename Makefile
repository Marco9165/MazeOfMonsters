CC = gcc
CFLAGS = -Wall -Wextra -pedantic -O2
LIBS = -lncurses -lm

OBJS = main.c
DEPS = dictionary.h

all: MazeOfMonsters

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# Regra de compilação
MazeOfMonsters: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

# Regra clean
clean:
	rm -f *.o MazeOfMonsters
