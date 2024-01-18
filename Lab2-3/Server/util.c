#include "util.h"
#include "file.h"

char* node_types[] = {
        [AST_NODE_TYPE__FILENAME_N] = "Filename",
        [AST_NODE_TYPE__INSERT_N] = "Insert",
        [AST_NODE_TYPE__SELECT_N] = "Select",
        [AST_NODE_TYPE__UPDATE_N] = "Update",
        [AST_NODE_TYPE__DELETE_N] = "Delete",
        [AST_NODE_TYPE__INDEX_N] = "Index",
        [AST_NODE_TYPE__NODE_N] = "Node",
        [AST_NODE_TYPE__LINK_N] = "Link",
        [AST_NODE_TYPE__CONDITIONAL_N] = "Conditional",
        [AST_NODE_TYPE__FILTER_N] = "Filter",
        [AST_NODE_TYPE__SET_N] = "Set",
        [AST_NODE_TYPE__VAL_N] = "Val",
        [AST_NODE_TYPE__NAME_N] = "Name",
        [AST_NODE_TYPE__LIST_N] = "List",
        [AST_NODE_TYPE__ALL_LINKS_N] = "All Links",
};

char*  insert_targets[] = {
        [INSERT_TARGET__I_NODE] = "I_Node",
        [INSERT_TARGET__I_LINK] = "I_Link",
        [INSERT_TARGET__I_INDEX] = "I_Index",
};

char* logical_ops[] = {
        [LOGICAL_OP__AND] = "And",
        [LOGICAL_OP__OR] = "Or",
        [LOGICAL_OP__NOT] = "Not",
};

char* cmps[] = {
        [CMP__GT] = "gt",
        [CMP__LT] = "l",
        [CMP__EQ] = "eq",
        [CMP__GT_EQ] = "gte",
        [CMP__LT_EQ] = "le",
        [CMP__SUBSTR] = "substr",
};

char* value_types[] = {
        [VALUE_TYPE__NONE] = "None",
        [VALUE_TYPE__NAME] = "Name",
        [VALUE_TYPE__INSERT_TARGET] = "Insert target",
        [VALUE_TYPE__SET] = "Set",
        [VALUE_TYPE__LOGICAL_OP] = "Logical Operation",
        [VALUE_TYPE__CMP] = "CMP",
        [VALUE_TYPE__LINK] = "Link",
        [VALUE_TYPE__CNT] = "CNT",
        [VALUE_TYPE__ATTR_DESC] = "Attr Desc",

};

char* val_types[] = {
        [VAL_TYPE__INT] = "Int",
        [VAL_TYPE__DOUBLE] = "Double",
        [VAL_TYPE__STRING] = "String",
        [VAL_TYPE__BOOL] = "Bool",
};

char* uint_array_to_str(uint64_t* arr, uint64_t n, uint64_t* retsz){
  char* ret = (char*)malloc(4096);
  char* retp = ret;

  for(uint64_t i = 0; i < n; i++){
    sprintf(retp, "%lld, ", arr[i]);
    uint64_t len = strlen(retp);
    retp[len] = ' ';
    retp += len+1;
  }
  *retsz = retp-ret;
  return ret;
}


char* double_array_to_str(double* arr, uint64_t n, uint64_t* retsz){
  char* ret = (char*)malloc(4096);
  char* retp = ret;

  for(uint64_t i = 0; i < n; i++){
    sprintf(retp, "%lf, ", arr[i]);
    uint64_t len = strlen(retp);
    retp[len] = ' ';
    retp += len+1;
  }
  *retsz = retp-ret;
  return ret;

}

char* string_saves_to_char(struct string_save* saver, uint64_t size){
        char* ret = malloc(0);
        uint64_t ptr = 0;
        uint64_t sizer = 0;

        for(uint64_t i = 0; i < size; i++){
                sizer += saver->size_of_string + 1;
                ret = realloc(ret, (sizer) * sizeof(char));
                for(uint64_t j = 0; j < saver->size_of_string; j++){
                        ret[ptr] = saver[j].string_line_ptr;
                        ptr++;
                }
                ret[ptr] = " ";
                ptr++;
        }

        printf("%s\n", ret);
        return ret;

}


uint64_t* found_nodes_ids = NULL;
uint64_t n_found_nodes_ids = 0;
int flag_parse = 0;
Node* my_answer = NULL;

struct node* my_node = NULL;
uint64_t size_of_struct_node = 0;

struct value_keeper* saver = NULL;


Node* run_request(Ast* ast){
        printf("\n\nIn switch case...\n");
        printf("%s\n", node_types[ast->type]);
	switch(ast->type){
                case AST_NODE_TYPE__ALL_LINKS_N:{
                        uint64_t id = ast->val->link->link_id_one;

                        // if (id == 0){
                        //         my_answer = full_links(my_answer, 0);
                        // }

                        if(find_node_by_id(id) == NULL){
                                my_answer->msg = "Node was not found!\n";
                                break;
                        }

                        else if (find_node_by_id(id) > 0){
                                my_answer = current_id_links(id, my_answer);
                        }

                        break;
                }
                case AST_NODE_TYPE__LINK_N:{
                        printf("%s\n", value_types[ast->type]);
                        int check = connect_nodes_checked(ast->val->link->link_id_one, ast->val->link->link_id_two);

                        if (check == -1){
                                my_answer->msg = "Connection failed. Probably There is no one of nodes\n";
                        }
                        if(check == 0){
                                my_answer->msg = "Nodes are already connected\n";
                        }

                        if(check == 1){
                                my_answer->msg = "Nodes connected\n";
                        }
                        break;
                }

                case AST_NODE_TYPE__UPDATE_N:{

                        saver = (struct value_keeper*) malloc(sizeof(struct value_keeper));
                        *saver = (struct value_keeper) {
                            .int_args_pointer = NULL,
                            .amount_of_ints = 0,
                            .double_args_pointer = NULL,
                            .amount_of_doubles = 0,
                            .string_args_pointer = NULL,
                            .amount_of_strings = 0
                        };

                        if(ast->left){
                                printf("\nFor Update left is %s\n", node_types[ast->left->type]);
                                run_request(ast->left);
                        }

                        if(ast->right){
                                printf("\nFor Update right is %s\n", node_types[ast->right->type]);
                                run_request(ast->right);
                        }

                        if(saver->amount_of_ints>0){

                                printf("Forming Node updater\n");
                                my_node = realloc(my_node, (size_of_struct_node+1) * sizeof(struct node));
                                my_node[size_of_struct_node] = *node_updater(saver->int_args_pointer[0], saver->amount_of_ints, saver->int_args_pointer, saver->amount_of_doubles, saver->double_args_pointer, saver->amount_of_strings, saver->string_args_pointer);
                                size_of_struct_node++;

                                my_answer = one_node_from_server_to_client_form_answer(&my_node[size_of_struct_node-1], my_answer);


                                printf("Creating output node\n");



                                my_answer->msg = "function update was applied to the node\n";

                        }
                        else{
                                my_answer->msg = "Node was not found\n";
                        }


                        break;
                }

                case AST_NODE_TYPE__DELETE_N: {
                        printf("Delete Case\b");


                        my_answer->msg = "Deleted";

                        if(ast->left){
                                printf("\nFor Delete left is %s\n", node_types[ast->left->type]);
                                run_request(ast->left);
                        }

                        if(ast->right){
                                printf("\nFor Delete right is %s\n", node_types[ast->right->type]);
                                run_request(ast->right);
                        }

                        printf("%llu\n", n_found_nodes_ids);
                        if(n_found_nodes_ids>0){

                                printf("Delete id is : %llu\n", found_nodes_ids[n_found_nodes_ids-1]);
                                delete_node_by_id(found_nodes_ids[n_found_nodes_ids-1]);
                                my_answer->msg = "Node was Deleted!\n";
                        }
                        else{
                                my_answer->msg = "Node was not found\n";
                        }


                        break;

                }

                case AST_NODE_TYPE__INSERT_N:{


                        saver = (struct value_keeper*) malloc(sizeof(struct value_keeper));
                        *saver = (struct value_keeper) {
                            .int_args_pointer = NULL,
                            .amount_of_ints = 0,
                            .double_args_pointer = NULL,
                            .amount_of_doubles = 0,
                            .string_args_pointer = NULL,
                            .amount_of_strings = 0
                        };


                        if(ast->left){
                                printf("\nFor Insert left is %s\n", node_types[ast->left->type]);
                                run_request(ast->left);
                        }

                        if(ast->right){
                                printf("\nFor Insert right is %s\n", node_types[ast->right->type]);
                                run_request(ast->right);
                        }



                        printf("Check node\n");
                        printf("%llu\n", saver->amount_of_ints);
                        printf("%llu\n", saver->int_args_pointer[0]);
                        printf("%llu\n", saver->amount_of_doubles);
                        printf("%lf\n", saver->double_args_pointer[0]);
                        printf("%llu\n", saver->amount_of_strings);

                        my_node = realloc(my_node, (size_of_struct_node + 1) * sizeof(struct node));

                        my_node[size_of_struct_node] = *create_node_params(saver->amount_of_ints, 
                                         saver->int_args_pointer, 
                                         saver->amount_of_doubles, 
                                         saver->double_args_pointer, 
                                         saver->amount_of_strings, 
                                         saver->string_args_pointer
                                         );
                        size_of_struct_node++;

                        my_answer = one_node_from_server_to_client_form_answer(&my_node[size_of_struct_node-1], my_answer);

                        my_answer->msg = "Insert finished";


                        printf("Insert function : create node finished!\n");
                        break;
                }

                case AST_NODE_TYPE__NODE_N:{
                        if(ast->left){
                                printf("\nFor Node left is %s\n", node_types[ast->left->type]);
                                run_request(ast->left);
                        }

                        if(ast->right){
                                printf("\nFor Node right is %s\n", node_types[ast->right->type]);
                                run_request(ast->right);
                        }
                        break;
                }

                case AST_NODE_TYPE__LIST_N:{
                        if(ast->left){
                                printf("\nFor List left is %s\n", node_types[ast->left->type]);
                                run_request(ast->left);
                        }

                        if(ast->right){
                                printf("\nFor List right is %s\n", node_types[ast->right->type]);
                                run_request(ast->right);
                        }
                        break;
                }

                case AST_NODE_TYPE__FILENAME_N:{
                        if(n_found_nodes_ids>0){
                                free(found_nodes_ids);
                                found_nodes_ids = NULL;
                                n_found_nodes_ids = 0;
                        }

                        if(size_of_struct_node>0){
                                free(my_node);
                                my_node = NULL;
                                size_of_struct_node = 0;
                        }

                        free(my_answer);
                        my_answer = malloc(sizeof(Node));
                        node__init(my_answer);

                
                        printf("\nOpen File : %s \n", ast->val->name);
                        if(ast->left){
                                printf("\nFor Filename left is %s\n", node_types[ast->left->type]);
                                if (file!=NULL){
                                        fclose(file);
                                        file = NULL;
                                }
                                file = open_file(ast->val->name);
                                run_request(ast->left);
                        }

                        return my_answer;
                        break;
                }

		case AST_NODE_TYPE__SELECT_N:{

                        saver = (struct value_keeper*) malloc(sizeof(struct value_keeper));
                        *saver = (struct value_keeper) {
                            .int_args_pointer = NULL,
                            .amount_of_ints = 0,
                            .double_args_pointer = NULL,
                            .amount_of_doubles = 0,
                            .string_args_pointer = NULL,
                            .amount_of_strings = 0
                        };

			if(ast->left){
                                printf("\nFor Select left is %s\n", node_types[ast->left->type]);
				run_request(ast->left);
			}

			if(ast->right){
                                printf("\nFor Select right is %s\n", node_types[ast->right->type]);
				run_request(ast->right);
			}

                        printf("Select is going to prepare the answer...\n");
                        printf("Init Message\n");

                        printf("Put Id\n");
                        
                        if(n_found_nodes_ids > 0){
                               my_answer = one_node_from_server_to_client_form_answer(&my_node[size_of_struct_node-1], my_answer);
                               printf("DEBUG %llu\n", my_answer->id);
                        }
                        else{
                               my_answer->msg = "No Such Node Exception";
                               printf("%s\n", my_answer->msg);
                        }


                        printf("Answer from Select is ready!\n");
                        break;
		}

		case AST_NODE_TYPE__INDEX_N:{
                        if(ast->val->name){
                                printf("Index value name is : %s\n", ast->val->name);
                        }

                        if(n_found_nodes_ids>=0){
                                if(find_node_by_id(atoll(ast->val->name))!=INVALID_PTR){
                                        n_found_nodes_ids++;
                                        found_nodes_ids = realloc(found_nodes_ids, n_found_nodes_ids * 8);
                                        found_nodes_ids[n_found_nodes_ids-1] = atoll(ast->val->name);
                                        printf("Check index : %llu", found_nodes_ids[n_found_nodes_ids-1]);

                                }
                        }

                        if(ast->left){
                                printf("\nFor Index left is %s\n", node_types[ast->left->type]);
                        }
                        if(ast->right){
                                printf("\nFor Index right is %s\n", node_types[ast->right->type]);

                        }

                        break;
		}

		case AST_NODE_TYPE__FILTER_N:{
			if(ast->left){
                                printf("\nFor Filter left is %s\n", node_types[ast->left->type]);
                                run_request(ast->left);
                        }
                        if(ast->right){
                                printf("\nFor Filter right is %s\n", node_types[ast->right->type]);
                                run_request(ast->right);

                        }
                        break;
		}

                case AST_NODE_TYPE__CONDITIONAL_N:{

                        if(ast->v_type){
                                printf("\nIn Conditional cmp opertion is %s\n", cmps[ast->val->cmp]);
                        }

                        if(ast->left){
                                printf("\nFor Conditional left is %s\n", node_types[ast->left->type]);
                                run_request(ast->left);
                        }
                        if(ast->right){
                                printf("\nFor Conditional right is %s\n", node_types[ast->right->type]);
                                run_request(ast->right);

                        }

                        printf("\nFor Conditional this is %s\n", node_types[ast->type]);
                        printf("Type of val is : %s\n", val_types[ast->right->val->attr->type]);


                        switch(ast->right->val->attr->type){
                                case VAL_TYPE__INT: {
                                        uint64_t param = saver->int_args_pointer[saver->amount_of_ints - 1];
                                        switch(ast->val->cmp){
                                                case CMP__EQ:{
                                                        printf("Eq for integer\n");
                                                        if(n_found_nodes_ids==0){
                                                                break;
                                                        }
                                                        if(find_by_id_param_i(param, found_nodes_ids[n_found_nodes_ids -1], 2) != NULL){
                                                                my_node = realloc(my_node, (size_of_struct_node+1) * sizeof(struct node));
                                                                my_node[size_of_struct_node] = *find_by_id_param_i(param, found_nodes_ids[n_found_nodes_ids - 1], 2);
                                                                size_of_struct_node++;

                                                                printf("CMP worked\n");
                                                        }

                                                        break;
                                                }
                                                case CMP__GT:{
                                                        printf("GT for integer\n");
                                                        if(n_found_nodes_ids==0){
                                                                break;
                                                        }
                                                        if(find_by_id_param_i(param, found_nodes_ids[n_found_nodes_ids -1], 0) != NULL){
                                                                my_node = realloc(my_node, (size_of_struct_node+1) * sizeof(struct node));
                                                                my_node[size_of_struct_node] = *find_by_id_param_i(param, found_nodes_ids[n_found_nodes_ids - 1], 0);
                                                                size_of_struct_node++;

                                                                printf("CMP worked\n");
                                                        }

                                                        break;
                                                        
                                                }
                                                case CMP__LT:{
                                                        printf("LT for integer\n");
                                                        if(n_found_nodes_ids==0){
                                                                break;
                                                        }
                                                        if(find_by_id_param_i(param, found_nodes_ids[n_found_nodes_ids -1], 1) != NULL){
                                                                my_node = realloc(my_node, (size_of_struct_node+1) * sizeof(struct node));
                                                                my_node[size_of_struct_node] = *find_by_id_param_i(param, found_nodes_ids[n_found_nodes_ids - 1], 1);
                                                                size_of_struct_node++;

                                                                printf("CMP worked\n");

                                                        }

                                                        break;
                                                        
                                                }
                                                case CMP__GT_EQ:{
                                                        printf("GTE for integer\n");
                                                        if(n_found_nodes_ids==0){
                                                                break;
                                                        }
                                                        if(find_by_id_param_i(param, found_nodes_ids[n_found_nodes_ids -1], 3) != NULL){
                                                               my_node = realloc(my_node, (size_of_struct_node+1) * sizeof(struct node));
                                                                my_node[size_of_struct_node] = *find_by_id_param_i(param, found_nodes_ids[n_found_nodes_ids - 1], 3);
                                                                size_of_struct_node++;

                                                                printf("CMP worked\n");
                                                        }

                                                        break;
                                                        
                                                }
                                                case CMP__LT_EQ:{
                                                        printf("LTE for integer\n");
                                                        if(n_found_nodes_ids==0){
                                                                break;
                                                        }
                                                        if(find_by_id_param_i(param, found_nodes_ids[n_found_nodes_ids -1], 4) != NULL){
                                                                my_node = realloc(my_node, (size_of_struct_node+1) * sizeof(struct node));
                                                                my_node[size_of_struct_node] = *find_by_id_param_i(param, found_nodes_ids[n_found_nodes_ids - 1], 4);
                                                                size_of_struct_node++;

                                                                printf("CMP worked\n");
                                                        }

                                                        break;
                                                        
                                                }
                                        }
                                        break;
                                }
                                case VAL_TYPE__DOUBLE:{
                                        double param = saver->double_args_pointer[saver->amount_of_doubles - 1];
                                        switch(ast->val->cmp){
                                                case CMP__EQ:{
                                                        printf("Eq for doubles\n");
                                                        if(n_found_nodes_ids==0){
                                                                break;
                                                        }
                                                        if(find_by_id_param_d(param, found_nodes_ids[n_found_nodes_ids -1], 2) != NULL){
                                                                my_node = realloc(my_node, (size_of_struct_node+1) * sizeof(struct node));
                                                                my_node[size_of_struct_node] = *find_by_id_param_d(param, found_nodes_ids[n_found_nodes_ids - 1], 2);
                                                                size_of_struct_node++;

                                                                printf("CMP worked\n");
                                                        }

                                                        break;
                                                }
                                                case CMP__GT:{
                                                        printf("GT for doubles\n");
                                                        if(n_found_nodes_ids==0){
                                                                break;
                                                        }
                                                        if(find_by_id_param_d(param, found_nodes_ids[n_found_nodes_ids -1], 0) != NULL){
                                                                my_node = realloc(my_node, (size_of_struct_node+1) * sizeof(struct node));
                                                                my_node[size_of_struct_node] = *find_by_id_param_d(param, found_nodes_ids[n_found_nodes_ids - 1], 0);
                                                                size_of_struct_node++;

                                                                printf("CMP worked\n");
                                                        }

                                                        break;
                                                        
                                                }
                                                case CMP__LT:{
                                                        printf("LT for doubles\n");
                                                        if(n_found_nodes_ids==0){
                                                                break;
                                                        }
                                                        if(find_by_id_param_d(param, found_nodes_ids[n_found_nodes_ids -1], 1) != NULL){
                                                                my_node = realloc(my_node, (size_of_struct_node+1) * sizeof(struct node));
                                                                my_node[size_of_struct_node] = *find_by_id_param_d(param, found_nodes_ids[n_found_nodes_ids - 1], 1);
                                                                size_of_struct_node++;

                                                                printf("CMP worked\n");
                                                        }

                                                        break;
                                                        
                                                }
                                                case CMP__GT_EQ:{
                                                        printf("GTE for doubles\n");
                                                        if(n_found_nodes_ids==0){
                                                                break;
                                                        }
                                                        if(find_by_id_param_d(param, found_nodes_ids[n_found_nodes_ids -1], 3) != NULL){
                                                                my_node = realloc(my_node, (size_of_struct_node+1) * sizeof(struct node));
                                                                my_node[size_of_struct_node] = *find_by_id_param_d(param, found_nodes_ids[n_found_nodes_ids - 1], 3);
                                                                size_of_struct_node++;

                                                                printf("CMP worked\n");
                                                        }

                                                        break;
                                                }
                                                case CMP__LT_EQ:{
                                                        printf("LTE for doubles\n");
                                                        if(n_found_nodes_ids==0){
                                                                break;
                                                        }
                                                        if(find_by_id_param_d(param, found_nodes_ids[n_found_nodes_ids -1], 4) != NULL){
                                                                my_node = realloc(my_node, (size_of_struct_node+1) * sizeof(struct node));
                                                                my_node[size_of_struct_node] = *find_by_id_param_d(param, found_nodes_ids[n_found_nodes_ids - 1], 4);
                                                                size_of_struct_node++;

                                                                printf("CMP worked\n");
                                                        }

                                                        break;
                                                        
                                                }
                                        }
                                }
                                case VAL_TYPE__STRING: {

                                        struct string_save* param = &(saver->string_args_pointer[saver->amount_of_strings -1]);
                                        if(n_found_nodes_ids==0){
                                                                break;
                                                        }
                                        switch(ast->val->cmp){
                                                case CMP__EQ:{


                                                        char* my_string = saver->string_args_pointer[saver->amount_of_strings - 1].string_line_ptr;
                                                        saver->int_args_pointer = realloc(saver->int_args_pointer, (saver->amount_of_ints+1)*sizeof(uint64_t));
                                                        saver->int_args_pointer[saver->amount_of_ints] = equalstr(my_string, found_nodes_ids[n_found_nodes_ids - 1]);
                                                        saver->amount_of_ints++;
                                                        saver->string_args_pointer = realloc(saver->string_args_pointer, (saver->amount_of_strings - 1)*sizeof(saver->amount_of_strings));
                                                        saver->amount_of_strings--;


                                                        break;

                                                }

                                                case CMP__SUBSTR:{


                                                        char* my_string = saver->string_args_pointer[saver->amount_of_strings - 1].string_line_ptr;
                                                        saver->int_args_pointer = realloc(saver->int_args_pointer, (saver->amount_of_ints+1)*sizeof(uint64_t));
                                                        saver->int_args_pointer[saver->amount_of_ints] = substrs(my_string, found_nodes_ids[n_found_nodes_ids - 1]);
                                                        saver->amount_of_ints++;
                                                        saver->string_args_pointer = realloc(saver->string_args_pointer, (saver->amount_of_strings - 1)*sizeof(saver->amount_of_strings));
                                                        saver->amount_of_strings--;
                                                        break;
                                                        
                                                }
                                        }
                                }
                        }

                        break;
                }

                case AST_NODE_TYPE__NAME_N:{
                        if(ast->left){
                                printf("\nFor Name left is %s\n", node_types[ast->left->type]);
                                run_request(ast->left);
                                
                        }
                        if(ast->right){
                                printf("\nFor Name right is %s\n", node_types[ast->right->type]);
                                run_request(ast->right);
                        }

                        if(ast->val->name){
                                printf("Data in Name is : %s\n", ast->val->name);
                        }

                        break;
                }

                case  AST_NODE_TYPE__VAL_N:{
                        if(ast->val->attr->type){
                                printf("Data in Val is : %s\n", val_types[ast->val->attr->type]);
                        }


                        printf("Enter Switch case in Val\n");
                        switch(ast->val->attr->type){
                                case VAL_TYPE__INT:{

                                        saver->amount_of_ints++;
                                        saver->int_args_pointer = realloc(saver->int_args_pointer, saver->amount_of_ints*8);
                                        saver->int_args_pointer[saver->amount_of_ints-1] = ast->val->attr->i;

                                        printf("Int Value is : %llu\n", saver->int_args_pointer[saver->amount_of_ints-1]);

                                        break;

                                }
                                case VAL_TYPE__DOUBLE:{

                                        saver->amount_of_doubles++;
                                        saver->double_args_pointer = realloc(saver->double_args_pointer, saver->amount_of_doubles * sizeof(double));
                                        saver->double_args_pointer[saver->amount_of_doubles-1] = ast->val->attr->d;

                                        printf("Double Value is : %lf\n", saver->double_args_pointer[saver->amount_of_doubles-1]);

                                        break;

                                }
                                case VAL_TYPE__STRING:{

                                        saver->amount_of_strings++;
                                        printf("++\n");
                                        saver->string_args_pointer = realloc(saver->string_args_pointer, saver->amount_of_strings * sizeof(struct string_save));
                                        printf("Alloc pointer Finished\n");
                                        saver->string_args_pointer[saver->amount_of_strings-1].size_of_string = ast->val->attr->str->size;
                                        printf("Set size Finished\n");
                                        char* my_str = (char*)ast->val->attr->str->str;
                                        printf("My string is %s set finished\n", my_str);
                                        saver->string_args_pointer[saver->amount_of_strings-1].string_line_ptr = my_str;
                                        printf("Set pointer Finished\n");
                                        printf("String pointer is : %llu set finished\n", saver->string_args_pointer[saver->amount_of_strings-1].string_line_ptr);
                                        printf("String size is : %llu set finished\n", saver->string_args_pointer[saver->amount_of_strings-1].size_of_string);
                                        printf("String amount is : %llu set finished\n", saver->amount_of_strings);

                                        break;
                                }
                        }
                        break;
                }
	}
}