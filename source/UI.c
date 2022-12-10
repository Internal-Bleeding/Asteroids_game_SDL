#include "UI.h"

void RenderHealth(SDL_Renderer *renderer, int lives)
{
    v2 angle = {0, 1};
    for (int n = 0; n < lives; n++)
    {
        v2 pos = {SCREEN_WIDTH * 0.03 + (n * 50), SCREEN_HEIGHT * 0.95f};
        SDL_Point points[5];
        points[0] = FromV2(Sum(pos, MultiplyFloat(angle, 15)));
        points[1] = FromV2(Sum(pos, MultiplyFloat(NormalV2(angle), 10)));
        points[2] = FromV2(Sum(pos, MultiplyFloat(angle, -25)));
        points[3] = FromV2(Sum(pos, MultiplyFloat(NormalV2(angle), -10)));
        points[4] = FromV2(Sum(pos, MultiplyFloat(angle, 15)));
        SDL_RenderDrawLines(renderer, points, 5);
    }

}

void RenderScore(SDL_Renderer *renderer, SDL_Surface *surface, SDL_Texture *texture, TTF_Font *font, SDL_Rect *rect, int Score)
{

    char text[32];
    sprintf(text, "%05d", Score);
    SDL_Color white = {255, 255, 255, 255};
    surface = TTF_RenderText_Solid(font, text, white);
    texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_RenderCopy(renderer, texture, NULL, rect);
}

void RenderText(SDL_Renderer *renderer, SDL_Surface *surface, SDL_Texture *texture, TTF_Font *font, SDL_Rect *rect, char *Text)
{
    SDL_Color white = {255, 255, 255, 255};
    surface = TTF_RenderText_Solid(font, Text, white);
    texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_RenderCopy(renderer, texture, NULL, rect);
}

/*void RenderScoreBoard(SDL_Renderer *renderer, SDL_Surface *surface, SDL_Texture *texture, TTF_Font *font, SDL_Rect *rect, HighScore *scores, int size)
{
    for (int n = 0; n < size; n++)
    {
        char text[32];
        sprintf(text, "%s %05d", scores->playerName ,scores->score);
        RenderText(renderer, surface, texture, font, &rect[n], text);
    }
}*/