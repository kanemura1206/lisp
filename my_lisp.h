char* input_formula();
char** resolve(char* input);
void free_token(char** token);
void dump_token(char** token);
struct cons_t* cons_cell(char** result);
void dump_tree(struct cons_t *work);
void free_tree(struct cons_t *work);

