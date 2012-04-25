#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct cons_t{
	int type;
	union{
		struct cons_t *car;
		int ivalue;
		char *svalue;
	};
	struct cons_t *cdr;
};

struct cell{
	char *key;
	int value;
};

static int tableSIZE = 2;
static int funcnumber = 0;
static int recursive_tableSIZE = 0;
static int recursive_prepare = 0;
static int recursive_point;
static struct cell **table;
static struct cons_t **function;
static struct cell **recursive_table;

enum{
	NUM,
	CHA,
	DIV,
	RECURSIVE
};

float calcular(struct cons_t *work);
float caladd(struct cons_t *work);
float calsub(struct cons_t *work);
float calmul(struct cons_t *work);
float caldiv(struct cons_t *work);
float calcom_r(struct cons_t *work);
float calcom_l(struct cons_t *work);
float branch(struct cons_t *work);
float calif(struct cons_t *work);
void calsetq(struct cons_t *work);
float value_of(struct cons_t *work);
void free_table();
int check_tree(struct cons_t *work);
int search_cha(struct cons_t *work);
void caldefun(struct cons_t *work);
int search_function(struct cons_t *work);
float call_function(struct cons_t *work,int i);
void function_setq(struct cons_t *work,struct cons_t *variable);
int check_recursive(struct cons_t *work,char *func);
void recursive_setq(struct cons_t *work,struct cons_t *variable);
void free_recursive();


void discriminate(struct cons_t *work)
{
	static int static_value;
	struct cell **tab;
	if (static_value == 0){
		tab = calloc(1,sizeof(struct cell));
		table = tab;
		struct cell *p = (struct cell*)calloc(1,sizeof(struct cell));
		p->key = "T"; p->value = 1;
		table[0] = p;
		struct cell *q = (struct cell*)calloc(1,sizeof(struct cell));
	    q->key = "Nil"; q->value = 0;
		table[1] = q;
		static_value = 1;
	}

	if (strcmp(work->svalue,"setq") == 0){
		calsetq(work->cdr);
	}
	else if (strcmp(work->svalue,"quit") == 0 || strcmp(work->svalue,"q") == 0){
		//free_table();
	}
	else if (strcmp(work->svalue,"defun") == 0){
		caldefun(work);
	}
	else if (search_function(work) != -1){
		printf("= %f\n",calcular(work));
	}
	else if (check_tree(work) == 0){
		if (strcmp(work->svalue,"<") ==0 || strcmp(work->svalue,">") == 0){
			float k = calcular(work);
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

float calcular(struct cons_t *work)
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
			return calcom_r(work->cdr);
		}
		else if (strcmp(work->svalue,">") == 0){
			return calcom_l(work->cdr);
		}
		else if (strcmp(work->svalue,"if") == 0){
			return calif(work->cdr);
		}
		else if (search_function(work) != -1){
			int i = search_function(work);
			int k = 0;
			float f = call_function(work->cdr,i);
			if (function[i]->type == RECURSIVE){
				struct cons_t *tmp;
				tmp = function[i]->cdr->car;
				int j = recursive_point;
				do{
					j--;
					recursive_table[j] = NULL;
					if(tmp->cdr != NULL){
						tmp = tmp->cdr;
					}
					else{
						k = 1;
					}
				}while (k == 0);
			}
			return f;
		}
		else{
			return value_of(work);
		}
	}
	else{
		return work->ivalue;
	}
}

float caladd(struct cons_t *work)
{
	if(work->cdr != NULL){
		return branch(work) + caladd(work->cdr);
	}
	else if(work->cdr == NULL){
		return branch(work);
	}
}

float calsub(struct cons_t *work)
{
	if(work->cdr != NULL){
		return branch(work) - caladd(work->cdr);
	}
	else if(work->cdr == NULL){
		return branch(work);
	}
}

float calmul(struct cons_t *work)
{
	if(work->cdr != NULL){
		return branch(work) * caladd(work->cdr);
	}
	else if(work->cdr == NULL){
		return branch(work);
	}
}

float caldiv(struct cons_t *work)
{
	if(work->cdr != NULL){
		return branch(work) / caladd(work->cdr);
	}
	else if(work->cdr == NULL){
		return branch(work);
	}
}

float calcom_r(struct cons_t *work)
{
	if (work->cdr != NULL){
		if (branch(work) < branch(work->cdr)){
			return calcom_r(work->cdr);
		}
		else{
			return 0;
		}
	}
	else if (work->cdr == NULL){
		return 1;
	}
}

float calcom_l(struct cons_t *work)
{
	if (work->cdr != NULL){
		if (branch(work) > branch(work->cdr)){
			return calcom_l(work->cdr);
		}
		else{
			return 0;
		}
	}
	else if (work->cdr == NULL){
 		return 1;
	}
}

float branch(struct cons_t *work)
{
	if (work->type == DIV){
		return calcular(work->car);
	}
	else{
		return calcular(work);
	}
} 

float calif(struct cons_t *work)
{
	if (branch(work) == 1){
		return branch(work->cdr);
	}
	else if (branch(work) == 0){
		return branch(work->cdr->cdr);
	}
}

void calsetq(struct cons_t *work)
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
	struct cell *tmp = (struct cell*)calloc(1,sizeof(struct cell));
	tmp->key = malloc(sizeof(char)*(len+1));
	strcpy(tmp->key,work->svalue);
	tmp->value = branch(work->cdr);
	table[j] = tmp;
}

float value_of(struct cons_t *work)
{
	if (work->type == CHA){
		int i = 0;
		do{
			if (strcmp(table[i]->key,work->svalue) == 0){
				return table[i]->value;
			}
			i++;
		}while (i < tableSIZE);
		int j = recursive_point;
		do{
			j--;
			if (recursive_table[j] != NULL){
				if (strcmp(recursive_table[j]->key,work->svalue) == 0){
					printf("recursive_table[%d] = %d\n",j,recursive_table[j]->value);
					return recursive_table[j]->value;
				}
			}
		}while (j > 0);
	}
	else{
		return work->ivalue;
	}
}

void free_table(){
	int i = 0;
	do{
		free(table[i]);
		i++;
	}while (i < tableSIZE);
}

int check_tree(struct cons_t *work)
{
	if (work->type != DIV){
		if(work->cdr != NULL){
			return search_cha(work) + check_tree(work->cdr);
		}
		else if (work->cdr == NULL){
			return search_cha(work);
		}
	}
	else if (work->type == DIV){
		if (work->cdr != NULL){
			return check_tree(work->car) + check_tree(work->cdr);
		}
		else if(work->cdr == NULL){
			return check_tree(work->car);
		}
	}
}

int search_cha(struct cons_t *work)
{
	if(work->type == CHA){
		if(strcmp(work->svalue,"+") != 0 && strcmp(work->svalue,"-") != 0 && strcmp(work->svalue,"*") != 0 && 
		   strcmp(work->svalue,"/") != 0 && strcmp(work->svalue,"<") != 0 && strcmp(work->svalue,">") != 0 && 
		   strcmp(work->svalue,"if") != 0 && strcmp(work->svalue,"setq") != 0 && strcmp(work->svalue,"defun") != 0){
			int i; int j = 1;
			for(i = 0; i < tableSIZE; i++){
				if(strcmp(table[i]->key,work->svalue) == 0){
					j = 0;
				}
			}
			for(i = 0; i < funcnumber; i++){
				if(strcmp(function[i]->svalue,work->svalue) == 0){
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
	else{
		return 0;
	}
}

void caldefun(struct cons_t *work)
{
	static int into = 0;
	if (into == 0){
		function = calloc(1,sizeof(struct cons_t*));
		into = 1;
	}
	function[funcnumber] = calloc(1,sizeof(struct cons_t));
	function[funcnumber] = work->cdr;
	work->cdr = NULL;
	if(check_recursive(function[funcnumber]->cdr,function[funcnumber]->svalue) != 0){
		function[funcnumber]->type = RECURSIVE;
	}
	funcnumber++;
}

int search_function(struct cons_t *work)
{
	if (function != NULL){
		int i = 0;
		do{
			if(strcmp(work->svalue,function[i]->svalue) == 0){
				return i;
			}
			i++;
		}while (i < funcnumber);
	}
	return -1;
}

float call_function(struct cons_t *work,int i)
{
	struct cons_t *variable;
	variable = function[i]->cdr->car;
	if (function[i]->type == RECURSIVE){
		recursive_setq(work,variable);
	}
	else{
		function_setq(work,variable);
	}
	return calcular(function[i]->cdr->cdr->car);
}

void function_setq(struct cons_t *work,struct cons_t *variable)
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
	struct cell *tmp = (struct cell*)calloc(1,sizeof(struct cell));
	tmp->key = malloc(sizeof(char)*(len+1));
	strcpy(tmp->key,variable->svalue);
	tmp->value = branch(work);
	table[j] = tmp;
	if(work->cdr != NULL){
		function_setq(work->cdr,variable->cdr);
	}
}

int check_recursive(struct cons_t *work,char *func)
{
	int i = 0;
	if (work->type == CHA){
		if(strcmp(func,work->svalue) == 0){
			i = 1;
		}
	}
	if (work->type != DIV){
		if(work->cdr != NULL){
			return i + check_recursive(work->cdr,func);
		}
		else if (work->cdr == NULL){
			return i;
		}
	}
	else if (work->type == DIV){
		if (work->cdr != NULL){
			return check_recursive(work->car,func) + check_recursive(work->cdr,func);
		}
		else if(work->cdr == NULL){
			return check_recursive(work->car,func);
		}
	}
}

void recursive_setq(struct cons_t *work,struct cons_t *variable)
{
	if (recursive_prepare == 0){
		recursive_table = calloc(1,sizeof(struct cell));
		recursive_prepare = 1;
	}
	int len = strlen(variable->svalue);
	struct cell *tmp = (struct cell*)calloc(1,sizeof(struct cell));
	tmp->key = malloc(sizeof(char)*(len+1));
	strcpy(tmp->key,variable->svalue);
	tmp->value = branch(work);
	recursive_table[recursive_tableSIZE] = tmp;
	if(work->cdr != NULL){
		recursive_setq(work->cdr,variable->cdr);
	}
	recursive_tableSIZE++;
	recursive_point = recursive_tableSIZE;
}
