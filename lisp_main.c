#include "my_lisp.h"

int cut(char *input);
int execute(char *formula);


int main(int argc, char *argv[])
{
	int quit = 0;
	char *input;
	using_history();
	if (argc == 1) {
		input = input_formula();
		quit = cut(input);
	}
	else if (argc == 2) {
		FILE *fp;
		if ((fp = fopen(argv[1],"r")) == NULL) {
			printf("Can't Open File\n\n");
			return -1;
		}
		else {
			int filelen;
			fseek(fp,0,SEEK_END);
			filelen = ftell(fp);
			fseek(fp,0,SEEK_SET);
			char *input = (char *)calloc(filelen + 1, sizeof(char));
			int i = 0;
			while ( (input[i] = fgetc(fp)) != EOF) {
				i++;
			}
			input[i] = '\0';
			printf("%s",input);
			cut(input);
			fclose(fp);
		}
		quit = 1;
	}
	if (quit == 0) {
		input = input_formula();
		while (cut(input) == 0) {
			input = input_formula();
		}
	}
	free_function();
	clear_history();
	printf("Bye.( '_')/~\n\n");
	return 0;
}

int cut(char *input)
{
	int len = strlen(input);
	int i = 0;
	int quit = 0;

	while (input[i] != '\0') {
		char *formula = (char*)calloc(len + 1, sizeof(char));;
		int R_parentheses = 0;
		int L_parentheses = 0;
		int j = 0;
		while (input[i] != '\0') {
			formula[j] = input[i];
			if (input[i] == '(') {
				R_parentheses++;
			}
			if (input[i] == ')') {
				L_parentheses++;
			}
			i++; j++;
			if (R_parentheses != 0 &&
				R_parentheses == L_parentheses) {
				break;
			}
		}
		if (formula[0] == '\0') {
			break;
		}
		formula[j] = '\0';
		if (R_parentheses == L_parentheses) {
			quit = execute(formula);
		}
		else {
			printf("'%s' is not correct\n\n",formula);
		}
	}
	free(input);
	return quit;
}

int execute(char *formula)
{
	int quit = 0;
	char **token;
	token = split(formula);
	cons_t *tree;
	tree = make_tree(token);
	if(tree != NULL) {
		if (tree->type == SYMBOL) {
			if (tree->ivalue == QUIT) {
				quit = 1;
			}
		}
	}
	discriminate(tree);
	free_tree(tree);
	free_token(token);
	return quit;
}



//--arg
//valgrind --leak-check=full
