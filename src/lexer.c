#include "../include/lexer.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Keyword keywords[] = {{"slide", TOKEN_SLIDE},
                      {"title", TOKEN_TITLE},
                      {"subtitle", TOKEN_SUBTITLE},
                      {"image", TOKEN_IMAGE},
                      {NULL, TOKEN_UNKNOWN}};

void init_lexer(Lexer *lexer, const char *source) {
    lexer->source = source;
    lexer->position = 0;
    lexer->line = 1;
}

char advance(Lexer *lexer) { return lexer->source[lexer->position++]; }

char peek(Lexer *lexer) { return lexer->source[lexer->position]; }

int is_at_end(Lexer *lexer) { return lexer->source[lexer->position] == '\0'; }

Token make_token(Lexer *lexer, TokenType type, const char *start,
                 const char *end) {
    Token token;
    size_t len = end - start;
    char *lexeme = malloc(len + 1);
    strncpy(lexeme, start, len);
    lexeme[len] = '\0';

    token.type = type;
    token.lexeme = lexeme;
    token.line = lexer->line;
    return token;
}

void free_token(Token token) {
    if (token.lexeme) {
        free(token.lexeme);
    }
}

TokenType lookup_keyword(const char *start, const char *end) {
    size_t len = end - start;
    for (int i = 0; keywords[i].word != NULL; i++) {
        if (strlen(keywords[i].word) == len &&
            strncmp(start, keywords[i].word, len) == 0) {
            return keywords[i].type;
        }
    }
    return TOKEN_IDENTIFIER;
}

Token get_next_token(Lexer *lexer) {
    while (!is_at_end(lexer)) {
        char c = advance(lexer);
        switch (c) {
        case ':':
            return make_token(lexer, TOKEN_COLON,
                              lexer->source + lexer->position - 1,
                              lexer->source + lexer->position);
        case '-':
            return make_token(lexer, TOKEN_BULLET,
                              lexer->source + lexer->position - 1,
                              lexer->source + lexer->position);
        case '"': {
            const char *start = lexer->source + lexer->position;
            while (peek(lexer) != '"' && !is_at_end(lexer)) {
                if (peek(lexer) == '\n')
                    lexer->line++;
                advance(lexer);
            }
            if (is_at_end(lexer)) {
                fprintf(stderr, "Unterminated string at line %d\n",
                        lexer->line);
                return make_token(lexer, TOKEN_UNKNOWN, start,
                                  lexer->source + lexer->position);
            }
            const char *end = lexer->source + lexer->position;
            advance(lexer); // consume closing quote
            return make_token(lexer, TOKEN_STRING, start, end);
        }
        case '\n':
            lexer->line++;
            continue;
            // return make_token(lexer, TOKEN_NEWLINE,
            //                   lexer->source + lexer->position - 1,
            //                   lexer->source + lexer->position);
        case ' ':
        case '\t':
        case '\r':
            continue; // skip whitespace
        default:
            if (isdigit(c)) {
                const char *start = lexer->source + lexer->position - 1;
                while (isdigit(peek(lexer)))
                    advance(lexer);
                return make_token(lexer, TOKEN_NUMBER, start,
                                  lexer->source + lexer->position);
            } else if (isalpha(c)) {
                const char *start = lexer->source + lexer->position - 1;
                while (isalnum(peek(lexer)) || peek(lexer) == '_')
                    advance(lexer);
                const char *end = lexer->source + lexer->position;
                TokenType t = lookup_keyword(start, end);
                return make_token(lexer, t, start, end);
            } else {
                return make_token(lexer, TOKEN_UNKNOWN,
                                  lexer->source + lexer->position - 1,
                                  lexer->source + lexer->position);
            }
        }
    }
    return make_token(lexer, TOKEN_EOF, "", "");
}
