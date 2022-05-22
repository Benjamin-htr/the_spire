#include "./../../../include/raylib.h"
#include "menu.h"
#include "./../utils/utils.h"
#include <stdio.h>
#include <stdlib.h>

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int finishScreen = 0;

static Sprite titleSprite = { 0 };


static int titlePositionY = 0;

void InitMenuScreen(void)
{
    printf("Menu Screen Init\n");

    finishScreen = 0;
    
    constructSprite(&titleSprite, "./asset/Misc/title.png", 2, 5, (Vector2){200, 200});

    titlePositionY = -400;
    
}
void UpdateMenuScreen(void)
{
    titlePositionY += 3;
    if (titlePositionY > 10) titlePositionY = 10;

    updateSprite(&titleSprite);

}
void DrawMenuScreen(void)
{
    float scaleBackground = (float)(GetScreenWidth() / (float)background.width);
    DrawTextureEx(background, (Vector2){0, 0}, 0, scaleBackground, WHITE);
    //DrawTexture(background, 0, 0, WHITE);

    const float scaleFactor = 1.1f;

    const Vector2 position = { GetScreenWidth()/2 - (titleSprite.frameRec.width*scaleFactor)/2, titlePositionY };
    drawSprite(&titleSprite, position, 0.0f, scaleFactor, WHITE);

    Vector2 fontPosition = { 40.0f, GetScreenHeight()/2.0f - 80.0f };

    //DrawTextEx(font, "Congrats! You created your first window !", fontPosition, 24, 0, BLACK);
    //DrawText("Congrats! You created your first window!", 190, 500, 20, LIGHTGRAY);
}
void UnloadMenuScreen(void)
{
    UnloadTexture(titleSprite.texture);
}
int FinishMenuScreen(void)
{
    return finishScreen;
}