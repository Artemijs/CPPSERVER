OBJ_DIR     := build/objects

CC          = gcc
CFLAGS      = -Wall -g -std=c++11
LFLAGS     := -L/usr/lib -lstdc++ -lm
INCLUDE     := -Iinclude/

SRC         := $(wildcard src/*.cpp)

TARGET      := program.out

OBJECTS     := $(SRC:.c=.o)


MAIN        = program.out

.PHONY: depend clean

all:$(MAIN)
	@echo Simple compiler named program chas been compiled

$(MAIN):$(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) -o $(MAIN) $(OBJECTS) $(LFLAGS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	$(RM) *.0 *~ $(MAIN)

depend: $(SRC)
	makedepend $(INCLUDE) $^
