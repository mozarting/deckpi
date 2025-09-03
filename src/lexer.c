#include "../include/lexer.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Token tokens[MAX_TOKENS];
int token_count = 0;

Keyword keywords[] = {{"slide", TOKEN_SLIDE},
                      {"title", TOKEN_TITLE},
                      {"subtitle", TOKEN_SUBTITLE},
                      {NULL, TOKEN_UNKNOWN}};

void init_lexer(Lexer *lexer, const char *source) {
    lexer->source = source;
    lexer->position = 0;
    lexer->line = 1;
}

const char *token_name(TokenType t) {
    switch (t) {
    case TOKEN_SLIDE:
        return "SLIDE";
    case TOKEN_TITLE:
        return "TITLE";
    case TOKEN_SUBTITLE:
        return "SUBTITLE";
    case TOKEN_STRING:
        return "STRING";
    case TOKEN_NUMBER:
        return "NUMBER";
    case TOKEN_COLON:
        return "COLON";
    case TOKEN_BULLET:
        return "BULLET";
    case TOKEN_NEWLINE:
        return "NEWLINE";
    case TOKEN_EOF:
        return "EOF";
    case TOKEN_UNKNOWN:
        return "UNKNOWN";
    default:
        return "???";
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

void add_token_with_lexeme(Lexer *lexer, TokenType type, const char *start,
                           const char *end) {
    if (token_count >= MAX_TOKENS) {
        fprintf(stderr, "ERROR: Too many tokens!\n");
        exit(1);
    }

    size_t len = end - start;
    char *lexeme = malloc(len + 1);
    if (!lexeme) {
        fprintf(stderr, "ERROR: Memory allocation failed\n");
        exit(1);
    }

    strncpy(lexeme, start, len);
    lexeme[len] = '\0';

    tokens[token_count].type = type;
    tokens[token_count].lexeme = lexeme;
    tokens[token_count].line = lexer->line;
    token_count++;
}

void add_token(Lexer *lexer, TokenType type) {
    add_token_with_lexeme(lexer, type, "", "");
}

void read_number(Lexer *lexer) {
    const char *start = lexer->source + lexer->position;
    while (isdigit(peek(lexer))) {
        advance(lexer);
    }
    const char *end = lexer->source + lexer->position;
    add_token_with_lexeme(lexer, TOKEN_NUMBER, start, end);
}

void read_string(Lexer *lexer) {
    advance(lexer);
    const char *start = lexer->source + lexer->position;
    while (peek(lexer) != '"' && peek(lexer) != '\0') {
        if (peek(lexer) == '\n') {
            fprintf(stderr, "Unterminated string literal at line %d!\n",
                    lexer->line);
            advance(lexer);
            lexer->line++;
            break;
        }
        advance(lexer);
    }
    const char *end = lexer->source + lexer->position;
    add_token_with_lexeme(lexer, TOKEN_STRING, start, end);
    if (peek(lexer) == '"') {
        advance(lexer);
    }
}

void print_tokens() {
    for (int i = 0; i < token_count; i++) {
        printf("%d: %s \"%s\"\n", i, token_name(tokens[i].type),
               tokens[i].lexeme ? tokens[i].lexeme : "");
    }
}

char advance(Lexer *lexer) { return lexer->source[lexer->position++]; }

char peek(Lexer *lexer) { return lexer->source[lexer->position]; }

void next_token(Lexer *lexer) {
    while (peek(lexer) != '\0') {
        switch (peek(lexer)) {
        case ':':
            add_token(lexer, TOKEN_COLON);
            advance(lexer);
            break;
        case '-':
            add_token(lexer, TOKEN_BULLET);
            advance(lexer);
            break;
        case '"': {
            read_string(lexer);
            break;
        }
        case ' ':
        case '\r':
        case '\t':
            advance(lexer);
            break;
        case '\n':
            add_token(lexer, TOKEN_NEWLINE);
            advance(lexer);
            lexer->line++;
            break;
        default:
            if (isdigit(peek(lexer))) {
                read_number(lexer);
                break;
            } else if (isalpha(peek(lexer))) {
                const char *start = lexer->source + lexer->position;
                while (isalpha(peek(lexer)))
                    advance(lexer);
                const char *end = lexer->source + lexer->position;
                TokenType t = lookup_keyword(start, end);
                add_token_with_lexeme(lexer, t, start, end);
                break;
            } else {
                add_token(lexer, TOKEN_UNKNOWN);
                advance(lexer);
                break;
            }
            break;
        }
    }
    add_token(lexer, TOKEN_EOF);
}

int lexer(char *file_content) {
    Lexer lexer;
    init_lexer(&lexer, file_content);
    next_token(&lexer);
    print_tokens();
    return 0;
}

void free_tokens(void) {
    for (int i = 0; i < token_count; i++) {
        free(tokens[i].lexeme);
        tokens[i].lexeme = NULL;
    }
    token_count = 0;
}
