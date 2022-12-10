#include "Projectile.h"
#include "debugmalloc.h"

Bullet *UpdateProjectile(SDL_Renderer *renderer, Bullet *first)
{
    Bullet *prev = NULL;
    Bullet *current = first;
    while (current != NULL)
    {
        v2 Loc = current->loc;
        v2 Angle = current->angle;

        OutOfBounds(&current->loc, 5);
        //Update position
        current->loc = Sum(current->loc, MultiplyFloat(current->angle, -current->speed));

        //Rendering
        SDL_RenderDrawLine(renderer ,Loc.x, Loc.y, Loc.x + Angle.x * 10, Loc.y + Angle.y * 10);

        //Check lifetime
        if (current->life_time > 0)
        {
            current->life_time -= 0.1f;
            prev = current;
            current = current->next;
        }
        else if (prev == NULL)
        {
            if (current->next == NULL)
            {
                free(current);
                return NULL;
            }
            Bullet *next = current->next;
            free(current);
            current = next;
        }
        else
        {
            Bullet *next = current->next;
            free(current);
            current = next;
            prev->next = current;
        }
    }
    return first;
}

Bullet *Shoot(Player *p, Bullet *first)
{
    Bullet *new = (Bullet*) malloc(sizeof(Bullet));

    new->loc = p->loc;
    new->angle = p->angle;
    new->speed = PROJECTILE_SPEED;
    new->life_time = 100;
    new->next = first;

    return new;
}

void FreeProjectile(Bullet *prev, Bullet *current)
{
    if (current == NULL)
    {
        return;
    }
    else if (prev == NULL)
    {
        Bullet *NewFirst = current->next;
        free(current);
        prev = NewFirst;
    }
    else
    {
        prev->next = current->next;
        free(current);
    }
}

Bullet *FreeAllProjectiles(Bullet *first)
{
    while (first != NULL)
    {
        Bullet *prev = first;
        first = first->next;
        free(prev);
    }
    return NULL;
}



