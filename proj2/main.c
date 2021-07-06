#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "list.h"
#include "bitmap.h"
#include "hash.h"

typedef struct list_item{
	int data;
	struct list_elem elem;
} list_item;

typedef struct hash_item{
	int data;
	struct hash_elem elem;
} hash_item;

typedef struct list_arr{
	char name[30];
	struct list* link;
} list_arr;

typedef struct hash_arr{
	char name[30];
	struct hash* link;
} hash_arr;

typedef struct bitmap_arr{
	char name[30];
	struct bitmap* link;
}bitmap_arr;

list_arr listArray[11];
hash_arr hashArray[11];
bitmap_arr bitmapArray[11];

int lSize=0, hSize=0, bSize=0;

enum dataType{
	list = 1,
	hash,
	bitmap
};

bool listCmp(const struct list_elem *a, const struct list_elem *b, void *aux){
	int x = list_entry(a, struct list_item, elem)->data;
	int y = list_entry(b, struct list_item, elem)->data;

	return (x<y) ? true : false;
}

unsigned hashFunc(const struct hash_elem *a, void *aux){
	return hash_int(hash_entry(a, struct hash_item, elem)->data);
}

bool hashCmp(const struct hash_elem *a, const struct hash_elem *b, void *aux){
	int x = hash_entry(a, struct hash_item, elem)->data;
	int y = hash_entry(b, struct hash_item, elem)->data;

	return (x<y) ? true : false;
}

void hash_square(struct hash_elem *a, void* aux){
	int x = hash_entry(a, struct hash_item, elem)->data;
	hash_entry(a, struct hash_item, elem)->data = x * x;
	
	return;
}

void hash_triple(struct hash_elem *a, void* aux){
	int x = hash_entry(a, struct hash_item, elem)->data;
	hash_entry(a, struct hash_item, elem)->data = x * x * x;
	
	return;
}

int main(){

	for(;;){
		char command[51];
		scanf("%s", command);
		
		if(!strcmp(command, "quit")) break;
		//create
		else if(command[0] == 'c'){
			
			char dataTmp[10];
			scanf("%s", dataTmp);
			
			int bitmapSize;
			enum dataType type;
			if(dataTmp[0] == 'l') type = list;
			else if(dataTmp[0] == 'h') type = hash;
			else if(dataTmp[0] == 'b') type = bitmap;
			
			char name[51];
			scanf("%s", name);
			switch(type){
				case list :
					listArray[lSize].link = (struct list*)malloc(sizeof(struct list));
					list_init(listArray[lSize].link);
					strcpy(listArray[lSize].name, name);
					lSize++;
					break;
				case hash : 
					hashArray[hSize].link = (struct hash*)malloc(sizeof(struct hash));
					hash_init(hashArray[hSize].link, hashFunc, hashCmp, NULL);
					strcpy(hashArray[hSize].name, name);
					hSize++;
					break;
				case bitmap :
					scanf("%d", &bitmapSize);
					bitmapArray[bSize].link = bitmap_create(bitmapSize);
					strcpy(bitmapArray[bSize].name, name);
					bSize++;	
			}	
		}
		else if(command[0] == 'd'){

			int flag = 0;
			char name[51];
			scanf("%s", name);
			//delete
			if(command[1] == 'e'){
				for(int i=0; i<10; i++){
					if(strcmp(listArray[i].name, name)==0){
						for(int j=i; j<9; j++){
							strcpy(listArray[j].name, listArray[j+1].name);
							listArray[j] = listArray[j+1];
						}
						break;
					}
					else if(strcmp(hashArray[i].name, name)==0){
						if(hashArray[i].link != NULL){
							free(hashArray[i].link);
							for(int j=i; j<9; j++){
								strcpy(hashArray[j].name, hashArray[j+1].name);
								hashArray[j] = hashArray[j+1];
							}
						}
					}
					else if(strcmp(bitmapArray[i].name,name)==0){
						bitmap_destroy(bitmapArray[i].link);
						break;
					}
				}
			}
			//dump
			else if(command[1] == 'u'){
				int flag=0;

				for(int i=0; i<lSize; i++){
					if(strcmp(listArray[i].name, name) == 0){
						for(struct list_elem *tmp = list_begin(listArray[i].link); tmp != list_end(listArray[i].link); tmp = list_next(tmp)){
							printf("%d ", list_entry(tmp, struct list_item, elem)->data);
						}
						printf("\n");
						flag = 1;
						break;

					}
				}
				if(flag) continue;

				for(int i=0; i<hSize; i++){
					if(strcmp(hashArray[i].name, name) == 0){
						
						struct hash_iterator *iter = (struct hash_iterator *)malloc(sizeof(struct hash_iterator));
						hash_first(iter, hashArray[i].link);
						for(;hash_next(iter);){
							printf("%d ", hash_entry(hash_cur(iter), struct hash_item, elem)->data);
						}
						printf("\n");
											
						flag = 1;
						break;

					}
				}
				if(flag) continue;
				for(int i=0; i<bSize; i++){
					if(strcmp(bitmapArray[i].name, name)==0){
						int size = bitmap_size(bitmapArray[i].link);
						for(int j=0; j<size; j++){
							if(bitmap_test(bitmapArray[i].link, j)) printf("1");
							else printf("0");
						}
						printf("\n");
						flag = 1;
						break;
					}			
				}

				if(flag) continue;

			}
		}
		else if(strcmp(command, "list_unique")==0){
			char buffer[51];
			char copy[51];
			char *a, *b;
			int idx, idx2;

			fgets(buffer, sizeof(buffer), stdin);
			buffer[strlen(buffer)-1] = '\0';
			strcpy(copy, buffer);
		
			a = strtok(copy, " ");
			b = strtok(NULL, " ");
			
			for(int i=0; i<lSize; i++){
				if(strcmp(listArray[i].name, a)==0){
					idx = i; break;
				}
			}
			if(b != NULL){

				for(int i=0; i<lSize; i++){
					if(strcmp(listArray[i].name, b) == 0){
						idx2 = i; break;
					}
				}
				list_unique(listArray[idx].link, listArray[idx2].link, listCmp, NULL);
			}	
			else {
				list_unique(listArray[idx].link, NULL, listCmp, NULL);
			}
		}
		//list
		else if(command[0] == 'l'){
			int idx, num;
			char name[51];
			struct list_elem *elem;
			struct list_item *item;

			scanf("%s", name);
			for(int i=0; i<lSize; i++){
				if(strcmp(listArray[i].name, name)==0){
					idx = i; break;
				}
			}


			if(strcmp(command, "list_front")==0){
				elem = list_front(listArray[idx].link);
				printf("%d\n", list_entry(elem, struct list_item, elem)->data);
			}
			else if(strcmp(command, "list_back")==0){
				elem = list_back(listArray[idx].link);
				printf("%d\n", list_entry(elem, struct list_item, elem)->data);
			}
			else if(strcmp(command, "list_push_back")==0){
				item = (struct list_item *)malloc(sizeof(struct list_item));
				scanf("%d", &num);
				item->data = num;
				list_push_back(listArray[idx].link, &item->elem);
			}
			else if(strcmp(command, "list_pop_back")==0){
				list_pop_back(listArray[idx].link);
			}
			else if(strcmp(command, "list_pop_front")==0){
				list_pop_front(listArray[idx].link);
			}
			else if(strcmp(command, "list_push_front")==0){
				item = (struct list_item *)malloc(sizeof(struct list_item));
				scanf("%d", &num);
				item->data = num;
				list_push_front(listArray[idx].link, &item->elem);
			}
			else if(strcmp(command, "list_insert")==0){
				int loc;
				scanf("%d %d", &loc, &num);
				
				elem = list_begin(listArray[idx].link);
				for(int i=0; i<loc; i++){
					elem = elem->next;
				}

				item = (struct list_item *)malloc(sizeof(struct list_item));
				item->data = num;
				list_insert(elem, &item->elem);
			}
			else if(strcmp(command, "list_remove")==0){
				scanf("%d", &num);

				elem = list_begin(listArray[idx].link);
				for(int i=0; i<num; i++){
					elem = elem->next;
				}
				list_remove(elem);
			}
			else if(strcmp(command, "list_reverse")==0){
				list_reverse(listArray[idx].link);
			}
			else if(strcmp(command, "list_shuffle")==0){
				list_shuffle(listArray[idx].link);
			}
			else if(strcmp(command, "list_sort")==0){
				list_sort(listArray[idx].link, listCmp, NULL);
			}
			else if(strcmp(command, "list_splice")==0){
				int loc, s, e;
				char name2[51];
				scanf("%d %s %d %d", &loc, name2, &s, &e);

				for(int i=0; i<lSize; i++){
					if(strcmp(listArray[i].name, name2)==0){
						struct list_elem *start, *end;
						elem = list_begin(listArray[idx].link);
						start = list_begin(listArray[i].link);
						end = list_begin(listArray[i].link);
						
						for(int j=0; j<loc; j++) {
							elem = elem->next;
						}
						for(int j=0; j<s; j++) {
							start = start->next;
						}
						for(int j=0; j<e; j++) {
							end = end->next;
						}

						list_splice(elem, start, end);
						break;
					}
				}
			}
			else if(strcmp(command, "list_swap")==0){
				int x,y;
				scanf("%d %d", &x, &y);

				elem = list_begin(listArray[idx].link);
				struct list_elem *elem2 = list_begin(listArray[idx].link);

				for(int i=0; i< x; i++){
					elem = elem->next;
				}
				for(int i=0; i<y; i++){
					elem2 = elem2->next;
				}
				list_swap(elem, elem2);
			}
			else if(strcmp(command, "list_insert_ordered")==0){
				scanf("%d", &num);

				item = (struct list_item *)malloc(sizeof(struct list_item));
				item->data = num;
				list_insert_ordered(listArray[idx].link, &item->elem, listCmp, NULL);
			}
			else if(strcmp(command, "list_max")==0){
				printf("%d\n", list_entry(list_max(listArray[idx].link, listCmp, NULL), struct list_item, elem)->data);
			}
			else if(strcmp(command, "list_min")==0){
				printf("%d\n", list_entry(list_min(listArray[idx].link, listCmp, NULL), struct list_item, elem)->data);
			}
			else if(strcmp(command, "list_size")==0){
				printf("%zu\n", list_size(listArray[idx].link));
			}
			else if(strcmp(command, "list_empty")==0){
				if(!list_empty(listArray[idx].link)) printf("false\n");
				else printf("true\n");
			}
		}
		//hash
		else if(command[0] == 'h'){
			int idx, num;
			char name[51], extra[51];
			struct hash_item *item;
			struct hash_elem *elem;
			
			scanf("%s", name);
			for(int i=0; i<hSize; i++){
				if(strcmp(hashArray[i].name, name)==0){
					idx = i; break;
				}
			}
			if(strcmp(command, "hash_apply")==0){
				scanf("%s", extra);
				if(strcmp(extra, "square")==0){
					hash_apply(hashArray[idx].link, hash_square);
				}
				else if(strcmp(extra, "triple")==0){
					hash_apply(hashArray[idx].link, hash_triple);
				}
			}
			else if(strcmp(command, "hash_delete")==0){
				scanf("%d", &num);
				item = (struct hash_item *)malloc(sizeof(struct hash_item));
				item->data = num;
				hash_delete(hashArray[idx].link, &item->elem);
			}
			else if(strcmp(command, "hash_find")==0){
				scanf("%d", &num);
				item = (struct hash_item *)malloc(sizeof(struct hash_item));
				item->data = num;
				if(hash_find(hashArray[idx].link, &item->elem) != NULL){
					printf("%d\n", item->data);
				}
			}
			else if(strcmp(command, "hash_insert")==0){
				scanf("%d", &num);
				item = (struct hash_item *)malloc(sizeof(struct hash_item));
				item->data = num;
				hash_insert(hashArray[idx].link, &item->elem);

			}
			else if(strcmp(command, "hash_replace")==0){
				scanf("%d", &num);
				item = (struct hash_item *)malloc(sizeof(struct hash_item));
				item->data = num;
				hash_replace(hashArray[idx].link, &item->elem);	
			}
			else if(strcmp(command, "hash_empty")==0){
				if(hash_empty(hashArray[idx].link) == false){
					printf("false\n");
				}
				else  printf("true\n");
			}
			else if(strcmp(command, "hash_size")==0){
				printf("%zu\n", hash_size(hashArray[idx].link));
			}
			else if(strcmp(command, "hash_clear")==0){
				hash_clear(hashArray[idx].link, NULL);
			}
			
		}
		//bitmap
		else if(command[0] == 'b'){
			int idx;
			int x, y;
			char name[51], extra[51];
			
			scanf("%s", name);
			for(int i=0; i<bSize; i++){
				if(strcmp(bitmapArray[i].name, name)==0){
					idx = i; break;
				}
			}

			if(strcmp(command, "bitmap_all")==0){
				scanf("%d %d", &x, &y);

				if(!bitmap_all(bitmapArray[idx].link, x, y)) printf("false\n");
				else printf("true\n");
			}
			else if(strcmp(command, "bitmap_any")==0){
				scanf("%d %d", &x, &y);
				if(!bitmap_any(bitmapArray[idx].link, x,y)) printf("false\n");
				else printf("true\n");
			}
			else if(strcmp(command, "bitmap_contains")==0){
				scanf("%d %d %s", &x, &y, extra);

				if(strcmp(extra, "true") != 0){
					if(bitmap_contains(bitmapArray[idx].link, x,y, false))printf("true\n");
					else printf("false\n");
				}
				else{
					if(bitmap_contains(bitmapArray[idx].link, x, y, true)) printf("true\n");
					else printf("false\n");
				}
			}
			else if(strcmp(command, "bitmap_count")==0){
				scanf("%d %d %s", &x, &y, extra);
				
				if(strcmp(extra, "true") != 0) printf("%zu\n", bitmap_count(bitmapArray[idx].link, x,y,false));
				else printf("%zu\n", bitmap_count(bitmapArray[idx].link, x,y,true));	
			}
			else if(strcmp(command, "bitmap_dump")==0){
				bitmap_dump(bitmapArray[idx].link);
			}
			else if(strcmp(command, "bitmap_expand")==0){
				scanf("%d", &x);
				bitmapArray[idx].link = bitmap_expand(bitmapArray[idx].link, x);
			}
			else if(strcmp(command, "bitmap_flip")==0){
				scanf("%d", &x);
				bitmap_flip(bitmapArray[idx].link, x);
			}
			else if(strcmp(command, "bitmap_mark")==0){
				scanf("%d", &x);
				bitmap_mark(bitmapArray[idx].link, x);
			}
			else if(strcmp(command, "bitmap_none")==0){
				scanf("%d %d", &x, &y);
				if(bitmap_none(bitmapArray[idx].link, x,y)) printf("true\n");
				else printf("false\n");
			}
			else if(strcmp(command, "bitmap_set_all")==0){
				scanf("%s", extra);
				if(strcmp(extra, "true") != 0) bitmap_set_all(bitmapArray[idx].link, false);
				else bitmap_set_all(bitmapArray[idx].link, true);	
			}
			else if(strcmp(command, "bitmap_set")==0){
				scanf("%d %s", &x, extra);
				if(strcmp(extra, "true") != 0) bitmap_set(bitmapArray[idx].link, x, false);
				else bitmap_set(bitmapArray[idx].link, x, true);
			}
			else if(strcmp(command, "bitmap_set_multiple")==0){
				scanf("%d %d %s", &x, &y, extra);
				if(strcmp(extra, "true") != 0) bitmap_set_multiple(bitmapArray[idx].link, x,y, false);
				else bitmap_set_multiple(bitmapArray[idx].link, x, y, true);
			}
			else if(strcmp(command, "bitmap_size")==0){
				printf("%zu\n", bitmap_size(bitmapArray[idx].link));
			}
			else if(strcmp(command, "bitmap_test")==0){
				scanf("%d", &x);
				if(bitmap_test(bitmapArray[idx].link, x)) printf("true\n");
				else printf("false\n");
			}
			else if(strcmp(command, "bitmap_reset")==0){
				scanf("%d", &x);
				bitmap_reset(bitmapArray[idx].link, x);	
			}
			else if(strcmp(command, "bitmap_scan_and_flip")==0){
				scanf("%d %d %s", &x, &y, extra);
				if(strcmp(extra, "true") != 0) printf("%lu\n", bitmap_scan_and_flip(bitmapArray[idx].link, x,y, false));
				else printf("%lu\n", bitmap_scan_and_flip(bitmapArray[idx].link, x,y, true));	
			}
			else if(strcmp(command, "bitmap_scan")==0){
				scanf("%d %d %s", &x, &y, extra);
				if(strcmp(extra, "true") != 0) printf("%lu\n", bitmap_scan(bitmapArray[idx].link, x,y, false));
				else printf("%lu\n", bitmap_scan(bitmapArray[idx].link, x,y, true));	
			}
				
		}

	}
	return 0;
}
