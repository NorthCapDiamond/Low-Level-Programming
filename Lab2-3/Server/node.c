#include "node.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>


uint64_t find_node_by_id(uint64_t id){
	struct header* file_header = read_file(0, sizeof(struct header));
	if(file_header->node_id == 0){return INVALID_PTR;}
	if(file_header->node_id < id){return INVALID_PTR;}
	uint64_t node_ptr = file_header->first_node_ptr;
	free(file_header);

	struct node* iter_node;
	while (node_ptr!=INVALID_PTR){
		iter_node = read_file(node_ptr, sizeof(struct node));
		if(iter_node->id == id){
			free(iter_node);
			return node_ptr;
		}
		node_ptr = iter_node->next_ptr;
		free(iter_node);
	}
	return INVALID_PTR;
}


uint64_t find_id_by_addr(uint64_t addr){
	struct node* read_node = read_file(addr, sizeof(struct node));
	uint64_t ans = read_node->id;
	free(read_node);
	return ans;
}


void create_node(void){
	struct header* file_header = read_file(0, sizeof(struct header));
	struct node* new_node = (struct node*) malloc(sizeof(struct node)); 

	file_header->node_id = file_header->node_id+1;
	new_node->id = file_header->node_id;
	new_node->prev_ptr = INVALID_PTR;
	new_node->next_ptr = INVALID_PTR;
	new_node->nodes = INVALID_PTR;

	// Set integers
	printf("How many integers? do you want to add to this node?\n");
	scanf("%llu", &(new_node->n_ints));
	if(new_node->n_ints){
		uint64_t* ints = (uint64_t*) malloc(new_node->n_ints);
		for (uint64_t i = 0; i < new_node->n_ints; i++){
			printf("Enter int\n");
			scanf("%llu", &(ints[i]));
		}
		new_node->ints_array_ptr = write_file(ints, new_node-> n_ints * sizeof(uint64_t));
		free(ints);
	} else {
		new_node->ints_array_ptr = INVALID_PTR;
	}
	
	// Set doubles
	printf("How many doubles do you want to add to this node?\n");
	scanf("%llu", &(new_node->n_doubles));
	if(new_node->n_doubles){
		double* doubles = (double*) malloc(new_node->n_doubles);

		for (int i = 0; i < new_node->n_doubles; i++){
			printf("Enter double\n");
			scanf("%lf", &(doubles[i]));
		}

		new_node->doubles_array_ptr = write_file(doubles, new_node-> n_doubles * sizeof(double));
		free(doubles);
	} else {
		new_node->doubles_array_ptr = INVALID_PTR;
	}

	// Set strings
	printf("How many strings do you want to add to this node?\n");
	scanf("%llu", &(new_node->n_strings));
	if(new_node->n_strings){
		struct string_save* strings = (struct string_save*) malloc(new_node->n_strings * sizeof(struct string_save));
		free(read_var_string());
		for(int i = 0; i < new_node->n_strings; i++){
			printf("Enter String line\n");
			struct string_save tmp;
			tmp.string_line_ptr = (uint64_t)read_var_string();
			tmp.size_of_string = strlen((char*)tmp.string_line_ptr);
			strings[i] = tmp;
			printf("Accepted %s\n", (char*)strings[i].string_line_ptr);
		}


		for (struct string_save* tmp = strings; tmp < strings+new_node->n_strings; tmp++) {
			uint64_t tmp_ptr = (uint64_t) tmp->string_line_ptr;
			tmp->string_line_ptr = write_file((char*) (tmp->string_line_ptr), tmp->size_of_string);
		}

		new_node->strings_array_ptr = write_file(strings, sizeof(struct string_save)*new_node->n_strings);
		free(strings);
	} else {
		new_node->n_strings = INVALID_PTR;
	}


	if(file_header->first_node_ptr==INVALID_PTR){
		file_header->first_node_ptr = write_file(new_node, sizeof(struct node));
		file_header->last_node_ptr = file_header->first_node_ptr;
	}
	else {
		new_node->prev_ptr = file_header->last_node_ptr;
		struct node* last_node = read_file(file_header->last_node_ptr, sizeof(struct node));
		last_node->next_ptr = write_file(new_node, sizeof(struct node));
		file_header->last_node_ptr = last_node->next_ptr;


		fseek(file, new_node->prev_ptr, SEEK_SET);
		fwrite(last_node, 1, sizeof(struct node), file);
		free(last_node);
	}

	fseek(file, 0, SEEK_SET);
	fwrite(file_header, 1, sizeof(struct header), file);
	free(file_header);
	free(new_node);
}



void delete_node_by_id(uint64_t id){
	struct header* file_header = read_file(0, sizeof(struct header));
	uint64_t address = find_node_by_id(id);
	if (address == INVALID_PTR){
		free(file_header);
		return;
	}

	struct node* delete_node = read_file(address, sizeof(struct node));

	// case 1: this is root node:
	if(file_header->first_node_ptr == address){
		file_header->first_node_ptr = delete_node->next_ptr;
	}

	// case 2: this is the last element of linked list:
	if(file_header->last_node_ptr == address){
		file_header->last_node_ptr = delete_node->prev_ptr;
	}

	// other:
	if(delete_node->prev_ptr != INVALID_PTR){
		struct node* prev_node = read_file(delete_node->prev_ptr, sizeof(struct node));
		prev_node->next_ptr = delete_node->next_ptr;
		delete_from_file(delete_node->next_ptr, sizeof(struct node));

		fseek(file, delete_node->prev_ptr, SEEK_SET);
		fwrite(prev_node, 1, sizeof(struct node), file);


		free(prev_node);
	}

	if(delete_node->next_ptr != INVALID_PTR){
		struct node* next_node = read_file(delete_node->next_ptr, sizeof(struct node));
		next_node->prev_ptr = delete_node->prev_ptr;
		delete_from_file(delete_node->next_ptr, sizeof(struct node));

		fseek(file, delete_node->next_ptr, SEEK_SET);
		fwrite(next_node, 1, sizeof(struct node), file);

		free(next_node);
	}

	uint64_t* nodes = read_file(delete_node->nodes, delete_node->d * sizeof(uint64_t));
	for (uint64_t i = 0; i < delete_node->d; i++){
		struct node* tmp_node = read_file(nodes[i], sizeof(struct node));
		uint64_t* nodes_array = read_file(tmp_node->nodes, sizeof(uint64_t)*(tmp_node->d));

		tmp_node->d--;


		//check on INVALID ptrs
		if (!(tmp_node->d)){
			tmp_node->nodes = INVALID_PTR;
			delete_from_file(tmp_node->nodes, sizeof(uint64_t) * (tmp_node->d + 1));
		}
		else {
			uint64_t* tmp_buf = (uint64_t*)malloc(sizeof(uint64_t)*tmp_node->d);
			uint64_t correct_pointer = 0;
			for(uint64_t j = 0; j < (tmp_node->d)+1; j++){
				if(j==correct_pointer){
					continue;
				}
				else{
					tmp_buf[correct_pointer] = nodes_array[j];
					correct_pointer++;
				}
			}

			delete_from_file(tmp_node->nodes, sizeof(uint64_t) * (tmp_node->d + 1));
			tmp_node->nodes = write_file(tmp_buf, (tmp_node->d) * sizeof(uint64_t));
			free(tmp_buf);
		}


		fseek(file, nodes[i], SEEK_SET);
		fwrite(tmp_node, 1, sizeof(struct node), file);

		free(tmp_node);
		free(nodes_array);
	}

	fseek(file, 0, SEEK_SET);
	fwrite(file_header, 1, sizeof(struct header), file);

	free(file_header);
	free(delete_node);

}


void connect_nodes(uint64_t id1, uint64_t id2){
	uint64_t p1 = find_node_by_id(id1);
	uint64_t p2 = find_node_by_id(id2);
	if(p1==INVALID_PTR || p2==INVALID_PTR){
		printf("Invalid pointers\n");
		return;
	}

	struct node* node1 = read_file(p1, sizeof(struct node));
	struct node* node2 = read_file(p2, sizeof(struct node));


	uint64_t* nodes1 = read_file(node1->nodes, sizeof(uint64_t)*node1->d);
	uint64_t* nodes2 = read_file(node2->nodes, sizeof(uint64_t)*node2->d);




	//check for duplicates:
	for (uint64_t i = 0; i < node1->d; i++){
		if(nodes1[i]==p2){
			printf("Nodes are already connected\n");
			return;
		}
	}

	for (uint64_t i = 0; i < node2->d; i++){
		if(nodes2[i]==p1){
			printf("Nodes are already connected\n");
			return;
		}
	}
	//continue

	if(node1->d){
		delete_from_file(node1->nodes, sizeof(uint64_t)*(node1->d));
	}
	if(node2->d){
		delete_from_file(node2->nodes, sizeof(uint64_t)*(node2->d));
	}


	nodes1 = realloc(nodes1, sizeof(uint64_t)*(node1->d + 1));
	nodes2 = realloc(nodes2, sizeof(uint64_t)*(node2->d + 1));

	nodes2[node2->d] = p1;
	nodes1[node1->d] = p2;
	node1->d++;
	node2->d++;


	node1->nodes = write_file(nodes1, node1->d * sizeof(uint64_t));
	node2->nodes = write_file(nodes2, node2->d * sizeof(uint64_t));


	fseek(file, p1, SEEK_SET);
	fwrite(node1, 1, sizeof(struct node), file);

	fseek(file, p2, SEEK_SET);
	fwrite(node2, 1, sizeof(struct node), file);

	free(node1);
	free(node2);
}


int connect_nodes_checked(uint64_t id1, uint64_t id2){
	uint64_t p1 = find_node_by_id(id1);
	uint64_t p2 = find_node_by_id(id2);
	if(p1==INVALID_PTR || p2==INVALID_PTR){
		printf("Invalid pointers\n");
		return -1;
	}

	struct node* node1 = read_file(p1, sizeof(struct node));
	struct node* node2 = read_file(p2, sizeof(struct node));


	uint64_t* nodes1 = read_file(node1->nodes, sizeof(uint64_t)*node1->d);
	uint64_t* nodes2 = read_file(node2->nodes, sizeof(uint64_t)*node2->d);




	//check for duplicates:
	for (uint64_t i = 0; i < node1->d; i++){
		if(nodes1[i]==p2){
			printf("Nodes are already connected\n");
			return 0;
		}
	}

	for (uint64_t i = 0; i < node2->d; i++){
		if(nodes2[i]==p1){
			printf("Nodes are already connected\n");
			return 0;
		}
	}
	//continue

	if(node1->d){
		delete_from_file(node1->nodes, sizeof(uint64_t)*(node1->d));
	}
	if(node2->d){
		delete_from_file(node2->nodes, sizeof(uint64_t)*(node2->d));
	}


	nodes1 = realloc(nodes1, sizeof(uint64_t)*(node1->d + 1));
	nodes2 = realloc(nodes2, sizeof(uint64_t)*(node2->d + 1));

	nodes2[node2->d] = p1;
	nodes1[node1->d] = p2;
	node1->d++;
	node2->d++;


	node1->nodes = write_file(nodes1, node1->d * sizeof(uint64_t));
	node2->nodes = write_file(nodes2, node2->d * sizeof(uint64_t));


	fseek(file, p1, SEEK_SET);
	fwrite(node1, 1, sizeof(struct node), file);

	fseek(file, p2, SEEK_SET);
	fwrite(node2, 1, sizeof(struct node), file);

	free(node1);
	free(node2);

	return 1;
}


void print_nodes(void){
	struct header* file_header = read_file(0, sizeof(struct header));
	uint64_t node_ptr = file_header->first_node_ptr;
	free(file_header);

	struct node* iter_node;
	printf("----------\n");
	while (node_ptr!=INVALID_PTR){
		iter_node = read_file(node_ptr, sizeof(struct node));
		printf("Node id : %llu\n", iter_node->id);
		printf("Node vertex : %llu\n", iter_node->d);
		printf("Node number of integers : %llu\n", iter_node->n_ints);
		printf("Node number of doubles : %llu\n", iter_node->n_doubles);
		printf("Node number of strings : %llu\n", iter_node->n_strings);
		printf("----------\n");
		node_ptr = iter_node->next_ptr;
		free(iter_node);
	}
	return;
}

void print_info_node(uint64_t id){
	uint64_t node_ptr = find_node_by_id(id);
	if(node_ptr==INVALID_PTR){
		printf("----------\n");
		printf("ERROR, Node does not exist\n");
		printf("----------\n");
		return;
	}
	struct node* iter_node = read_file(node_ptr, sizeof(struct node));
	printf("----------\n");
	printf("Node id : %llu\n", iter_node->id);
	printf("Node vertex : %llu\n", iter_node->d);
	uint64_t* nodes = read_file(iter_node->nodes, sizeof(uint64_t) * iter_node->d);
	

	for (uint64_t i = 0; i < iter_node->d; i++){
		struct node* tmp_node = read_file(nodes[i], sizeof(struct node));
		printf("Neighbour %llu, id : %llu\n", i+1, tmp_node->id);
		free(tmp_node);
	}
	free(nodes);

	printf("Node number of integers : %llu\n", iter_node->n_ints);
	if(!(iter_node->n_ints)){
		printf("No integers found!\n");
	}
	else{
		uint64_t* ints = read_file(iter_node->ints_array_ptr, sizeof(uint64_t) * iter_node->n_ints);

		printf("Elements of the array\n");
		for (uint64_t i = 0; i < iter_node->n_ints; i++){
			printf("El%llu : %llu\n", i+1, ints[i]);
		}
		free(ints);
	}
	

	printf("Node number of doubles : %llu\n", iter_node->n_doubles);
	if(!(iter_node->n_doubles)){
		printf("No doubles found!\n");
	}
	else{
		double* doubles = read_file(iter_node->doubles_array_ptr, sizeof(double) * iter_node->n_doubles);

		printf("Elements of the array\n");
		for (uint64_t i = 0; i < iter_node->n_doubles; i++){
			printf("El%llu : %lf\n", i+1, doubles[i]);
		}
		free(doubles);
	}

	printf("Node number of strings : %llu\n", iter_node->n_strings);
	if(!(iter_node->n_strings)){
		printf("No strings found\n");
	}
	else { 
		struct string_save* strings = read_file(iter_node->strings_array_ptr, sizeof(struct string_save) * iter_node->n_strings); 
		for (uint64_t i = 0; i < iter_node->n_strings; i++){
			printf("El%llu : %s\n", i+1, (char*)read_file(strings[i].string_line_ptr, strings[i].size_of_string));
		}
		free(strings);
	}

	printf("----------\n");
	free(iter_node);
	return;
}

void append_int(uint64_t id){
	uint64_t node_ptr = find_node_by_id(id);
	if(node_ptr==INVALID_PTR){
		printf("----------\n");
		printf("ERROR, Node does not exist\n");
		printf("----------\n");
		return;
	}
	struct node* iter_node = read_file(node_ptr, sizeof(struct node));
	printf("----------\n");
	printf("Node id : %llu\n", iter_node->id);
	printf("Node number of integers : %llu\n", iter_node->n_ints);
	
	uint64_t new_size;
	uint64_t i = 0;
	// Set integers
	printf("How many integers? do you want to add to this node?\n");
	scanf("%llu", &(new_size));



	if(iter_node->n_ints){
		uint64_t* old_ints = read_file(iter_node->ints_array_ptr, sizeof(uint64_t) * iter_node->n_ints);
		uint64_t* ints = (uint64_t*) malloc(iter_node->n_ints + new_size);
		while(i < iter_node->n_ints){
			ints[i] = old_ints[i];
			i++;
		}

		while(i < iter_node->n_ints + new_size){
			printf("Enter int\n");
			scanf("%llu", &(ints[i]));
			i++;
		}

		delete_from_file(iter_node->ints_array_ptr, sizeof(uint64_t) * iter_node->n_ints);
		iter_node->n_ints+=new_size;
		iter_node->ints_array_ptr = write_file(ints, iter_node->n_ints * sizeof(uint64_t));
		free(ints);

	} else {
		uint64_t* ints = (uint64_t*) malloc(iter_node->n_ints + new_size);
		while(i < iter_node->n_ints + new_size){
			printf("Enter int\n");
			scanf("%llu", &(ints[i]));
			i++;
		}

		iter_node->n_ints+=new_size;
		iter_node->ints_array_ptr = write_file(ints, iter_node->n_ints * sizeof(uint64_t));
		free(ints);

	}

	fseek(file, node_ptr, SEEK_SET);
	fwrite(iter_node, 1, sizeof(struct node), file);
	free(iter_node);
}


void append_double(uint64_t id){
	uint64_t node_ptr = find_node_by_id(id);
	if(node_ptr==INVALID_PTR){
		printf("----------\n");
		printf("ERROR, Node does not exist\n");
		printf("----------\n");
		return;
	}
	struct node* iter_node = read_file(node_ptr, sizeof(struct node));
	printf("----------\n");
	printf("Node id : %llu\n", iter_node->id);
	printf("Node number of doubles : %llu\n", iter_node->n_doubles);
	
	uint64_t new_size;
	uint64_t i = 0;
	// Set integers
	printf("How many doubles do you want to add to this node?\n");
	scanf("%llu", &(new_size));



	if(iter_node->n_doubles){
		double* old_doubles = read_file(iter_node->doubles_array_ptr, sizeof(double) * iter_node->n_doubles);
		double* doubles = (double*) malloc(iter_node->n_doubles + new_size);
		while(i < iter_node->n_doubles){
			doubles[i] = old_doubles[i];
			i++;
		}

		while(i < iter_node->n_doubles + new_size){
			printf("Enter double\n");
			scanf("%lf", &(doubles[i]));
			i++;
		}

		delete_from_file(iter_node->doubles_array_ptr, sizeof(double) * iter_node->n_doubles);
		iter_node->n_doubles+=new_size;
		iter_node->doubles_array_ptr = write_file(doubles, iter_node->n_doubles * sizeof(double));
		free(doubles);

	} else {
		double* doubles = (double*) malloc(iter_node->n_doubles + new_size);
		while(i < iter_node->n_doubles + new_size){
			printf("Enter double\n");
			scanf("%lf", &(doubles[i]));
			i++;
		}

		iter_node->n_doubles+=new_size;
		iter_node->doubles_array_ptr = write_file(doubles, iter_node->n_doubles * sizeof(double));
		free(doubles);

	}

	fseek(file, node_ptr, SEEK_SET);
	fwrite(iter_node, 1, sizeof(struct node), file);
	free(iter_node);
}


void append_string(uint64_t id){
	uint64_t node_ptr = find_node_by_id(id);
	if(node_ptr==INVALID_PTR){
		printf("----------\n");
		printf("ERROR, Node does not exist\n");
		printf("----------\n");
		return;
	}
	struct node* iter_node = read_file(node_ptr, sizeof(struct node));
	printf("----------\n");
	printf("Node id : %llu\n", iter_node->id);
	printf("Node number of strings : %llu\n", iter_node->n_strings);
	
	uint64_t new_size;
	uint64_t i = 0;
	// Set integers
	printf("How many strings do you want to add to this node?\n");
	scanf("%llu", &(new_size));


	struct string_save* strings = (struct string_save*) malloc(new_size * sizeof(struct string_save));
	free(read_var_string());
	for(int i = 0; i < new_size; i++){
		printf("Enter String line\n");
		struct string_save tmp;
		tmp.string_line_ptr = (uint64_t)read_var_string();
		tmp.size_of_string = strlen((char*)tmp.string_line_ptr);
		strings[i] = tmp;
		printf("Accepted %s\n", (char*)strings[i].string_line_ptr);
	}


	for (struct string_save* tmp = strings; tmp < strings+new_size; tmp++) {
		uint64_t tmp_ptr = (uint64_t) tmp->string_line_ptr;
		tmp->string_line_ptr = write_file((char*) (tmp->string_line_ptr), tmp->size_of_string);
		free(tmp_ptr);
	}

	if(iter_node->n_strings){
		struct string_save* strings_array = read_file(iter_node->strings_array_ptr, sizeof(struct string_save)*iter_node->n_strings);
		delete_from_file(iter_node->strings_array_ptr, sizeof(struct string_save)*iter_node->n_strings);
		strings_array = realloc(strings_array, (iter_node->n_strings + new_size)*sizeof(struct string_save));

		for (uint64_t i = iter_node->n_strings; i < iter_node->n_strings + new_size; i++){
			strings_array[i] = strings[i-(iter_node->n_strings)];
		}
		iter_node->n_strings+=new_size;


		iter_node->strings_array_ptr = write_file(strings_array, sizeof(struct string_save)*iter_node->n_strings);
		fseek(file, node_ptr, SEEK_SET);
		fwrite(iter_node, 1, sizeof(struct node), file);
		free(strings_array);
	} else {
		iter_node->n_strings+=new_size;
		iter_node->strings_array_ptr = write_file(strings, sizeof(struct string_save)*iter_node->n_strings);
		fseek(file, node_ptr, SEEK_SET);
		fwrite(iter_node, 1, sizeof(struct node), file);
	}
	free(strings);
	free(iter_node);
}

char* read_var_string(){
	char* inp = NULL;
	char tmp[128];
	uint64_t inplen = 0;
	uint64_t tmplen = 0;

	do{
		fgets(tmp, 128, stdin);
		tmp[strcspn(tmp, "\n")] = 0;
		tmplen = strlen(tmp);
		inp = realloc(inp, inplen+tmplen+1);
		strcpy(inp+inplen, tmp);
		inplen += tmplen;
	} while(tmplen == 128-1 && tmp[126] != '\n');
	return inp;
}


void remover(){
	struct header* file_header = read_file(0, sizeof(struct header));

	struct node* iter_node = read_file(file_header->last_node_ptr, sizeof(struct node));


	uint64_t idx = iter_node->id;
	free(file_header);
	free(iter_node);

	for (uint64_t i = 1; i < idx+1; i++){
		delete_node_by_id(i);
	}
}



void remove_idxes(uint64_t i1, uint64_t i2){
	struct header* file_header = read_file(0, sizeof(struct header));
	struct node* iter_node = read_file(file_header->last_node_ptr, sizeof(struct node));


	for (uint64_t i = i1; i < i2+1; i++){
		delete_node_by_id(i);
	}

	free(file_header);
	free(iter_node);
}



void create_node_test(uint64_t int_amount, uint64_t* int_ptr, uint64_t double_amount, double* double_ptr, uint64_t string_amount, struct string_save* string_ptr){
	struct header* file_header = read_file(0, sizeof(struct header));
	struct node* new_node = (struct node*) malloc(sizeof(struct node)); 

	file_header->node_id = file_header->node_id+1;
	new_node->id = file_header->node_id;
	new_node->prev_ptr = INVALID_PTR;
	new_node->next_ptr = INVALID_PTR;
	new_node->nodes = INVALID_PTR;

	new_node->n_ints = int_amount;
	new_node->n_doubles = double_amount;
	new_node->n_strings = string_amount;

	if(new_node->n_ints){
		new_node->ints_array_ptr = write_file(int_ptr, new_node->n_ints * sizeof(uint64_t));
	} else {
		new_node->ints_array_ptr = INVALID_PTR;
	}


	if(new_node->n_doubles){
		new_node->doubles_array_ptr = write_file(double_ptr, new_node->n_doubles * sizeof(double));
	} else {
		new_node->doubles_array_ptr = INVALID_PTR;
	}

	if (new_node->n_strings){

		for (struct string_save* tmp = string_ptr; tmp < string_ptr+new_node->n_strings; tmp++) {
			uint64_t tmp_ptr = (uint64_t) tmp->string_line_ptr;
			tmp->string_line_ptr = write_file((char*) (tmp->string_line_ptr), tmp->size_of_string);
		}

		new_node->strings_array_ptr = write_file(string_ptr, new_node->n_strings * sizeof(struct string_save));
	} else {
		new_node->strings_array_ptr = INVALID_PTR;
	}


	if(file_header->first_node_ptr==INVALID_PTR){
		file_header->first_node_ptr = write_file(new_node, sizeof(struct node));
		file_header->last_node_ptr = file_header->first_node_ptr;
	}
	else {
		new_node->prev_ptr = file_header->last_node_ptr;
		struct node* last_node = read_file(file_header->last_node_ptr, sizeof(struct node));
		last_node->next_ptr = write_file(new_node, sizeof(struct node));
		file_header->last_node_ptr = last_node->next_ptr;




		fseek(file, new_node->prev_ptr, SEEK_SET);
		fwrite(last_node, 1, sizeof(struct node), file);
		free(last_node);
	}

	fseek(file, 0, SEEK_SET);
	fwrite(file_header, 1, sizeof(struct header), file);
	free(file_header);
	free(new_node);
}



struct node* create_node_params(uint64_t int_amount, uint64_t* int_ptr, uint64_t double_amount, double* double_ptr, uint64_t string_amount, struct string_save* string_ptr){
	struct header* file_header = read_file(0, sizeof(struct header));
	struct node* new_node = (struct node*) malloc(sizeof(struct node)); 

	file_header->node_id = file_header->node_id+1;
	new_node->id = file_header->node_id;
	
	new_node->prev_ptr = INVALID_PTR;
	new_node->next_ptr = INVALID_PTR;
	new_node->nodes = INVALID_PTR;

	new_node->n_ints = int_amount;
	new_node->n_doubles = double_amount;
	new_node->n_strings = string_amount;

	if(new_node->n_ints){
		new_node->ints_array_ptr = write_file(int_ptr, new_node->n_ints * sizeof(uint64_t));
	} else {
		new_node->ints_array_ptr = INVALID_PTR;
	}


	if(new_node->n_doubles){
		new_node->doubles_array_ptr = write_file(double_ptr, new_node->n_doubles * sizeof(double));
	} else {
		new_node->doubles_array_ptr = INVALID_PTR;
	}

	if (new_node->n_strings){

		for (struct string_save* tmp = string_ptr; tmp < string_ptr+new_node->n_strings; tmp++) {
			uint64_t tmp_ptr = (uint64_t) tmp->string_line_ptr;
			tmp->string_line_ptr = write_file((char*) (tmp->string_line_ptr), tmp->size_of_string);
		}

		new_node->strings_array_ptr = write_file(string_ptr, new_node->n_strings * sizeof(struct string_save));
	} else {
		new_node->strings_array_ptr = INVALID_PTR;
	}


	if(file_header->first_node_ptr==INVALID_PTR){
		file_header->first_node_ptr = write_file(new_node, sizeof(struct node));
		file_header->last_node_ptr = file_header->first_node_ptr;
	}
	else {
		new_node->prev_ptr = file_header->last_node_ptr;
		struct node* last_node = read_file(file_header->last_node_ptr, sizeof(struct node));
		last_node->next_ptr = write_file(new_node, sizeof(struct node));
		file_header->last_node_ptr = last_node->next_ptr;




		fseek(file, new_node->prev_ptr, SEEK_SET);
		fwrite(last_node, 1, sizeof(struct node), file);
		free(last_node);
	}

	fseek(file, 0, SEEK_SET);
	fwrite(file_header, 1, sizeof(struct header), file);
	free(file_header);
	return new_node;
}

struct node* find_by_id_param_i(uint64_t value, uint64_t id, int op){


	struct header* file_header = read_file(0, sizeof(struct header));
	uint64_t address = find_node_by_id(id);
	if (address == INVALID_PTR){
		free(file_header);
		return NULL;
	}

	uint64_t counter = 0;

	struct node* iter_node = read_file(address, sizeof(struct node));

	if(!(iter_node->n_ints)){
		printf("No integers found!\n");
	}
	else {
		uint64_t* ints = read_file(iter_node->ints_array_ptr, sizeof(uint64_t) * iter_node->n_ints);

		printf("Elements of the array\n");
		for (uint64_t i = 0; i < iter_node->n_ints; i++){
			if(op == 0){
				if(ints[i] > value){
					counter++;
				}
			}
			else if(op == 1){
				if(ints[i] < value){
					counter++;
				}
			}

			else if(op == 2){
				if (ints[i] == value){
					counter++;
				}
			}
			else if(op == 3){
				if (ints[i] >= value){
					counter++;
				}
			}
			else if(op == 4){
				if (ints[i] <= value){
					counter++;
				}
			}
		}

		return iter_node;
	}

	return NULL;
}


struct node* find_by_id_param_d(double value, uint64_t id, int op){


	struct header* file_header = read_file(0, sizeof(struct header));
	uint64_t address = find_node_by_id(id);
	if (address == INVALID_PTR){
		free(file_header);
		return NULL;
	}

	uint64_t counter = 0;
	double* answer;

	struct node* iter_node = read_file(address, sizeof(struct node));

	if(!(iter_node->n_ints)){
		printf("No integers found!\n");
	}
	else{
		double* ints = read_file(iter_node->ints_array_ptr, sizeof(double) * iter_node->n_ints);

		printf("Elements of the array\n");
		for (uint64_t i = 0; i < iter_node->n_ints; i++){
			if(op == 0){
				if(ints[i] > value){
					counter++;
				}
			}
			else if(op == 1){
				if(ints[i] < value){
					counter++;
				}
			}

			else if(op == 2){
				if (ints[i] == value){
					counter++;
				}
			}
			else if(op == 3){
				if (ints[i] >= value){
					counter++;
				}
			}
			else if(op == 4){
				if (ints[i] <= value){
					counter++;
				}
			}
		}

		return iter_node;
	}
	return NULL;
}


struct node* return_node(uint64_t id){
	struct header* file_header = read_file(0, sizeof(struct header));
	uint64_t address = find_node_by_id(id);
	if (address == INVALID_PTR){
		free(file_header);
		return NULL;
	}

	struct node* iter_node = read_file(address, sizeof(struct node));

	return iter_node;
}


struct node* substrs(char* string, uint64_t id){
	struct header* file_header = read_file(0, sizeof(struct header));
	uint64_t address = find_node_by_id(id);
	if (address == INVALID_PTR){
		free(file_header);
		return NULL;
	}

	uint64_t counter = 0;
	uint64_t answer = 0;

	struct node* iter_node = read_file(address, sizeof(struct node));


	if(!(iter_node->n_strings)){
		printf("No strings found\n");
	}
	else { 
		struct string_save* strings = read_file(iter_node->strings_array_ptr, sizeof(struct string_save) * iter_node->n_strings); 
		for (uint64_t i = 0; i < iter_node->n_strings; i++){
			if(strstr((char*)read_file(strings[i].string_line_ptr, strings[i].size_of_string), string) != NULL){
				answer++;
			}
			printf("El%llu : %s\n", i+1, (char*)read_file(strings[i].string_line_ptr, strings[i].size_of_string));
		}
		free(strings);
	}

	if (answer>0){
		return iter_node;
	}

	return NULL;
}

struct node* equalstr(char* string, uint64_t id){

	struct header* file_header = read_file(0, sizeof(struct header));
	uint64_t address = find_node_by_id(id);
	if (address == INVALID_PTR){
		free(file_header);
		return NULL;
	}

	uint64_t counter = 0;
	uint64_t answer = 0;

	struct node* iter_node = read_file(address, sizeof(struct node));


	if(!(iter_node->n_strings)){
		printf("No strings found\n");
	}
	else { 
		struct string_save* strings = read_file(iter_node->strings_array_ptr, sizeof(struct string_save) * iter_node->n_strings); 
		for (uint64_t i = 0; i < iter_node->n_strings; i++){
			if(!strcmp((char*)read_file(strings[i].string_line_ptr, strings[i].size_of_string), string)){
				answer++;
			}
			printf("El%llu : %s\n", i+1, (char*)read_file(strings[i].string_line_ptr, strings[i].size_of_string));
		}
		free(strings);
	}
	if (answer>0){
		return iter_node;
	}

	return NULL;
}

struct node* node_updater(uint64_t id, uint64_t int_amount, uint64_t* int_ptr, uint64_t double_amount, double* double_ptr, uint64_t string_amount, struct string_save* string_ptr){
	uint64_t node_ptr = find_node_by_id(id);
	if(node_ptr==INVALID_PTR){
		printf("----------\n");
		printf("ERROR, Node does not exist\n");
		printf("----------\n");
		return NULL;
	}
	struct node* iter_node = read_file(node_ptr, sizeof(struct node));
	uint64_t new_size = int_amount-1;
	uint64_t i = 0;
	uint64_t runner = 1;
	// Set integers

	if(iter_node->n_ints){
		uint64_t* old_ints = read_file(iter_node->ints_array_ptr, sizeof(uint64_t) * iter_node->n_ints);
		uint64_t* ints = (uint64_t*) malloc(iter_node->n_ints + new_size);
		while(i < iter_node->n_ints){
			ints[i] = old_ints[i];
			i++;
		}

		while(i < iter_node->n_ints + new_size){
			printf("Enter int\n");
			ints[i] = int_ptr[runner];
			runner++;
			i++;
		}

		delete_from_file(iter_node->ints_array_ptr, sizeof(uint64_t) * iter_node->n_ints);
		iter_node->n_ints+=new_size;
		iter_node->ints_array_ptr = write_file(ints, iter_node->n_ints * sizeof(uint64_t));

	} else {
		uint64_t* ints = (uint64_t*) malloc(iter_node->n_ints + new_size);
		while(i < iter_node->n_ints + new_size){
			printf("Enter int\n");
			ints[i] = int_ptr[runner];
			runner++;
			i++;
		}

		iter_node->n_ints+=new_size;
		iter_node->ints_array_ptr = write_file(ints, iter_node->n_ints * sizeof(uint64_t));

	}

	new_size = double_amount;
	i = 0;
	runner = 0;

	if(iter_node->n_doubles){
		double* old_doubles = read_file(iter_node->doubles_array_ptr, sizeof(double) * iter_node->n_doubles);
		double* doubles = (double*) malloc(iter_node->n_doubles + new_size);
		while(i < iter_node->n_doubles){
			doubles[i] = old_doubles[i];
			i++;
		}

		while(i < iter_node->n_doubles + new_size){
			printf("Enter double\n");
			doubles[i] = double_ptr[runner];
			runner++;
			i++;
		}

		delete_from_file(iter_node->doubles_array_ptr, sizeof(double) * iter_node->n_doubles);
		iter_node->n_doubles+=new_size;
		iter_node->doubles_array_ptr = write_file(doubles, iter_node->n_doubles * sizeof(double));

	} else {
		double* doubles = (double*) malloc(iter_node->n_doubles + new_size);
		while(i < iter_node->n_doubles + new_size){
			printf("Enter double\n");
			doubles[i] = double_ptr[runner];
			i++;
			runner++;
		}

		iter_node->n_doubles+=new_size;
		iter_node->doubles_array_ptr = write_file(doubles, iter_node->n_doubles * sizeof(double));
	}

	printf("Going to STR\n");

	
	new_size = string_amount;
	i = 0;
	runner = 0;

	struct string_save* strings = (struct string_save*) malloc(new_size * sizeof(struct string_save));
	for(int i = 0; i < new_size; i++){
		printf("Enter String line\n");
		struct string_save tmp;
		tmp.string_line_ptr = (uint64_t)string_ptr[i].string_line_ptr;
		tmp.size_of_string = strlen((char*)tmp.string_line_ptr);
		strings[i] = tmp;
		printf("Accepted %s\n", (char*)strings[i].string_line_ptr);
	}


	for (struct string_save* tmp = strings; tmp < strings+new_size; tmp++) {
		uint64_t tmp_ptr = (uint64_t) tmp->string_line_ptr;
		tmp->string_line_ptr = write_file((char*) (tmp->string_line_ptr), tmp->size_of_string);
	}

	if(iter_node->n_strings){
		struct string_save* strings_array = read_file(iter_node->strings_array_ptr, sizeof(struct string_save)*iter_node->n_strings);
		delete_from_file(iter_node->strings_array_ptr, sizeof(struct string_save)*iter_node->n_strings);
		strings_array = realloc(strings_array, (iter_node->n_strings + new_size)*sizeof(struct string_save));

		for (uint64_t i = iter_node->n_strings; i < iter_node->n_strings + new_size; i++){
			strings_array[i] = strings[i-(iter_node->n_strings)];
		}
		iter_node->n_strings+=new_size;


		iter_node->strings_array_ptr = write_file(strings_array, sizeof(struct string_save)*iter_node->n_strings);
		fseek(file, node_ptr, SEEK_SET);
		fwrite(iter_node, 1, sizeof(struct node), file);
	} else {
		iter_node->n_strings+=new_size;
		iter_node->strings_array_ptr = write_file(strings, sizeof(struct string_save)*iter_node->n_strings);
		fseek(file, node_ptr, SEEK_SET);
		fwrite(iter_node, 1, sizeof(struct node), file);
	}
	return iter_node;
}


Node* one_node_from_server_to_client_form_answer(struct node* iter_node, Node* my_answer){
	UintArray* array_of_ints = malloc(sizeof(UintArray));
	uint_array__init(array_of_ints);

	DoubleArray* array_of_doubles = malloc(sizeof(DoubleArray));
	double_array__init(array_of_doubles);

	StringArray* array_of_strings = malloc(sizeof(StringArray));
	string_array__init(array_of_strings);

	UintArray* nodes = malloc(sizeof(UintArray));
	uint_array__init(nodes);


	my_answer->id = iter_node->id;
	my_answer->n_ints = iter_node->n_ints;

	if(my_answer->n_ints){
		my_answer->ints_array_ptr = array_of_ints;
		uint64_t* ints = read_file(iter_node->ints_array_ptr, sizeof(uint64_t) * iter_node->n_ints);

		for (uint64_t i = 0; i < iter_node->n_ints; i++){
			 array_of_ints->value = ints[i];
			 array_of_ints->next = malloc(sizeof(UintArray));
			 uint_array__init(array_of_ints->next);
			 array_of_ints = array_of_ints->next;
		}
	}

	my_answer->d = iter_node->d;
	if(my_answer->d){
		my_answer->nodes = nodes;
		uint64_t* ints = read_file(iter_node->nodes, sizeof(struct node) * iter_node->d);

		for (uint64_t i = 0; i < iter_node->d; i++){
			 nodes->value = find_id_by_addr(ints[i]);
			 nodes->next = malloc(sizeof(UintArray));
			 uint_array__init(nodes->next);
			 nodes = nodes->next;
		}
	}


	my_answer->n_doubles = iter_node->n_doubles;
	if(my_answer->n_doubles){

		my_answer->doubles_array_ptr = array_of_doubles;
		double* doubles = read_file(iter_node->doubles_array_ptr, sizeof(double) * iter_node->n_doubles);

		for (uint64_t i = 0; i < iter_node->n_doubles; i++){
			 array_of_doubles->value = doubles[i];
			 array_of_doubles->next = malloc(sizeof(DoubleArray));
			 double_array__init(array_of_doubles->next);
			 array_of_doubles = array_of_doubles->next;
		}
	}


	my_answer->n_strings = iter_node->n_strings;
	if(my_answer->n_strings){

		struct string_save* strings = read_file(iter_node->strings_array_ptr, sizeof(struct string_save) * iter_node->n_strings); 

		my_answer->strings_array_ptr = array_of_strings;

		for (uint64_t i = 0; i < iter_node->n_strings; i++){
			array_of_strings->value = (char*)read_file(strings[i].string_line_ptr, strings[i].size_of_string);
			array_of_strings->next = malloc(sizeof(StringArray));
			string_array__init(array_of_strings->next);
			array_of_strings = array_of_strings->next;
		}
	}

	return my_answer;
}

Node* all_nodes_from_server_to_client_form_answer(Node* my_answer, uint64_t size_of_struct_node){

	Node* pointer = my_answer;
	struct header* file_header = read_file(0, sizeof(struct header));
	uint64_t node_ptr = file_header->first_node_ptr;
	free(file_header);

	struct node* iter_node;
	while (node_ptr!=INVALID_PTR){
		iter_node = read_file(node_ptr, sizeof(struct node));

		pointer->next_node = malloc(sizeof(Node));
		node__init(pointer->next_node);
		pointer = pointer->next_node;

		one_node_from_server_to_client_form_answer(iter_node, pointer);


		node_ptr = iter_node->next_ptr;
		free(iter_node);
	}
	return my_answer;
}

Node* current_id_links(uint64_t id, Node* my_answer){
	if(find_node_by_id(id) == NULL){
		return NULL;
	}

	struct node* iter_node = read_file(find_node_by_id(id), sizeof(struct node));
	my_answer = one_node_from_server_to_client_form_answer(iter_node, my_answer);
	Node* pointer = my_answer;

	uint64_t* array_of_nodes = read_file(iter_node->nodes, sizeof(struct node) * iter_node->d);

	for(uint64_t i = 0; i < iter_node->d; i++){
		pointer->next_node = malloc(sizeof(Node));
		node__init(pointer->next_node);
		pointer = pointer->next_node;

		pointer = one_node_from_server_to_client_form_answer(read_file(array_of_nodes[i], sizeof(struct node)), pointer);

	}
	return my_answer;
}


Node* current_id_links_last(uint64_t id, Node* my_answer){
	if(find_node_by_id(id) == NULL){
		return NULL;
	}

	struct node* iter_node = read_file(find_node_by_id(id), sizeof(struct node));
	my_answer = one_node_from_server_to_client_form_answer(iter_node, my_answer);
	Node* pointer = my_answer;

	uint64_t* array_of_nodes = read_file(iter_node->nodes, sizeof(struct node) * iter_node->d);

	for(uint64_t i = 0; i < iter_node->d; i++){
		pointer->next_node = malloc(sizeof(Node));
		node__init(pointer->next_node);
		pointer = pointer->next_node;

		pointer = one_node_from_server_to_client_form_answer(read_file(array_of_nodes[i], sizeof(struct node)), pointer);

	}
	return pointer;
}

Node* full_links(Node* my_answer, uint64_t id){
	Node* pointer = my_answer;
	struct header* file_header = read_file(0, sizeof(struct header));
	uint64_t node_ptr = file_header->first_node_ptr;
	struct node* iter_node = read_file(node_ptr, sizeof(struct node));
	pointer = current_id_links_last(iter_node->id, pointer);
	if(iter_node->id + 1 <= find_id_by_addr(file_header->last_node_ptr) && find_node_by_id(iter_node->id + 1)!= NULL){
		pointer->next_node = malloc(sizeof(Node));
		node__init(pointer->next_node);
	 	pointer->next_node = full_links(pointer->next_node, iter_node->id+1);
	 }
	return my_answer;
}












