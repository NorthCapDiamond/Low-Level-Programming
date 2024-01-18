#include "input.pb-c.h"
#include "output.pb-c.h"
#include "node.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

struct value_keeper{
	uint64_t* int_args_pointer;
	uint64_t amount_of_ints;

	double* double_args_pointer;
	uint64_t amount_of_doubles;

	struct string_save* string_args_pointer;
	uint64_t amount_of_strings;
} __attribute__((packed));

extern char* node_types[];

extern char*  insert_targets[];

extern char* logical_ops[];

extern char* cmps[];

//Answer* run_request(Ast*);
Node* run_request(Ast* ast);

char* uint_array_to_str(uint64_t* arr, uint64_t n, uint64_t* retsz);
char* double_array_to_str(double* arr, uint64_t n, uint64_t* retsz);

char* string_saves_to_char(struct string_save* saver, uint64_t size);