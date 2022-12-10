#ifndef ASTEROIDS_PROJECTILE_H
#define ASTEROIDS_PROJECTILE_H

#include "SDL.h"
#include "Player.h"
#include <stdio.h>

#define PROJECTILE_SPEED 0.6f
#define RATE_OF_FIRE 150

typedef struct Bullet{
    v2 loc;
    v2 angle;
    float speed;
    float life_time;
    struct Bullet *next;
}Bullet;

Bullet *UpdateProjectile(SDL_Renderer *renderer, Bullet *first);

Bullet *Shoot(Player *p, Bullet *first);

void FreeProjectile(Bullet *prev, Bullet *current);

Bullet *FreeAllProjectiles(Bullet *first);



#endif //ASTEROIDS_PROJECTILE_H
