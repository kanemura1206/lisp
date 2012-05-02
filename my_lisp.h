#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef struct cons_t{
	int type;
	union{
		struct cons_t *car;
		int ivalue;
		char *svalue;
	};
	struct cons_t *cdr;
}cons_t;

typedef struct cell{
	char *key;
	int value;
}cell;

typedef struct func_t{
	char *func_name;
	int arg_SIZE;
	cons_t *tree;
}func_t;

char* input_formula();
char** split(char* input);
void free_token(char** token);
void dump_token(char** token);
cons_t* make_tree(char** token);
void dump_tree(cons_t* work);
void free_tree(cons_t* work);
void discriminate(cons_t* work);
void free_function();

enum{
	NUM,
	CHA,
	CAR,
	ARG,
	SYMBOL
};

enum{
	PLUS,
	MINUS,
	ASTERISK,
	SLASH,
	IF,
	LESS_THAN_SIGN,
	LESS_THAN_OR_EQUAL_TO,
	GREATER_THAN_SIGN,
	GREATER_THAN_EQUAL_TO,
	SETQ,
	DEFUN,
	QUIT
};

