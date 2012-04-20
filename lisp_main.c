#include <stdio.h>
#include <string.h>
#include "my_lisp.h"

typedef struct cons_t{
	int type;
	union{
		struct cons_t *car;
		int ivalue;
		char *svalue;
	};
	struct cons_t *cdr;
}cons_t;

#define STRSIZE 126

int main(int argc, char *argv[])
{
	int quit = 0;
	do{
		char *input;
		if (argc == 1){
			input = input_formula();
		}
		else if (argc == 2){
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
		char **result;
		result = split(input);
		struct cons_t *tree;
		tree = cons_cell(result);
		dump_tree(tree);
		discriminate(tree);
		if (strcmp(tree->svalue,"quit") == 0 || strcmp(tree->svalue,"q") == 0){
			quit = 1;
		}
		free_token(result);
		free_tree(tree);
		printf("\n");
	}while (quit == 0);
	return 0;
}


//--arg
//valgrind --leak-check=full
