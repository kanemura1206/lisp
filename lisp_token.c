#include "my_lisp.h"


char* input_formula()
{
	char *c = readline(">>>");
	add_history(c);
	return c;
}


char** split(char* formula)
{
	int len = strlen(formula);
	char **token = (char **)calloc(len,sizeof(char*));
	int i,j,k;
	j = 0; k = 0;
	do {
		if ((int)formula[j] > 32) {
			char *str = (char *)calloc(len,sizeof(char));
			i = 0;
			if (formula[j] == '(' || formula[j] == ')') {
				str[0] = formula[j];
				j++;
			}
			else {
				do {
					str[i] = formula[j];
					i++; j++;
				} while ((int)formula[j] > 32 && formula[j] != '(' && formula[j] != ')');
			}
			int slen = strlen(str);
			token[k]  = malloc(sizeof(char)*(slen+1));
			strcpy(token[k],str);
			free(str);
			k++;
			token[k] = NULL;
		}
		else if ((int)formula[j] < 33 && formula[j] != '\0') {
			j++;
		}
	} while (formula[j] != '\0');
	return token;
}


void dump_token(char** token) {
	int i;
	for (i = 0; token[i] != NULL; i++) {
		printf("%s\n",token[i]);
	}
}


void free_token(char** token)
{
	if (token == NULL) {
		return;
	}
	int i;
	for (i = 0; token[i] != NULL; i++) {
		free(token[i]);
		token[i] = NULL;
	}
	free(token);
	token = NULL;
}
