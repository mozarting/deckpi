#ifndef LEXER_H
#define LEXER_H

#include "token.h"

typedef struct {
    const char *source;
    int position;
    int line;
} Lexer;

void init_lexer(Lexer *lexer, const char *source);

int lexer(char *file_content);

char advance(Lexer *lexer);
char peek(Lexer *lexer);
int is_at_end(Lexer *lexer);

Token get_next_token(Lexer *lexer);
void free_token(Token token);

TokenType lookup_keyword(const char *start, const char *end);

#endif // LEXER_H
