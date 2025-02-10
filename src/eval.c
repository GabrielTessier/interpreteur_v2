
#include <stdbool.h>
#include <stdint.h>

#include "../external/list/list.h"
#include "parser.h"

struct var_s {
  char* name;
  int type;
  intptr_t val;
};
typedef struct var_s var_t;

struct scope_s {
  struct scope_s* parent;
  list_t* vars;
};
typedef struct scope_s scope_t;

scope_t* init_scope(scope_t* parent, list_t* vars) {
  scope_t* sc = (scope_t*) malloc(sizeof(scope_t));
  sc->parent = parent;
  sc->vars = vars;
  return sc;
}

bool eval(ast_t* ast) {
  return eval_code(ast->child[0], NULL);
}

bool eval_code(ast_t* ast, scope_t scope_parent) {
  scope_t* scope = init_scope(scope_parent, init_list_t());
  while (ast->nb_child != 0) {
    bool ok = eval_sm_commande(ast->child[0], scope);
    if (!ok) return false;
    ast = ast->child[1];
  }
}

bool eval_sm_commande(ast_t* ast, scope_t* scope) {
  switch (ast->child[0]->type) {
  case SYM_commande:
    return eval_commande(ast->child[0], scope);
  case SYM_TOKEN:
    return eval_code(ast->child[1], scope);
  }
  return false;
}

bool eval_commande(ast_t* ast, scope_t* scope) {
  switch (ast->child[0]->type) {
  case SYM_creation_var:
    return eval_creation_var(ast->child[0], scope);
  case SYM_expr:
    return eval_expr(ast->child[0], scope);
  case SYM_condition:
    return eval_condition(ast->child[0], scope);
  case SYM_si:
    return eval_si(ast->child[0], scope);
  case SYM_TOKEN:
    return eval_print(ast->child[2], scope);
  }
  return false;
}

bool eval_print(ast_t* ast, scope_t* scope) {
  
}
