#include "my_lisp.h"

#define MEMORY_SIZE 32

//todo static
int format(char *c);
void free_tree(cons_t *work);
void distribute_cha(cons_t *work,char *str);

cons_t* make_tree(char** token)
{
	if (token[0] == NULL) {
		return NULL;
	}
	cons_t *work = (cons_t*)calloc(1,sizeof(cons_t));
	int i = 0, j = 1;
	cons_t *start;
	start = work;
	cons_t **memory = (cons_t**)calloc(MEMORY_SIZE,sizeof(cons_t*));
	memory[0] = start;

	if (token[0][0] == '(') {
		i = 1;
	}
	while (token[i] != NULL) {
		if (token[i][0] == '(') {
			work->type = CAR;
			memory[j] = work;
			cons_t *new = (cons_t*)calloc(1,sizeof(cons_t));
			work->car = new;
			work = new;
			i++; j++;
		}
		else {
			if (token[i][0] == ')') {
				i++; j--;
				work->cdr = NULL;
				work = memory[j];
			}
			else {
				if (format(token[i]) == NUM) {
					work->type = NUM;
					work->ivalue = atoi(token[i]);
				}
				else {
					distribute_cha(work,token[i]);
				}
				i++;
			}
			if (token[i] != NULL && token[i][0] != ')') {
				cons_t *next = (cons_t*)calloc(1,sizeof(cons_t));
				work->cdr = next;
				work = next;
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

void distribute_cha(cons_t *work,char *str)
{
	work->type = SYMBOL;
	switch (str[0]) {
	case ('+') : work->ivalue = PLUS; return;
	case ('-') : work->ivalue = MINUS; return;
	case ('*') : work->ivalue = ASTERISK; return;
	case ('/') : work->ivalue = SLASH; return;
	case ('<') :
		if (str[1] == '=') {
			work->ivalue = LESS_THAN_OR_EQUAL_TO;
		}
		else {
			work->ivalue = LESS_THAN_SIGN;
		}
		return;
	case ('>') :
		if (str[1] == '=') {
			work->ivalue = GREATER_THAN_EQUAL_TO;
		}
		else {
			work->ivalue = GREATER_THAN_SIGN;
		}
		return;
	}
	if (strcmp(str,"if") == 0) {
		work->ivalue = IF;
	}
	else if (strcmp(str,"setq") == 0) {
		work->ivalue = SETQ;
	}
	else if (strcmp(str,"defun") == 0) {
		work->ivalue = DEFUN;
	}
	else if (strcmp(str,"quit") == 0 ||
			 strcmp(str,"q") == 0) {
		work->ivalue = QUIT;
	}
	else {
		work->type = CHA;
		int slen = strlen(str);
		work->svalue = calloc(slen+1,sizeof(char));
		strcpy(work->svalue,str);
	}
}
