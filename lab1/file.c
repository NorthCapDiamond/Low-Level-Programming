#include "file.h"

#define INVALID_PTR 0

# define file_size(size, fl){\
	fseek(fl, 0, SEEK_END);\
	size = ftell(fl);\
}




void* read_file(size_t offset, size_t length){
	fseek(file, offset, SEEK_SET);
	void* buffer = malloc(length);
	fread(buffer, 1, length, file);
	return buffer;
	// someone must make free after
}


size_t try_to_write_in_hole(void* data_ptr, size_t size_of_data){
	struct header* file_header = read_file(0, sizeof(struct header));
	size_t hole_ptr = file_header->first_hole_ptr;

	size_t save_first_hole_ptr = file_header->first_hole_ptr;

	free(file_header);
	struct hole* iter_hole;
	while(hole_ptr!=INVALID_PTR){
		iter_hole = read_file(hole_ptr, sizeof(struct hole));
		
		if(iter_hole->size_of_hole == size_of_data){
			if (hole_ptr!=iter_hole->hole_ptr){

				size_t save_ptr = iter_hole->hole_ptr;

				fseek(file, iter_hole->hole_ptr, SEEK_SET);
				fwrite(data_ptr, 1, size_of_data, file);

				iter_hole->size_of_hole = sizeof(struct hole);
				iter_hole->hole_ptr = hole_ptr;



				fseek(file, hole_ptr, SEEK_SET);
				fwrite(iter_hole, 1, sizeof(struct hole), file);

				free(iter_hole);
				return save_ptr;

			}else{

				if(hole_ptr==save_first_hole_ptr){
						file_header = read_file(0, sizeof(struct header));
						file_header->first_hole_ptr = iter_hole->next_ptr;

						fseek(file, 0, SEEK_SET);
						fwrite(file_header, 1, sizeof(struct header), file);
						free(file_header);

					}

				size_t save_ptr = iter_hole->hole_ptr;

				if(iter_hole->next_ptr!=INVALID_PTR){
					struct hole* next_hole = read_file(iter_hole->next_ptr, sizeof(struct hole));
					next_hole->prev_ptr = iter_hole->prev_ptr;

					fseek(file, iter_hole->next_ptr, SEEK_SET);
					fwrite(next_hole, 1, sizeof(struct hole), file);

					free(next_hole);

				}


				if(iter_hole->prev_ptr!=INVALID_PTR){
					struct hole* prev_hole = read_file(iter_hole->prev_ptr, sizeof(struct hole));
					prev_hole->next_ptr = iter_hole->next_ptr;

					fseek(file, iter_hole->prev_ptr, SEEK_SET);
					fwrite(prev_hole, 1, sizeof(struct hole), file);

					free(prev_hole);
				}

				fseek(file, iter_hole->hole_ptr, SEEK_SET);
				fwrite(data_ptr, 1, size_of_data, file);
				free(iter_hole);

				return save_ptr;
			}
		}else{
			if(iter_hole->size_of_hole > size_of_data){
				if(hole_ptr!=iter_hole->hole_ptr){

					size_t save_ptr = iter_hole->hole_ptr;

					fseek(file, iter_hole->hole_ptr, SEEK_SET);
					fwrite(data_ptr, 1, size_of_data, file);
					iter_hole->hole_ptr+=size_of_data;
					iter_hole->size_of_hole-=size_of_data;

					fseek(file, hole_ptr, SEEK_SET);
					fwrite(iter_hole, 1, sizeof(struct hole), file);
					free(iter_hole);
					return save_ptr;
				}
			}
		}

		hole_ptr = iter_hole->next_ptr;
		free(iter_hole);
	}

	return INVALID_PTR;
}

size_t write_file(void* data_ptr, size_t size_of_data){
	size_t size;
	file_size(size, file);

	size_t return_try_hole = try_to_write_in_hole(data_ptr, size_of_data);

	if(return_try_hole!=INVALID_PTR){
		return return_try_hole;
	}

	fseek(file, 0, SEEK_END);
	fwrite(data_ptr, 1, size_of_data, file);
	return size;
}


void delete_from_file(size_t offset, size_t length){
	struct header* file_header = read_file(0, sizeof(struct header));
	if(file_header->first_hole_ptr == INVALID_PTR){
		struct hole first_real_hole = (struct hole) {hole_ptr: offset, size_of_hole:length, prev_ptr:INVALID_PTR, next_ptr:INVALID_PTR};	
		file_header->first_hole_ptr = write_file(&first_real_hole, sizeof(struct hole));
		fseek(file, 0, SEEK_SET);
		fwrite(file_header, 1, sizeof(file_header), file);
		free(file_header);
		return;
	} 

	struct hole new_hole = (struct hole) {hole_ptr: offset, size_of_hole:length, prev_ptr:INVALID_PTR, next_ptr:file_header->first_hole_ptr};

	struct hole* first_hole = read_file(file_header->first_hole_ptr, sizeof(struct hole));
	first_hole->prev_ptr = write_file(&new_hole, sizeof(struct hole));


	fseek(file, file_header->first_hole_ptr, SEEK_SET);
	fwrite(first_hole, 1, sizeof(struct hole), file);
	free(first_hole);

	fseek(file, 0, SEEK_SET);
	fwrite(file_header, 1, sizeof(file_header), file);
	free(file_header);
	return;
}



FILE* open_file(char* name){
	FILE* f = fopen(name, "r+");

	if(f==NULL){
		return NULL;
	}

	size_t size_of_file;
	file_size(size_of_file, f);

	

	if(size_of_file < sizeof(struct header)){
		ftrunc(fileno(f), sizeof(struct header));
	}

	fseek(f, 0, SEEK_SET);
	struct header* file_header = (struct header*)malloc(sizeof(struct header));
	fread(file_header, 1, sizeof(struct header), f);


	uint32_t signature = file_header->signature;

	if(signature == 0xDEADDEAD){
		return f;
	}

	

	file_header->signature = 0xDEADDEAD;
	file_header->first_hole_ptr = INVALID_PTR;
	file_header->first_node_ptr= INVALID_PTR;
	file_header->node_id = 0;


	ftrunc(fileno(f), sizeof(struct header));
	fseek(f, 0, SEEK_SET);
	fwrite(file_header, 1, sizeof(struct header), f);
	printf("AKAK\n");
	free(file_header);
	return f;
}


int main(int argc, char** argv){

	file = open_file("HUI");
	// char buff[4] = "HUI";
	// write_file(buff, 4);
    // char buff[0x20] = "HIIIIIIIIIIIIIIIIIIIIIIIIIIIIII";
	// write_file(buff, 0x20); 
	delete_from_file(sizeof(struct header), 4);
	char buff[4] = "HUI";
	write_file(buff, 4);

	return 0;
}

