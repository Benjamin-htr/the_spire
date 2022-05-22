#include "./../../../include/raylib.h"
#include "menu.h"
#include "./../utils/utils.h"
#include <stdio.h>
#include <stdlib.h>

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int finishScreen = 0;

static Texture2D texTitle = { 0 };
static Sprite sprite = { 0 };


static int titlePositionY = 0;

void InitMenuScreen(void)
{
    printf("Menu Screen Init\n");

    finishScreen = 0;

    texTitle = LoadTexture("./asset/Misc/title.gif");
    
    constructSprite(&sprite, "./asset/Misc/title.png", 2, 5, (Vector2){200, 200});

    titlePositionY = -400;
    
}
void UpdateMenuScreen(void)
{
    titlePositionY += 3;
    if (titlePositionY > 40) titlePositionY = 40;

    updateSprite(&sprite);

}
void DrawMenuScreen(void)
{
    const float scaleFactor = 1.5f;

    const Vector2 position = { GetScreenWidth()/2 - (sprite.frameRec.width*scaleFactor)/2, titlePositionY };

    //DrawTextureEx(texTitle, position, 0, scaleFactor, WHITE);
    drawSprite(&sprite, position, 0.0f, scaleFactor, WHITE);


    //DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
}
void UnloadMenuScreen(void)
{
    UnloadTexture(texTitle);
    UnloadTexture(sprite.texture);
}
int FinishMenuScreen(void)
{
    return finishScreen;
}