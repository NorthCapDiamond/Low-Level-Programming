main: main.o node.o file.o 
	gcc main.o node.o file.o -o main -Wno-coverage-mismatch

main.o: main.c
	gcc main.c -c -Wno-coverage-mismatch

test: test.o node.o file.o 
	gcc test.o node.o file.o -o test -Wno-coverage-mismatch

test.o: test.c
	gcc test.c -c -Wno-coverage-mismatch

node.o: node.h node.c
	gcc node.c -c -Wno-coverage-mismatch

file.o: file.h file.c 
	gcc file.c -c -Wno-coverage-mismatch