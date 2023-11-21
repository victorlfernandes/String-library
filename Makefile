all:
	gcc *.c -o exec -g -Wall -Werror

run:
	./exec