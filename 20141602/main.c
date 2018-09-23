#include "bitmap.h"
#include "list.h"
#include "debug.h"
#include "hash.h"
#include "round.h"
#include "limits.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void quit();

int find_datastruct_number(int type, char *name);

///////////////////////////////////// for list

struct list_data{
	struct list_elem elem;
	int data;
};

struct list_array{
	char list_name[15];
	struct list n_list;
	bool exist;
}list_arr[10];

struct list_elem *list_e0, *list_e1, *list_e2;
bool list_less(const struct list_elem *a, const struct list_elem *b, void *aux);
void list_swap(struct list_elem *a, struct list_elem *b);
void list_shuffle(struct list *list);


struct list_data *list_data_tmp;


///////////////////////////////////// for hash	


struct hash_data *hash_data_tmp;

unsigned hash_int_2(int i);

struct hash_data{
	struct hash_elem elem;
	int data;
};

struct hash_array{
	char hash_name[15];
	struct hash n_hash;
	bool exist;
}hash_arr[10];

struct hash_iterator hash_i0;
struct hash_elem *hash_e0, *hash_e1;

bool hash_less(const struct hash_elem *a, const struct hash_elem *b, void *aux);
unsigned hash_func(const struct hash_elem *e, void *aux);

void hash_destruct(struct hash_elem *e, void *aux){
	if(aux == NULL){
		free(hash_entry(e, struct hash_data, elem));
	}
}

void hash_square(struct hash_elem *e, void *aux){

	int tmpres;
	hash_data_tmp = hash_entry(e, struct hash_data, elem);
	tmpres = hash_data_tmp->data;
	if (aux == NULL){
		hash_data_tmp->data = tmpres * tmpres;
	}
}

void hash_triple(struct hash_elem *e, void *aux){
	int tmpres;
	hash_data_tmp = hash_entry(e, struct hash_data, elem);
	tmpres = hash_data_tmp->data;
	if (aux == NULL){
		hash_data_tmp->data = tmpres * tmpres * tmpres;
	}
}

////////////////////////////////////// for bitmap


struct bitmap_array{
	char bitmap_name[15];
	struct bitmap *n_bitmap;
	bool exist;
}bitmap_arr[10];

struct bitmap *bitmap_expand(struct bitmap *bitmap, int size);


int main(){

	char input_command[1001];
	char *token_command[10];
	int i, j, k;
	int data_structure_number;


	for (i = 0 ; i < 10 ; i++){
		list_arr[i].exist = false;
		hash_arr[i].exist = false;
		bitmap_arr[i].exist = false;
	}

	while(1){
		
		fgets(input_command, 1001, stdin);
		input_command[strlen(input_command)-1] = '\0';		
		
		i = 0;
		token_command[i++] = strtok(input_command, " \t\0");
		while(token_command[i++] = strtok(NULL, " \t\0"));
		
		
		i = j = k = 0;
		if (!strcmp(token_command[0], "create")){
			if (!strcmp(token_command[1], "list")){
				while(i < 10){
					if (list_arr[i].exist == false)
						break;
					i++;
				}
				strcpy(list_arr[i].list_name, token_command[2]);
				list_arr[i].exist = true;
				list_init(&(list_arr[i].n_list));
			}
			else if (!strcmp(token_command[1] , "hashtable")){
				while(i<10){
					if (hash_arr[i].exist == false)
						break;
					i++;
				}
				strcpy(hash_arr[i].hash_name, token_command[2]);
				hash_arr[i].exist = true;
				hash_init(&hash_arr[i].n_hash, hash_func, hash_less ,NULL);	
			}
			else if (!strcmp(token_command[1], "bitmap")){
				while(i < 10){
					if (bitmap_arr[i].exist == false)
						break;
					i++;
				}
				strcpy(bitmap_arr[i].bitmap_name, token_command[2]);
				bitmap_arr[i].exist = true;
				bitmap_arr[i].n_bitmap = bitmap_create(atoi(token_command[3]));
			}
		}
		else if(!strcmp(token_command[0], "delete")){
			j = find_datastruct_number(0, token_command[1]);
			if (j < 10){
				while(!list_empty(&(list_arr[j].n_list))){
					list_e0 = list_pop_front(&(list_arr[j].n_list));
					list_data_tmp = list_entry(list_e0, struct list_data, elem);	
					free(list_data_tmp);
				}
				strcpy(list_arr[j].list_name, "\0");
				list_arr[j].exist = false;
			}
			else{
				j = find_datastruct_number(1, token_command[1]);
				if (j<10){
					hash_destroy(&(hash_arr[j].n_hash), hash_destruct);
					hash_arr[j].exist = false;
				}
				else{
					j = find_datastruct_number(2, token_command[1]);
					bitmap_destroy(bitmap_arr[j].n_bitmap);
					bitmap_arr[j].exist = false;
				}
			}
		
		}

		else if (!strcmp(token_command[0], "dumpdata")){
			j = find_datastruct_number(0, token_command[1]);
			if (j<10){
				list_e0 = list_begin(&(list_arr[j].n_list));
				i = 0;
				bool tmpflag = false;
				while(1){
					if(list_e0 == list_end(&(list_arr[j].n_list))){
						break;
					}
					list_data_tmp = list_entry(list_e0, struct list_data, elem);
					printf("%d ", list_data_tmp->data);
					list_e0 = list_e0->next;
					tmpflag = true;
				}
				if (tmpflag == true)
					printf("\n");
			}
			else{
				j = find_datastruct_number(1, token_command[1]);
				if(j<10){	
					hash_first(&hash_i0, &(hash_arr[j].n_hash));
					i = 0;
					hash_next(&hash_i0);
					while(1){
						if (i == hash_size(&(hash_arr[j].n_hash)) ){
							break;
						}
						hash_data_tmp = hash_entry(hash_cur(&hash_i0), struct hash_data, elem);
						printf("%d ", hash_data_tmp->data);
						hash_next(&hash_i0);
						i++;
					}
					if (i!= 0)
						printf("\n");
				}
				else{
					j = find_datastruct_number(2, token_command[1]);
					int size = bitmap_size(bitmap_arr[j].n_bitmap);

					for (i = 0 ; i < size ; i++){
						printf("%d", bitmap_test(bitmap_arr[j].n_bitmap, i));
					}
					printf("\n");
				}
			}
		}
		else if (!strcmp(token_command[0], "quit")){
			for (j = 0 ;j < 10 ; j++){
				if (list_arr[j].exist){
					while(!list_empty(&(list_arr[j].n_list))){
						list_e0 = list_pop_front(&(list_arr[j].n_list));
						list_data_tmp = list_entry(list_e0, struct list_data, elem);	
						free(list_data_tmp);
					}
					strcpy(list_arr[j].list_name, "\0");
					list_arr[j].exist = false;
				}
				if (hash_arr[j].exist){
					hash_destroy(&(hash_arr[j].n_hash), hash_destruct);
					hash_arr[j].exist = false;
				}
				if (bitmap_arr[j].exist){
					bitmap_destroy(bitmap_arr[j].n_bitmap);
					bitmap_arr[j].exist = false;
				}
			}
			
			return 0;
		}
		else if(!strcmp(token_command[0], "list_insert")){
			list_data_tmp = (struct list_data*)malloc(sizeof(struct list_data));
			list_data_tmp->data = atoi(token_command[3]);
			k = atoi(token_command[2]);

			i = find_datastruct_number(0, token_command[1]);
			list_e0 = list_begin(&(list_arr[i].n_list));
			for (i = 0 ; i < k ; i++){
				list_e0 = list_next(list_e0);
			}
			list_insert(list_e0, &(list_data_tmp->elem));
		}
		else if(!strcmp(token_command[0], "list_splice")){

			j = find_datastruct_number(0, token_command[1]);
			k = find_datastruct_number(0, token_command[3]);
	
			list_e0 = list_begin(&(list_arr[j].n_list));
			list_e1 = list_begin(&(list_arr[k].n_list));
			list_e2 = list_begin(&(list_arr[k].n_list));
			
			j = atoi(token_command[2]);
			for (i = 0 ; i < j ; i++)
				list_e0 = list_next(list_e0);
			
			j = atoi(token_command[4]);
			for (i = 0 ; i < j ; i++)
				list_e1 = list_next(list_e1);
			
			j = atoi(token_command[5]);
			for (i = 0 ; i < j ; i++)
				list_e2 = list_next(list_e2);

			list_splice(list_e0, list_e1, list_e2);
		}
		else if(!strcmp(token_command[0], "list_push_front")){
			list_data_tmp = (struct list_data*)malloc(sizeof(struct list_data));
			j = find_datastruct_number(0, token_command[1]);
			list_data_tmp->data = atoi(token_command[2]);
			list_push_front(&(list_arr[j].n_list), &(list_data_tmp->elem));
		}
		else if(!strcmp(token_command[0], "list_push_back")){
			list_data_tmp = (struct list_data*)malloc(sizeof(struct list_data));
			j = find_datastruct_number(0, token_command[1]);
			list_data_tmp->data = atoi(token_command[2]);
			list_push_back(&(list_arr[j].n_list), &(list_data_tmp->elem));
		}
		else if(!strcmp(token_command[0], "list_remove")){
			j = find_datastruct_number(0, token_command[1]);
			k = atoi(token_command[2]);
			list_e0 = list_begin(&(list_arr[j].n_list));
			for (i = 0 ; i < k ; i++)
				list_e0 = list_next(list_e0);
			list_remove(list_e0);
		}
		else if(!strcmp(token_command[0], "list_pop_front")){
			j = find_datastruct_number(0, token_command[1]);
			list_pop_front(&(list_arr[j].n_list));
		}
		else if(!strcmp(token_command[0], "list_pop_back")){
			j = find_datastruct_number(0, token_command[1]);
			list_pop_back(&(list_arr[j].n_list));	
		}
		else if(!strcmp(token_command[0], "list_front")){
			j = find_datastruct_number(0, token_command[1]);
			list_e0 = list_front(&(list_arr[j].n_list));
			list_data_tmp = list_entry(list_e0, struct list_data, elem);
			printf("%d\n", list_data_tmp->data);
		}
		else if(!strcmp(token_command[0], "list_back")){
			j = find_datastruct_number(0, token_command[1]);
			list_e0 = list_back(&(list_arr[j].n_list));
			list_data_tmp = list_entry(list_e0, struct list_data, elem);
			printf("%d\n", list_data_tmp->data);	
		}
		else if(!strcmp(token_command[0], "list_size")){
			j = find_datastruct_number(0, token_command[1]);
			k = list_size(&(list_arr[j].n_list));
			printf("%d\n", k);
		}
		else if(!strcmp(token_command[0], "list_empty")){
			j = find_datastruct_number(0, token_command[1]);
			if (list_empty(&(list_arr[j].n_list)) == true){
				printf("true\n");
			}
			else{
				printf("false\n");
			}
		}
		else if(!strcmp(token_command[0], "list_reverse")){
			j = find_datastruct_number(0, token_command[1]);
			list_reverse(&(list_arr[j].n_list));
		}
		else if(!strcmp(token_command[0], "list_sort")){
			j = find_datastruct_number(0, token_command[1]);
			list_sort(&(list_arr[j].n_list), list_less, NULL);	
		}
		else if(!strcmp(token_command[0], "list_insert_ordered")){
			j = find_datastruct_number(0, token_command[1]);
			list_data_tmp = (struct list_data*)malloc(sizeof(struct list_data));
			list_data_tmp->data = atoi(token_command[2]);
			list_insert_ordered(&(list_arr[j].n_list), &(list_data_tmp->elem), list_less, NULL);	
		}
		else if(!strcmp(token_command[0], "list_unique")){
			j = find_datastruct_number(0, token_command[1]);
			if (token_command[2] != '\0')
			k = find_datastruct_number(0, token_command[2]);

			if (k != 10){
				list_unique(&(list_arr[j].n_list), &(list_arr[k].n_list), list_less, NULL);
			}
			else{
				list_unique(&(list_arr[j].n_list), NULL, list_less, NULL);
			}

		}
		else if(!strcmp(token_command[0], "list_max")){
			j = find_datastruct_number(0, token_command[1]);
			list_e0 = list_max(&(list_arr[j].n_list), list_less, NULL);
			list_data_tmp = list_entry(list_e0, struct list_data, elem);
			printf("%d\n", list_data_tmp->data);	
		}
		else if(!strcmp(token_command[0], "list_min")){
			j = find_datastruct_number(0, token_command[1]);
			list_e0 = list_min(&(list_arr[j].n_list), list_less, NULL);
			list_data_tmp = list_entry(list_e0, struct list_data, elem);
			printf("%d\n", list_data_tmp->data);	

		}
		else if(!strcmp(token_command[0], "list_swap")){
			j = find_datastruct_number(0, token_command[1]);

			k = atoi(token_command[2]);
			list_e0 = list_begin(&(list_arr[j].n_list));

			for (i = 0 ; i < k ; i++)
				list_e0 = list_next(list_e0);
			
			k = atoi(token_command[3]);
			list_e1 = list_begin(&(list_arr[j].n_list));
			
			for (i = 0 ; i < k ; i++)
				list_e1 = list_next(list_e1);

			list_swap(list_e0, list_e1);
		}
		else if(!strcmp(token_command[0], "list_shuffle")){
			j = find_datastruct_number(0, token_command[1]);
			list_shuffle(&(list_arr[j].n_list));
		}

		else if(!strcmp(token_command[0], "hash_insert")){
			hash_data_tmp = (struct hash_data*)malloc(sizeof(struct hash_data));
			j = find_datastruct_number(1, token_command[1]);
			hash_data_tmp->data = atoi(token_command[2]);
			hash_insert(&(hash_arr[j].n_hash), &(hash_data_tmp->elem));
		}
		else if(!strcmp(token_command[0], "hash_replace")){
			hash_data_tmp = (struct hash_data*)malloc(sizeof(struct hash_data));
			j = find_datastruct_number(1, token_command[1]);
			hash_data_tmp->data = atoi(token_command[2]);
			hash_replace(&(hash_arr[j].n_hash), &(hash_data_tmp->elem));	
		}
		else if(!strcmp(token_command[0], "hash_find")){
			hash_data_tmp = (struct hash_data*)malloc(sizeof(struct hash_data));
			j = find_datastruct_number(1, token_command[1]);
			hash_data_tmp->data = atoi(token_command[2]);
			hash_e0 = hash_find(&(hash_arr[j].n_hash), &(hash_data_tmp->elem));	

			hash_data_tmp = hash_entry(hash_e0, struct hash_data, elem);
			if (hash_data_tmp != NULL)
				printf("%d\n", hash_data_tmp->data);
		}
		else if(!strcmp(token_command[0], "hash_delete")){
			hash_data_tmp = (struct hash_data*)malloc(sizeof(struct hash_data));
			j = find_datastruct_number(1, token_command[1]);
			hash_data_tmp->data = atoi(token_command[2]);
			hash_delete(&(hash_arr[j].n_hash), &(hash_data_tmp->elem));	
		}
		else if(!strcmp(token_command[0], "hash_clear")){
			j = find_datastruct_number(1, token_command[1]);
			hash_clear(&(hash_arr[j].n_hash), hash_destruct);		
		}
		else if(!strcmp(token_command[0], "hash_size")){
			j = find_datastruct_number(1, token_command[1]);
			printf("%d\n", hash_size(&(hash_arr[j].n_hash)));	
		}
		else if(!strcmp(token_command[0], "hash_empty")){
			j = find_datastruct_number(1, token_command[1]);
			if (hash_empty(&(hash_arr[j].n_hash)) == true)
				printf("true\n");
			else
				printf("false\n");
		}
		else if(!strcmp(token_command[0], "hash_apply")){
			j = find_datastruct_number(1, token_command[1]);
			if (!strcmp(token_command[2], "square")){
				hash_apply(&(hash_arr[j].n_hash), hash_square);
			}
			else if (!strcmp(token_command[2], "triple")){
				hash_apply(&(hash_arr[j].n_hash), hash_triple);
			}
		}
		else if(!strcmp(token_command[0], "hash_int_2")){
		}
		else if(!strcmp(token_command[0], "bitmap_size")){
			j = find_datastruct_number(2, token_command[1]);
			printf("%d\n", bitmap_size(bitmap_arr[j].n_bitmap));	
	
		}
		else if(!strcmp(token_command[0], "bitmap_set")){
			bool tmp;
			j = find_datastruct_number(2, token_command[1]);
			k = atoi(token_command[2]);
			if (!strcmp(token_command[3], "true"))
				tmp = true;
			else
				tmp = false;

			bitmap_set(bitmap_arr[j].n_bitmap, k, tmp);
		}
		else if(!strcmp(token_command[0], "bitmap_mark")){
			j = find_datastruct_number(2, token_command[1]);
			k = atoi(token_command[2]);

			bitmap_mark(bitmap_arr[j].n_bitmap, k);	
		}
		else if(!strcmp(token_command[0], "bitmap_reset")){
			j = find_datastruct_number(2, token_command[1]);
			k = atoi(token_command[2]);

			bitmap_reset(bitmap_arr[j].n_bitmap, k);	
		}
		else if(!strcmp(token_command[0], "bitmap_flip")){
			j = find_datastruct_number(2, token_command[1]);
			k = atoi(token_command[2]);

			bitmap_flip(bitmap_arr[j].n_bitmap, k);	
		}
		else if(!strcmp(token_command[0], "bitmap_test")){
			bool tmp;
			j = find_datastruct_number(2, token_command[1]);
			k = atoi(token_command[2]);

			tmp = bitmap_test(bitmap_arr[j].n_bitmap, k);	
			if (tmp)
				printf("true\n");
			else
				printf("false\n");
		}
		else if(!strcmp(token_command[0], "bitmap_set_all")){
			bool tmp;
			j = find_datastruct_number(2, token_command[1]);
			if (!strcmp(token_command[2], "true"))
				tmp = true;
			else
				tmp = false;

			bitmap_set_all(bitmap_arr[j].n_bitmap, tmp);
		}
		else if(!strcmp(token_command[0], "bitmap_set_multiple")){
			bool tmp;
			j = find_datastruct_number(2, token_command[1]);
			k = atoi(token_command[2]);
			i = atoi(token_command[3]);
			if (!strcmp(token_command[4], "true"))
				tmp = true;
			else
				tmp = false;

			bitmap_set_multiple(bitmap_arr[j].n_bitmap, k, i, tmp);
		}
		else if(!strcmp(token_command[0], "bitmap_count")){
			bool tmp;
			j = find_datastruct_number(2, token_command[1]);
			k = atoi(token_command[2]);
			i = atoi(token_command[3]);
			if (!strcmp(token_command[4], "true"))
				tmp = true;
			else
				tmp = false;

			printf("%d\n", bitmap_count(bitmap_arr[j].n_bitmap, k, i, tmp));
		}
		else if(!strcmp(token_command[0], "bitmap_contains")){
			bool tmp;
			j = find_datastruct_number(2, token_command[1]);
			k = atoi(token_command[2]);
			i = atoi(token_command[3]);
			if (!strcmp(token_command[4], "true"))
				tmp = true;
			else
				tmp = false;

			tmp = bitmap_contains(bitmap_arr[j].n_bitmap, k, i, tmp);
			if (tmp)
				printf("true\n");
			else
				printf("false\n");
		}
		else if(!strcmp(token_command[0], "bitmap_any")){
			bool tmp;
			j = find_datastruct_number(2, token_command[1]);
			k = atoi(token_command[2]);
			i = atoi(token_command[3]);

			tmp = bitmap_any(bitmap_arr[j].n_bitmap, k, i);
	
			if (tmp)
				printf("true\n");
			else
				printf("false\n");
		}
		else if(!strcmp(token_command[0], "bitmap_none")){
			bool tmp;
			j = find_datastruct_number(2, token_command[1]);
			k = atoi(token_command[2]);
			i = atoi(token_command[3]);

			tmp = bitmap_none(bitmap_arr[j].n_bitmap, k, i);
	
			if (tmp)
				printf("true\n");
			else
				printf("false\n");
		}
		else if(!strcmp(token_command[0], "bitmap_all")){
			bool tmp;
			j = find_datastruct_number(2, token_command[1]);
			k = atoi(token_command[2]);
			i = atoi(token_command[3]);

			tmp = bitmap_all(bitmap_arr[j].n_bitmap, k, i);
	
			if (tmp)
				printf("true\n");
			else
				printf("false\n");

		}
		else if(!strcmp(token_command[0], "bitmap_scan")){
			bool tmp;
			j = find_datastruct_number(2, token_command[1]);
			k = atoi(token_command[2]);
			i = atoi(token_command[3]);
			if (!strcmp(token_command[4], "true"))
				tmp = true;
			else
				tmp = false;

			printf("%u\n", bitmap_scan(bitmap_arr[j].n_bitmap, k, i, tmp));	
		}
		else if(!strcmp(token_command[0], "bitmap_scan_and_flip")){
			bool tmp;
			j = find_datastruct_number(2, token_command[1]);
			k = atoi(token_command[2]);
			i = atoi(token_command[3]);
			if (!strcmp(token_command[4], "true"))
				tmp = true;
			else
				tmp = false;

			printf("%u\n", bitmap_scan_and_flip(bitmap_arr[j].n_bitmap, k, i, tmp));	
		}
		else if(!strcmp(token_command[0], "bitmap_dump")){
			j = find_datastruct_number(2, token_command[1]);
			bitmap_dump(bitmap_arr[j].n_bitmap);
		}
		else if (!strcmp(token_command[0], "bitmap_expand")){
			j = find_datastruct_number(2, token_command[1]);
			k = atoi(token_command[2]);
			bitmap_arr[j].n_bitmap = bitmap_expand(bitmap_arr[j].n_bitmap, k);
		}
	}



	return 0;
}

int find_datastruct_number(int type, char *name){

	int i;
	
	if (type == 0){
		for (i = 0 ; i < 10 ; i++){
			if (!strcmp(name, list_arr[i].list_name))
				break;
		}
		return i;
	}
	else if (type == 1){
		for (i = 0 ; i < 10 ; i++){
			if(!strcmp(name, hash_arr[i].hash_name))
				break;
		}
		return i;
	}
	else if (type == 2){
		for (i = 0 ; i < 10 ; i++){
			if(!strcmp(name, bitmap_arr[i].bitmap_name))
				break;
		}
		return i;
	}

}
bool list_less(const struct list_elem *a, const struct list_elem *b, void *aux){
	if (aux == NULL){
		if (list_entry(a, struct list_data, elem)->data < list_entry(b, struct list_data, elem)->data)
			return true;
		else
			return false;
	}
	else return false;
}
void list_swap(struct list_elem *a, struct list_elem *b){

	struct list_elem *na = a->next;
	struct list_elem *pa = a->prev;

	if (a==b) return;

	if (a->next == b){
		a->next = b->next;
		b->prev = a->prev;

		a->next->prev = a;
		b->prev->next = b;

		b->next = a;
		a->prev = b;
	}
	else if (b->next == a){
		b->next = a->next;
		a->prev = b->prev;
		
		b->next->prev = b;
		a->prev->next = a;

		a->next = b;
		b->prev = a;
	}
	else{
		a->next = b->next;
		b->next = na;

		a->prev = b->prev;
		b->prev = pa;

		a->next->prev = a;
		a->prev->next = a;

		b->next->prev = b;
		b->prev->next = b;
	
	}
}
void list_shuffle(struct list *list){
	srand(time(NULL));
	int i, j, k;
	int random = rand();
	int size = list_size(list);
	struct list_elem *start, *step, *tmp;
	
	start = list_begin(list);

	for (i = 0 ; i < size ; i++){
		step = list_begin(list);
		for(j = 0 ; j < size ; j++){
			if (!(rand() % 3)){
				list_swap(start, step);
				tmp = start;
				start = step;
				step = tmp;
			}
			step = step->next;
		}
		start = start->next;
	}

}
unsigned hash_int_2(int i) {
	unsigned tmp = 1, res = 0, radix = 1;
	while (tmp < i) {
		res += i % tmp * radix++;
		tmp *= 10;
	}
	return res %142857 ;
}

struct bitmap *bitmap_expand(struct bitmap *bitmap, int size) {

	size_t newsize = size + bitmap_size(bitmap);
	int i;
	struct bitmap *new_bit ;//= (struct bitmap*)malloc(sizeof(struct bitmap));

	new_bit = bitmap_create(newsize);
	bitmap_set_all(new_bit, false);

	for (i = 0 ; i < newsize - size ; i++){
		if (bitmap_test(bitmap, i))
			bitmap_set(new_bit, i, true);
		else
			bitmap_set(new_bit, i, false);
	}

	free(bitmap);
	bitmap = new_bit;
	return new_bit;
}


bool hash_less(const struct hash_elem *a, const struct hash_elem *b, void *aux){
	struct hash_data *tmp1, *tmp2;

	tmp1 = hash_entry(a, struct hash_data, elem);
	tmp2 = hash_entry(b, struct hash_data, elem);

	if (tmp1->data < tmp2->data) return true;
	else return false;
}
unsigned hash_func(const struct hash_elem *e, void *aux){
	struct hash_data *tmp;
	if(aux==NULL){	
		tmp = hash_entry(e, struct hash_data, elem);
		return hash_int(tmp->data);
	}
	else return 1;

}
