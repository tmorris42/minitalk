all:
	gcc playground.c -o playground
	gcc -fsanitize=address main.c -o server
