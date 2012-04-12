#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "my_lisp.h"

int main(int argc, char *argv[])
{
	char *input;

	if (argc == 1){
	input = input_formula();
	}
	else if (argc == 2){
		FILE *fp;
		int i = 0;
		fp = fopen(argv[1],"r");
		while ((input[i] = getc(fp)) != EOF){
			i++;
		}
		i++;
		input[i] = '\0';
		fclose(fp);
	}
	char **result;
	result = resolve(input);
	cons_cell(result);
	free_token(result);
	return 0;
}
