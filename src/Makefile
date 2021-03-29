UNAME := $(shell uname -s)
CC = g++
OBJ = game.o astro.o
LDFLAGS = -lGL -lGLU -lglut

game : $(OBJ)
	$(CC) -o game $(OBJ) $(LDFLAGS)

game.o : game.cpp headers.h
	$(CC) -c -g game.cpp

astro.o : astro.cpp headers.h
	$(CC) -c -g astro.cpp

clean :
	rm $(OBJ)
