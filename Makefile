CC := gcc
FLAGS := -Wall -g3 -Werror
all: shell.o main.o
	$(CC) $(FLAGS) main.o shell.o -o w4118_sh
main.o: main.c
	$(CC) $(FLAGS) -c main.c -o main.o
shell.o: shell.h shell.c
	$(CC) $(FLAGS) -c shell.c -o shell.o
clean:
	rm *.o w4118_sh