#include "file.h"
#include "output.pb-c.h"
#define INVALID_PTR 0

extern struct value_keeper;

struct string_save{
	uint64_t size_of_string;
	uint64_t string_line_ptr;
}; __attribute__((packed));


struct node {
	uint64_t id; // helps to count max node_id
	uint64_t d; // graph vertex degree
	uint64_t nodes; // pointer on connected nodes
	uint64_t prev_ptr; // pointer on the previous node in linked list
	uint64_t next_ptr; // pointer on the next node in linked list

	uint64_t n_ints; // amount of integers in node
	uint64_t ints_array_ptr; // pointer on integer array

	uint64_t n_doubles; // amount of doubles in node
	uint64_t doubles_array_ptr; // pointer on doubles in node

	uint64_t n_strings; // amount of strings in node
	uint64_t strings_array_ptr; // pointer os a special struct string_save, containing size of line and pointer on line
} __attribute__((packed));



char* read_var_string(void);
uint64_t find_node_by_id(uint64_t id); // searches node from list of  nodes by its id
uint64_t find_id_by_addr(uint64_t addr);
void delete_node_by_id(uint64_t id); // delete node from list of  nodes by its id
void create_node(void); 
void connect_nodes(uint64_t id1, uint64_t id2);
int connect_nodes_checked(uint64_t a, uint64_t b);
void print_nodes(void);
void print_info_node(uint64_t id);
void append_int(uint64_t id);
void append_double(uint64_t id);
void append_string(uint64_t id);
void remover(void);
void create_node_test(uint64_t int_amount, uint64_t* int_ptr, uint64_t double_amount, double* double_ptr, uint64_t string_amount, struct string_save* string_ptr);
struct node* create_node_params(uint64_t int_amount, uint64_t* int_ptr, uint64_t double_amount, double* double_ptr, uint64_t string_amount, struct string_save* string_ptr);
void remove_idxes(uint64_t i1, uint64_t i2);
struct node* find_by_id_param_i(uint64_t value, uint64_t id, int op);
struct node* find_by_id_param_d(double value, uint64_t id, int op);
struct node* substrs(char* string, uint64_t id);
struct node* equalstr(char* string, uint64_t id);
struct node* node_updater(uint64_t id, uint64_t int_amount, uint64_t* int_ptr, uint64_t double_amount, double* double_ptr, uint64_t string_amount, struct string_save* string_ptr);
Node* one_node_from_server_to_client_form_answer(struct node* iter_node, Node* my_answer);
Node* current_id_links(uint64_t id, Node* my_answer);
Node* full_links(Node* my_answer, uint64_t id);

