#ifndef ASTEROIDS_METEOR_H
#define ASTEROIDS_METEOR_H

#include "Vectors.h"
#include "Projectile.h"
#include <time.h>
#include <stdbool.h>
#include <math.h>

typedef struct Meteor{
    v2 pos;
    int level;
    v2 angle;
    float radius;
    v2 points[12];
    float speed;
    int score;
    struct Meteor *next;
}Meteor;

void MeteorOutOfBounds(Meteor *m, int threshold);

void PlayerHitDetection(Player *p, Meteor *meteors);

void HitDetection(Meteor **first, Bullet **b, Player *player);

Meteor *SpawnMeteor(v2 pos, int level, Meteor *meteors);

void SpreadMeteors(Meteor *meteors);

void RenderMeteors(SDL_Renderer *renderer, Meteor *m);

Meteor *Spawner(Meteor *meteors);

Meteor *FreeAllMeteors(Meteor *first);

#endif //ASTEROIDS_METEOR_H
