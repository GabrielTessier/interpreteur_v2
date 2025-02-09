
#include <stdio.h>
#include <stdlib.h>

#include "../external/list/list.h"
#include "token.h"
#include "parser.h"


char* symbole_name[20] = {"s", "code", "sm_commande", "commande", "print", "creation_var", "crea_var_2", "expr", "expr_var", "var_aff", "condition", "condition_2", "ou_loop", "et_condition", "et_loop", "ou_condition", "comp", "si", "sinon", "TOKEN"};


token_t* get_tok(list_t* tokens) {
  return (token_t*) tokens->d->val;
}

ast_t* init_ast(symbole_e type, token_t* tok, int nb_child, ast_t** child) {
  ast_t* ast = (ast_t*) malloc(sizeof(ast_t));
  ast->type = type;
  ast->tok = tok;
  ast->nb_child = nb_child;
  ast->child = child;
  return ast;
}

ast_t* extract_token_as_ast(list_t* tokens, token_e type) {
  token_t* tok = get_tok(tokens);
  if (tok->tok != type) {
    fprintf(stderr, "extract Syntax error, line : %d, token trouver : %s (%d), token attendu : %d\n", tok->line_number, token_to_string(*tok), tok->tok, type);
    return NULL;
  }
  return init_ast(SYM_TOKEN, extract_list_t(tokens, DEBUT), 0, NULL);
}

ast_t* parser(list_t* tokens) {
  token_t* tok = get_tok(tokens);
  switch (tok->tok) {
  case EOF_TOK:
  case LACCOLADE:
  case TYPE:
  case VARIABLE:
  case NOMBRE:
  case INTERROGATION:
  case SI:
  case PRINT:
    {
      int nb_child = 2;
      ast_t** asts = (ast_t**) malloc(sizeof(ast_t*)*nb_child);
      ast_t* sub_ast = parser_code(tokens);
      if (sub_ast == NULL) return NULL;
      asts[0] = sub_ast;
      ast_t* sub_ast2 = extract_token_as_ast(tokens, EOF_TOK);
      if (sub_ast == NULL) return NULL;
      asts[1] = sub_ast2;
      return init_ast(SYM_s, NULL, nb_child, asts);
    }
  default:
    break;
  }
  fprintf(stderr, "Syntax error, line : %d, token : %s\n", tok->line_number, token_to_string(*tok));
  return NULL;
}


ast_t* parser_code(list_t* tokens) {
  token_t* tok = get_tok(tokens);
  switch (tok->tok) {
  case LACCOLADE:
  case TYPE:
  case VARIABLE:
  case NOMBRE:
  case INTERROGATION:
  case SI:
  case PRINT:
    {
      ast_t* sub_ast1 = parser_sm_commande(tokens);
      if (sub_ast1 == NULL) return NULL;
      ast_t* sub_ast2 = parser_code(tokens);
      if (sub_ast2 == NULL) return NULL;
      ast_t** asts = (ast_t**) malloc(sizeof(ast_t*)*2);
      asts[0] = sub_ast1;
      asts[1] = sub_ast2;
      return init_ast(SYM_code, NULL, 2, asts);
    }
  case EOF_TOK:
  case RACCOLADE:
    return init_ast(SYM_code, NULL, 0, NULL);
  default:
    break;
  }
  fprintf(stderr, "Syntax error, line : %d, token : %s\n", tok->line_number, token_to_string(*tok));
  return NULL;
}

ast_t* parser_sm_commande(list_t* tokens) {
  token_t* tok = get_tok(tokens);
  switch (tok->tok) {
  case TYPE:
  case VARIABLE:
  case NOMBRE:
  case INTERROGATION:
  case SI:
  case PRINT:
    {
      ast_t* sub_ast = parser_commande(tokens);
      if (sub_ast == NULL) return NULL;
      ast_t** asts = (ast_t**) malloc(sizeof(ast_t*));
      asts[0] = sub_ast;
      return init_ast(SYM_sm_commande, NULL, 1, asts);
    }
  case LACCOLADE:
    {
      int nb_child = 3;
      ast_t** asts = (ast_t**) malloc(sizeof(ast_t*)*nb_child);
      asts[0] = extract_token_as_ast(tokens, LACCOLADE);

      ast_t* sub_ast = parser_code(tokens);
      if (sub_ast == NULL) return NULL;
      asts[1] = sub_ast;

      ast_t* sub_ast2 = extract_token_as_ast(tokens, RACCOLADE);
      if (sub_ast2 == NULL) return NULL;
      asts[2] = sub_ast2;

      return init_ast(SYM_sm_commande, NULL, nb_child, asts);
    }
  default:
    break;
  }
  fprintf(stderr, "Syntax error, line : %d, token : %s\n", tok->line_number, token_to_string(*tok));
  return NULL;
}

ast_t* parser_commande(list_t* tokens) {
  token_t* tok = get_tok(tokens);
  switch (tok->tok) {
  case TYPE:
    {
      ast_t* sub_ast = parser_creation_var(tokens);
      if (sub_ast == NULL) return NULL;
      ast_t** asts = (ast_t**) malloc(sizeof(ast_t*));
      asts[0] = sub_ast;
      return init_ast(SYM_commande, NULL, 1, asts);
    }
  case VARIABLE:
  case NOMBRE:
    {
      int nb_child = 2;
      ast_t** asts = (ast_t**) malloc(sizeof(ast_t*)*nb_child);
      ast_t* sub_ast = parser_expr(tokens);
      if (sub_ast == NULL) return NULL;
      asts[0] = sub_ast;
      ast_t* sub_ast2 = extract_token_as_ast(tokens, POINT_VIRGULE);
      if (sub_ast2 == NULL) return NULL;
      asts[1] = sub_ast2;
      return init_ast(SYM_commande, NULL, nb_child, asts);
    }
  case INTERROGATION:
    {
      int nb_child = 2;
      ast_t** asts = (ast_t**) malloc(sizeof(ast_t*)*nb_child);
      ast_t* sub_ast = parser_condition(tokens);
      if (sub_ast == NULL) return NULL;
      asts[0] = sub_ast;
      ast_t* sub_ast2 = extract_token_as_ast(tokens, POINT_VIRGULE);
      if (sub_ast2 == NULL) return NULL;
      asts[1] = sub_ast2;
      return init_ast(SYM_commande, NULL, nb_child, asts);
    }
  case SI:
    {
      int nb_child = 1;
      ast_t** asts = (ast_t**) malloc(sizeof(ast_t*)*nb_child);
      ast_t* sub_ast = parser_si(tokens);
      if (sub_ast == NULL) return NULL;
      asts[0] = sub_ast;
      return init_ast(SYM_commande, NULL, nb_child, asts);
    }
  case PRINT:
    {
      int nb_child = 5;
      ast_t** asts = (ast_t**) malloc(sizeof(ast_t*)*nb_child);
      asts[0] = extract_token_as_ast(tokens, PRINT);
      ast_t* sub_ast1 = extract_token_as_ast(tokens, LPARENTHESE);
      if (sub_ast1 == NULL) return NULL;
      asts[1] = sub_ast1;
      ast_t* sub_ast2 = parser_print(tokens);
      if (sub_ast2 == NULL) return NULL;
      asts[2] = sub_ast2;
      ast_t* sub_ast3 = extract_token_as_ast(tokens, RPARENTHESE);
      if (sub_ast3 == NULL) return NULL;
      asts[3] = sub_ast3;
      ast_t* sub_ast4 = extract_token_as_ast(tokens, POINT_VIRGULE);
      if (sub_ast4 == NULL) return NULL;
      asts[4] = sub_ast4;
      return init_ast(SYM_commande, NULL, nb_child, asts);
    }
  default:
    break;
  }
  fprintf(stderr, "Syntax error, line : %d, token : %s\n", tok->line_number, token_to_string(*tok));
  return NULL;
}

ast_t* parser_print(list_t* tokens) {
  token_t* tok = get_tok(tokens);
  switch (tok->tok) {
  case VARIABLE:
  case NOMBRE:
    {
      int nb_child = 1;
      ast_t** asts = (ast_t**) malloc(sizeof(ast_t*)*nb_child);
      ast_t* sub_ast = parser_expr(tokens);
      if (sub_ast == NULL) return NULL;
      asts[0] = sub_ast;
      return init_ast(SYM_print, NULL, nb_child, asts);
    }
  case INTERROGATION:
    {
      int nb_child = 1;
      ast_t** asts = (ast_t**) malloc(sizeof(ast_t*)*nb_child);
      ast_t* sub_ast = parser_condition(tokens);
      if (sub_ast == NULL) return NULL;
      asts[0] = sub_ast;
      return init_ast(SYM_print, NULL, nb_child, asts);
    }
  default:
    break;
  }
  fprintf(stderr, "Syntax error, line : %d, token : %s\n", tok->line_number, token_to_string(*tok));
  return NULL;
}

ast_t* parser_creation_var(list_t* tokens) {
  token_t* tok = get_tok(tokens);
  switch (tok->tok) {
  case TYPE:
    {
      int nb_child = 3;
      ast_t** asts = (ast_t**) malloc(sizeof(ast_t*)*nb_child);
      asts[0] = extract_token_as_ast(tokens, TYPE);
      ast_t* sub_ast1 = extract_token_as_ast(tokens, VARIABLE);
      if (sub_ast1 == NULL) return NULL;
      asts[1] = sub_ast1;
      ast_t* sub_ast2 = parser_crea_var_2(tokens);
      if (sub_ast2 == NULL) return NULL;
      asts[2] = sub_ast2;
      return init_ast(SYM_creation_var, NULL, nb_child, asts);
    }
  default:
    break;
  }
  fprintf(stderr, "Syntax error, line : %d, token : %s\n", tok->line_number, token_to_string(*tok));
  return NULL;
}

ast_t* parser_crea_var_2(list_t* tokens) {
  token_t* tok = get_tok(tokens);
  switch (tok->tok) {
  case AFFECTATION:
    {
      int nb_child = 3;
      ast_t** asts = (ast_t**) malloc(sizeof(ast_t*)*nb_child);
      asts[0] = extract_token_as_ast(tokens, AFFECTATION);
      ast_t* sub_ast1 = parser_expr(tokens);
      if (sub_ast1 == NULL) return NULL;
      asts[1] = sub_ast1;
      ast_t* sub_ast2 = extract_token_as_ast(tokens, POINT_VIRGULE);
      if (sub_ast2 == NULL) return NULL;
      asts[2] = sub_ast2;
      return init_ast(SYM_crea_var_2, NULL, nb_child, asts);
    }
  case POINT_VIRGULE:
    {
      int nb_child = 1;
      ast_t** asts = (ast_t**) malloc(sizeof(ast_t*)*nb_child);
      asts[0] = extract_token_as_ast(tokens, POINT_VIRGULE);
      return init_ast(SYM_crea_var_2, NULL, nb_child, asts);
    }
  default:
    break;
  }
  fprintf(stderr, "Syntax error, line : %d, token : %s\n", tok->line_number, token_to_string(*tok));
  return NULL;
}

ast_t* parser_expr(list_t* tokens) {
  token_t* tok = get_tok(tokens);
  switch (tok->tok) {
  case VARIABLE:
    {
      int nb_child = 2;
      ast_t** asts = (ast_t**) malloc(sizeof(ast_t*)*nb_child);
      asts[0] = extract_token_as_ast(tokens, VARIABLE);
      ast_t* sub_ast1 = parser_expr_var(tokens);
      if (sub_ast1 == NULL) return NULL;
      asts[1] = sub_ast1;
      return init_ast(SYM_expr, NULL, nb_child, asts);
    }
  case NOMBRE:
    {
      int nb_child = 1;
      ast_t** asts = (ast_t**) malloc(sizeof(ast_t*)*nb_child);
      asts[0] = extract_token_as_ast(tokens, NOMBRE);
      return init_ast(SYM_expr, NULL, nb_child, asts);
    }
  default:
    break;
  }
  fprintf(stderr, "Syntax error, line : %d, token : %s\n", tok->line_number, token_to_string(*tok));
  return NULL;
}

ast_t* parser_expr_var(list_t* tokens) {
  token_t* tok = get_tok(tokens);
  switch (tok->tok) {
  case AFFECTATION:
    {
      int nb_child = 1;
      ast_t** asts = (ast_t**) malloc(sizeof(ast_t*)*nb_child);
      ast_t* sub_ast = parser_var_aff(tokens);
      if (sub_ast == NULL) return NULL;
      asts[0] = sub_ast;
      return init_ast(SYM_expr_var, NULL, nb_child, asts);
    }
  case POINT_VIRGULE:
  case RPARENTHESE:
    return init_ast(SYM_expr_var, NULL, 0, NULL);
  default:
    break;
  }
  fprintf(stderr, "Syntax error, line : %d, token : %s\n", tok->line_number, token_to_string(*tok));
  return NULL;
}

ast_t* parser_var_aff(list_t* tokens) {
  token_t* tok = get_tok(tokens);
  switch (tok->tok) {
  case AFFECTATION:
    {
      int nb_child = 2;
      ast_t** asts = (ast_t**) malloc(sizeof(ast_t*)*nb_child);
      asts[0] = extract_token_as_ast(tokens, AFFECTATION);
      ast_t* sub_ast = parser_expr(tokens);
      if (sub_ast == NULL) return NULL;
      asts[1] = sub_ast;
      return init_ast(SYM_var_aff, NULL, nb_child, asts);
    }
  default:
    break;
  }
  fprintf(stderr, "Syntax error, line : %d, token : %s\n", tok->line_number, token_to_string(*tok));
  return NULL;
}

ast_t* parser_condition(list_t* tokens) {
  token_t* tok = get_tok(tokens);
  switch (tok->tok) {
  case INTERROGATION:
    {
      int nb_child = 4;
      ast_t** asts = (ast_t**) malloc(sizeof(ast_t*)*nb_child);
      asts[0] = extract_token_as_ast(tokens, INTERROGATION);
      ast_t* sub_ast1 = extract_token_as_ast(tokens, LPARENTHESE);
      if (sub_ast1 == NULL) return NULL;
      asts[1] = sub_ast1;
      ast_t* sub_ast2 = parser_condition_2(tokens);
      if (sub_ast2 == NULL) return NULL;
      asts[2] = sub_ast2;
      ast_t* sub_ast3 = extract_token_as_ast(tokens, RPARENTHESE);
      if (sub_ast3 == NULL) return NULL;
      asts[3] = sub_ast3;
      return init_ast(SYM_condition, NULL, nb_child, asts);
    }
  default:
    break;
  }
  fprintf(stderr, "Syntax error, line : %d, token : %s\n", tok->line_number, token_to_string(*tok));
  return NULL;
}

ast_t* parser_condition_2(list_t* tokens) {
  token_t* tok = get_tok(tokens);
  switch (tok->tok) {
  case VRAI:
  case FAUX:
  case VARIABLE:
  case LPARENTHESE:
    {
      int nb_child = 2;
      ast_t** asts = (ast_t**) malloc(sizeof(ast_t*)*nb_child);
      ast_t* sub_ast1 = parser_et_condition(tokens);
      if (sub_ast1 == NULL) return NULL;
      asts[0] = sub_ast1;
      ast_t* sub_ast2 = parser_ou_loop(tokens);
      if (sub_ast2 == NULL) return NULL;
      asts[1] = sub_ast2;
      return init_ast(SYM_condition_2, NULL, nb_child, asts);
    }
  default:
    break;
  }
  fprintf(stderr, "Syntax error, line : %d, token : %s\n", tok->line_number, token_to_string(*tok));
  return NULL;
}

ast_t* parser_ou_loop(list_t* tokens) {
  token_t* tok = get_tok(tokens);
  switch (tok->tok) {
  case OU:
    {
      int nb_child = 3;
      ast_t** asts = (ast_t**) malloc(sizeof(ast_t*)*nb_child);
      asts[0] = extract_token_as_ast(tokens, OU);
      ast_t* sub_ast1 = parser_et_condition(tokens);
      if (sub_ast1 == NULL) return NULL;
      asts[1] = sub_ast1;
      ast_t* sub_ast2 = parser_ou_loop(tokens);
      if (sub_ast2 == NULL) return NULL;
      asts[2] = sub_ast2;
      return init_ast(SYM_ou_loop, NULL, nb_child, asts);
    }
  case RPARENTHESE:
      return init_ast(SYM_ou_loop, NULL, 0, NULL);
  default:
    break;
  }
  fprintf(stderr, "Syntax error, line : %d, token : %s\n", tok->line_number, token_to_string(*tok));
  return NULL;
}

ast_t* parser_et_condition(list_t* tokens) {
  token_t* tok = get_tok(tokens);
  switch (tok->tok) {
  case VRAI:
  case FAUX:
  case VARIABLE:
  case LPARENTHESE:
    {
      int nb_child = 2;
      ast_t** asts = (ast_t**) malloc(sizeof(ast_t*)*nb_child);
      ast_t* sub_ast1 = parser_ou_condition(tokens);
      if (sub_ast1 == NULL) return NULL;
      asts[0] = sub_ast1;
      ast_t* sub_ast2 = parser_et_loop(tokens);
      if (sub_ast2 == NULL) return NULL;
      asts[1] = sub_ast2;
      return init_ast(SYM_et_condition, NULL, nb_child, asts);
    }
  default:
    break;
  }
  fprintf(stderr, "Syntax error, line : %d, token : %s\n", tok->line_number, token_to_string(*tok));
  return NULL;
}

ast_t* parser_et_loop(list_t* tokens) {
  token_t* tok = get_tok(tokens);
  switch (tok->tok) {
  case ET:
    {
      int nb_child = 3;
      ast_t** asts = (ast_t**) malloc(sizeof(ast_t*)*nb_child);
      asts[0] = extract_token_as_ast(tokens, ET);
      ast_t* sub_ast1 = parser_ou_condition(tokens);
      if (sub_ast1 == NULL) return NULL;
      asts[1] = sub_ast1;
      ast_t* sub_ast2 = parser_et_loop(tokens);
      if (sub_ast2 == NULL) return NULL;
      asts[2] = sub_ast2;
      return init_ast(SYM_et_loop, NULL, nb_child, asts);
    }
  case OU:
  case RPARENTHESE:
      return init_ast(SYM_et_loop, NULL, 0, NULL);
  default:
    break;
  }
  fprintf(stderr, "Syntax error, line : %d, token : %s\n", tok->line_number, token_to_string(*tok));
  return NULL;
}

ast_t* parser_ou_condition(list_t* tokens) {
  token_t* tok = get_tok(tokens);
  switch (tok->tok) {
  case VRAI:
    {
      int nb_child = 1;
      ast_t** asts = (ast_t**) malloc(sizeof(ast_t*)*nb_child);
      asts[0] = extract_token_as_ast(tokens, VRAI);
      return init_ast(SYM_ou_condition, NULL, nb_child, asts);
    }
  case FAUX:
    {
      int nb_child = 1;
      ast_t** asts = (ast_t**) malloc(sizeof(ast_t*)*nb_child);
      asts[0] = extract_token_as_ast(tokens, FAUX);
      return init_ast(SYM_ou_condition, NULL, nb_child, asts);
    }
  case VARIABLE:
    {
      int nb_child = 2;
      ast_t** asts = (ast_t**) malloc(sizeof(ast_t*)*nb_child);
      asts[0] = extract_token_as_ast(tokens, VARIABLE);
      ast_t* sub_ast = parser_comp(tokens);
      if (sub_ast == NULL) return NULL;
      asts[1] = sub_ast;
      return init_ast(SYM_ou_condition, NULL, nb_child, asts);
    }
  case LPARENTHESE:
    {
      int nb_child = 3;
      ast_t** asts = (ast_t**) malloc(sizeof(ast_t*)*nb_child);
      asts[0] = extract_token_as_ast(tokens, LPARENTHESE);
      ast_t* sub_ast = parser_condition_2(tokens);
      if (sub_ast == NULL) return NULL;
      asts[1] = sub_ast;
      ast_t* sub_ast2 = extract_token_as_ast(tokens, RPARENTHESE);
      if (sub_ast2 == NULL) return NULL;
      asts[2] = sub_ast2;
      return init_ast(SYM_ou_condition, NULL, nb_child, asts);
    }
  default:
    break;
  }
  fprintf(stderr, "Syntax error, line : %d, token : %s\n", tok->line_number, token_to_string(*tok));
  return NULL;
}

ast_t* parser_comp(list_t* tokens) {
  token_t* tok = get_tok(tokens);
  switch (tok->tok) {
  case COMPARAISON:
    {
      int nb_child = 2;
      ast_t** asts = (ast_t**) malloc(sizeof(ast_t*)*nb_child);
      asts[0] = extract_token_as_ast(tokens, COMPARAISON);
      ast_t* sub_ast1 = extract_token_as_ast(tokens, VARIABLE);
      if (sub_ast1 == NULL) return NULL;
      asts[1] = sub_ast1;
      return init_ast(SYM_comp, NULL, nb_child, asts);
    }
  case ET:
  case OU:
  case RPARENTHESE:
    return init_ast(SYM_comp, NULL, 0, NULL);
  default:
    break;
  }
  fprintf(stderr, "Syntax error, line : %d, token : %s\n", tok->line_number, token_to_string(*tok));
  return NULL;
}

ast_t* parser_si(list_t* tokens) {
  token_t* tok = get_tok(tokens);
  switch (tok->tok) {
  case SI:
    {
      int nb_child = 4;
      ast_t** asts = (ast_t**) malloc(sizeof(ast_t*)*nb_child);
      asts[0] = extract_token_as_ast(tokens, SI);
      ast_t* sub_ast = parser_condition(tokens);
      if (sub_ast == NULL) return NULL;
      asts[1] = sub_ast;
      ast_t* sub_ast2 = parser_sm_commande(tokens);
      if (sub_ast2 == NULL) return NULL;
      asts[2] = sub_ast2;
      ast_t* sub_ast3 = parser_sinon(tokens);
      if (sub_ast3 == NULL) return NULL;
      asts[3] = sub_ast3;
      return init_ast(SYM_si, NULL, nb_child, asts);
    }
  default:
    break;
  }
  fprintf(stderr, "Syntax error, line : %d, token : %s\n", tok->line_number, token_to_string(*tok));
  return NULL;
}

ast_t* parser_sinon(list_t* tokens) {
  token_t* tok = get_tok(tokens);
  switch (tok->tok) {
  case SINON:
    {
      int nb_child = 2;
      ast_t** asts = (ast_t**) malloc(sizeof(ast_t*)*nb_child);
      asts[0] = extract_token_as_ast(tokens, SINON);
      ast_t* sub_ast = parser_sm_commande(tokens);
      if (sub_ast == NULL) return NULL;
      asts[1] = sub_ast;
      return init_ast(SYM_sinon, NULL, nb_child, asts);
    }
  case LACCOLADE:
  case TYPE:
  case VARIABLE:
  case NOMBRE:
  case INTERROGATION:
  case SI:
  case PRINT:
      return init_ast(SYM_sinon, NULL, 0, NULL);
  default:
    break;
  }
  fprintf(stderr, "Syntax error, line : %d, token : %s\n", tok->line_number, token_to_string(*tok));
  return NULL;
}


void free_ast(ast_t* ast) {
  if (ast->tok != NULL) free_token(ast->tok);
  for (int i=0; i<ast->nb_child; i++) {
    free_ast(ast->child[i]);
  }
  free(ast->child);
  free(ast);
}

void print_ast_json(ast_t* ast) {
  char* str = (ast->tok != NULL)?token_to_string(*(ast->tok)):"";
  printf("{\n\"type\" : \"%s\",\n \"tokName\" : \"%s\",\n \"childs\":[\n", symbole_name[ast->type], str);
  if (ast->tok != NULL) free(str);
  for (int i=0; i<ast->nb_child; i++) {
    print_ast_json(ast->child[i]);
    if (i!=ast->nb_child-1) printf(",\n");
  }
  printf("]}");
}
