#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_lisp.h"

struct cons_t{
	int type;
	union{
		struct cons_t *car;
		int ivalue;
		char *svalue;
	};
	struct cons_t *cdr;
};

#define STRSIZE 126

int main(int argc, char *argv[])
{
	int i = 0;
	char *input;
	using_history();
	read_history(".my_history");
	if (argc == 1){
		input = input_formula();
		add_history(input);
	}
	else if (argc == 2){
		i = 1;
		char str[STRSIZE];
		FILE *fp;
		if ((fp=fopen(argv[1],"r")) == NULL) {
			printf("Can't Open File\n\n");
			return -1;
		}
		else {
			// TODO fix strsize. is STRSIZE too small!?
			fgets(str,STRSIZE,fp);
			int i;
			for(i = 0; str[i] != '\0'; i++){
				if(str[i] == '\n'){
					str[i] = '\0';
				}
			}
			input = str;
			printf("%s\n",input);
			fclose(fp);
		}		
	}
	int quit = 0;
	do{
		char **token;
		token = split(input);
		if(i == 0){
			free(input);
		}
		i = 0;
		struct cons_t *tree;
		tree = cons_cell(token);
		//dump_tree(tree);
		discriminate(tree);
		if (strcmp(tree->svalue,"quit") == 0 || strcmp(tree->svalue,"q") == 0){
			quit = 1;
		}
		free_tree(tree);
		free_token(token);
		printf("\n");
		if (quit == 0){
			input = input_formula();
		}
	}while (quit == 0);
	return 0;
}


//--arg
//valgrind --leak-check=full
