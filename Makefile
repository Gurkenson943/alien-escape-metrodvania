CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lm
SRCS = src/main.c src/game.c src/player.c src/world.c src/npc.c src/boss.c src/collision.c src/rendering.c
OBJS = $(SRCS:.c=.o)
TARGET = alien-escape

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET) savegame.dat

.PHONY: all run clean
