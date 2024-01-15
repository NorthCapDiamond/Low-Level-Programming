#include "node.h"


void h(){
	printf("----------\n");
	printf("h -- help\n");
	printf("q -- exit\n");
	printf("i -- show all nodes\n");
	printf("p / p uint64 -- detailed info for concrete node\n");
	printf("n -- add new node\n");
	printf("u / u uint64 -- append new unsigned integer to node with id = param");
	printf("d / d uint64 -- append new double to node with id = param\n");
	printf("s / s uint64 -- append new string to node with id = param\n");
	printf("c / c uint64 uint64 -- connect 2 nodes by id");
	printf("r / r uint64 -- remove node by id\n");
	printf("t -- remove nodes\n");
	printf("----------\n");
}



int main(int argc, char **argv){
	uint64_t integer; 
	uint64_t bouns;
	char tmp;
	char request;
	file = open_file("data");
	printf("Welcome to Drobina's App\n");
	printf("Type 'h' to see help options\n");

	while(request!=0x71){
		printf("Enter your command!\n");
		scanf("%c", &request);
		if(request != '\n'){
			scanf("%c", &tmp);
		}
		switch(request){
			case 'h':
				h();
				break;
			case 'i':
				print_nodes();
				break;
			case 'p':
				printf("Enter Node Id\n");
				scanf("%llu", &integer);
				scanf("%c", &tmp);
				print_info_node(integer);
				break;
			case 'n':
				create_node();
				break;
			case 'u':
				printf("Enter Node Id\n");
				scanf("%llu", &integer);
				append_int(integer);
				scanf("%c", &tmp);
				break;
			case 'd':
				printf("Enter Node Id\n");
				scanf("%llu", &integer);
				append_double(integer);
				scanf("%c", &tmp);
				break;
			case 's':
				printf("Enter Node Id\n");
				scanf("%llu", &integer);
				append_string(integer);
				break;
			case 'c':
				printf("Enter Node1 Id if didn't yet\n");
				scanf("%llu", &integer);
				scanf("%c", &tmp);
				printf("Enter Node2 Id if didn't yet\n");
				scanf("%llu", &bouns);
				connect_nodes(integer, bouns);
				scanf("%c", &tmp);
				printf("Done, Check this using command 'p' on one of this nodes\n");
				break;
			case 'r':
				printf("Enter Node Id\n");
				scanf("%llu", &integer);
				delete_node_by_id(integer);
				scanf("%c", &tmp);
				break;
			case 'q':
				printf("Exit command in process\n");
				break;
			case 't':
				printf("Delete all nodes\n");
				remover();
				break;
			default:
				printf("No Such Request\n");
		}
	}

	printf("Good Bye! Have a nice day\n");
	return 0;

}


