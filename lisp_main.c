#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "my_lisp.h"

int cut(char *input);
int execute(char *formula);

#define STRSIZE 128

int main(int argc, char *argv[])
{
	int quit = 0;
	char *input;
	using_history();
	read_history(".my_history");
	if (argc == 1){
		input = input_formula();
		quit = cut(input);
		free(input);
	}
	else if (argc == 2){
		char str[STRSIZE];
		FILE *fp;
		if ((fp=fopen(argv[1],"r")) == NULL) {
			printf("Can't Open File\n\n");
			return -1;
		}
		else {
			/* // TODO fix strsize. is STRSIZE too small!? */
			fgets(str,STRSIZE,fp);
			char tmp[STRSIZE];
			while(fgets(tmp,STRSIZE,fp) != NULL){
				strcat(str,tmp);
			}
			input = str;
			printf("%s",input);
			cut(input);
			fclose(fp);
		}
		quit = 1;
	}
	if (quit == 0){
		input = input_formula();
		while (cut(input) == 0){
			free(input);
			input = input_formula();
		}
		free(input);
	}
	free_function();
	clear_history();
	return 0;
}

int cut(char*input)
{
	int i = 0;
	int quit = 0;

	while (input[i] != '\0'){
		char formula[STRSIZE];
		int R_parentheses = 0;
		int L_parentheses = 0;
		int j = 0;
		while (input[i] != '\0'){
			formula[j] = input[i];
			if (input[i] == '('){
				R_parentheses++;
			}
			if (input[i] == ')'){
				L_parentheses++;
			}
			i++; j++;
			if (R_parentheses != 0 && R_parentheses == L_parentheses){
				break;
			}
		}
		if (formula[0] == '\0'){
			break;
		}
		formula[j] = '\0';
		if (R_parentheses == L_parentheses){
			quit = execute(formula);
		}
		else{
			printf("'%s' is not correct\n",formula);
		}
	}
	return quit;
}

int execute(char *formula)
{
	int quit = 0;
	char **token;
	token = split(formula);
	cons_t *tree;
	tree = make_tree(token);
	if(tree != NULL){
		if (tree->type == SYMBOL){
			if (tree->ivalue == QUIT){
				quit = 1;
			}
		}
	}
	discriminate(tree);
	free_tree(tree);
	free_token(token);
	printf("\n");
	return quit;
}



//--arg
//valgrind --leak-check=full
