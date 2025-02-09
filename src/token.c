
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "../external/list/list.h"
#include "token.h"

char token_preview = '\0';
int line_number = 1;

#define NB_TYPE 2
const char* list_type[NB_TYPE] = {"int", "float"};

bool is_letter(char c) {
  return (c>='a' && c<='z') || (c>='A' && c<='Z');
}

bool is_number(char c) {
  return (c>='0' && c<='9');
}

bool is_blank(char c) {
  return (c=='\0' || c==' ' || c=='\n');
}

void next_char(FILE* prog) {
  if (token_preview == '\n') line_number++;
  token_preview = getc(prog);
}

char* get_word(FILE* prog) {
  list_t* word_list = init_list_t();
  int size = 0;
  while (is_letter(token_preview) || is_number(token_preview) || token_preview == '_' || token_preview == '-') {
    add_list_t(word_list, (void*) (intptr_t) token_preview, FIN);
    size++;
    next_char(prog);
  }
  char* word = (char*) malloc(sizeof(char)*(size+1));
  for (int i=0; i<size; i++) {
    word[i] = (char) (intptr_t) extract_list_t(word_list, DEBUT);
  }
  word[size] = '\0';
  free_list_t(word_list, NULL);
  return word;
}

char* get_number(FILE* prog) {
  list_t* word_list = init_list_t();
  int size = 0;
  while (is_number(token_preview)) {
    add_list_t(word_list, (void*) (intptr_t) token_preview, FIN);
    size++;
    next_char(prog);
  }
  char* word = (char*) malloc(sizeof(char)*(size+1));
  for (int i=0; i<size; i++) {
    word[i] = (char) (intptr_t) extract_list_t(word_list, DEBUT);
  }
  word[size] = '\0';
  free_list_t(word_list, NULL);
  return word;
}

bool next_token(token_t* tok, FILE* prog) {
  while (is_blank(token_preview)) {
    next_char(prog);
  }
  tok->str = NULL;
  tok->line_number = line_number;
  switch (token_preview) {
  case 'a' ... 'z':
  case 'A' ... 'Z':
    tok->tok = VARIABLE;
    tok->str = get_word(prog);
    tok->line_number = line_number;
    for (int i=0; i<NB_TYPE; i++) {
      if (strcmp(tok->str, list_type[i]) == 0) {
        tok->tok = TYPE;
        break;
      }
    }
    if (strcmp(tok->str, "vrai") == 0) {
      tok->tok = VRAI;
    } else if (strcmp(tok->str, "faux") == 0) {
      tok->tok = FAUX;
    } else if (strcmp(tok->str, "si") == 0) {
      tok->tok = SI;
    } else if (strcmp(tok->str, "sinon") == 0) {
      tok->tok = SINON;
    }
    break;
  case '0' ... '9':
    tok->tok = NOMBRE;
    tok->str = get_number(prog);
    tok->line_number = line_number;
    break;
  case '=':
    next_char(prog);
    if (token_preview == '=') {
      tok->tok = COMPARAISON;
      next_char(prog);
    } else {
      tok->tok = AFFECTATION;
    }
    break;
  case '<':
  case '>':
    char ft = token_preview;
    next_char(prog);
    tok->tok = COMPARAISON;
    tok->line_number = line_number;
    if (token_preview == '=') {
      tok->str = (char*) malloc(sizeof(char)*3);
      tok->str[0] = ft;
      tok->str[1] = '=';
      tok->str[2] = '\0';
      next_char(prog);
    } else {
      tok->str = (char*) malloc(sizeof(char)*2);
      tok->str[0] = ft;
      tok->str[1] = '\0';
    }
    break;
  case '(':
    tok->tok = LPARENTHESE;
    next_char(prog);
    break;
  case ')':
    tok->tok = RPARENTHESE;
    next_char(prog);
    break;
  case '[':
    tok->tok = LCROCHET;
    next_char(prog);
    break;
  case ']':
    tok->tok = RCROCHET;
    next_char(prog);
    break;
  case '{':
    tok->tok = LACCOLADE;
    next_char(prog);
    break;
  case '}':
    tok->tok = RACCOLADE;
    next_char(prog);
    break;
  case ';':
    tok->tok = POINT_VIRGULE;
    next_char(prog);
    break;
  case '&':
    next_char(prog);
    if (token_preview != '&') return false;
    next_char(prog);
    tok->tok = ET;
    break;
  case '|':
    next_char(prog);
    if (token_preview != '|') return false;
    next_char(prog);
    tok->tok = OU;
    break;
  case '?':
    tok->tok = INTERROGATION;
    next_char(prog);
    break;
  case EOF:
    tok->tok = EOF_TOK;
    tok->str = NULL;
    tok->line_number = line_number;
    break;
  default:
    return false;
  }
  return true;
}


char* token_to_string(token_t tok) {
  char* res;
  switch (tok.tok) {
  case VARIABLE:
  case NOMBRE:
  case COMPARAISON:
  case TYPE:
  case SI:
  case SINON:
    size_t size = strlen(tok.str)+1;
    res = (char*) malloc(sizeof(char)*size);
    memcpy(res, tok.str, size);
    break;
  case AFFECTATION:
    res = (char*) malloc(sizeof(char)*2);
    res[0] = '='; res[1] = '\0';
    break;
  case LPARENTHESE:
    res = (char*) malloc(sizeof(char)*2);
    res[0] = '('; res[1] = '\0';
    break;
  case RPARENTHESE:
    res = (char*) malloc(sizeof(char)*2);
    res[0] = ')'; res[1] = '\0';
    break;
  case LCROCHET:
    res = (char*) malloc(sizeof(char)*2);
    res[0] = '['; res[1] = '\0';
    break;
  case RCROCHET:
    res = (char*) malloc(sizeof(char)*2);
    res[0] = ']'; res[1] = '\0';
    break;
  case LACCOLADE:
    res = (char*) malloc(sizeof(char)*2);
    res[0] = '{'; res[1] = '\0';
    break;
  case RACCOLADE:
    res = (char*) malloc(sizeof(char)*2);
    res[0] = '}'; res[1] = '\0';
    break;
  case POINT_VIRGULE:
    res = (char*) malloc(sizeof(char)*2);
    res[0] = ';'; res[1] = '\0';
    break;
  case ET:
    res = (char*) malloc(sizeof(char)*3);
    res[0] = '&'; res[1] = '&'; res[2] = '\0';
    break;
  case OU:
    res = (char*) malloc(sizeof(char)*3);
    res[0] = '|'; res[1] = '|'; res[2] = '\0';
    break;
  case VRAI:
    res = (char*) malloc(sizeof(char)*2);
    res[0] = 'V'; res[1] = '\0';
    break;
  case FAUX:
    res = (char*) malloc(sizeof(char)*2);
    res[0] = 'F'; res[1] = '\0';
    break;
  case INTERROGATION:
    res = (char*) malloc(sizeof(char)*2);
    res[0] = '?'; res[1] = '\0';
    break;
  case EOF_TOK:
    res = (char*) malloc(sizeof(char)*4);
    res[0] = 'E'; res[1] = 'O'; res[2] = 'F'; res[3] = '\0';
    break;
  }
  return res;
}
