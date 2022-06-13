#include "./../../../include/raylib.h"
#include "combat.h"
#include "./../utils/utils.h"
#include <stdio.h>

static Texture2D StatBar = {0};
static Texture2D HeartIcon = {0};

static Sprite ennemySprite = {0};

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int finishScreen = 0;

void InitCombatScreen(void)
{
    printf("Combat Screen Init\n");

    StatBar = LoadTexture("./asset/Board/Bar/StatBar.png");
    HeartIcon = LoadTexture("./asset/Board/Bar/unit/heart.png");

    constructSprite(&ennemySprite, "./asset/monsters/jawurm.png", 4, 1);
}
void UpdateCombatScreen(void)
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        showInGameMenu = !showInGameMenu;
    }

    updateSprite(&ennemySprite);
}
void DrawCombatScreen(void)
{
    ClearBackground(BLACK);

    float scaleEnnemy = 2.0f;
    const Vector2 ennemyPos = {GetScreenWidth() / 2 - ennemySprite.frameRec.width * scaleEnnemy / 2, 20};
    drawSprite(&ennemySprite, ennemyPos, 0.0f, scaleEnnemy, WHITE);
}
void UnloadCombatScreen(void)
{
}
int FinishCombatScreen(void)
{
    return finishScreen;
}