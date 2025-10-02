#include "../include/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Presentation presentation;
Token tokens[MAX_TOKENS];
int token_count = 0;
int current_token = 0;

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
    case TOKEN_IMAGE:
        return "IMAGE";
    case TOKEN_EOF:
        return "EOF";
    case TOKEN_UNKNOWN:
        return "UNKNOWN";
    default:
        return "???";
    }
}

void init_parser() {
    presentation.slides = malloc(MAX_SLIDES * sizeof(Slide));
    presentation.slide_count = 0;
    token_count = 0;
    current_token = 0;
}

Token peek_token() {
    if (current_token < token_count) {
        return tokens[current_token];
    }
    Token eof_token = {TOKEN_EOF, "", 0};
    return eof_token;
}

Token consume_token() {
    if (current_token < token_count) {
        return tokens[current_token++];
    }
    Token eof_token = {TOKEN_EOF, "", 0};
    return eof_token;
}

Token expect_token(TokenType type, const char *message) {
    Token token = peek_token();
    if (token.type != type) {
        fprintf(stderr, "Error at line %d: %s, got %s (%s)\n", token.line,
                message, token_name(token.type),
                token.lexeme ? token.lexeme : "");
        exit(EXIT_FAILURE);
    }
    return consume_token();
}

void parse_bullet(Slide *slide) {
    expect_token(TOKEN_BULLET, "Expected bullet point marker '-'");
    Token string_token =
        expect_token(TOKEN_STRING, "Expected string after bullet point marker");

    if (slide->bullet_count >= MAX_BULLETS) {
        fprintf(stderr, "Error at line %d: Too many bullets in slide %d\n",
                string_token.line, slide->number);
        exit(EXIT_FAILURE);
    }

    Bullet bullet;
    bullet.text = strdup(string_token.lexeme);
    slide->bullets[slide->bullet_count++] = bullet;
}

void parse_slide() {
    if (presentation.slide_count >= MAX_SLIDES) {
        fprintf(stderr, "Error: Too many slides\n");
        exit(EXIT_FAILURE);
    }

    Slide slide = {0};
    slide.title = NULL;
    slide.subtitle = NULL;
    slide.bullets = malloc(MAX_BULLETS * sizeof(Bullet));
    slide.bullet_count = 0;

    expect_token(TOKEN_SLIDE, "Expected 'slide' keyword");
    Token number_token = expect_token(TOKEN_NUMBER, "Expected slide number");
    slide.number = atoi(number_token.lexeme);
    expect_token(TOKEN_COLON, "Expected ':' after slide number");

    expect_token(TOKEN_TITLE, "Expected 'title' keyword");
    expect_token(TOKEN_COLON, "Expected ':' after title keyword");
    Token title_token = expect_token(TOKEN_STRING, "Expected string for title");
    slide.title = strdup(title_token.lexeme);

    if (peek_token().type == TOKEN_SUBTITLE) {
        consume_token();
        expect_token(TOKEN_COLON, "Expected ':' after subtitle keyword");
        Token subtitle_token =
            expect_token(TOKEN_STRING, "Expected string for subtitle");
        slide.subtitle = strdup(subtitle_token.lexeme);
    }

    while (peek_token().type != TOKEN_SLIDE && peek_token().type != TOKEN_EOF) {
        Token t = peek_token();

        switch (t.type) {
        case TOKEN_IMAGE:
            consume_token();
            expect_token(TOKEN_COLON, "Expected ':' after image keyword");
            Token image_token =
                expect_token(TOKEN_STRING, "Expected string for image");
            slide.image = strdup(image_token.lexeme);
            break;

        case TOKEN_BULLET:
            parse_bullet(&slide);
            break;

        default:
            fprintf(stderr, "Unexpected token '%s' in slide\n", t.lexeme);
            exit(EXIT_FAILURE);
        }
    }

    if (peek_token().type == TOKEN_IMAGE) {
        consume_token();
        expect_token(TOKEN_COLON, "Expected ':' after image keyword");
        Token image_token =
            expect_token(TOKEN_STRING, "Expected string for subtitle");
        slide.image = strdup(image_token.lexeme);
    }

    while (peek_token().type == TOKEN_BULLET) {
        parse_bullet(&slide);
    }

    presentation.slides[presentation.slide_count++] = slide;
}

void parse_tokens() {
    while (peek_token().type != TOKEN_EOF) {
        parse_slide();
    }
}

void free_presentation() {
    for (int i = 0; i < presentation.slide_count; i++) {
        Slide *slide = &presentation.slides[i];
        free(slide->title);
        if (slide->subtitle) {
            free(slide->subtitle);
        }
        for (int j = 0; j < slide->bullet_count; j++) {
            free(slide->bullets[j].text);
        }
        free(slide->bullets);
    }
    free(presentation.slides);
}
