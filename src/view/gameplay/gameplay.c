#include "./../../../include/raylib.h"
#include "gameplay.h"
#include "./../utils/utils.h"

#include <stdio.h>

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int finishScreen = 0;

void InitGameplayScreen(void)
{
    printf("Gameplay Screen Init\n");
    
}
void UpdateGameplayScreen(void)
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        showInGameMenu = !showInGameMenu;
    }
}
void DrawGameplayScreen(void)
{
    DrawText("GAMEPLAY SCREEN", 190, 200, 20, LIGHTGRAY);
}
void UnloadGameplayScreen(void)
{
    
}
int FinishGameplayScreen(void)
{
    return finishScreen;
}