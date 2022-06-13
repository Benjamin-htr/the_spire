#include "./../../../include/raylib.h"
#include "menu.h"
#include "./../utils/utils.h"
#include <stdio.h>
#include <stdlib.h>

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int finishScreen = 0;

static Sprite titleSprite = {0};

static int titlePositionY = 0;

void InitMenuScreen(void)
{
    printf("Menu Screen Init\n");

    constructSprite(&titleSprite, "./asset/Misc/title.png", 2, 5);

    titlePositionY = -400;
}
void UpdateMenuScreen(void)
{
    titlePositionY += 7;
    if (titlePositionY > 10)
        titlePositionY = 10;

    updateSprite(&titleSprite);
}
void DrawMenuScreen(void)
{
    float scaleBackground = (float)(GetScreenWidth() / (float)background.width);
    DrawTextureEx(background, (Vector2){0, 0}, 0, scaleBackground, WHITE);

    const float scaleFactor = 1.1f;

    const Vector2 position = {GetScreenWidth() / 2 - (titleSprite.frameRec.width * scaleFactor) / 2, titlePositionY};
    drawSprite(&titleSprite, position, 0.0f, scaleFactor, WHITE);

    int buttonWidth = 300;
    int buttonHeight = 80;
    int gap = 30;
    if (GuiButton((Rectangle){GetScreenWidth() / 2 - buttonWidth / 2, 300, buttonWidth, buttonHeight}, "NOUVELLE PARTIE", -1))
    {
        TransitionToScreen(GAMEPLAY);
    }
    if (GuiButton((Rectangle){GetScreenWidth() / 2 - buttonWidth / 2, 300 + buttonHeight + gap, buttonWidth, buttonHeight}, "CREDITS", -1))
    {
        TransitionToScreen(CREDITS);
    }
    if (GuiButton((Rectangle){GetScreenWidth() / 2 - buttonWidth / 2, 300 + (2 * (buttonHeight + gap)), buttonWidth, buttonHeight}, "QUITTER", -1))
    {
        shouldClose = true;
    }
}
void UnloadMenuScreen(void)
{
    UnloadTexture(titleSprite.texture);
}
int FinishMenuScreen(void)
{
    return finishScreen;
}