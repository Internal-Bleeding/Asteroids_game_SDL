#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <windows.h>

#include "Vectors.h"
#include "Projectile.h"
#include "Player.h"
#include "Meteors.h"
#include "ScoreBoard.h"
#include "UI.h"



void HandleKeyboardStates(Uint8 *state, Player *p)
{
    if(state[SDL_SCANCODE_UP])
        p->UP_KEY = true;
    else {
        p->UP_KEY = false;
    }
    if(state[SDL_SCANCODE_RIGHT])
        p->RIGHT_KEY = true;
    else {
        p->RIGHT_KEY = false;
    }
    if(state[SDL_SCANCODE_LEFT])
        p->LEFT_KEY = true;
    else {
        p->LEFT_KEY = false;
    }
    if(state[SDL_SCANCODE_S] || state[SDL_SCANCODE_SPACE])
    {
        p->SHOOT = true;
    } else {
        p->SHOOT = false;
    }
}

void InsertName(SDL_Renderer *renderer, SDL_Surface *surface, SDL_Texture *Text, TTF_Font *DOS_f, SDL_Rect rect,char *name)
{
    RenderText(renderer, surface, Text, DOS_f, &rect, name);
}

int main(int argc, char *argv[]) {

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("SDL_Error: %s", SDL_GetError());
        exit(1);
    }
    if( TTF_Init() == -1 )
    {
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (window == NULL) {
        SDL_Log("SDL_Error: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        SDL_Log("SDL_Error: %s", SDL_GetError());
        exit(1);
    }
    else
    {
        SDL_GetWindowSurface( window );
    }
    TTF_Font *DOS_f = TTF_OpenFont( "GemunuLibre-VariableFont_wght.ttf", 28 );
    if( DOS_f == NULL )
    {
        SDL_Log( "SDL_ttf Error: %s\n", TTF_GetError() );
        exit(1);
    }

    SDL_Surface *surface;
    SDL_Texture *Text;
    SDL_Texture *Title;
    SDL_Texture *Instructions;
    SDL_Texture *GameOver;
    SDL_Texture *Name;

    SDL_Rect Name_rect =  {SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.5f, 0, 40};
    SDL_Rect Score_rect = {0, 0, 100, 40};
    SDL_Rect Title_rect = {SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT * 0.2f, 300, 120};
    SDL_Rect GameOver_rect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT * 0.2f, 200, 80};
    SDL_Rect Instructions_rect = {SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT * 0.7f, 300, 20};

    AllocConsole();

    HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
    int hCrt = _open_osfhandle((long) handle_out, _O_TEXT);
    FILE* hf_out = _fdopen(hCrt, "w");
    setvbuf(hf_out, NULL, _IONBF, 1);
    *stdout = *hf_out;

    HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
    hCrt = _open_osfhandle((long) handle_in, _O_TEXT);
    FILE* hf_in = _fdopen(hCrt, "r");
    setvbuf(hf_in, NULL, _IONBF, 128);
    *stdin = *hf_in;

    bool running = true;
    bool menu = true;
    bool game = false;
    bool gameOver = false;

    v2 zero = {0, 0};

    Player player;
    player.loc.x = SCREEN_WIDTH / 2;
    player.loc.y = SCREEN_HEIGHT / 2;
    player.max_speed = 0.20f;
    player.speed = zero;
    player.acceleration = 0.003f;
    player.rotation_speed = 0.2f * Pi / 180;
    player.angle.y = 1.0f;
    player.angle.x = 0.0f;
    player.UP_KEY = true;
    player.RIGHT_KEY = false;
    player.LEFT_KEY = false;
    player.SHOOT = false;
    player.SCORE = 0;
    player.lives = 3;
    player.fire_rate = RATE_OF_FIRE;
    player.immortality = 0;

    Bullet *bullets = NULL;
    Meteor *meteors = NULL;
    HighScore *Scoreboard = NULL;
    LoadFile(&Scoreboard);

    Uint8 *state = SDL_GetKeyboardState(NULL);
    SDL_Event event;
    SDL_StartTextInput();
    int cursor = 0;
    while (running)
    {
        while (menu)
        {
            while ((SDL_PollEvent(&event)) != 0)
            {
                if (event.type == SDL_QUIT)
                {
                    menu = false;
                    running = false;
                    SDL_StopTextInput();
                }
                else if (event.type == SDL_KEYDOWN  && event.key.keysym.sym == SDLK_SPACE)
                {
                    if (cursor == 0) {
                        strcpy(player.playerName, "Player1");
                    } else {
                        player.playerName[cursor] = '\0';
                    }
                    menu = false;
                    game = true;
                }
                else if (event.type == SDL_TEXTINPUT)
                {
                    if (cursor < 7 && ((event.text.text[0] > 96 && event.text.text[0] < 123) || (event.text.text[0] > 59 && event.text.text[0] < 91)))
                    {
                        player.playerName[cursor] = event.text.text[0];
                        Name_rect.w += 30;
                        Name_rect.x -= 15;
                        cursor++;
                    }
                }
            }
            SDL_SetRenderDrawColor(renderer,0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer,255, 255, 255, 255);
            RenderText(renderer, surface, Title, DOS_f, &Title_rect, "Asteroids");
            RenderText(renderer, surface, Instructions, DOS_f, &Instructions_rect,  "Press space to start");
            RenderText(renderer, surface, Name, DOS_f, &Name_rect, player.playerName);
            SDL_RenderPresent( renderer );
        }

        while (game)
        {
            //Update key states:
            while ((SDL_PollEvent(&event)) != 0)
            {
                if (event.type == SDL_QUIT)
                {
                    game = false;
                    running = false;
                }
            }
            //BEFORE RENDER: ////////////////////////////////////////////////////////////
            HandleKeyboardStates(state, &player);
            PlayerMovement(&player);
            PlayerHitDetection(&player, meteors);
            OutOfBounds(&player.loc, 10);
            SpreadMeteors(meteors);
            meteors = Spawner(meteors);
            HitDetection(&meteors, &bullets, &player);

            if (player.fire_rate >= 0) {
                player.fire_rate--;
            }
            if (player.SHOOT && player.fire_rate <= 0)
            {
                player.fire_rate = RATE_OF_FIRE;
                bullets = Shoot(&player, bullets);
            }

            //Clear Screen: //////////////////////////////////////////////////////////////
            SDL_SetRenderDrawColor(renderer,0, 0, 0, 255);
            SDL_RenderClear(renderer);

            //Draw on screen: //////////////////////////////////////////////////////////////
            SDL_SetRenderDrawColor(renderer,255, 255, 255, 255);
            RenderPlayer(renderer, &player);
            bullets = UpdateProjectile(renderer, bullets);
            RenderMeteors(renderer, meteors);
            RenderScore(renderer, surface, Text, DOS_f, &Score_rect, player.SCORE);
            //Update Screen: //////////////////////////////////////////////////////////////
            SDL_RenderPresent( renderer );

            //Check FPS: //////////////////////////////////////////////////////////////////
            //printf("%d \n",FrameRate(&PreviousFrame, &CurrentFrame));

            if (player.lives < 0)
            {
                game = false;
                gameOver = true;
                bullets = FreeAllProjectiles(bullets);
                meteors = FreeAllMeteors(meteors);
                UpdateScore(Scoreboard, &player);
                SaveFile(Scoreboard);
                cursor = 0;
                strcpy(player.playerName, "");
                break;
            }
        }

        while (gameOver)
        {
            while ((SDL_PollEvent(&event)) != 0)
            {
                if (event.type == SDL_QUIT)
                {
                    gameOver = false;
                    running = false;
                }
                else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
                {
                    RenderScoreBoard(Scoreboard);
                    player.loc.x = SCREEN_WIDTH / 2;
                    player.loc.y = SCREEN_HEIGHT / 2;
                    player.lives = 3;
                    player.speed = zero;
                    player.angle.y = 1.0f;
                    player.angle.x = 0.0f;
                    player.SCORE = 0;
                    gameOver = false;
                    menu = true;
                }
            }
            SDL_SetRenderDrawColor(renderer,100, 0, 0, 255);
            SDL_RenderClear(renderer);
            RenderText(renderer, surface, GameOver, DOS_f, &GameOver_rect, "Game Over");
            RenderText(renderer, surface, Instructions, DOS_f, &Instructions_rect, "Press space to exit");
            RenderScore(renderer, surface, Text, DOS_f, &Score_rect, player.SCORE);
            SDL_RenderPresent( renderer );

        }
    }
    SaveFile(Scoreboard);

    bullets = FreeAllProjectiles(bullets);
    meteors = FreeAllMeteors(meteors);
    DeleteScoreBoard(Scoreboard);

    TTF_CloseFont(DOS_f);

    SDL_DestroyTexture(Text);
    SDL_DestroyTexture(Title);
    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );

    DOS_f = NULL;
    window = NULL;
    renderer = NULL;

    TTF_Quit();
    SDL_Quit();


    return 0;
}
