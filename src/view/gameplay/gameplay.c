#include "./../../../include/raylib.h"
#include "gameplay.h"
#include "./../utils/utils.h"

#include <stdio.h>

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int finishScreen = 0;

static Texture2D arrowButton = {0};

static Sprite roomSprite = {0};

void InitGameplayScreen(void)
{
    printf("Gameplay Screen Init\n");

    arrowButton = LoadTexture("./asset/UI_assets/arrow.png");

    constructSprite(&roomSprite, "./asset/map/room_start.png", 3, 1, (Vector2){GetScreenWidth() / 2, GetScreenHeight() / 2});
}
void UpdateGameplayScreen(void)
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        showInGameMenu = !showInGameMenu;
    }

    updateSprite(&roomSprite);
}
void DrawGameplayScreen(void)
{
    ClearBackground(BLACK);

    Vector2 textSize = MeasureTextEx(font, "ETAGE 1", 20, 1);
    DrawTextEx(font, "ETAGE 1", (Vector2){GetScreenWidth()- (textSize.x+10), GetScreenHeight()-(textSize.y+10)}, 20, 1, LIGHTGRAY);

    float scaleFactor = 4.0f;
    DrawTextureEx(arrowButton, (Vector2){GetScreenWidth() / 2 - arrowButton.width * scaleFactor / 2, GetScreenHeight() / 2 - arrowButton.height * scaleFactor / 2}, 0, scaleFactor, WHITE);

    scaleFactor = 1.2f;

    const Vector2 position = {GetScreenWidth() / 2 - (roomSprite.frameRec.width * scaleFactor) / 2, GetScreenHeight() / 2 - (roomSprite.frameRec.height * scaleFactor) / 2};
    drawSprite(&roomSprite, position, 0.0f, scaleFactor, WHITE);
}
void UnloadGameplayScreen(void)
{
    UnloadTexture(arrowButton);
}
int FinishGameplayScreen(void)
{
    return finishScreen;
}