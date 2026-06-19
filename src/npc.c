#include "npc.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

NPC* npc_create(float x, float y, const char *name, int type) {
    NPC *npc = (NPC*)malloc(sizeof(NPC));
    if (!npc) return NULL;

    npc->x = x;
    npc->y = y;
    npc->width = 30;
    npc->height = 40;
    strncpy(npc->name, name, 49);
    npc->is_alive = 1;
    npc->is_freed = 0;
    npc->quest = QUEST_NONE;
    npc->quest_progress = 0;
    npc->quest_reward = 0;
    npc->quest_completed = 0;

    switch(type) {
        case 0:
            strcpy(npc->dialogue, "Hilf mir! Sammle 3 Energy Crystals!");
            npc->quest = QUEST_COLLECT;
            npc->quest_reward = 10;
            break;
        case 1:
            strcpy(npc->dialogue, "Ich brauche Baupläne! Suche sie in der Fabrik!");
            npc->quest = QUEST_FIND_BLUEPRINT;
            npc->quest_reward = 20;
            break;
        case 2:
            strcpy(npc->dialogue, "Besiege die Bosse! Zeige mir deine Kraft!");
            npc->quest = QUEST_DEFEAT_BOSS;
            npc->quest_reward = 30;
            break;
        case 3:
            strcpy(npc->dialogue, "Ich kenne das Schiff. Lass mich dir helfen.");
            npc->quest = QUEST_NONE;
            break;
    }

    return npc;
}

Prisoner* prisoner_create(float x, float y, const char *name, int type) {
    Prisoner *p = (Prisoner*)malloc(sizeof(Prisoner));
    if (!p) return NULL;

    p->x = x;
    p->y = y;
    p->width = 25;
    p->height = 35;
    strncpy(p->name, name, 49);

    return p;
}

void npc_update(NPC *npc, float dt) {
    if (!npc->is_alive) return;
}

void npc_render(NPC *npc, SDL_Renderer *renderer, float camera_x, float camera_y) {
    if (!npc->is_alive) return;

    SDL_Rect rect = {
        (int)(npc->x - camera_x),
        (int)(npc->y - camera_y),
        (int)npc->width,
        (int)npc->height
    };

    if (npc->quest_completed) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    } else if (npc->quest != QUEST_NONE) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, 100, 200, 255, 255);
    }

    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderDrawRect(renderer, &rect);
}

void npc_interact(NPC *npc, int *player_quest) {
    if (!npc->is_alive) return;

    printf("NPC: %s - %s\n", npc->name, npc->dialogue);

    if (npc->quest != QUEST_NONE && !npc->quest_completed) {
        printf("Quest akzeptiert: %d\n", npc->quest);
        *player_quest = npc->quest;
        npc->quest_completed = 1;
    } else if (npc->quest_completed) {
        printf("Quest abgeschlossen! Du erhaeltst %d Punkte.\n", npc->quest_reward);
    }
}

void npc_complete_quest(NPC *npc) {
    if (npc->quest != QUEST_NONE) {
        npc->quest_completed = 1;
        printf("%s: Danke! Du bist ein Held!\n", npc->name);
    }
}

void npc_cleanup(NPC *npc) {
    free(npc);
}
