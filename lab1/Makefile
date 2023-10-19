MYWARNINGS:=-Wall -Wextra -Wpedantic

main: main.o node.o file.o test
	clang-format -i *.c *.h
	gcc $(MYWARNINGS) main.o node.o file.o -o main

main.o: main.c
	gcc $(MYWARNINGS) main.c -c

test: test.o node.o file.o 
	gcc $(MYWARNINGS) test.o node.o file.o -o test
test.o: test.c
	gcc $(MYWARNINGS) test.c -c

node.o: node.h node.c
	gcc $(MYWARNINGS) node.c -c

file.o: file.h file.c 
	gcc $(MYWARNINGS) file.c -c
