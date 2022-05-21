#include "./../../../include/raylib.h"
#include "menu.h"
#include <stdio.h>

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int finishScreen = 0;

static Texture2D texTitle = { 0 };

static int titlePositionY = 0;

void InitMenuScreen(void)
{
    printf("Menu Screen Init\n");

    finishScreen = 0;

    texTitle = LoadTexture("./asset/Misc/title.gif");

    titlePositionY = -200;
    
}
void UpdateMenuScreen(void)
{
    titlePositionY += 3;
    if (titlePositionY > 40) titlePositionY = 40;
}
void DrawMenuScreen(void)
{
    const float scaleFactor = 2;

    const Vector2 position = { GetScreenWidth()/2 - (texTitle.width*scaleFactor)/2, GetScreenHeight()/2 - (texTitle.height*scaleFactor)/2 };

    DrawTextureEx(texTitle, position, 0, scaleFactor, WHITE);
    
    DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
    
}
void UnloadMenuScreen(void)
{
    UnloadTexture(texTitle);
}
int FinishMenuScreen(void)
{
    return finishScreen;
}