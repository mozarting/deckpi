#include "../include/file.h"
#include "../include/lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "ERROR: incorrect usage\n");
        fprintf(stderr, "Correct usage: ./%s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *file_contents = read_file(argv[1]);
    if (file_contents == NULL) {
        return EXIT_FAILURE;
    }

    // lexer(file_contents);

    Lexer lexer;
    init_lexer(&lexer, file_contents);

    Token token;
    do {
        token = get_next_token(&lexer);
        printf("%s \"%s\"\n", token_name(token.type),
               token.lexeme ? token.lexeme : "");
        free_token(token);
    } while (token.type != TOKEN_EOF);

    free(file_contents);

    return EXIT_SUCCESS;
}
