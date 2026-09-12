#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_video.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "levels/levels.h"

#define WINDOW_WIDTH 800
#define WIDNOW_HEIGHT 500

typedef struct {
    SDL_FRect rect;
    SDL_Color bg;
    SDL_Color text;
    const char *label;
    SDL_Texture *textTexture;
    SDL_FRect textRect;
} Button;

typedef struct{
    bool top;
    bool right;
    bool bottom;
    bool left;
} Movement;

// DEMO
struct Position {
    float x;
    float y;
};

struct Transform {
    struct Position position;
    float rotation;
};

struct Object {
    struct Transform transform;
    Sprite sprite;
    const char *tag;
};
// END DEMO

Button create_button( SDL_Renderer *renderer, TTF_Font *font, const char *label, float x, float y, float w, float h )
{
    Button b;

    b.rect = (SDL_FRect){x, y, w, h};
    b.bg   = (SDL_Color){70, 70, 70, 255};
    b.text = (SDL_Color){255, 255, 255, 255};
    b.label = label;

    SDL_Surface *surf = TTF_RenderText_Blended(font, label, 0, b.text);

    b.textTexture = SDL_CreateTextureFromSurface(renderer, surf);

    b.textRect.w = surf->w;
    b.textRect.h = surf->h;

    // center text
    b.textRect.x = x + (w - b.textRect.w) / 2.0f;
    b.textRect.y = y + (h - b.textRect.h) / 2.0f;

    SDL_DestroySurface(surf);

    return b;
}

void draw_button(SDL_Renderer *renderer, Button *b) 
{
    SDL_SetRenderDrawColor(renderer, b->bg.r, b->bg.g, b->bg.b, 255);
    SDL_RenderFillRect(renderer, &b->rect);

    SDL_RenderTexture(renderer, b->textTexture, NULL, &b->textRect);
}

void scale_buttons_with_window_size(Button *btn_arr[], float w, float h) {

    for (int i = 0; i < 5; i++) {
        if (i < 3)
        {
            btn_arr[i]->rect = (SDL_FRect){ w/2 - w/8, (i+1)*h/4 - h/10, w/4, h/5 };
        }
        else if (i == 3)
        {
            btn_arr[i]->rect.w = w/10;
            btn_arr[i]->rect.h = h/10;
            btn_arr[i]->rect.x = w/4 - btn_arr[i]->rect.w * 0.5f;
            btn_arr[i]->rect.y = h/4 - btn_arr[i]->rect.h * 0.5f;
        }
        else 
        {
            btn_arr[i]->rect.w = w/2;
            btn_arr[i]->rect.h = h/10;
            btn_arr[i]->rect.x = w/2 - btn_arr[i]->rect.w * 0.5f;
            btn_arr[i]->rect.y = h/2 - btn_arr[i]->rect.h * 0.5f;
        }
        btn_arr[i]->textRect.x = btn_arr[i]->rect.x + (btn_arr[i]->rect.w - btn_arr[i]->textRect.w)/2.0f;
        btn_arr[i]->textRect.y = btn_arr[i]->rect.y + (btn_arr[i]->rect.h - btn_arr[i]->textRect.h)/2.0f;
    }
}

void update(SDL_Renderer *renderer);

bool check_collision(SDL_FRect obj1, SDL_FRect obj2, Movement *mov, int offset) {

    // TOP-RIGHT
    if (obj1.x + obj1.w - offset >= obj2.x && obj1.x + obj1.w + offset <= obj2.x + obj2.w &&
            obj1.y + obj1.h - offset >= obj2.y && obj1.y + obj1.h + offset <= obj2.y + obj2.h) {

        if ((obj1.x + obj1.w - obj2.x) > (obj1.y + obj1.h - obj2.y)) {
            mov->bottom = false;
            return true;
        }
        else {
            mov->right = false;
            return true;
        }
    }
    // TOP-LEFT
    else if (obj1.x + offset <= obj2.x + obj2.w && obj1.x - offset >= obj2.x &&
            obj1.y + obj1.h - offset >= obj2.y && obj1.y + obj1.h + offset <= obj2.y + obj2.h) {
        
        if ((obj2.x + obj2.w - obj1.x ) > (obj1.y + obj1.h - obj2.y)) {
            mov->bottom = false;
            return true;
        }
        else {
            mov->left = false;
            return true;
        }
    }
    // BOTTOM-RIGHT
    else if (obj1.x + obj1.w - offset >= obj2.x && obj1.x + obj1.w + offset <= obj2.x + obj2.w &&
            obj1.y + offset <= obj2.y + obj2.h && obj1.y - offset >= obj2.y) {
        
        if ((obj1.x + obj1.w - obj2.x) > (obj2.y + obj2.h - obj1.y)) {
            mov->top = false;
            return true;
        }
        else {
            mov->right = false;
            return true;
        }
    }
    // BOTTOM-LEFT
    else if (obj1.x + offset <= obj2.x + obj2.w && obj1.x - offset >= obj2.x &&
            obj1.y + offset <= obj2.y + obj2.h && obj1.y - offset >= obj2.y) {
        
        if ((obj2.x + obj2.w - obj1.x) > (obj2.y + obj2.h - obj1.y)) {
            mov->top = false;
            return true;
        }
        else {
            mov->left = false;
            return true;
        }
    }

    // TOP & BOT (obj1 LARGER than obj2)
    else if (obj1.x <= obj2.x && obj1.x + obj1.w >= obj2.x + obj2.w) 
    {
        if (obj1.y + obj1.h - offset >= obj2.y && obj1.y < obj2.y) {
            mov->bottom = false;
            return true;
        }
        else if ((obj1.y + offset <= obj2.y + obj2.h) && (obj1.y + obj1.h > obj2.y + obj2.h)) {
            mov->top = false;
            return true;
        }
    }

    // RIGHT & LEFT (obj1 LARGER than obj2)
    else if ((obj1.y <= obj2.y) && (obj1.y + obj1.h >= obj2.y + obj2.h)) 
    {
        if (obj1.x + obj1.w - offset >= obj2.x && obj1.x < obj2.x) {
            mov->right = false;
            return true;
        }
        else if ((obj1.x + offset <= obj2.x + obj2.w) && (obj1.x + obj1.w > obj2.x + obj2.w)) {
            mov->left = false;
            return true;
        }
    }
    return false;
}

int main(int agrc, char **agrv ) {

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window  *main_window = SDL_CreateWindow("GGE", WINDOW_WIDTH, WIDNOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(main_window, NULL);

    TTF_Init();
    TTF_Font *font = TTF_OpenFont("./Fonts/segoeui.ttf", 24);	

    Button PlayBtn = create_button(renderer, font, "PLAY", WINDOW_WIDTH/2 - WINDOW_WIDTH/8, WIDNOW_HEIGHT / 4 - WIDNOW_HEIGHT/10, WINDOW_WIDTH/4, WIDNOW_HEIGHT/5);
    Button OptionsBtn = create_button(renderer, font, "OPTIONS", WINDOW_WIDTH/2 - WINDOW_WIDTH/8, WIDNOW_HEIGHT / 2 - WIDNOW_HEIGHT/10, WINDOW_WIDTH/4, WIDNOW_HEIGHT/5);
    Button ExitBtn = create_button(renderer, font, "EXIT", WINDOW_WIDTH/2 - WINDOW_WIDTH/8, 3* WIDNOW_HEIGHT / 4 - WIDNOW_HEIGHT/10, WINDOW_WIDTH/4, WIDNOW_HEIGHT/5);

    Button go_back_btn = create_button(renderer, font, "BACK", WINDOW_WIDTH/4 - WINDOW_WIDTH/20, WIDNOW_HEIGHT/4 - WIDNOW_HEIGHT/20, WINDOW_WIDTH/10, WIDNOW_HEIGHT/10);
    Button fullscreen_btn = create_button(renderer, font, "TURN FULLSCREEN ON/OFF", WINDOW_WIDTH/2 - WINDOW_WIDTH/4, WIDNOW_HEIGHT/2 - WIDNOW_HEIGHT/20, WINDOW_WIDTH/2, WIDNOW_HEIGHT/10);

    Button *btn_arr[] = { &PlayBtn, &OptionsBtn, &ExitBtn, &go_back_btn, &fullscreen_btn };

    Movement movement = { true, true, true, true };

    SDL_FRect camera = { 0, 0, WINDOW_WIDTH, WIDNOW_HEIGHT};

    bool is_menu = true;
    bool is_options = false;
    uint8_t level = 0;

    // RAT
    Sprite rat;
    rat.texture.sdl_texture = create_texture(renderer, "./textures/rat.png");
    rat.rect = (SDL_FRect) { 0, 100, 15 * 6, 10 * 6 };
    bool looking_right = true;
    float player_speed = 500.0f;

    // CAT
    Cat cats[32];
    uint8_t cats_c = 0;
    float cat_speed = 400.0f;

    // CHEESE
    Sprite cheese;
    cheese.texture.sdl_texture = create_texture(renderer, "./textures/cheese.png");
    cheese.rect = (SDL_FRect) {300, 200, 10 * 5, 8 * 5};
    bool cheese_eaten = false;

    // RAT TRAP
    Sprite rat_trap[32];
    uint8_t rat_trap_c = 0;

    // GRASS
    Texture grass;
    grass.sdl_texture = create_texture(renderer, "./textures/2D-grass-64text.png");
    grass.w = 64;
    grass.h = 64;

    // FENCE
    Texture fence;
    fence.sdl_texture = create_texture(renderer, "./textures/fence.png");
    fence.w = 8 * 4;
    fence.h = 16 * 4;
    SDL_SetTextureBlendMode(fence.sdl_texture, SDL_BLENDMODE_BLEND);
    float dig_pos = 1000;
    int hole = -100;
    bool behind_fence = false;

    // FENCE WITH HOLE
    Texture fence_with_hole;
    fence_with_hole.sdl_texture = create_texture(renderer, "./textures/fence_with_hole.png");
    SDL_SetTextureBlendMode(fence_with_hole.sdl_texture, SDL_BLENDMODE_BLEND);

    int w = WINDOW_WIDTH;
    int h = WIDNOW_HEIGHT;

    Uint64 start_time = SDL_GetTicks();
    bool running = true;
    while (running) {

        Uint64 current_time = SDL_GetTicks();
        float delta_time = (current_time - start_time) / 1000.0f;
        start_time = current_time;

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) 
            {
                running = false;
                break;
            }
            else if (event.type == SDL_EVENT_KEY_DOWN)
            {
                if (event.key.key == SDLK_F) 
                {
                    if (SDL_GetWindowFlags(main_window) & SDL_WINDOW_FULLSCREEN) 
                    {
                        int w_before, h_before;
                        SDL_GetWindowSize(main_window, &w_before, &h_before);
                        SDL_SetWindowFullscreen(main_window, 0);
                        SDL_GetWindowSize(main_window, &w, &h);
                        
                        float scale_rate_x = (float)w/w_before;
                        float scale_rate_y = (float)h/h_before;

                        scale_buttons_with_window_size(btn_arr, w, h);
                    }
                    else 
                    {
                        int w_before, h_before;
                        SDL_GetWindowSize(main_window, &w_before, &h_before);
                        SDL_SetWindowFullscreen(main_window, true);
                        SDL_GetWindowSize(main_window, &w, &h);

                        float scale_rate_x = (float)w/w_before;
                        float scale_rate_y = (float)h/h_before;

                        scale_buttons_with_window_size(btn_arr, w, h);
                    }
                }
                else if (event.key.key == SDLK_P)
                    is_menu = true;
            }
            else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                if (is_menu)
                {
                    if (event.button.button == 1) 
                    {
                        float clickX = event.button.x;
                        float clickY = event.button.y;

                        if (clickX >= PlayBtn.rect.x && clickX <= PlayBtn.rect.x + PlayBtn.rect.w && clickY >= PlayBtn.rect.y && clickY <= PlayBtn.rect.y + PlayBtn.rect.h) {
                            is_menu = false;
                        }
                        
                        else if (clickX >= OptionsBtn.rect.x && clickX <= OptionsBtn.rect.x + OptionsBtn.rect.w && clickY >= OptionsBtn.rect.y && 
                                clickY <= OptionsBtn.rect.y + OptionsBtn.rect.h)
                        {
                            is_menu = false;
                            is_options = true;
                        }

                        else if (clickX >= ExitBtn.rect.x && clickX <= ExitBtn.rect.x + ExitBtn.rect.w && clickY >= ExitBtn.rect.y && clickY <= ExitBtn.rect.y + ExitBtn.rect.h)
                            running = false;
                    }
                }
                else if (is_options)
                {
                     if (event.button.button == 1) 
                    {
                        float clickX = event.button.x;
                        float clickY = event.button.y;

                        if (clickX >= go_back_btn.rect.x  && clickX <= go_back_btn.rect.x + go_back_btn.rect.w && clickY >= go_back_btn.rect.y && clickY <= go_back_btn.rect.y +
                            go_back_btn.rect.h) {
                            is_options = false;
                            is_menu = true;
                        }

                        else if (clickX >= fullscreen_btn.rect.x  && clickX <= fullscreen_btn.rect.x + fullscreen_btn.rect.w && clickY >= fullscreen_btn.rect.y && 
                                clickY <= fullscreen_btn.rect.y + fullscreen_btn.rect.h) {
                            if (SDL_GetWindowFlags(main_window) & SDL_WINDOW_FULLSCREEN)
                            {
                                SDL_SetWindowFullscreen(main_window, 0);
                                SDL_GetWindowSize(main_window, &w, &h);
                                scale_buttons_with_window_size(btn_arr, w, h);
                            }
                            else
                            {
                                SDL_SetWindowFullscreen(main_window, true);
                                SDL_GetWindowSize(main_window, &w, &h);
                                scale_buttons_with_window_size(btn_arr, w, h);
                            }
                        }
                    }
                }
            }
        }

        if (is_menu)
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            draw_button(renderer, &PlayBtn);
            draw_button(renderer, &OptionsBtn);
            draw_button(renderer, &ExitBtn);

            SDL_RenderPresent(renderer);
            continue;
        }
        else if (is_options)
        {

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            draw_button(renderer, &go_back_btn);
            draw_button(renderer, &fullscreen_btn);
            
            SDL_RenderPresent(renderer);
            continue;
        }

        // COLLISION DETECTION
        movement.top = true;
        movement.left = true;
        movement.bottom = true;
        movement.right = true;

        if (!cheese_eaten && check_collision(rat.rect, cheese.rect, &movement, 0))
            cheese_eaten = true;

        for (int i = 0; i < rat_trap_c; i++) {
            if (check_collision(rat.rect, rat_trap[i].rect, &movement, 20)) {
                level0(renderer, &rat_trap_c, &cats_c, &cheese);
                rat.rect.x = 0;
                rat.rect.y = 100;
                level = 0;
                is_menu = true;
                cheese_eaten = false;
            }
        }

        for (int i = 0; i < cats_c; i++) {
            if (check_collision(rat.rect, cats[i].rect, &movement, 20) && !behind_fence) {
                level0(renderer, &rat_trap_c, &cats_c, &cheese);
                rat.rect.x = 0;
                rat.rect.y = 100;
                level = 0;
                is_menu = true;
                cheese_eaten = false;
            }
        }

        // CAT MOVEMENT
        for (int i = 0; i < cats_c; i++) {

            float start_pos_y = cats[i].start_pos.y;
            float target_pos_y = cats[i].targt_pos.y;

            if (cats[i].going_target) {
                if (target_pos_y > start_pos_y) {
                    cats[i].rect.y += cat_speed * delta_time;
                    if (cats[i].rect.y >= target_pos_y)
                        cats[i].going_target = false;
                }
                else {
                    cats[i].rect.y -= cat_speed * delta_time;
                    if (cats[i].rect.y <= target_pos_y)
                        cats[i].going_target = false;
                }
            }
            else {
                if (target_pos_y > start_pos_y) {
                    cats[i].rect.y -= cat_speed * delta_time;
                    if (cats[i].rect.y <= start_pos_y)
                        cats[i].going_target = true;
                }
                else {
                    cats[i].rect.y += cat_speed * delta_time;
                    if (cats[i].rect.y >= start_pos_y)
                        cats[i].going_target = true;
                }
            }
        }

        // INPUT
        const bool *keys = SDL_GetKeyboardState(NULL);

        float dx = 0.0f;
        float dy = 0.0f;
        
        if (keys[SDL_SCANCODE_W] && movement.top) dy -= 1.0f;
        if (keys[SDL_SCANCODE_S] && movement.bottom) dy += 1.0f;
        if (keys[SDL_SCANCODE_A] && movement.left) {
            dx -= 1.0f;
            looking_right = false;
        }
        if (keys[SDL_SCANCODE_D] && movement.right) {
            dx += 1.0f;
            looking_right = true    ;
        }
        if (dx != 0.0f || dy != 0.0f) {
            float len = SDL_sqrtf(dx*dx + dy*dy);
            dx /= len;
            dy /= len;

            rat.rect.x += dx * player_speed * delta_time;
            rat.rect.y += dy * player_speed * delta_time;
        }

        if (rat.rect.y <= fence.h + 10) {
            if (keys[SDL_SCANCODE_E]) {
                dig_pos = rat.rect.x;
            }
        }

        if (behind_fence) {
            SDL_SetTextureAlphaMod(fence.sdl_texture, 128);
            SDL_SetTextureAlphaMod(fence_with_hole.sdl_texture, 128);

            if (rat.rect.x < 0) rat.rect.x = 0;
            else if (rat.rect.x + rat.rect.w > w) rat.rect.x = w - rat.rect.w;
            if ((hole < rat.rect.x + fence.w && hole >= rat.rect.x)) {
                if (rat.rect.y < fence.h) { 
                    behind_fence = true;
                    if (rat.rect.y < 0) rat.rect.y = 0;
                }
                else behind_fence = false;
            }
            else if (rat.rect.y > 0 || rat.rect.y < 0) rat.rect.y = 0;
        }
        else {
            SDL_SetTextureAlphaMod(fence.sdl_texture, 255);
            SDL_SetTextureAlphaMod(fence_with_hole.sdl_texture, 255);

            if (rat.rect.x < 0) rat.rect.x = 0;
            else if ((hole < rat.rect.x + fence.w && hole >= rat.rect.x)) {
                if (rat.rect.y < fence.h) behind_fence = true;
                else {
                    behind_fence = false;
                    if (rat.rect.y + rat.rect.h> h) rat.rect.y = h - rat.rect.h;
                }
            }
            else if (rat.rect.y < fence.h) rat.rect.y = fence.h;
            else if (rat.rect.y + rat.rect.h> h) rat.rect.y = h - rat.rect.h;
            else if (rat.rect.x + rat.rect.w > w && !cheese_eaten) rat.rect.x = w - rat.rect.w;
            else if (rat.rect.x + rat.rect.w > w + 100 && cheese_eaten) {
                rat.rect.x = 0;
                dig_pos = 1000;
                hole = -100;
                level++;
                cheese_eaten = false;
                switch (level) {
                    case 0: level0(renderer, &rat_trap_c, &cats_c ,&cheese);
                            break;
                    case 1: level1(renderer, rat_trap, &rat_trap_c, &cheese);
                            break;
                    case 2: level2(renderer, rat_trap, &rat_trap_c, &cheese);
                            break;
                    case 3: level3(renderer, rat_trap, &rat_trap_c, &cheese);
                            break;
                    case 4: level4(renderer, rat_trap, &rat_trap_c, &cheese, cats, &cats_c);
                            break;
                    case 5: level5(renderer, rat_trap, &rat_trap_c, &cheese, cats, &cats_c);
                            break;
                    case 6: level6(renderer, rat_trap, &rat_trap_c, &cheese, cats, &cats_c);
                            break;
                    case 7: level7(renderer, rat_trap, &rat_trap_c, &cheese, cats, &cats_c);
                            break;
                    default: level = 0;
                            level0(renderer, &rat_trap_c, &cats_c, &cheese);
                }
            }
        }

        // RENDERING
        for (int i = 0; i < h ; i += grass.h) {
            for (int j = 0; j < w; j += grass.w) {
                SDL_FRect pos = (SDL_FRect) { j, i, grass.w, grass.h };
                SDL_RenderTexture(renderer, grass.sdl_texture, NULL, &pos);
            }
        }

        if (looking_right)
            SDL_RenderTexture(renderer, rat.texture.sdl_texture, NULL, &rat.rect);
        else
            SDL_RenderTextureRotated(renderer, rat.texture.sdl_texture, NULL, &rat.rect, 0.0f, NULL, SDL_FLIP_HORIZONTAL);

        for (int i = 0; i < rat_trap_c; i++) {
            SDL_RenderTexture(renderer, rat_trap[i].texture.sdl_texture, NULL, &rat_trap[i].rect);
        }

        if (!cheese_eaten)
            SDL_RenderTexture(renderer, cheese.texture.sdl_texture, NULL, &cheese.rect);

        for (int i = 0; i < cats_c; i++) {
            SDL_RenderTexture(renderer, cats[i].texture.sdl_texture, NULL, &cats[i].rect);
        }

        for (int i = 0; i < h; i += fence.h) {
            for (int j = 0; j < w; j += fence.w) {
                if (i == 0 || i + fence.h >= h) {
                    SDL_FRect pos = (SDL_FRect) { j, i, fence.w, fence.h };
                    if (i == 0 && (j < dig_pos + fence.w && j >= dig_pos)) {
                        hole = j;
                        SDL_RenderTexture(renderer, fence_with_hole.sdl_texture, NULL, &pos);
                        continue;
                    }
                    SDL_RenderTexture(renderer, fence.sdl_texture, NULL, &pos);
                }
            }
        }

        
        SDL_RenderPresent(renderer);
    }

    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(main_window);
    SDL_Quit();

    return 0;
}