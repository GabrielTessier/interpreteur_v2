
#ifndef PARSER_H
#define PARSER_H

#include "../external/list/list.h"

enum symbole {SYM_s, SYM_code, SYM_sm_commande, SYM_commande, SYM_print, SYM_creation_var, SYM_crea_var_2, SYM_expr, SYM_expr_var, SYM_var_aff, SYM_condition, SYM_condition_2, SYM_ou_loop, SYM_et_condition, SYM_et_loop, SYM_ou_condition, SYM_comp, SYM_si, SYM_sinon, SYM_TOKEN};
typedef enum symbole symbole_e;

struct ast_s {
  symbole_e type;
  token_t* tok;
  int nb_child;
  struct ast_s** child;
};
typedef struct ast_s ast_t;

ast_t* parser(list_t* tokens);
ast_t* parser_code(list_t* tokens);
ast_t* parser_sm_commande(list_t* tokens);
ast_t* parser_commande(list_t* tokens);
ast_t* parser_print(list_t* tokens);
ast_t* parser_creation_var(list_t* tokens);
ast_t* parser_crea_var_2(list_t* tokens);
ast_t* parser_expr(list_t* tokens);
ast_t* parser_expr_var(list_t* tokens);
ast_t* parser_var_aff(list_t* tokens);
ast_t* parser_condition(list_t* tokens);
ast_t* parser_condition_2(list_t* tokens);
ast_t* parser_ou_loop(list_t* tokens);
ast_t* parser_et_condition(list_t* tokens);
ast_t* parser_et_loop(list_t* tokens);
ast_t* parser_ou_condition(list_t* tokens);
ast_t* parser_comp(list_t* tokens);
ast_t* parser_si(list_t* tokens);
ast_t* parser_sinon(list_t* tokens);

void free_ast(ast_t* ast);
void print_ast_json(ast_t* ast);

#endif
