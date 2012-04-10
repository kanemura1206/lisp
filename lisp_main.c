#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "my_lisp.h"

int main(void)
{
	char *input;
	input = input_formula();
	char **result;
	result = resolve(input);
	cons_cell(result);
	dump_token(result);
	free_token(result);
	return 0;
}
