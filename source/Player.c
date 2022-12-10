#include "Player.h"

void PlayerMovement(Player *p)
{
    if (p->RIGHT_KEY)
    {
        float x = cosf(p->rotation_speed) * p->angle.x - sinf(p->rotation_speed) * p->angle.y;
        float y = sinf(p->rotation_speed) * p->angle.x + cosf(p->rotation_speed) * p->angle.y;
        p->angle.x = x;
        p->angle.y = y;
        //printf("Angle: %f %f \n",  p->angle.x,  p->angle.y);
    }
    if (p->LEFT_KEY)
    {
        float x = cosf(-p->rotation_speed) * p->angle.x - sinf(-p->rotation_speed) * p->angle.y;
        float y = sinf(-p->rotation_speed) * p->angle.x + cosf(-p->rotation_speed) * p->angle.y;
        p->angle.x = x;
        p->angle.y = y;
    }
    if (p->UP_KEY)
    {
        if (p->speed.x < p->max_speed && p->speed.x > -p->max_speed && p->speed.y < p->max_speed && p->speed.y > -p->max_speed)
        {
            p->speed.x += -p->max_speed * p->angle.x * p->acceleration;
            p->speed.y += -p->max_speed * p->angle.y * p->acceleration;
        }
        else
        {

        }
    }
    else
    {
        if (p->speed.x != 0 || p->speed.y != 0)
        {
            p->speed.x *= 1.0f - p->acceleration;
            p->speed.y *=  1.0f - p->acceleration;
        }
    }

    p->loc = Sum(p->loc, p->speed);
}

void RenderPlayer(SDL_Renderer *renderer, Player *p)
{
    SDL_Point points[5];
    points[0] = FromV2(Sum(p->loc, MultiplyFloat(p->angle, 15)));
    points[1] = FromV2(Sum(p->loc, MultiplyFloat(NormalV2(p->angle), 10)));
    points[2] = FromV2(Sum(p->loc, MultiplyFloat(p->angle, -25)));
    points[3] = FromV2(Sum(p->loc, MultiplyFloat(NormalV2(p->angle), -10)));
    points[4] = FromV2(Sum(p->loc, MultiplyFloat(p->angle, 15)));

    if (p->immortality > 0)
    {
        SDL_SetRenderDrawColor(renderer,255, 0, 0, 255);
    }
    SDL_RenderDrawLines(renderer, points, 5);
    SDL_SetRenderDrawColor(renderer,255, 255, 255, 255);
    RenderHealth(renderer, p->lives);

}