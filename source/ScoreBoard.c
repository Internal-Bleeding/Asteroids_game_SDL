#include "ScoreBoard.h"

void LoadFile(HighScore **first)
{
    FILE *fptr; //File pointer
    char Line[50];
    fptr = fopen("Scores.data", "r");
    if (fptr == NULL)
        return;
    printf("\n");
    while(fgets(Line, sizeof(Line), fptr) != NULL)
    {
        HighScore *new = (HighScore*) malloc(sizeof(HighScore));
        if (new == NULL) {
            return;
        }
        if(sscanf(Line, "%s %d", new->playerName, &new->score) != 2) {
            printf("[ERROR] wrong file format!");
            return;
        }
        new->next = NULL;
        if(*first == NULL)
        {
            *first = new;
        }
        else
        {
            HighScore *current = *first;
            while (current->next != NULL)
            {
                current = current->next;
            }
            current->next = new;
        }
    }
    fclose(fptr);
}

void SaveFile(HighScore *Scores)
{
    FILE *fptr; //File pointer

    fptr = fopen("Scores.data", "w");
    if (fptr == NULL) {
        return;
    }
    while (Scores != NULL)
    {
        fprintf(fptr, "%s %05d\n", Scores->playerName, Scores->score);
        Scores = Scores->next;
    }
    fclose(fptr);
}

void UpdateScore(HighScore *first, Player *player)
{
    char name[20];
    int score;
    bool placed = false;
    while (first != NULL)
    {
        if (placed)
        {
            int tempScore = first->score;
            first->score = score;
            char tempName[20];
            strcpy(tempName, first->playerName);
            strcpy(first->playerName, name);
            strcpy(name, tempName);
            score = tempScore;
        }
        if (player->SCORE > first->score && !placed)
        {
            placed = true;
            score = first->score;
            first->score = player->SCORE;
            strcpy(name, first->playerName);
            strcpy(first->playerName, player->playerName);
        }
        first = first->next;
    }
}

void RenderScoreBoard(HighScore *first)
{
    printf("\nScore Board:\n");
    while (first != NULL)
    {
        printf("%s %05d\n", first->playerName, first->score);
        first = first->next;
    }
}

void ResetScore(HighScore *first)
{
    HighScore *temp = first;
    while (temp != NULL)
    {
        strcpy(temp->playerName, "______");
        temp->score = 0;
        temp = temp->next;
    }
    SaveFile(first);
}

HighScore *DeleteScoreBoard(HighScore *first)
{
    HighScore *current = first;
    while (current != NULL)
    {
        HighScore *temp = current;
        current = current->next;
        free(temp);
    }
    first = NULL;
    return first;
}