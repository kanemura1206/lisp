#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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

static int j = 2;


enum{
	NUM,
	CHA,
	DIV
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
int search_table(char *key);
void free_table();

static struct cell **table;

void discriminate(struct cons_t *work)
{
	struct cell **tab = (struct cell**)calloc(1,sizeof(struct cell));
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
	if (strcmp(work->svalue,"quit") == 0 || strcmp(work->svalue,"q") == 0){
		free_table();
		free(tab);
	}
	else if (strcmp(work->svalue,"setq") == 0){
		calsetq(work->cdr);
	}
    else{
		if (strcmp(work->svalue,"+") == 0 || strcmp(work->svalue,"-") == 0 ||
			strcmp(work->svalue,"*") == 0 || strcmp(work->svalue,"/") == 0){
			printf("= %f\n",calcular(work));
		}
		else if (strcmp(work->svalue,"<") ==0 || strcmp(work->svalue,">") == 0){
			float k = calcular(work);
			if(k == 1){
				printf("T\n");
			}
			else if(k == 0){
				printf("Nil\n");
			}
		}
		else if (strcmp(work->svalue,"if") == 0){
			printf("%f\n",calcular(work));
		}
		else{
			printf("%f\n",value_of(work));
		}
	}
}

float calcular(struct cons_t *work)
{
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
}

float caladd(struct cons_t *work)
{
	if (work->type != DIV){
		if(work->cdr != NULL){
			return value_of(work) + caladd(work->cdr);
		}
		else if (work->cdr == NULL){
			return value_of(work);
		}
	}
	else if (work->type == DIV){
		if (work->cdr != NULL){
			return calcular(work->car) + caladd(work->cdr);
		}
		else if(work->cdr == NULL){
			return calcular(work->car);
		}
	}
}

float calsub(struct cons_t *work)
{
	if (work->type != DIV){
		if(work->cdr != NULL){
			return value_of(work) - caladd(work->cdr);
		}
		else if (work->cdr == NULL){
			return value_of(work);
		}
	}
	else if (work->type == DIV){
		if (work->cdr != NULL){
			return calcular(work->car) - caladd(work->cdr);
		}
		else if(work->cdr == NULL){
			return calcular(work->car);
		}
	}
}
float calmul(struct cons_t *work)
{
	if (work->type != DIV){
		if(work->cdr != NULL){
			return value_of(work) * caladd(work->cdr);
		}
		else if (work->cdr == NULL){
			return value_of(work);
		}
	}
	else if (work->type == DIV){
		if (work->cdr != NULL){
			return calcular(work->car) * caladd(work->cdr);
		}
		else if(work->cdr == NULL){
			return calcular(work->car);
		}
	}
}

float caldiv(struct cons_t *work)
{
	if (work->type != DIV){
		if(work->cdr != NULL){
			return value_of(work) / caladd(work->cdr);
		}
		else if (work->cdr == NULL){
			return value_of(work);
		}
	}
	else if (work->type == DIV){
		if (work->cdr != NULL){
			return calcular(work->car) / caladd(work->cdr);
		}
		else if(work->cdr == NULL){
			return calcular(work->car);
		}
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
		return value_of(work);
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
	int k = 0;
	do{
		if(strcmp(table[i]->key,work->svalue) == 0){
			printf("'%s' is already exists\n",work->svalue);
			k = 1;
		}
		i++;
	}while (i < j);
	if (k == 0){
		j++;
		struct cell *tmp = (struct cell*)calloc(1,sizeof(struct cell));
		int len = strlen(work->svalue);
		tmp->key = (char*)malloc(sizeof(char)*len-1);
		strncpy(tmp->key,work->svalue,len);
		tmp->value = branch(work->cdr);
		table[i] = tmp;
	}
//	table[i+1] = NULL;
}

float value_of(struct cons_t *work)
{
	if (work->type == CHA){
		return table[search_table(work->svalue)]->value;
	}
	else{
		return work->ivalue;
	}
}

int search_table(char *key)
{
	int i = 0;
	do{
		if(strcmp(table[i]->key,key) == 0){
			return i;
		}
		i++;
	}while (i < j);
	printf("'%s' is not exist\n",key);
	return -1;
}

void free_table(){
	int i = 0;
	do{
		free(table[i]);
		i++;
	}while (i < j);
}
