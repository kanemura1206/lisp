#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TABLE_SIZE 100;

typedef struct cons_t{
	int type;
	union{
		struct cons_t *car;
		int ivalue;
		char *svalue;
	};
	struct cons_t *cdr;
}cons_t;

typedef struct cell{
	char *key;
	int value;
}cell;

//struct cell *table[TABLE_SIZE];


enum{
	NUM,
	CHA,
	DIV
};

float calcular(struct cons_t *work,struct cell **table);
float caladd(struct cons_t *work,struct cell **table);
float calsub(struct cons_t *work,struct cell **table);
float calmul(struct cons_t *work,struct cell **table);
float caldiv(struct cons_t *work,struct cell **table);
float calcom_r(struct cons_t *work,struct cell **table);
float calcom_l(struct cons_t *work,struct cell **table);
float branch(struct cons_t *work,struct cell **table);
float calif(struct cons_t *work,struct cell **table);
float calsetq(struct cons_t *work,struct cell **table);
float value_of(struct cons_t *work,struct cell **table);
float search_table(char *key,struct cell **table);

void discriminate(struct cons_t *work)
{ 
	struct cell **tab = (struct cell**)calloc(1,sizeof(struct cell));
	static struct cell **table;
	static int static_value;
	if (static_value == 0){
		table = tab;
		struct cell *p = (struct cell*)calloc(1,sizeof(struct cell));
		p->key = "T"; p->value = 1;
		table[0] = p;
		struct cell *q = (struct cell*)calloc(1,sizeof(struct cell));
	    q->key = "Nil"; q->value = 0;
		table[1] = q;
		static_value++;
	}
	if (strcmp(work->svalue,"+") == 0 || strcmp(work->svalue,"-") == 0 ||
	   strcmp(work->svalue,"*") == 0 || strcmp(work->svalue,"/") == 0){
		printf("= %f\n\n",calcular(work,table));
	}
	else if (strcmp(work->svalue,"<") ==0 || strcmp(work->svalue,">") == 0){
		float k = calcular(work,table);
		if(k == 1){
			printf("T\n\n");
		}
		else if(k == 0){
			printf("Nil\n\n");
		}
	}
	else if (strcmp(work->svalue,"if") == 0){
		printf("%f\n\n",calcular(work,table));
	}
	else if (strcmp(work->svalue,"setq") == 0){
		calsetq(work->cdr,table);
		printf("\n");
	}
	else{
		printf("%f\n\n",value_of(work,table));
	}
}

float calcular(struct cons_t *work,struct cell **table)
{
	if (strcmp(work->svalue,"+") == 0){
		return caladd(work->cdr,table);
	}
	else if (strcmp(work->svalue,"-") == 0){
		return calsub(work->cdr,table);
	}
	else if (strcmp(work->svalue,"*") == 0){
		return calmul(work->cdr,table);
	}
	else if (strcmp(work->svalue,"/") == 0){
		return caldiv(work->cdr,table);
	}
	else if (strcmp(work->svalue,"<") == 0){
		return calcom_r(work->cdr,table);
	}
	else if (strcmp(work->svalue,">") == 0){
		return calcom_l(work->cdr,table);
	}
	else if (strcmp(work->svalue,"if") == 0){
		return calif(work->cdr,table);
	}
	else if (strcmp(work->svalue,"setq") == 0){
		return calsetq(work->cdr,table);
	}
	else{
		printf("%f\n\n",value_of(work,table));
	}
}

float caladd(struct cons_t *work,struct cell **table)
{
	if (work->type != DIV){
		if(work->cdr != NULL){
			return value_of(work,table) + caladd(work->cdr,table);
		}
		else if (work->cdr == NULL){
			return value_of(work,table);
		}
	}
	else if (work->type == DIV){
		if (work->cdr != NULL){
			return calcular(work->car,table) + caladd(work->cdr,table);
		}
		else if(work->cdr == NULL){
			return calcular(work->car,table);
		}
	}
}

float calsub(struct cons_t *work,struct cell **table)
{
	if (work->type != DIV){
		if(work->cdr != NULL){
			return value_of(work,table) - caladd(work->cdr,table);
		}
		else if (work->cdr == NULL){
			return value_of(work,table);
		}
	}
	else if (work->type == DIV){
		if (work->cdr != NULL){
			return calcular(work->car,table) - caladd(work->cdr,table);
		}
		else if(work->cdr == NULL){
			return calcular(work->car,table);
		}
	}
}
float calmul(struct cons_t *work,struct cell **table)
{
	if (work->type != DIV){
		if(work->cdr != NULL){
			return value_of(work,table) * caladd(work->cdr,table);
		}
		else if (work->cdr == NULL){
			return value_of(work,table);
		}
	}
	else if (work->type == DIV){
		if (work->cdr != NULL){
			return calcular(work->car,table) * caladd(work->cdr,table);
		}
		else if(work->cdr == NULL){
			return calcular(work->car,table);
		}
	}
}

float caldiv(struct cons_t *work,struct cell **table)
{
	if (work->type != DIV){
		if(work->cdr != NULL){
			return value_of(work,table) / caladd(work->cdr,table);
		}
		else if (work->cdr == NULL){
			return value_of(work,table);
		}
	}
	else if (work->type == DIV){
		if (work->cdr != NULL){
			return calcular(work->car,table) / caladd(work->cdr,table);
		}
		else if(work->cdr == NULL){
			return calcular(work->car,table);
		}
	}
}

float calcom_r(struct cons_t *work,struct cell **table)
{
	if (work->cdr != NULL){
		if (branch(work,table) < branch(work->cdr,table)){
			return calcom_r(work->cdr,table);
		}
		else{
			return 0;
		}
	}
	else if (work->cdr == NULL){
		return 1;
	}
}

float calcom_l(struct cons_t *work,struct cell **table)
{
	if (work->cdr != NULL){
		if (branch(work,table) > branch(work->cdr,table)){
			return calcom_l(work->cdr,table);
		}
		else{
			return 0;
		}
	}
	else if (work->cdr == NULL){
 		return 1;
	}
}

float branch(struct cons_t *work,struct cell **table)
{
	if (work->type == DIV){
		return calcular(work->car,table);
	}
	else{
		return value_of(work,table);
	}
} 

float calif(struct cons_t *work,struct cell **table)
{
	if (branch(work,table) == 1){
		return branch(work->cdr,table);
	}
	else if (branch(work,table) == 0){
		return branch(work->cdr->cdr,table);
	}
}

float calsetq(struct cons_t *work,struct cell **table)
{
	int i = 0;
	static int j = 2;
	do{
		if(strcmp(table[i]->key,work->svalue) == 0){
			printf("'%s' is already exists/n/n",work->svalue);
			exit(1);
		}
		i++;
	}while (i < j);
	j++;
	struct cell *tmp = (struct cell*)calloc(1,sizeof(struct cell));
	int len = strlen(work->svalue);
	tmp->key = (char*)malloc(sizeof(char)*len-1);
	strncpy(tmp->key,work->svalue,len);
	tmp->value = branch(work->cdr,table);
	table[i] = tmp;
	return table[i]->value;
}

float value_of(struct cons_t *work,struct cell **table)
{
	if (work->type == CHA){
		return search_table(work->svalue,table);
	}
	else{
		return work->ivalue;
	}
}

float search_table(char *key,struct cell **table)
{
	int i = 0;
	do{
		if(strcmp(table[i]->key,key) == 0){
			return table[i]->value;
		}
		i++;
	}while (table[i] != NULL);
	printf("'%s' is not exist\n\n",key);
	exit(1);
}
