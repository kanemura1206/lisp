#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


typedef struct cons_t{
	int type;
	union{
		struct cons_t *car;
		int ivalue;
		char *svalue;
	};
	struct cons_t *cdr;
}cons_t;

enum{
	NUM,
	CHA,
	DIV,
};

int format(char *c);
void free_memory(struct cons_t **memory);

struct cons_t* cons_cell(char** result){
	struct cons_t *work = (struct cons_t*)calloc(1,sizeof(struct cons_t));
	int i = 0;
	int j = 0;
	struct cons_t *start;
	start = work;
	struct cons_t **memory = (struct cons_t**)calloc(1,sizeof(struct cons_t*));
	if (strcmp(result[0],"(") == 0){
		i = 1;
	}
	do{
		if (strcmp(result[i],"(") == 0){
			work->type = DIV;
			memory[j] = work;
			struct cons_t *new = (struct cons_t*)calloc(1,sizeof(struct cons_t));
			work->car = new;
			work = new;
			i++; j++;
		}
		else{
			if (format(result[i]) == NUM){
				work->type = NUM;
				work->ivalue = atoi(result[i]);
				i++;
			}
			else if ( (format(result[i]) == CHA) && (strcmp(result[i],")") != 0) ){
				work->type = CHA;
				work->svalue = result[i];
				i++;
			}
			if (result[i] != NULL){
				if (strcmp(result[i],")") == 0){
					i++; j--;
					work->cdr = NULL;
					work = memory[j];
				}
				if (result[i] != NULL && strcmp(result[i],")") != 0){
					struct cons_t *next = (struct cons_t*)calloc(1,sizeof(struct cons_t));
					work->cdr = next;
					work = next;
				}
			}
		}	
	}while (result[i] != NULL);
	
	free(memory);
	return start;
}


int format(char *c)
{
	if (isdigit(c[0]) != 0) {
		return NUM;
	}
	if (c[0] == '-' && isdigit(c[1]) != 0) {
		return NUM;
	}
	else{
		return CHA;
	}
}

void dump_tree(struct cons_t *work)
{
	if (work->type == NUM){
		printf("%d ",work->ivalue);
		if (work->cdr == NULL){
			printf("\n");
		}
		else if (work->cdr != NULL){
			dump_tree(work->cdr);
		}
	}
	else if (work->type == CHA){
		printf("%s ",work->svalue);
		if (work->cdr == NULL){
			printf("\n");
		}
		else if (work->cdr != NULL){
			dump_tree(work->cdr);
		}
	}
	else if (work->type == DIV){
		printf("car ");
		if (work->cdr == NULL){
			printf("\n");
		}
		else if (work->cdr != NULL){
			dump_tree(work->cdr);
		}
		dump_tree(work->car);
	}
}


void free_tree(struct cons_t *work)
{
	if (work->type == DIV){
		free_tree(work->car);
	}
	if (work->cdr != NULL){
		free_tree(work->cdr);
	}
	else if (work->cdr == NULL){
		free(work);
	}
}

void free_memory(struct cons_t **memory)
{
	int i = 0;
	do{
		free(memory[i]);
		i++;
	}while (memory[i] != NULL);
	free(memory);
}
