#include "my_lisp.h"

#define MEMORY_SIZE 32

//todo static
int format(char *c);
void free_tree(cons_t *work);
void distribute_cha(cons_t *work,char **token,int i);

cons_t* make_tree(char** token)
{
	if (token[0] == NULL) {
		return NULL;
	}
	cons_t *work = (cons_t*)calloc(1,sizeof(cons_t));
	int i = 0;
	int j = 1;
	cons_t *start;
	start = work;
	cons_t **memory = (cons_t**)calloc(MEMORY_SIZE,sizeof(cons_t*));
	memory[0] = start;

	if (strcmp(token[0],"(") == 0) {
		i = 1;
	}
	while (token[i] != NULL) {
		if (strcmp(token[i],"(") == 0) {
			work->type = CAR;
			memory[j] = work;
			cons_t *new = (cons_t*)calloc(1,sizeof(cons_t));
			work->car = new;
			work = new;
			i++; j++;
		}
		else {
			if (format(token[i]) == NUM) {
				work->type = NUM;
				work->ivalue = atoi(token[i]); //strtol
				i++;
			}
			else if ( (format(token[i]) == CHA) &&
					  (strcmp(token[i],")") != 0) ) {
				distribute_cha(work,token,i);
				i++;
			}
			if (token[i] != NULL) {
				if (strcmp(token[i],")") == 0) {
					i++; j--;
					work->cdr = NULL;
					work = memory[j];
				}
			}
			if (token[i] != NULL) {
				if (strcmp(token[i],")") != 0) {
					cons_t *next = (cons_t*)calloc(1,sizeof(cons_t));
					work->cdr = next;
					work = next;
				}
			}
		}	
	}
	free(memory);
	return start;
}


int format(char *c)
{
	if (isdigit(c[0]) != 0) {
		return NUM;
	}
	if (c[0] == '-' &&
		isdigit(c[1]) != 0) {
		return NUM;
	}
	else {
		return CHA;
	}
}

void free_tree(cons_t *work)
{
	if (work == NULL) {
		return;
	}
	if (work->type == CAR) {
		free_tree(work->car);
	}
	if (work->cdr != NULL) {
		free_tree(work->cdr);
	}
	if (work->type == CHA) {
		free(work->svalue);
		work->svalue = NULL;
	}
	free(work);
	work = NULL;
}

void distribute_cha(cons_t *work,char **token,int i)
{
	switch (token[i]) {
		case "+" : work->ivalue = PLUS; work->type = SYMBOL; break;
		case "-" : work->ivalue = MINUS; work->type = SYMBOL; break;
		case "*" : work->ivalue = ASTERISK; work->type = SYMBOL; break;
		case "/" : work->ivalue = SLASH; work->type = SYMBOL; break;
		case "if" : work->ivalue = IF; work->type = SYMBOL; break;
		case "<" : work->ivalue = LESS_THAN_SIGN; work->type = SYMBOL; break;
		case "<=" : work->ivalue = LESS_THAN_OR_EQUAL_TO; work->type = SYMBOL; break;
		case ">" : work->ivalue = GREATER_THAN_SIGN; work->type = SYMBOL; break;
		case ">=" : work->ivalue = GREATER_OR_EQUAL_TO; work->type = SYMBOL; break;
		case "setq" : work->ivalue = SETQ; work->type = SYMBOL; break;
		case "defun" : work->ivalue = DEFUN; work->type = SYMBOL; break;
		case "quit" : work->ivalue = QUIT; work->type = SYMBOL; break;
	}
	if (work->type != SYMBOL) {
		work->type = CHA;
		int slen = strlen(token[i]);
		work->svalue = calloc(slen+1,sizeof(char));
		strcpy(work->svalue,token[i]);
	}
}
