#ifndef ASTEROIDS_PLAYER_H
#define ASTEROIDS_PLAYER_H

#include "Vectors.h"
#include "SDL.h"
#include "UI.h"
#include <stdbool.h>
#include <math.h>

typedef struct Player
{
    v2 loc;
    v2 angle;
    v2 speed;
    float max_speed;
    float acceleration;
    float rotation_speed;
    float fire_rate;

    int immortality;
    int lives;

    char playerName[20];
    int SCORE;

    bool UP_KEY;
    bool LEFT_KEY;
    bool RIGHT_KEY;
    bool SHOOT;
}Player;

void PlayerMovement(Player *p);

void RenderPlayer(SDL_Renderer *renderer, Player *p);

#endif //ASTEROIDS_PLAYER_H
