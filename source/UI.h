//
// Created by Németh on 2022. 11. 20..
//

#ifndef ASTEROIDS_UI_H
#define ASTEROIDS_UI_H

#include "Vectors.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include "stdio.h"
#include "stdbool.h"


void RenderHealth(SDL_Renderer *renderer, int lives);

void RenderText(SDL_Renderer *renderer, SDL_Surface *surface, SDL_Texture *texture, TTF_Font *font, SDL_Rect *rect, char *Text);

void RenderScore(SDL_Renderer *renderer, SDL_Surface *surface, SDL_Texture *texture, TTF_Font *font, SDL_Rect *rect, int Score);

#endif //ASTEROIDS_UI_H
