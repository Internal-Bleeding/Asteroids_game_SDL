#include "Meteors.h"
#include <stdio.h>
#include "debugmalloc.h"

#define METEOR_SPEED 0.3f

void MeteorOutOfBounds(Meteor *m, int threshold)
{
    v2 Pos = m->pos;
    if(Pos.x < -threshold)
    {
        for (int n = 0; n < 12; n++)
        {
            m->points[n].x =  SCREEN_WIDTH -  m->points[n].x;
        }
        Pos.x = SCREEN_WIDTH - Pos.x;
    }
    else if (Pos.x > SCREEN_WIDTH + threshold)
    {
        for (int n = 0; n < 12; n++)
        {
            m->points[n].x = m->points[n].x - SCREEN_WIDTH;
        }
        Pos.x = Pos.x - SCREEN_WIDTH;
    }
    if(Pos.y < -threshold)
    {
        for (int n = 0; n < 12; n++)
        {
            m->points[n].y =  SCREEN_HEIGHT -  m->points[n].y;
        }
        Pos.y = SCREEN_HEIGHT - Pos.y;
    }
    else if (Pos.y > SCREEN_HEIGHT + threshold)
    {
        for (int n = 0; n < 12; n++)
        {
            m->points[n].y = m->points[n].y - SCREEN_HEIGHT;
        }
        Pos.y = Pos.y - SCREEN_HEIGHT;
    }
    m->pos = Pos;
}

void HitDetection(Meteor **first, Bullet **bullet, Player *player)
{
    Bullet *prevB = NULL;
    Bullet *b = *bullet;
    while (b != NULL)
    {
        Meteor *current = *first;
        Meteor *prev = NULL;
        bool Hit = false;
        while (current != NULL)
        {
            float r = current->radius * current->radius;
            float n = (b->loc.x - current->pos.x) * (b->loc.x - current->pos.x) + (b->loc.y - current->pos.y) * (b->loc.y - current->pos.y);
            if (r >= n)
            {
                //Delete Meteor:
                Hit = true;
                player->SCORE += current->score;

                int level = current->level - 1;
                v2 Pos = current->pos;
                if (prev == NULL)
                {
                    *first = current->next;
                    Meteor *temp = current;
                    free(temp);
                    current = *first;
                }
                else
                {
                    prev->next = current->next;
                    Meteor *temp = current;
                    free(temp);
                    current = prev->next;
                }
                v2 test = {0, 10};
                *first = SpawnMeteor(Sum(Pos, NormalV2(test)), level, *first);
                *first = SpawnMeteor(Sum(Pos, test), level, *first);
                //Delete Bullet:
                if (prevB == NULL)
                {
                    *bullet = b->next;
                    Bullet *temp = b;
                    free(temp);
                    b = *bullet;
                }
                else
                {
                    prevB->next = b->next;
                    Bullet *temp = b;
                    free(temp);
                    b = prevB->next;
                }
                break;
            }
            else
            {
                prev = current;
                current = current->next;
            }
        }
        if (!Hit)
        {
            prevB = b;
            b = b->next;
        }
    }
}

void PlayerHitDetection(Player *p, Meteor *meteors)
{
    while (meteors != NULL)
    {
        float r = meteors->radius * meteors->radius + 20;
        float n = (p->loc.x - meteors->pos.x) * (p->loc.x - meteors->pos.x) + (p->loc.y - meteors->pos.y) * (p->loc.y - meteors->pos.y);
        if (r >= n && p->immortality <= 0)
        {
            p->immortality = 1000;
            p->lives--;
        }
        meteors = meteors->next;
    }
    if (p->immortality > 0)
    {
        p->immortality -= 1;
    }
}

void SpreadMeteors(Meteor *meteors)
{
    Meteor *m1 = meteors;

    while (m1 != NULL)
    {
        Meteor *m2 = meteors;
        while (m2 != NULL)
        {
            if(m1 != m2)
            {
                float X = m1->pos.x - m2->pos.x;
                float Y = m1->pos.y - m2->pos.y;
                float radius = m1->radius + m2->radius - 20.0f;
                float distance = sqrt((X * X) + (Y * Y));
                if (distance < radius)
                {
                    v2 newDir = {X, Y};
                    newDir = NormalizationV2(newDir);
                    m2->angle = newDir;
                    m1->angle = MultiplyFloat(newDir, -1);
                }
            }
            m2 = m2->next;


        }
        m1 = m1->next;
    }
}

Meteor *SpawnMeteor(v2 pos, int level, Meteor *meteors)
{
    if (level <= 0)
    {
        return meteors;
    }
    Meteor *m = (Meteor*) malloc(sizeof(Meteor));
    if (m == NULL) {
        return NULL;
    }

    if (pos.y == 0 && pos.x == 0)
    {
        pos.x = rand()%((SCREEN_WIDTH-19)-20) + 20;
        pos.y = rand()%((SCREEN_HEIGHT-19)-20) + 20;

    }
    v2 p[12];
    float distance = 25 * level; //max level is 3
    //Create a circle from points
    for (int n = 0; n < 11; n++)
    {
        int rng = rand()%(((int)distance+1)-10*level) + 10*level;

        p[n].x = pos.x + cosf(n*10*Pi/55) * rng;
        p[n].y = pos.y + sinf(n*10*Pi/55) * rng;
    }
    p[11] = p[0];

    for (int n = 0; n < 12; n++)
    {
        m->points[n] = p[n];
    }
    m->pos = pos;
    v2 angle = {0, 1};

    int rng = rand()%(6);
    float x = cosf(rng) * angle.x - sinf(rng) * angle.y;
    float y = sinf(rng) * angle.x + cosf(rng) * angle.y;
    m->angle.x = x;
    m->angle.y = y;
    switch (level)
    {
        case 3: m->score = 25;
            break;
        case 2: m->score = 100;
            break;
        case 1: m->score = 250;
            break;
        default:
            break;
    }
    m->level = level;
    m->radius = distance;
    m->speed = METEOR_SPEED / (float)level;
    m->next = meteors;
    return m;
}

Meteor *Spawner(Meteor *meteors)
{
    int NumOfMeteors = 0;
    Meteor *temp = meteors;
    while (temp != NULL)
    {
        NumOfMeteors++;
        temp = temp->next;
    }
    if (NumOfMeteors < 6)
    {
        v2 zero = {0, 0};
        for (int n = 0; n < 6 - NumOfMeteors; n++)
        {
            meteors = SpawnMeteor(zero, 3, meteors);
        }
    }
    return meteors;
}

void RenderMeteors(SDL_Renderer *renderer, Meteor *m)
{
    while (m != NULL)
    {
        m->pos = Sum(m->pos, MultiplyFloat(m->angle, -m->speed));
        SDL_Point points[12];
        for (int n = 0; n < 12; n++)
        {
            m->points[n] = Sum(m->points[n], MultiplyFloat(m->angle, -m->speed));
            points[n] = FromV2(m->points[n]);
        }
        MeteorOutOfBounds(m, 60);
        SDL_RenderDrawLines(renderer, points, 12);
        m = m->next;
    }
}

Meteor *FreeAllMeteors(Meteor *first)
{
    while (first != NULL)
    {
        Meteor *prev = first;
        first = first->next;
        free(prev);
    }
    return NULL;
}

