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

char* input_formula();
char** split(char* input);
void free_token(char** token);
void dump_token(char** token);
cons_t* make_tree(char** token);
void dump_tree(cons_t *work);
void free_tree(cons_t *work);
void discriminate(cons_t *work);


enum{
	NUM,
	CHA,
	CAR,
	RECURSIVE
};
