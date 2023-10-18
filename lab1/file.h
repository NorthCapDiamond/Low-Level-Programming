#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
FILE* file;


struct header{
	uint32_t signature;
	uint64_t first_hole_ptr;
	uint64_t first_node_ptr;
	uint64_t last_node_ptr;
	uint64_t node_id;
} __attribute__((packed));

struct hole{
	uint64_t hole_ptr;
	uint64_t size_of_hole;
	uint64_t prev_ptr;
	uint64_t next_ptr;
} __attribute__((packed));


FILE* open_file(char* name);
void* read_file(uint64_t offset, uint64_t length);
uint64_t write_file(void* data_ptr, uint64_t size_of_data);
void delete_from_file(uint64_t offset, uint64_t length);







