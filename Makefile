CC = g++
CFLAGS = -g -c -Wall -std=c++11
LFLAGS = -g -Wall -std=c++11

all: tgaimage.o render.o
	$(CC) $(LFLAGS) main.cpp  render.o tgaimage.o -o renderer

tgaimage.o: tgaimage.cpp tgaimage.h
	$(CC) $(CFLAGS) tgaimage.cpp -o $@

render.o: render.cpp render.h
	$(CC) $(CFLAGS) render.cpp -o $@

geomtest:
	$(CC) $(LFLAGS) geomTest.cpp -o geomTest
