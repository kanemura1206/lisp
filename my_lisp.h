char* input_formula();
char** split(char* input);
void free_token(char** token);
void dump_token(char** token);
struct cons_t* cons_cell(char** token);
void dump_tree(struct cons_t *work);
void free_tree(struct cons_t *work);
void discriminate(struct cons_t *work);

