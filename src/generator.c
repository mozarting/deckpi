#include "../include/parser.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static TTF_Font *title_font = NULL;
static TTF_Font *subtitle_font = NULL;
static TTF_Font *bullet_font = NULL;

void draw_text(const char *text, TTF_Font *font, int x, int y,
               SDL_Color color) {
    if (!text)
        return;
    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text, color);
    if (!surface) {
        fprintf(stderr, "TTF_RenderUTF8_Blended error: %s\n", TTF_GetError());
        return;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dst = {x, y, surface->w, surface->h};
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);
}

int draw_centered_text(const char *text, TTF_Font *font, int y,
                       SDL_Color color) {
    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text, color);
    if (!surface)
        return 0;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dst = {WINDOW_WIDTH / 2, y, surface->w, surface->h};
    SDL_FreeSurface(surface);

    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);

    return dst.h;
}

void render_slide(Slide *slide) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Color white = {255, 255, 255, 255};

    int y = WINDOW_HEIGHT / 2; // starting top margin

    // Title
    if (slide->title) {
        y += draw_centered_text(slide->title, title_font, y, white) + 20;
    }

    // Subtitle
    if (slide->subtitle) {
        y += draw_centered_text(slide->subtitle, subtitle_font, y, white) + 20;
    }

    // Image
    if (slide->image) {
        SDL_Surface *image_sur = IMG_Load(slide->image);
        if (!image_sur) {
            fprintf(stderr, "Error loading image: %s\n", IMG_GetError());
        } else {
            SDL_Texture *image_tex =
                SDL_CreateTextureFromSurface(renderer, image_sur);
            int iw = image_sur->w;
            int ih = image_sur->h;
            SDL_FreeSurface(image_sur);

            int max_w = 600, max_h = 400;
            float aspect = (float)iw / (float)ih;
            if (iw > max_w) {
                iw = max_w;
                ih = (int)(max_w / aspect);
            }
            if (ih > max_h) {
                ih = max_h;
                iw = (int)(max_h * aspect);
            }

            SDL_Rect dest = {WINDOW_WIDTH / 2, y, iw, ih};
            SDL_RenderCopy(renderer, image_tex, NULL, &dest);
            SDL_DestroyTexture(image_tex);

            y += ih + 20;
        }
    }

    // Bullets
    for (int i = 0; i < slide->bullet_count; i++) {
        char buffer[1024];
        snprintf(buffer, sizeof(buffer), "• %s", slide->bullets[i].text);
        y += draw_centered_text(buffer, bullet_font, y, white) + 10;
    }

    SDL_RenderPresent(renderer);
}

int run_slides(Presentation *presentation) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() != 0) {
        fprintf(stderr, "TTF_Init Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        fprintf(stderr, "SDL2_image Error: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("SDL2 Slides", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH,
                              WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    title_font = TTF_OpenFont("fonts/Roboto-Bold.ttf", 48);
    subtitle_font = TTF_OpenFont("fonts/Roboto-Regular.ttf", 32);
    bullet_font = TTF_OpenFont("fonts/Roboto-Regular.ttf", 24);
    if (!title_font || !subtitle_font || !bullet_font) {
        fprintf(stderr, "Error loading fonts: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    int current = 0;
    int running = 1;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    running = 0;
                } else if (e.key.keysym.sym == SDLK_RIGHT) {
                    if (current < presentation->slide_count - 1)
                        current++;
                } else if (e.key.keysym.sym == SDLK_LEFT) {
                    if (current > 0)
                        current--;
                }
            }
        }

        render_slide(&presentation->slides[current]);
        SDL_Delay(16);
    }

    TTF_CloseFont(title_font);
    TTF_CloseFont(subtitle_font);
    TTF_CloseFont(bullet_font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
