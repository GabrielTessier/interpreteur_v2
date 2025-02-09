
#include <stdio.h>
#include <stdlib.h>

#include "token.h"
#include "interpreteur.h"

void print_help(int argc, char** argv) {
  if (argc==0){}
  printf("%s <prog>\n", argv[0]);
}

int main(int argc, char** argv) {

  if (argc != 2) {
    print_help(argc, argv);
    return 0;
  }

  FILE* prog = fopen(argv[1], "r");
  if (prog == NULL) {
    printf("No such file or directory: '%s'\n", argv[1]);
    return 0;
  }

  token_t tok;
  do {
    if (!next_token(&tok, prog)) {
      printf("TOK inconue\n");
      return 1;
    }
    char* str = token_to_string(tok);
    printf("%d : %s (%d)\n", tok.line_number, str, tok.tok);
    free(str);
    free(tok.str);
  } while (tok.tok != EOF_TOK);

  fclose(prog);

  return 0;
}
