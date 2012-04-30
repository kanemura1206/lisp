#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "my_lisp.h"

#define MAX 64

static int tableSIZE = 2;
static int arg_tableSIZE = 0;
static int func_number;
static int func_SIZE = 0;
static int caldefun_prepare = 0;
static int recursive_prepare = 0;
static cell **table;
static func_t **function;
static int arg_table[MAX];
static int arg[MAX];

int calculate(cons_t *work);
int caladd(cons_t *work);
int calsub(cons_t *work);
int calmul(cons_t *work);
int caldiv(cons_t *work);
int calcom_R(cons_t *work);
int calcom_L(cons_t *work);
int branch(cons_t *work);
int calif(cons_t *work);
void calsetq(cons_t *work);
int value_of(cons_t *work);
int check_tree(cons_t *work);
int search_cha(cons_t *work);
void caldefun(cons_t *work);
void rewrite_function(cons_t *work,cons_t *variable);
int search_function(cons_t *work);
int call_function(cons_t *work,int i);
void arg_setq(cons_t *work,int i);
int check_recursive(cons_t *work,char *func);
void free_table();
void free_function(cons_t *work);

int (*po[])(cons_t*) = {caladd, calsub, calmul, caldiv,
						calif, calcom_R, calcom_L};

void discriminate(cons_t *work)
{
	if(work == NULL){
		return;
	}
	static int static_value;
	if (static_value == 0){
		table = calloc(32,sizeof(cell*));
		cell *p = (cell*)calloc(1,sizeof(cell));
		p->key = "T"; p->value = 1;
		table[0] = p;
		cell *q = (cell*)calloc(1,sizeof(cell));
		q->key = "Nil"; q->value = 0;
		table[1] = q;
		static_value = 1;
	}
	if (work->type == SYMBOL){
		if (work->ivalue == SETQ){
			calsetq(work->cdr);
			return;
		}
		else if (work->ivalue == DEFUN){
			caldefun(work->cdr);
			return;
		}
		else if (work->ivalue == QUIT){
			return;
		}
	}
	if (check_tree(work) == 0){
		if (work->type == SYMBOL){
			if (work->ivalue == LESS_THAN_SIGN || 
				work->ivalue == GREATER_THAN_SIGN){
				int k = calculate(work);
				if(k == 1){
					printf("T\n");
				}
				else if(k == 0){
					printf("Nil\n");
				}
			}
			else{
				printf("= %d\n",calculate(work));
			}
		}
		else{
			printf("= %d\n",calculate(work));
		}
	}
}


	int calculate(cons_t *work)
	{
		if (work->type == SYMBOL){
			return (*po[work->ivalue])(work->cdr);
		}
		else if (work->type == NUM ||  work->type == ARG){
			return value_of(work);
		}
		else if(work->type == CHA){
			func_number = search_function(work);
			if (func_number == -1){
				return value_of(work);
			}
			else if (func_number != -1){
			return call_function(work->cdr,func_number);
		}
	}
}

int caladd(cons_t *work)
{
	if(work->cdr != NULL){
		return branch(work) + caladd(work->cdr);
	}
	else if(work->cdr == NULL){
		return branch(work);
	}
}

int calsub(cons_t *work)
{
	if(work->cdr != NULL){
		return branch(work) - calsub(work->cdr);
	}
	else if(work->cdr == NULL){
		return branch(work);
	}
}

int calmul(cons_t *work)
{
	if(work->cdr != NULL){
		return branch(work) * calmul(work->cdr);
	}
	else if(work->cdr == NULL){
		return branch(work);
	}
}

int caldiv(cons_t *work)
{
	if(work->cdr != NULL){
		return branch(work) / caldiv(work->cdr);
	}
	else if(work->cdr == NULL){
		return branch(work);
	}
}

int calcom_R(cons_t *work)
{
	if (work->cdr != NULL){
		if (branch(work) < branch(work->cdr)){
			return calcom_R(work->cdr);
		}
		else{
			return 0;
		}
	}
	else if (work->cdr == NULL){
		return 1;
	}
}

int calcom_L(cons_t *work)
{
	if (work->cdr != NULL){
		if (branch(work) > branch(work->cdr)){
			return calcom_L(work->cdr);
		}
		else{
			return 0;
		}
	}
	else if (work->cdr == NULL){
 		return 1;
	}
}

int branch(cons_t *work)
{
	if (work->type == CAR){
		return calculate(work->car);
	}
	else{
		return calculate(work);
	}
} 

int calif(cons_t *work)
{
	if (branch(work) == 1){
		return branch(work->cdr);
	}
	else if (branch(work) == 0){
		return branch(work->cdr->cdr);
	}
}

void calsetq(cons_t *work)
{
	int i = 0;
	int j;
	int k = 0;
	do{
		if(strcmp(table[i]->key,work->svalue) == 0){
			k = 1;
			j = i;
		}
		i++;
	}while (i < tableSIZE);
	if (k == 0){
		j = tableSIZE;
		tableSIZE++;
	}
	int len = strlen(work->svalue);
	cell *tmp = (cell*)calloc(1,sizeof(cell));
	tmp->key = malloc(sizeof(char)*(len+1));
	strcpy(tmp->key,work->svalue);
	tmp->value = branch(work->cdr);
	table[j] = tmp;
}

int value_of(cons_t *work)
{
	if (work->type == CHA){
		int i = 0;
		do{
			if (strcmp(table[i]->key,work->svalue) == 0){
				return table[i]->value;
			}
			i++;
		}while (i < tableSIZE);
	}
	else if (work->type == ARG){
		return arg_table[arg_tableSIZE - (function[func_number]->arg_SIZE - work->ivalue + 1)];
	}
	else if (work->type == NUM){
		return work->ivalue;
	}
}

int check_tree(cons_t *work)
{
	if (work->type != CAR){
		if(work->cdr != NULL){
			return search_cha(work) + check_tree(work->cdr);
		}
		else if (work->cdr == NULL){
			return search_cha(work);
		}
	}
	else if (work->type == CAR){
		if (work->cdr != NULL){
			return check_tree(work->car) + check_tree(work->cdr);
		}
		else if(work->cdr == NULL){
			return check_tree(work->car);
		}
	}
}

int search_cha(cons_t *work)
{
	if(work->type == CHA){
		int i; int j = 1;
		for(i = 0; i < tableSIZE; i++){
			if(strcmp(table[i]->key,work->svalue) == 0){
				j = 0;
			}
		}
		for(i = 0; i < func_SIZE; i++){
			if(strcmp(function[i]->func_name,work->svalue) == 0){
				j = 0;
			}
		}
		if(j == 1){
			printf("'%s' is not exist\n",work->svalue);
		}
		return j;
	}
	else{
		return 0;
	}
}

void caldefun(cons_t *work)
{
	if (caldefun_prepare == 0){
		function = calloc(MAX,sizeof(func_t*));
		caldefun_prepare = 1;
	}
	function[func_SIZE] = calloc(1,sizeof(func_t));
	function[func_SIZE]->tree = work->cdr->cdr->car;
	work->cdr->cdr->car = NULL;
	cons_t *variable = work->cdr->car;
	cons_t *tmp = variable;
	function[func_SIZE]->arg_SIZE = 0;
	while (tmp->cdr != NULL){
		tmp = tmp->cdr;
		function[func_SIZE]->arg_SIZE++;
	}
	rewrite_function(function[func_SIZE]->tree,variable);
	int len = strlen(work->svalue);
	function[func_SIZE]->func_name = malloc(sizeof(char)*(len+1));
	strcpy(function[func_SIZE]->func_name,work->svalue);
	func_SIZE++;
}

void rewrite_function(cons_t *work,cons_t *variable)
{
	if (work->type == CAR){
		rewrite_function(work->car,variable);
	}
	if (work->cdr != NULL){
		rewrite_function(work->cdr,variable);
	}
	if (work->type == CHA){
		int i = 0;
		do{
			if (strcmp(work->svalue,variable->svalue) == 0){
				work->type = ARG;
				work->ivalue = i;
				return;
			}
			i++;
			variable = variable->cdr;
		}while(i < function[func_SIZE]->arg_SIZE + 1);
	}
}

int search_function(cons_t *work)
{
	if (function != NULL){
		int i = 0;
		do{
			if(strcmp(work->svalue,function[i]->func_name) == 0){
				return i;
			}
			i++;
		}while (i < func_SIZE);
	}
	return -1;
}

int call_function(cons_t *work,int i)
{
	int k = 0;
	arg_setq(work,0);
	int result =  calculate(function[func_number]->tree);
	arg_tableSIZE = arg_tableSIZE - (function[func_number]->arg_SIZE + 1);
	return result;
}

void arg_setq(cons_t *work,int i)
{
	arg_table[arg_tableSIZE] = branch(work);
	arg_tableSIZE++;
	if (i < function[func_number]->arg_SIZE){
		arg_setq(work->cdr,i+1);
	}
}

void free_function(cons_t *work)
{
	if (caldefun_prepare == 0){
		return;
	}
	int i;
	for (i = 0; function[i] != NULL; i++){
		free(function[i]->func_name);
		function[i]->func_name = NULL;
		free_tree(function[i]->tree);
		function[i]->tree = NULL;
		free(function[i]);
		function[i] = NULL;
	}
	free(function);
	function = NULL;
}
