#include "collision.h"
#include <math.h>

int check_rect_collision(SDL_Rect a, SDL_Rect b) {
    return SDL_HasIntersection(&a, &b);
}

int check_circle_collision(float x1, float y1, float r1, float x2, float y2, float r2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float distance = sqrtf(dx * dx + dy * dy);
    return distance < (r1 + r2);
}

int check_point_rect(float px, float py, SDL_Rect rect) {
    return px >= rect.x && px <= rect.x + rect.w &&
           py >= rect.y && py <= rect.y + rect.h;
}

SDL_Rect float_rect_to_sdl(float x, float y, float w, float h) {
    SDL_Rect rect;
    rect.x = (int)x;
    rect.y = (int)y;
    rect.w = (int)w;
    rect.h = (int)h;
    return rect;
}
