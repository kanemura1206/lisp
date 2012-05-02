#include "my_lisp.h"

#define table_limit 128
#define function_limit 128
#define arg_limit 512

static int tableSIZE = 2;
static int arg_tableSIZE = 0;
static int func_number;
static int func_SIZE = 0;
static int caldefun_prepare = 0;
static cell **table;
static func_t **function;
static int arg_table[arg_limit];

int calculate(cons_t *work);
int caladd(cons_t *work);
int calsub(cons_t *work);
int calmul(cons_t *work);
int caldiv(cons_t *work);
int calcom_L(cons_t *work);
int calcom_LE(cons_t *work);
int calcom_G(cons_t *work);
int calcom_GE(cons_t *work);
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
void free_function();

int (*po[])(cons_t*) = {caladd, calsub, calmul, caldiv, calif,
						calcom_L, calcom_LE, calcom_G, calcom_GE};

void discriminate(cons_t *work)
{
	if (work == NULL) {
		return;
	}
	static int static_value;
	if (static_value == 0) {
		table = calloc(table_limit,sizeof(cell*));
		cell *p = (cell*)calloc(1,sizeof(cell));
		p->key = "T"; p->value = 1;
		table[0] = p;
		cell *q = (cell*)calloc(1,sizeof(cell));
		q->key = "Nil"; q->value = 0;
		table[1] = q;
		static_value = 1;
	}
	if (work->type == SYMBOL) {
		if (work->ivalue == SETQ) {
			calsetq(work->cdr);
			return;
		}
		else if (work->ivalue == DEFUN) {
			caldefun(work->cdr);
			return;
		}
		else if (work->ivalue == QUIT) {
			return;
		}
	}
	if (check_tree(work) == 0) {
		if (work->type == SYMBOL) {
			if (work->ivalue == LESS_THAN_SIGN || 
				work->ivalue == LESS_THAN_OR_EQUAL_TO ||
				work->ivalue == GREATER_THAN_SIGN ||
				work->ivalue == GREATER_THAN_EQUAL_TO) {
				int k = calculate(work);
				if (k == 1) {
					printf("T\n\n");
				}
				else if (k == 0) {
					printf("Nil\n\n");
				}
			}
			else {
				printf("= %d\n\n",calculate(work));
			}
		}
		else {
			printf("= %d\n\n",calculate(work));
		}
	}
}


int calculate(cons_t *work)
{
	if (work->type == SYMBOL) {
		return (*po[work->ivalue])(work->cdr);
	}
	else if (work->type == NUM ||
			 work->type == ARG) {
		return value_of(work);
	}
	else {
		int i = search_function(work);
		if (i == -1) {
			return value_of(work);
		}
		else {
			func_number = i;
			return call_function(work->cdr,func_number);
		}
	}
}

int caladd(cons_t *work)
{
	if (work->cdr != NULL) {
		return branch(work) + caladd(work->cdr);
	}
	else {
		return branch(work);
	}
}

int calsub(cons_t *work) //todo
{
	if (work->cdr != NULL) {
		return branch(work) + calsub(work->cdr);
	}
	else {
		return -branch(work);
	}
}

int calmul(cons_t *work)
{
	if (work->cdr != NULL) {
		return branch(work) * calmul(work->cdr);
	}
	else {
		return branch(work);
	}
}

int caldiv(cons_t *work)
{
	if (work->cdr != NULL) {
		return branch(work) / caldiv(work->cdr);
	}
	else {
		return branch(work);
	}
}

int calcom_L(cons_t *work)
{
	if (work->cdr != NULL) {
		if (branch(work) < branch(work->cdr)) {
			return calcom_L(work->cdr);
		}
		else {
			return 0;
		}
	}
	else {
		return 1;
	}
}

int calcom_LE(cons_t *work)
{
	if (work->cdr != NULL) {
		if (branch(work) <= branch(work->cdr)) {
			return calcom_LE(work->cdr);
		}
		else {
			return 0;
		}
	}
	else {
		return 1;
	}
}

int calcom_G(cons_t *work)
{
	if (work->cdr != NULL) {
		if (branch(work) > branch(work->cdr)) {
			return calcom_G(work->cdr);
		}
		else {
			return 0;
		}
	}
	else {
 		return 1;
	}
}

int calcom_GE(cons_t *work)
{
	if (work->cdr != NULL) {
		if (branch(work) >= branch(work->cdr)) {
			return calcom_GE(work->cdr);
		}
		else {
			return 0;
		}
	}
	else {
 		return 1;
	}
}

int branch(cons_t *work) //rename
{
	if (work->type == CAR) {
		return calculate(work->car);
	}
	else {
		return calculate(work);
	}
} 

int calif(cons_t *work) //todo
{
	if (branch(work) == 1) {
		return branch(work->cdr);
	}
	else {
		return branch(work->cdr->cdr);
	}
}

void calsetq(cons_t *work)
{
	int i = 0;
	int j = 0;
	int k = 0;
	do {
		if (strcmp(table[i]->key,work->svalue) == 0) {
			k = 1;
			j = i;
		}
		i++;
	} while (i < tableSIZE);
	if (k == 0) {
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
	if (work->type == CHA) {
		int i = 0;
		do{
			if (strcmp(table[i]->key,work->svalue) == 0) {
				break;
			}
			i++;
		}while (i < tableSIZE);
		return table[i] ->value;
	}
	else if (work->type == ARG) {
		return arg_table[arg_tableSIZE - (function[func_number]->arg_SIZE - work->ivalue + 1)];
	}
	else {
		return work->ivalue;
	}
}

int check_tree(cons_t *work)
{
	if (work->type != CAR) {
		if (work->cdr != NULL) {
			return search_cha(work) + check_tree(work->cdr);
		}
		else {
			return search_cha(work);
		}
	}
	else {
		if (work->cdr != NULL) {
			return check_tree(work->car) + check_tree(work->cdr);
		}
		else {
			return check_tree(work->car);
		}
	}
}

int search_cha(cons_t *work)
{
	if (work->type == CHA) {
		int i; int j = 1;
		for(i = 0; i < tableSIZE; i++) {
			if (strcmp(table[i]->key,work->svalue) == 0) {
				j = 0;
			}
		}
		for(i = 0; i < func_SIZE; i++) {
			if (strcmp(function[i]->func_name,work->svalue) == 0) {
				j = 0;
			}
		}
		if (j == 1) {
			printf("'%s' is not exist\n",work->svalue);
		}
		return j;
	}
	else {
		return 0;
	}
}

void caldefun(cons_t *work)
{
	if (caldefun_prepare == 0) { //remove caldefun_prepare
		function = calloc(function_limit,sizeof(func_t*)); //todo
		caldefun_prepare = 1;
	}
	function[func_SIZE] = calloc(1,sizeof(func_t)); //todo
	function[func_SIZE]->tree = work->cdr->cdr->car;
	work->cdr->cdr->car = NULL;
	cons_t *variable = work->cdr->car;
	cons_t *tmp = variable;
	function[func_SIZE]->arg_SIZE = 0;
	while (tmp->cdr != NULL) {
		tmp = tmp->cdr;
		function[func_SIZE]->arg_SIZE++;
	}
	rewrite_function(function[func_SIZE]->tree,variable);
	int len = strlen(work->svalue);
	function[func_SIZE]->func_name = malloc(sizeof(char)*(len+1));
	strcpy(function[func_SIZE]->func_name,work->svalue); //todo strcpy => strncpy
	func_SIZE++;
}

void rewrite_function(cons_t *work,cons_t *variable)
{
	if (work->type == CAR) {
		rewrite_function(work->car,variable);
	}
	if (work->cdr != NULL) {
		rewrite_function(work->cdr,variable);
	}
	if (work->type == CHA) {
		int i = 0;
		do{
			if (strcmp(work->svalue,variable->svalue) == 0) {
				free(work->svalue);
				work->svalue = NULL;
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
	if (function != NULL) {
		int i = 0;
		do{
			if (strcmp(work->svalue,function[i]->func_name) == 0) {
				return i;
			}
			i++;
		}while (i < func_SIZE);
	}
	return -1;
}

int call_function(cons_t *work,int i)
{
	arg_setq(work,0);
	int result =  calculate(function[func_number]->tree);
	arg_tableSIZE = arg_tableSIZE - (function[func_number]->arg_SIZE + 1);
	return result;
}

void arg_setq(cons_t *work,int i)
{
	arg_table[arg_tableSIZE] = branch(work);
	arg_tableSIZE++;
	if (i < function[func_number]->arg_SIZE) {
		arg_setq(work->cdr, i+1);
	}
}

void free_function()
{
	if (caldefun_prepare == 0) {
		return;
	}
	int i;
	for (i = 0; function[i] != NULL; i++) {
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
