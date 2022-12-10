#ifndef ASTEROIDS_VECTORS_H
#define ASTEROIDS_VECTORS_H

#include <SDL2/SDL.h>
#include <math.h>
#define Pi 3.14159265359f
#define SCREEN_HEIGHT  720
#define SCREEN_WIDTH  1280

typedef struct v2{
    float x, y;
}v2;

v2 Sum(v2 a, v2 b);

v2 MultiplyV2(v2 a, v2 b);

v2 NormalizationV2(v2 a);

v2 NormalV2(v2 a);

v2 MultiplyFloat(v2 a, float l);

SDL_Point FromV2(v2 a);

v2 FromDSL_Point(SDL_Point p);

void OutOfBounds(v2 *pos, int threshold);

#endif //ASTEROIDS_VECTORS_H
