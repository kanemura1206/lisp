#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "my_lisp.h"

#define function_SIZE 32
#define recursive_tableSIZE 64

static int tableSIZE = 2;
static int funcnumber = 0;
static int recursive_tablePOINT = 0;
static int caldefun_prepare = 0;
static int recursive_prepare = 0;
static cell **table;
static func_t **function;
static int  **recursive_table[recursive_tableSIZE];
static int arg[function_SIZE];

int calcular(cons_t *work);
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
int search_cha(char *svalue);
void caldefun(cons_t *work);
int search_function(cons_t *work);
int call_function(cons_t *work,int i);
void function_setq(cons_t *work);
int check_recursive(cons_t *work,char *func);
void recursive_setq(cons_t *work,cons_t *variable);
void free_table();
void free_function(cons_t *work);


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

	if (strcmp(work->svalue,"setq") == 0){
		calsetq(work->cdr);
	}
	else if (strcmp(work->svalue,"quit") == 0 || strcmp(work->svalue,"q") == 0){
		return;
	}
	else if (strcmp(work->svalue,"defun") == 0){
		caldefun(work->cdr);
	}
	else if (search_function(work) != -1){
		printf("= %f\n",calcular(work));
	}
	else if (check_tree(work) == 0){
		if (strcmp(work->svalue,"<") ==0 || strcmp(work->svalue,">") == 0){
			int k = calcular(work);
			if(k == 1){
				printf("T\n");
			}
			else if(k == 0){
				printf("Nil\n");
			}
		}
		else{
			printf("= %f\n",calcular(work));
		}
	}
}

int calcular(cons_t *work)
{
	if(work->type == CHA){
		if (strcmp(work->svalue,"+") == 0){
			return caladd(work->cdr);
		}
		else if (strcmp(work->svalue,"-") == 0){
			return calsub(work->cdr);
		}
		else if (strcmp(work->svalue,"*") == 0){
			return calmul(work->cdr);
		}
		else if (strcmp(work->svalue,"/") == 0){
			return caldiv(work->cdr);
		}
		else if (strcmp(work->svalue,"<") == 0){
			return calcom_R(work->cdr);
		}
		else if (strcmp(work->svalue,">") == 0){
			return calcom_L(work->cdr);
		}
		else if (strcmp(work->svalue,"if") == 0){
			return calif(work->cdr);
		}
		else if (search_function(work) != -1){
			int i = search_function(work);
			return call_function(work->cdr,i);
		}
		else{
			return value_of(work);
		}
	}
	else{
		return work->ivalue;
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
		return calcular(work->car);
	}
	else{
		return calcular(work);
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
		int j = recursive_tablePOINT;
		do{
			j--;
			if (recursive_table[j] != NULL){
				if (strcmp(recursive_table[j]->key,work->svalue) == 0){
					return recursive_table[j]->value;
				}
			}
		}while (j > 0);
	}
	else{
		return work->ivalue;
	}
}

int check_tree(cons_t *work)
{
	if (work->type != CAR){
		if(work->cdr != NULL){
			return search_cha(work->svalue) + check_tree(work->cdr);
		}
		else if (work->cdr == NULL){
			return search_cha(work->svalue);
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

int search_cha(char *svalue)
{
	if(work->type == CHA){
		if(strcmp(work->svalue,"+") != 0 && strcmp(work->svalue,"-") != 0 && strcmp(work->svalue,"*") != 0 && 
		   strcmp(work->svalue,"/") != 0 && strcmp(work->svalue,"<") != 0 && strcmp(work->svalue,">") != 0 && 
		   strcmp(work->svalue,"if") != 0 && strcmp(work->svalue,"setq") != 0 && strcmp(work->svalue,"defun") != 0){
			int i; int j = 1;
			for(i = 0; i < tableSIZE; i++){
				if(strcmp(table[i]->key,svalue) == 0){
					j = 0;
				}
			}
			for(i = 0; i < funcnumber; i++){
				if(strcmp(function[i]->func_name,svalue) == 0){
					j = 0;
				}
			}
			if(j == 1){
				printf("'%s' is not exist\n",svalue);
			}
			return j;
		}
		else{
			return 0;
		}
	}
	else{
		return 0;
	}
}

void caldefun(cons_t *work)
{
	if (caldefun_prepare == 0){
		function = calloc(function_SIZE,sizeof(func_t*));
		caldefun_prepare = 1;
	}
	function[funcnumber]->tree = work->cdr->cdr->car;
	work->cdr->cdr->car = NULL;
	cons_t *variable = work->cdr->car;
	cons_t *tmp = variable;
	function[funcnumber]->arg_SIZE = 0;
	while (tmp->cdr != NULL){
		tmp = tmp->cdr;
		function[funcnumber]->arg_SIZE++;
	}
	rewrite_function(function[funcnumber]->tree,variable);
	int len = strlen(work->svalue);
	function[funcnumber]->func_name = malloc(sizeof(char)*(len+1));
	strcpy(function[funcnumber]->func_name,work->svalue);
	if(check_recursive(function[funcnumber]->tree,function[funcnumber]->func_name) != 0){
		function[funcnumber]->type = RECURSIVE;
	}
	else{
		function[funcnumber]->type = 0;
	}
	funcnumber++;
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
				work->type = NUM;
				work->ivalue = arg[i];
				return;
			}
			i++;
			variable = varaible->cdr;
		}while(i <= arg_SIZE)
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
		}while (i < funcnumber);
	}
	return -1;
}

int call_function(cons_t *work,int i)
{
	if (function[i]->type == RECURSIVE){
		recursive_setq(work);
	}
	else{
		function_setq(work);
	}
	int f =  calcular(function[i]->cdr->cdr->car);
	if (function[i]->type == RECURSIVE){
		int k = 0;
		cons_t *tmp;
		tmp = function[i]->cdr->car;
		int j = recursive_tablePOINT;
		while(tmp->cdr != NULL){
			j--;
			tmp = tmp->cdr;
			free(recursive_table[j]->key);
			recursive_table[j]->key = NULL;
			free(recursive_table[j]);
			recursive_table[j] = NULL;
		}
		j--;
		free(recursive_table[j]->key);
		recursive_table[j]->key = NULL;
		free(recursive_table[j]);
		recursive_table[j] = NULL;
		recursive_tablePOINT = j;
	}
	return f;
}

void function_setq(cons_t *work)
{
	int i = 0;
	int j;
	int k = 0;
	do{
		if(strcmp(table[i]->key,variable->svalue) == 0){
			k = 1;
			j = i;
		}
		i++;
	}while (i < tableSIZE);
	if (k == 0){
		j = tableSIZE;
		tableSIZE++;
	}
	int len = strlen(variable->svalue);
	cell *tmp = (cell*)calloc(1,sizeof(cell));
	tmp->key = malloc(sizeof(char)*(len+1));
	strcpy(tmp->key,variable->svalue);
	tmp->value = branch(work);
	table[j] = tmp;
	if(work->cdr != NULL){
		function_setq(work->cdr,variable->cdr);
	}
}

int check_recursive(func_t *work,char *func_name)
{
	int i = 0;
	if (work->type == CHA){
		if(strcmp(work->svalue,func_name) == 0){
			i = 1;
		}
	}
	if (work->type != CAR){
		if(work->cdr != NULL){
			return i + check_recursive(work->cdr,func_name);
		}
		else if (work->cdr == NULL){
			return i;
		}
	}
	else if (work->type == CAR){
		if (work->cdr != NULL){
			return check_recursive(work->car,func_name) + check_recursive(work->cdr,func_name);
		}
		else if(work->cdr == NULL){
			return check_recursive(work->car,func_name);
		}
	}
}


void recursive_setq(cons_t *work,cons_t *variable)
{
	recursive_table[recursive_tablePOINT] = ;
	if(work->cdr != NULL){
		recursive_setq(work->cdr,variable->cdr);
	}
	recursive_tablePOINT++;
}

void free_table()
{
	int i;
	for(i = 0; i < tableSIZE; i++){
		free(table[i]);
		table[i] = NULL;
	}
	free(table);
	table = NULL;

	if(recursive_prepare == 1){
		for(i = 0; recursive_table[i] != NULL; i++){
			free(recursive_table[i]);
			recursive_table[i] = NULL;
		}
		free(recursive_table);
		recursive_table = NULL;
	}
}

void free_function(cons_t *work)
{
	if (caldefun_prepare == 0){
		return;
	}
	int i;
	for (i = 0; function[i] != NULL; i++){
		free_tree(function[i]);
	}
	free(function);
	function = NULL;
}
