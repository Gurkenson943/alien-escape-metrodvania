#include "boss.h"
#include <stdlib.h>
#include <math.h>

Boss* boss_create(BossType type, float x, float y) {
    Boss *boss = (Boss*)malloc(sizeof(Boss));
    if (!boss) return NULL;

    boss->x = x;
    boss->y = y;
    boss->vx = 0;
    boss->vy = 0;
    boss->width = 50;
    boss->height = 60;
    boss->type = type;
    boss->phase = 1;
    boss->attack_cooldown = 0;
    boss->move_timer = 0;
    boss->attacking = 0;
    boss->damage_cooldown = 0;

    switch(type) {
        case BOSS_WARDEN:
            boss->health = 80;
            boss->max_health = 80;
            break;
        case BOSS_DESTRUKTOR:
            boss->health = 100;
            boss->max_health = 100;
            break;
        case BOSS_MUTATION:
            boss->health = 120;
            boss->max_health = 120;
            break;
        case BOSS_ALIEN_KING:
            boss->health = 200;
            boss->max_health = 200;
            break;
    }

    return boss;
}

void boss_update(Boss *boss, float player_x, float player_y, float dt) {
    if (boss->health <= 0) return;

    if (boss->health < boss->max_health / 2) {
        boss->phase = 2;
    }

    if (boss->attack_cooldown > 0) {
        boss->attack_cooldown -= dt;
    }
    if (boss->move_timer > 0) {
        boss->move_timer -= dt;
    }
    if (boss->damage_cooldown > 0) {
        boss->damage_cooldown -= dt;
    }

    switch(boss->type) {
        case BOSS_WARDEN:
            if (boss->move_timer <= 0) {
                if (rand() % 2) {
                    boss->vy = -400;
                }
                boss->move_timer = 1.5f;
            }
            if (boss->attack_cooldown <= 0) {
                boss_attack(boss, player_x, player_y);
                boss->attack_cooldown = 2.0f;
            }
            break;

        case BOSS_DESTRUKTOR:
            if (player_x > boss->x) {
                boss->vx = 200;
            } else {
                boss->vx = -200;
            }
            if (boss->attack_cooldown <= 0) {
                boss_attack(boss, player_x, player_y);
                boss->attack_cooldown = 1.5f;
            }
            break;

        case BOSS_MUTATION:
            if (boss->move_timer <= 0) {
                boss->vx = (rand() % 400) - 200;
                boss->vy = (rand() % 400) - 200;
                boss->move_timer = 1.0f;
            }
            if (boss->attack_cooldown <= 0) {
                boss_attack(boss, player_x, player_y);
                boss->attack_cooldown = 2.0f * (1.0f - boss->health / boss->max_health);
            }
            break;

        case BOSS_ALIEN_KING:
            if (boss->phase == 1) {
                if (player_x > boss->x) {
                    boss->vx = 150;
                } else {
                    boss->vx = -150;
                }
            } else {
                if (player_x > boss->x) {
                    boss->vx = 300;
                } else {
                    boss->vx = -300;
                }
            }

            if (boss->attack_cooldown <= 0) {
                boss_attack(boss, player_x, player_y);
                boss->attack_cooldown = boss->phase == 1 ? 2.0f : 1.0f;
            }
            break;
    }

    boss->vy += 600 * dt;
    if (boss->vy > 600) boss->vy = 600;

    boss->x += boss->vx * dt;
    boss->y += boss->vy * dt;

    if (boss->x < 0) {
        boss->x = 0;
        boss->vx *= -1;
    }
    if (boss->x + boss->width > 3000) {
        boss->x = 3000 - boss->width;
        boss->vx *= -1;
    }

    if (boss->y + boss->height >= 1200) {
        boss->y = 1200 - boss->height;
        boss->vy = 0;
    }
}

void boss_render(SDL_Renderer *renderer, Boss *boss, float camera_x, float camera_y) {
    SDL_Rect rect = {
        (int)(boss->x - camera_x),
        (int)(boss->y - camera_y),
        (int)boss->width,
        (int)boss->height
    };

    switch(boss->type) {
        case BOSS_WARDEN:
            SDL_SetRenderDrawColor(renderer, 200, 100, 100, 255);
            break;
        case BOSS_DESTRUKTOR:
            SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
            break;
        case BOSS_MUTATION:
            SDL_SetRenderDrawColor(renderer, 200, 100, 200, 255);
            break;
        case BOSS_ALIEN_KING:
            SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
            break;
    }

    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);

    int bar_width = (int)boss->width;
    int bar_height = 5;
    int bar_x = (int)(boss->x - camera_x);
    int bar_y = (int)(boss->y - camera_y) - 10;

    SDL_Rect bg_bar = {bar_x, bar_y, bar_width, bar_height};
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &bg_bar);

    int health_width = (int)(bar_width * boss->health / boss->max_health);
    SDL_Rect health_bar = {bar_x, bar_y, health_width, bar_height};
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &health_bar);
}

void boss_attack(Boss *boss, float player_x, float player_y) {
    boss->attacking = 1;
}

void boss_take_damage(Boss *boss, int damage) {
    boss->health -= damage;
    if (boss->health < 0) boss->health = 0;
    boss->damage_cooldown = 0.5f;
}

int boss_is_defeated(Boss *boss) {
    return boss->health <= 0;
}

void boss_cleanup(Boss *boss) {
    free(boss);
}
