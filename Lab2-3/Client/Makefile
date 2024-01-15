all: client

output : output.proto
	protoc-c --c_out=. output.proto

input : input.proto
	protoc-c --c_out=. input.proto

clean: clean_client
	rm -f output.pb-c.*

clean_client:
	rm -f parser.tab.c parser.tab.h parser.output lex.yy.c client

parser: parser.y
	bison -d parser.y

lexer: lexer.l
	flex lexer.l

client: parser lexer ast.c printer.c converter.c client.c output input
	gcc parser.tab.c lex.yy.c ast.c printer.c converter.c client.c output.pb-c.c input.pb-c.c -o client -lprotobuf-c

.PHONY: client output
