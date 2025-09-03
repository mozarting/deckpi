#include <ctype.h>
#include "../include/file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    TOKEN_SLIDE,
    TOKEN_TITLE,
    TOKEN_SUBTITLE,
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
Token tokens[MAX_TOKENS];
int token_count = 0;

typedef struct {
    const char *word;
    TokenType type;
} Keyword;

Keyword keywords[] = {{"slide", TOKEN_SLIDE},
                      {"title", TOKEN_TITLE},
                      {"subtitle", TOKEN_SUBTITLE},
                      {NULL, TOKEN_UNKNOWN}};

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

void add_token_with_lexeme(TokenType type, const char *start, const char *end) {
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
    token_count++;
}

void add_token(TokenType type) { add_token_with_lexeme(type, "", ""); }

void read_number(const char **src) {
    const char *start = *src;

    while (isdigit(**src)) {
        (*src)++;
    }

    const char *end = *src;
    add_token_with_lexeme(TOKEN_NUMBER, start, end);
}

void read_string(const char **src) {
    const char *start = ++(*src);

    while (**src != '"' && **src != '\0') {
        if (**src == '\n') {
            fprintf(stderr, "Unterminated string literal!\n");
            break;
        }
        (*src)++;
    }

    const char *end = *src;
    add_token_with_lexeme(TOKEN_STRING, start, end);

    if (**src == '"')
        (*src)++;
}

void print_tokens() {
    for (int i = 0; i < token_count; i++) {
        printf("%d: %s \"%s\"\n", i, token_name(tokens[i].type),
               tokens[i].lexeme ? tokens[i].lexeme : "");
    }
}

void tokenise(const char *src) {
    while (*src != '\0') {
        switch (*src) {
        case ':':
            add_token(TOKEN_COLON);
            src++;
            break;
        case '-':
            add_token(TOKEN_BULLET);
            src++;
            break;
        case '"': {
            read_string(&src);
            src++;
            break;
        }
        case ' ':
        case '\r':
        case '\t':
            src++;
            break;
        case '\n':
            add_token(TOKEN_NEWLINE);
            src++;
            break;
        default:
            if (isdigit(*src)) {
                read_number(&src);
                src++;
                break;
            } else if (isalpha(*src)) {
                const char *start = src;
                while (isalpha(*src))
                    src++;
                TokenType t = lookup_keyword(start, src);
                add_token_with_lexeme(t, start, src);
                break;
            } else {
                add_token(TOKEN_UNKNOWN);
                src++;
                break;
            }
            break;
        }
    }
    add_token(TOKEN_EOF);
}

int lexer(char *file_content) {
    tokenise(file_content);
    print_tokens();
    return 0;
}

char *read_file(char *file_path) {
    FILE *fp = fopen(file_path, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: cannot open file '%s'\n", file_path);
        return NULL;
    }

    if (fseek(fp, 0, SEEK_END) != 0) {
        fprintf(stderr, "ERROR: failed to seek file '%s'\n", file_path);
        fclose(fp);
        return NULL;
    }
    long filesize = ftell(fp);
    if (filesize < 0) {
        fprintf(stderr, "ERROR: failed to get file size of '%s'\n", file_path);
        fclose(fp);
        return NULL;
    }
    rewind(fp);

    char *buffer = malloc(filesize + 1);
    if (buffer == NULL) {
        fprintf(stderr, "ERROR: memory allocation failed\n");
        fclose(fp);
        return NULL;
    }

    size_t read_size = fread(buffer, 1, filesize, fp);
    if (read_size != (size_t)filesize) {
        fprintf(stderr, "ERROR: failed to read entire file '%s'\n", file_path);
        free(buffer);
        fclose(fp);
        return NULL;
    }

    buffer[filesize] = '\0';
    fclose(fp);
    return buffer;
}

int main(int argc, char **argv) {
    printf("Hello, World");
    if (argc != 2) {
        fprintf(stderr, "ERROR: incorrect usage\n");
        fprintf(stderr, "Correct usage: ./%s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *file_contents = read_file(argv[1]);
    if (file_contents == NULL) {
        return EXIT_FAILURE;
    }

    lexer(file_contents);
    free(file_contents);
    return EXIT_SUCCESS;
}
