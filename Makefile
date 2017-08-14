CC = gcc 

all: test.c
	$(CC) test.c -o test -lglut -lGL -lGLU

clean:
	rm test
