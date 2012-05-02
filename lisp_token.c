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
		if (formula[j] != '(' &&
			formula[j] != ')' &&
			formula[j] != ' ' &&
			formula[j] != '\n' &&
			formula[j] != '\t') {
			char *str = (char *)calloc(len,sizeof(char));
			i = 0;
			do {
				str[i] = formula[j];
				i++; j++;
			} while (formula[j] != '(' &&
					 formula[j] != ')' &&
					 formula[j] != ' ' &&
					 formula[j] != '\n' &&
					 formula[j] != '\t' &&
					 formula[j] != '\0');
			int slen = strlen(str);
			token[k]  = malloc(sizeof(char)*(slen+1));
			strcpy(token[k],str);
			free(str);
			k++;
		}
		else if (formula[j] == '(' ||
				 formula[j] == ')') {
			char *str = (char *)calloc(len,sizeof(char));
			str[0] = formula[j];
			int slen = strlen(str);
			token[k] = malloc(sizeof(char)*(slen + 1));
			strcpy(token[k],str);
			free(str);
			j++; k++;
			token[k] = NULL;
		}
		else if (formula[j] == ' ' ||
				 formula[j] == '\n' ||
				 formula[j] == '\t') {
//isspace
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
