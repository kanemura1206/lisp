#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

int distinction(char *c);

typedef struct cons_t{
	int type;
	union{
		struct cons_t *car;
		int ivalue;
		char *svalue;
	};
	struct cons_t *cdr;
}cons_t;

char* input_formula()
{
	char *c = readline(">>>");
	return c;
}


char** resolve(char* input)
{
	int len = strlen(input);
	char **token = (char **)calloc(len,sizeof(char*));
	int i,j,k;
	j = 0; k = 0;
	do{
		if(input[j] != '(' && input[j] != ')' && input[j] != ' '){
			char *str = (char *)calloc(len,sizeof(char));
			i = 0;
			do{
				str[i] = input[j];
				i++; j++;
				if(input[j] == '(' || input[j] == ')' || input[j] == ' '){
					token[k] = str;
					k++;
				}
			}while(input[j] != '(' && input[j] != ')' && input[j] != ' ');
		}
		else if(input[j] == '(' || input[j] == ')'){
			char *str = (char *)calloc(len,sizeof(char));
			str[0] = input[j];
			token[k] = str;
			j++; k++;
		}
		else if(input[j] == ' ')
			j++;
	}while(input[j] != '\0');
	return token;
}


void dump_token(char** token){
	int i;
	for(i = 0; token[i] != NULL; i++){
		printf("%s\n",token[i]);
	}
}


void free_token(char** token)
{
	int i;
	for(i = 0; token[i-1] != NULL; i++){
		free(token[i]);
	}
	free(token);
}


void cons_cell(char** result){
	struct cons_t *work = (struct cons_t*)calloc(1,sizeof(struct cons_t));
	int i = 0;
	int j = 0;
	if(result[0] == "("){ 
		i = 1;
	}
	struct cons_t *start;
	start = work;
	struct cons_t **memory = (struct cons_t**)calloc(1,sizeof(struct cons_t*));;
	do{
		if(result[i] == "("){
			memory[j] = work;
			struct cons_t *new = (struct cons_t*)calloc(1,sizeof(struct cons_t));
			work->car = new;
			work = new;
			i++; j++;
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
				j--;
				work->cdr = NULL;
				work = memory[j];
				i++;
			}
			if(result[i] != NULL){
				struct cons_t *next = (struct cons_t*)calloc(1,sizeof(struct cons_t));
				work->cdr = next;
				work = next;
			}
		}
	}while(result[i] != NULL);
	free(work);
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
