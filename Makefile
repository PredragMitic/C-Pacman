GO = pacman
CC = gcc
FLAG = -lm
OGL = -lGL -lGLU -lglut

$(GO): main.o 
	$(CC) -o $@ $^ $(FLAG) $(OGL)

main.o: main.c 
	$(CC) -o $@ $< -c

.PHONY:
	clean

clean:
	rm -rf $(GOAL) *.o
