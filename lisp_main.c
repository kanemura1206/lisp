#include <stdio.h>
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


int main(int argc, char *argv[])
{
	char *input;
	if (argc == 1){
		input = input_formula();
	}
	else if (argc == 2){
		char moji[100];
		FILE *fp;
		if((fp=fopen(argv[1],"r"))==NULL){
			printf("ファイルを開けません。\n");
			return -1;
		}
		else{
			fgets(moji,100,fp);
			int i;
			for(i = 0; moji[i] != '\0'; i++){
				if(moji[i] == '\n'){
					moji[i] = '\0';
				}
			}
			input = moji;
			printf("%s\n",input);
			fclose(fp);
		}		
	}
	char **result;
	result = resolve(input);
	struct cons_t *tree;
	tree = cons_cell(result);
	dump_tree(tree);
	free_token(result);
	free_tree(tree);
	return 0;
}


//--arg
//valgrind --leak-check=full
