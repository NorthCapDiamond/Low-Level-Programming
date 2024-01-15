server : input output server.o node.o file.o util.o data
	gcc server.o input.pb-c.c node.o file.o util.o output.pb-c.c -o server -lprotobuf-c

input : input.proto
	protoc-c --c_out=. input.proto

output : output.proto
	protoc-c --c_out=. output.proto

file.o : file.c file.h
	gcc file.c -c

node.o : node.c node.h
	gcc node.c -c

util.o : util.c util.h
	gcc util.c -c -lprotobuf-c

server.o: server.c
	gcc server.c -c -lprotobuf-c

data :
	rm data.db
	touch data.db


clean :
	rm -f parser.tab.c parser.tab.h parser.output lex.yy.c server util input.pb-c.h input.pb-c.c output.pb-c.c output.pb-c.h *.o
