#include "node.h"
#include <time.h>

int main(int argc, char **argv){
	uint64_t index = 0;
	uint64_t stopper = 1;
	file = open_file("data");
	FILE* file_add = fopen("add.csv","r+");

	if (file_add == NULL){
		printf("No file\n");
		return 1;
	}


	fprintf(file_add, "index,time\n");
	FILE* file_delete = fopen("delete.csv","r+");

	if(file_delete == NULL){
		printf("No file\n");
		return 1;
	}
	fprintf(file_delete, "index,time\n");

	fclose(file_add);
	fclose(file_delete);
	while(stopper){
		file_delete = fopen("delete.csv","a");
		file_add = fopen("add.csv","a");
		uint64_t clock_counter_add = 0;
		uint64_t clock_counter_remove = 0;
		uint64_t clock_start = 0;
		uint64_t clock_end = 0;

		uint64_t new_add =  rand() % 150 + 50;
		index+=new_add;

		struct string_save* stringer= (struct string_save*) malloc(sizeof(struct string_save) * 10);
		char* string_line = "Hello world by DimaSSSSSSS";
		uint64_t* ints_array = (uint64_t*)malloc(sizeof(uint64_t)*10);
		double* doubles_array = (double*)malloc(sizeof(double)*10);

		for (uint64_t i = 0; i < new_add; i++){
			uint64_t amount_of_strings = rand()%10 + 1;
			uint64_t amount_of_doubles = rand()%10 + 1;
			uint64_t amount_of_ints = rand()%10 + 1;
			uint64_t string_size;

			for(uint64_t int_iter = 0; int_iter < amount_of_ints; int_iter++){
				ints_array[int_iter] = rand()%1000 + 1;
			}

			for(uint64_t double_iter = 0; double_iter < amount_of_doubles; double_iter++){
				doubles_array[double_iter] = rand()%1000 + 1;
			}

			for(uint64_t string_iter = 0; string_iter < amount_of_strings; string_iter++){
				string_size = rand() % 20 + 1;
				stringer[string_iter].size_of_string = string_size;
				stringer[string_iter].string_line_ptr = string_line;
			}

			clock_start = clock();
			create_node_test(amount_of_ints, ints_array, amount_of_doubles, doubles_array, amount_of_strings, stringer);
			clock_end = clock();
			clock_counter_add += (clock_end - clock_start);

			clock_start = clock();
			remove_idxes(index-40,index);
			clock_end = clock();
			clock_counter_remove += clock_end - clock_start;
		}

		printf("Current idx %llu\n", index);
		fprintf(file_add, "%llu,%llu\n", index, clock_counter_add/new_add);
		fclose(file_add);
		printf("Current clock_add : %llu\n", clock_counter_add/new_add);
		fprintf(file_delete, "%llu,%llu\n", index, clock_counter_remove/50);
		fclose(file_delete);
		printf("Current clock_remove : %llu\n", clock_counter_remove/50);
	}

	return 0;
}