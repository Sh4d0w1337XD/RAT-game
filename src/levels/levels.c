#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_video.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "levels.h"

SDL_Texture* create_texture(SDL_Renderer *renderer, const char *path) {
    SDL_Surface *surf = IMG_Load(path);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    SDL_DestroySurface(surf);

    return texture;
}

void level0(SDL_Renderer *renderer, uint8_t *rat_trap_c, uint8_t *cats_c, Sprite *cheese) {
    cheese->texture.sdl_texture = create_texture(renderer, "./textures/cheese.png");
    cheese->rect = (SDL_FRect) {300, 200, 10 * 5, 8 * 5};

    *cats_c     = 0;
    *rat_trap_c = 0;
}

void level1(SDL_Renderer *renderer, Sprite *rat_trap, uint8_t *rat_trap_c, Sprite *cheese) {

    *rat_trap_c = 1;
    rat_trap[0].texture.sdl_texture = create_texture(renderer, "./textures/rat_trap.png");
    rat_trap[0].rect = (SDL_FRect) { 200, 200, 26 * 3, 30 * 3 };

    cheese->rect.x = 500;
    cheese->rect.y = 430;
}

void level2(SDL_Renderer *renderer, Sprite *rat_trap, uint8_t *rat_trap_c, Sprite *cheese) {
    
    *rat_trap_c = 2;
    rat_trap[1].texture.sdl_texture = create_texture(renderer, "./textures/rat_trap.png");
    rat_trap[0].rect = (SDL_FRect) { 650, 100, 26 * 3, 30 * 3 };
    rat_trap[1].rect = (SDL_FRect) { 700, 150, 26 * 3, 30 * 3 };

    cheese->rect.x = 750;
    cheese->rect.y = 100;
}

void level3(SDL_Renderer *renderer, Sprite *rat_trap, uint8_t *rat_trap_c, Sprite *cheese) {

    *rat_trap_c = 0;

    cheese->rect.x = 400;
    cheese->rect.y = 0;
}

void level4(SDL_Renderer *renderer, Sprite *rat_trap, uint8_t *rat_trap_c, Sprite *cheese, Cat *cats, uint8_t *cats_c) {

    cheese->rect = (SDL_FRect) {400, 300, 10 * 5, 8 * 5};

    *cats_c = 1;
    cats[0].texture.sdl_texture = create_texture(renderer, "./textures/cat.png");
    cats[0].rect = (SDL_FRect) { 400, 600, 32 * 4, 23 * 4 };
    cats[0].start_pos = (v2) { 400.0f, 600.0f };
    cats[0].targt_pos = (v2) { 400.0f, 100.0f };
    cats[0].going_target = true;
}

void level5(SDL_Renderer *renderer, Sprite *rat_trap, uint8_t *rat_trap_c, Sprite *cheese, Cat *cats, uint8_t *cats_c) {

    cheese->rect = (SDL_FRect) {450, 400, 10 * 5, 8 * 5};

    *cats_c = 2;
    
    cats[0].texture.sdl_texture = create_texture(renderer, "./textures/cat.png");
    cats[0].rect = (SDL_FRect) { 300, 400, 32 * 4, 23 * 4 };
    cats[0].start_pos = (v2) { 300.0f, 400.0f };
    cats[0].targt_pos = (v2) { 300.0f, 50.0f };
    cats[0].going_target = true;

    cats[1].texture.sdl_texture = create_texture(renderer, "./textures/cat.png");
    cats[1].rect = (SDL_FRect) { 500, 50, 32 * 4, 23 * 4 };
    cats[1].start_pos = (v2) { 400.0f, 50.0f };
    cats[1].targt_pos = (v2) { 400.0f, 400.0f };
    cats[1].going_target = true;
}

void level6(SDL_Renderer *renderer, Sprite *rat_trap, uint8_t *rat_trap_c, Sprite *cheese, Cat *cats, uint8_t *cats_c) {


    cheese->rect.x = 400;
    cheese->rect.y = 250;

    *cats_c     = 3;

    cats[0].texture.sdl_texture = create_texture(renderer, "./textures/cat.png");
    cats[0].rect = (SDL_FRect) { 200, 50, 32 * 4, 23 * 4 };
    cats[0].start_pos = (v2) { 200.0f, 50.0f };
    cats[0].targt_pos = (v2) { 200.0f, 400.0f };
    cats[0].going_target = true;

    cats[1].texture.sdl_texture = create_texture(renderer, "./textures/cat.png");
    cats[1].rect = (SDL_FRect) { 400, 400, 32 * 4, 23 * 4 };
    cats[1].start_pos = (v2) { 400.0f, 400.0f };
    cats[1].targt_pos = (v2) { 400.0f, 50.0f };
    cats[1].going_target = true;

    cats[2].texture.sdl_texture = create_texture(renderer, "./textures/cat.png");
    cats[2].rect = (SDL_FRect) { 600, 50, 32 * 4, 23 * 4 };
    cats[2].start_pos = (v2) { 600.0f, 50.0f };
    cats[2].targt_pos = (v2) { 600.0f, 400.0f };
    cats[2].going_target = true;

    *rat_trap_c = 3;

    rat_trap[0].texture.sdl_texture = create_texture(renderer, "./textures/rat_trap.png");
    rat_trap[0].rect = (SDL_FRect) { 150, 200, 26 * 3, 30 * 3 };
    
    rat_trap[1].texture.sdl_texture = create_texture(renderer, "./textures/rat_trap.png");
    rat_trap[1].rect = (SDL_FRect) { 300, 400, 26 * 3, 30 * 3 };

    rat_trap[2].texture.sdl_texture = create_texture(renderer, "./textures/rat_trap.png");
    rat_trap[2].rect = (SDL_FRect) { 500, 300, 26 * 3, 30 * 3 };
}

void level7(SDL_Renderer *renderer, Sprite *rat_trap, uint8_t *rat_trap_c, Sprite *cheese, Cat *cats, uint8_t *cats_c) {


    cheese->rect.x = 470;
    cheese->rect.y = 300;

    *cats_c     = 5;

    cats[0].texture.sdl_texture = create_texture(renderer, "./textures/cat.png");
    cats[0].rect = (SDL_FRect) { 140, 400, 32 * 4, 23 * 4 };
    cats[0].start_pos = (v2) { 140.0f, 400.0f };
    cats[0].targt_pos = (v2) { 140.0f, 50.0f };
    cats[0].going_target = true;

    cats[1].texture.sdl_texture = create_texture(renderer, "./textures/cat.png");
    cats[1].rect = (SDL_FRect) { 280, 50, 32 * 4, 23 * 4 };
    cats[1].start_pos = (v2) { 280.0f, 50.0f };
    cats[1].targt_pos = (v2) { 280.0f, 400.0f };
    cats[1].going_target = true;

    cats[2].texture.sdl_texture = create_texture(renderer, "./textures/cat.png");
    cats[2].rect = (SDL_FRect) { 420, 400, 32 * 4, 23 * 4 };
    cats[2].start_pos = (v2) { 420.0f, 400.0f };
    cats[2].targt_pos = (v2) { 420.0f, 50.0f };
    cats[2].going_target = true;

    cats[3].texture.sdl_texture = create_texture(renderer, "./textures/cat.png");
    cats[3].rect = (SDL_FRect) { 560, 50, 32 * 4, 23 * 4 };
    cats[3].start_pos = (v2) { 560.0f, 50.0f };
    cats[3].targt_pos = (v2) { 560.0f, 400.0f };
    cats[3].going_target = true;

    cats[4].texture.sdl_texture = create_texture(renderer, "./textures/cat.png");
    cats[4].rect = (SDL_FRect) { 700, 400, 32 * 4, 23 * 4 };
    cats[4].start_pos = (v2) { 700.0f, 400.0f };
    cats[4].targt_pos = (v2) { 700.0f, 50.0f };
    cats[4].going_target = true;

    *rat_trap_c = 5;

    rat_trap[0].texture.sdl_texture = create_texture(renderer, "./textures/rat_trap.png");
    rat_trap[0].rect = (SDL_FRect) { 70, 350, 26 * 3, 30 * 3 };
    
    rat_trap[1].texture.sdl_texture = create_texture(renderer, "./textures/rat_trap.png");
    rat_trap[1].rect = (SDL_FRect) { 210, 200, 26 * 3, 30 * 3 };

    rat_trap[2].texture.sdl_texture = create_texture(renderer, "./textures/rat_trap.png");
    rat_trap[2].rect = (SDL_FRect) { 350, 300, 26 * 3, 30 * 3 };

    rat_trap[3].texture.sdl_texture = create_texture(renderer, "./textures/rat_trap.png");
    rat_trap[3].rect = (SDL_FRect) { 490, 500, 26 * 3, 30 * 3 };

    rat_trap[4].texture.sdl_texture = create_texture(renderer, "./textures/rat_trap.png");
    rat_trap[4].rect = (SDL_FRect) { 630, 50, 26 * 3, 30 * 3 };
}