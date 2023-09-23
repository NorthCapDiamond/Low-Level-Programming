#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
FILE* file;


struct header{
	uint32_t signature;
	size_t first_hole_ptr;
	size_t first_node_ptr;
	size_t node_id;
} __attribute__((packed));

struct hole{
	size_t hole_ptr;
	size_t size_of_hole;
	size_t prev_ptr;
	size_t next_ptr;
} __attribute__((packed));


FILE* open_file(char* name);
void* read_file(size_t offset, size_t length);
size_t write_file(void* data_ptr, size_t size_of_data);
void delete_from_file(size_t offset, size_t length);






