#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>


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
			if(input[j] != '(' && input[j] != ')' && input[j] != ' '){
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
			else
				j++;
		}
		else
			j++;
	}while(input[j] != ')');
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