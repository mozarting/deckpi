#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_SLIDE,
    TOKEN_TITLE,
    TOKEN_SUBTITLE,
    TOKEN_IMAGE,
    TOKEN_STRING,
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_COLON,
    TOKEN_BULLET,
    TOKEN_EOF,
    TOKEN_UNKNOWN,
    TOKEN_NEWLINE
} TokenType;

typedef struct {
    TokenType type;
    char *lexeme;
    int line;
} Token;

#define MAX_TOKENS 1024

extern Token tokens[MAX_TOKENS];
extern int token_count;

typedef struct {
    const char *word;
    TokenType type;
} Keyword;

extern Keyword keywords[];

#endif // TOKEN_H
