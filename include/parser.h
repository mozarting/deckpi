#ifndef PARSER_H
#define PARSER_H

#include "token.h"

#define MAX_SLIDES 100
#define MAX_BULLETS 50

typedef struct {
    char *text;
} Bullet;

typedef struct {
    int number;
    char *title;
    char *subtitle; // NULL if no subtitle
    Bullet *bullets;
    int bullet_count;
} Slide;

typedef struct {
    Slide *slides;
    int slide_count;
} Presentation;

extern Presentation presentation;

void init_parser();
void parse_tokens();
void free_presentation();

#endif // PARSER_H
