#include "../include/file.h"
#include "../include/lexer.h"
#include "../include/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    Lexer lexer;
    init_lexer(&lexer, file_contents);

    init_parser();

    Token token;
    do {
        token = get_next_token(&lexer);
        if (token_count < MAX_TOKENS) {
            tokens[token_count++] = token;
        } else {
            fprintf(stderr, "Error: Too many tokens\n");
            free_token(token);
            break;
        }
    } while (token.type != TOKEN_EOF);

    parse_tokens();

    for (int i = 0; i < presentation.slide_count; i++) {
        Slide *slide = &presentation.slides[i];
        printf("Slide %d:\n", slide->number);
        printf("  Title: %s\n", slide->title);
        if (slide->subtitle) {
            printf("  Subtitle: %s\n", slide->subtitle);
        }
        for (int j = 0; j < slide->bullet_count; j++) {
            printf("  - %s\n", slide->bullets[j].text);
        }
        printf("\n");
    }

    for (int i = 0; i < token_count; i++) {
        free_token(tokens[i]);
    }
    free_presentation();
    free(file_contents);

    return EXIT_SUCCESS;
}
