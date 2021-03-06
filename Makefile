CC = g++
CFLAGS = -g -c -Wall -std=c++11
LFLAGS = -g -Wall -std=c++11

all: tgaimage.o render.o geometry.o
	$(CC) $(LFLAGS) main.cpp geometry.o render.o tgaimage.o -o renderer

tgaimage.o: tgaimage.cpp tgaimage.h
	$(CC) $(CFLAGS) tgaimage.cpp -o $@

render.o: render.cpp render.h
	$(CC) $(CFLAGS) render.cpp -o $@

geometry.o: geometry.cpp geometry.h
	$(CC) $(CFLAGS) geometry.cpp -o $@

clean:
	rm *.o renderer output.tga
