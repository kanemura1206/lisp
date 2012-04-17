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

void discriminate(struct cons_t *work)
{
	if (strcmp(work->svalue,"+") == 0 || strcmp(work->svalue,"-") == 0 ||
	   strcmp(work->svalue,"*") == 0 || strcmp(work->svalue,"/") == 0){
		printf(" = %f\n\n",calcular(work));
	}
	else if ( strcmp(work->svalue,"<") || strcmp(work->svalue,">") ){
		float k = calcular(work);
		if(k == 1){
			printf("T\n\n");
		}
		else if(k == 0){
			printf("Nil\n\n");
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
}

float caladd(struct cons_t *work)
{
	if (work->type != DIV){
		if(work->cdr != NULL){
			return work->ivalue + caladd(work->cdr);
		}
		else if (work->cdr == NULL){
			return work->ivalue;
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
			return work->ivalue - caladd(work->cdr);
		}
		else if (work->cdr == NULL){
			return work->ivalue;
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
			return work->ivalue * caladd(work->cdr);
		}
		else if (work->cdr == NULL){
			return work->ivalue;
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
			return work->ivalue / caladd(work->cdr);
		}
		else if (work->cdr == NULL){
			return work->ivalue;
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
		return work->ivalue;
	}
} 
