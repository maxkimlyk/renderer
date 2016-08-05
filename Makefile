CC = g++
CFLAGS = -c -Wall
LFLAGS = -Wall

all: tgaimage.o
	$(CC) $(LFLAGS) main.cpp tgaimage.o -o renderer

tgaimage.o: tgaimage.cpp tgaimage.h
	$(CC) $(CFLAGS) tgaimage.cpp -o $@
