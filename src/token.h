
#ifndef TOKEN_H
#define TOKEN_H

#include <stdbool.h>

//enum token {EOF_TOK, NUM, TYPE, VAR, EQ, EQ_OP, FUNCTION, LPAR, RPAR, LACC, RACC, SEMI_COLON, LCRO, RCRO, COMMA, INF, SUP, PRINT};
enum token {TYPE, VARIABLE, NOMBRE, AFFECTATION, COMPARAISON, LPARENTHESE, RPARENTHESE, LCROCHET, RCROCHET, LACCOLADE, RACCOLADE, POINT_VIRGULE, EOF_TOK};
typedef enum token token_e;

struct token_s {
  token_e tok;
  char* str;
  int line_number;
};
typedef struct token_s token_t;


bool is_letter(char c);
bool is_number(char c);
bool is_blank(char c);
void next_char(FILE* prog);
char* get_word(FILE* prog);
char* get_number(FILE* prog);
bool next_token(token_t* tok, FILE* prog);
char* token_to_string(token_t tok);

#endif
