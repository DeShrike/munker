CC = gcc
LIBS = -lpng -lm
FLAGS = -O3 -Wall

all: munker-white

munker-white: munker-white.o pnghelper.o
	$(CC) munker-white.o pnghelper.o -o munker-white $(LIBS)

pnghelper.o: pnghelper.c pnghelper.h
	$(CC) -c $< $(FLAGS)

munker-white.o: munker-white.c pnghelper.h
	$(CC) -c $< $(FLAGS)

clean:
	rm -v *.o munker-white
