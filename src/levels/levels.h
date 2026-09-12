#ifndef LEVELS_H
#define LEVELS_H

typedef struct {
    SDL_Texture *sdl_texture;
    int w;
    int h;
} Texture;

typedef struct {
    SDL_FRect rect;
    Texture texture;
} Sprite;

typedef struct {
    float x;
    float y;
} v2;

typedef struct {
    SDL_FRect rect;
    Texture texture;
    v2 start_pos;
    v2 targt_pos;
    bool going_target;
} Cat;


SDL_Texture *create_texture(SDL_Renderer *renderer, const char *path);
void level0(SDL_Renderer *renderer, uint8_t *rat_trap_c, uint8_t *cats_c, Sprite *cheese);
void level1(SDL_Renderer *renderer, Sprite *rat_trap, uint8_t *rat_trap_c, Sprite *cheese);
void level2(SDL_Renderer *renderer, Sprite *rat_trap, uint8_t *rat_trap_c, Sprite *cheese);
void level3(SDL_Renderer *renderer, Sprite *rat_trap, uint8_t *rat_trap_c, Sprite *cheese);
void level4(SDL_Renderer *renderer, Sprite *rat_trap, uint8_t *rat_trap_c, Sprite *cheese, Cat *cats, uint8_t *cats_c);
void level5(SDL_Renderer *renderer, Sprite *rat_trap, uint8_t *rat_trap_c, Sprite *cheese, Cat *cats, uint8_t *cats_c);
void level6(SDL_Renderer *renderer, Sprite *rat_trap, uint8_t *rat_trap_c, Sprite *cheese, Cat *cats, uint8_t *cats_c);
void level7(SDL_Renderer *renderer, Sprite *rat_trap, uint8_t *rat_trap_c, Sprite *cheese, Cat *cats, uint8_t *cats_c);

#endif