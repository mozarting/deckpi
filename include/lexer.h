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
void free_token(Token token);
void free_tokens(void);
TokenType lookup_keyword(const char *start, const char *end);
void add_token_with_lexeme(Lexer *lexer, TokenType type, const char *start,
                           const char *end);
void add_token(Lexer *lexer, TokenType type);
int is_at_end(Lexer *lexer);
void read_number(Lexer *lexer);
void read_string(Lexer *lexer);
void next_token(Lexer *lexer);

#endif // LEXER_H
