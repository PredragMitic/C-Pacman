GO = pacman
CC = gcc
FLAG = -lm
OGL = -lGL -lGLU -lglut

$(GO): main.o image.o
	$(CC) -o $@ $^ $(FLAG) $(OGL)

main.o: main.c image.h
	$(CC) -o $@ $< -c

image.o: image.c image.h
	$(CC) -o $@ $< -c

.PHONY:
	clean

clean:
	rm -rf $(GOAL) *.o
