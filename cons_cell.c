#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct cons_t{
	int type;
	union{
		struct cons_t *car;
		int ivalue;
		char *svalue;
	};
	struct cons_t *cdr;
}cons_t;

int distinction(char *c);


int main()
{
	char **result = (char **)calloc(100,sizeof(char*));
	result[0] = "("; result[1] = "+"; result[2] = "(", result[3] = "-"; result[4] = "(";
	result[5] = "*"; result[6] = "1"; result[7] = "2"; result[8] = ")"; result[9] = "3";
    result[10] = ")"; result[11] = "4"; result[12] = ")";

	struct cons_t *work = (struct cons_t*)calloc(1,sizeof(struct cons_t));
	int i = 0;
	int j = 0;
	if(result[0] == "("){ 
		i = 1;
	}
	struct cons_t *start;
	start = work;
	struct cons_t **memory = (struct cons_t**)calloc(1,sizeof(struct cons_t*));;
	do{
		if(result[i] == "("){
			memory[j] = work;
			struct cons_t *new = (struct cons_t*)calloc(1,sizeof(struct cons_t));
			work->car = new;
			work = new;
			i++; j++;
		}
		else{
			if(distinction(result[i]) == 0){
				work->type = 0;
				work->ivalue = atoi(result[i]);
			}
			else{
				work->type = 1;
				work->svalue = result[i];
			}
			i++;
			if(result[i] == ")"){
				j--;
				work->cdr = NULL;
				work = memory[j];
				i++;
			}
			if(result[i] != NULL){
				struct cons_t *next = (struct cons_t*)calloc(1,sizeof(struct cons_t));
				work->cdr = next;
				work = next;
			}
		}
	}while(result[i] != NULL);
	asm("int3");

	free(work);
	return 0;
}


int distinction(char *c)
{
	if(isdigit(c[0]) != 0)
		return 0;
	if(c[0] == '-' && isdigit(c[1]) != 0)
		return 0;
	else
		return 1;
}

