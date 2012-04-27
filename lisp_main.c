#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "my_lisp.h"

int execute(char *input);

#define STRSIZE 128

int main(int argc, char *argv[])
{
	int quit = 0;
	char *input;
	using_history();
	read_history(".my_history");
	if (argc == 1){
		input = input_formula();
		quit = execute(input);
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
			execute(input);
			fclose(fp);
		}
	}
	if (quit == 0){
		input = input_formula();
		while (execute(input) == 0){
			free(input);
			input = input_formula();
		}
		free(input);
	}
	clear_history();
	return 0;
}

int execute(char *input)
{
	int quit = 0;
	char **token;
	token = split(input);
	struct cons_t *tree;
	tree = cons_cell(token);
	dump_tree(tree);
	discriminate(tree);
	if (strcmp(tree->svalue,"quit") == 0 || strcmp(tree->svalue,"q") == 0){
		quit = 1;
	}
	free_tree(tree);
	free_token(token);
	printf("\n");
	return quit;
}


//--arg
//valgrind --leak-check=full
