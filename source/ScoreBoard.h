#ifndef ASTEROIDS_SCOREBOARD_H
#define ASTEROIDS_SCOREBOARD_H

#include "Vectors.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include "stdio.h"
#include "stdbool.h"
#include "Player.h"
#include "string.h"

typedef struct HighScore
{
    char playerName[20];
    int score;
    struct HighScore *next;

}HighScore;

void LoadFile(HighScore **Scores);

void UpdateScore(HighScore *first, Player *player);

HighScore *DeleteScoreBoard(HighScore *first);

void RenderScoreBoard(HighScore *first);

void ResetScore(HighScore *first);

void SaveFile(HighScore *Scores);

#endif //ASTEROIDS_SCOREBOARD_H
