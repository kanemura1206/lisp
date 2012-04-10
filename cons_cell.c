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
	result[0] = "("; result[1] = "+"; result[2] = "(", result[3] = "*"; result[4] = "x";
	result[5] = "2"; result[6] = ")"; result[7] = "("; result[8] = "-"; result[9] = "y";
    result[10] = "4"; result[11] = ")"; result[12] = "z"; result[13] = ")";

	struct cons_t *work = (struct cons_t*)calloc(1,sizeof(struct cons_t));
	int i = 0;
	if(result[0] == "("){ 
		i = 1;
	}
	struct cons_t *start;
	start = work;
	struct cons_t *memory;
	do{
		if(result[i] == "("){
			memory = work;
			struct cons_t *new = (struct cons_t*)calloc(1,sizeof(struct cons_t));
			work->car = new;
			work = new;
			i++;
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
				work->cdr = NULL;
				work = memory;
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

