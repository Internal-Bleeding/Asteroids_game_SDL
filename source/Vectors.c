#include "Vectors.h"

v2 Sum(v2 a, v2 b)
{
    a.x += b.x;
    a.y += b.y;
    return a;
}

v2 MultiplyV2(v2 a, v2 b)
{
    a.x *= b.x;
    a.y *= b.y;
    return a;
}

v2 NormalV2(v2 a)
{
    float X = a.x;
    a.x = a.y;
    a.y = -X;
    return a;
}

v2 NormalizationV2(v2 a)
{
    v2 UnitVector;
    float X = a.x;
    float Y = a.y;
    float distance = sqrt(X * X + Y * Y);
    UnitVector.x = a.x / distance;
    UnitVector.y = a.y / distance;
    return UnitVector;
}

v2 MultiplyFloat(v2 a, float l)
{
    a.x *= l;
    a.y *= l;
    return a;
}

SDL_Point FromV2(v2 a)
{
    SDL_Point p = {a.x, a.y};
    return p;
}

v2 FromDSL_Point(SDL_Point p)
{
    v2 vector;
    vector.x = p.x;
    vector.y = p.y;
    return vector;
}

void OutOfBounds(v2 *pos, int threshold)
{
    v2 Pos = *pos;
    if(Pos.x < -threshold)
    {
        Pos.x = SCREEN_WIDTH - Pos.x;
    }
    else if (Pos.x > SCREEN_WIDTH + threshold)
    {
        Pos.x = Pos.x - SCREEN_WIDTH;
    }
    if(Pos.y < -threshold)
    {
        Pos.y = SCREEN_HEIGHT - Pos.y;
    }
    else if (Pos.y > SCREEN_HEIGHT + threshold)
    {
        Pos.y = Pos.y - SCREEN_HEIGHT;
    }
    *pos = Pos;
}