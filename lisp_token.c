#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>


char* input_formula()
{
	char *c = readline(">>>");
	add_history(c);
	write_history(".my_history");
	return c;
}


char** split(char* input)
{
	int len = strlen(input);
	char **token = (char **)calloc(128,sizeof(char*));
	int i,j,k;
	j = 0; k = 0;
	do{
		if (input[j] != '(' && input[j] != ')' && input[j] != ' ' && input[j] != '\n' && input[j] != '\t'){
			char *str = (char *)calloc(len,sizeof(char));
			i = 0;
			do{
				str[i] = input[j];
				i++; j++;
			}while (input[j] != '(' && input[j] != ')' && input[j] != ' ' && input[j] != '\n' && 
					input[j] != '\t' && input[j] != '\0');
			int slen = strlen(str);
			token[k]  = malloc(sizeof(char)*(slen+1));
			strcpy(token[k],str);
			free(str);
			k++;
		}
		else if (input[j] == '(' || input[j] == ')'){
			char *str = (char *)calloc(len,sizeof(char));
			str[0] = input[j];
			int slen = strlen(str);
			token[k] = malloc(sizeof(char)*(slen + 1));
			strcpy(token[k],str);
			free(str);
			j++; k++;
		}
		else if (input[j] == ' ' || input[j] == '\n' || input[j] == '\t'){
			j++;
		}
	}while (input[j] != '\0');
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
	for(i = 0; token[i] != NULL; i++){
		if(i == 37){
			asm("int3");
		}
		free(token[i]);
	}
	free(token);
}
