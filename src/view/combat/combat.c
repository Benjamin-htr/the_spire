#include "./../../../include/raylib.h"
#include "combat.h"
#include "./../utils/utils.h"
#include <stdio.h>

static Texture2D StatBar = {0};
static Texture2D HeartIcon = {0};
static Texture2D Statboard = {0};

static Sprite ennemySprite = {0};

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int finishScreen = 0;

void drawStatBoard()
{
    int HpMax = 75;
    int HpActuel = 70;

    int ManaMax = 100;
    int ManaActuel = 50;

    float scaleMain = 3.0f;

    int padding = 10;
    int textLeftargin = 10;

    int fontSize = 15;

    // We draw the stat background asset :
    Vector2 StatBoardPos = (Vector2){GetScreenWidth() - Statboard.width * scaleMain - padding, GetScreenHeight() - Statboard.height * scaleMain - padding};
    DrawTextureEx(Statboard, StatBoardPos, 0, scaleMain, WHITE);

    //-----------------------------------------------------------
    // Health :
    // We calculate the HealhBar container position :
    Vector2 HealthBarPos = (Vector2){StatBoardPos.x + Statboard.width * scaleMain * 0.20f, StatBoardPos.y + Statboard.height * scaleMain * 0.5f};

    // We draw the red rectangle of the current life :
    float HpFillWidth = (float)HpActuel / (float)HpMax * StatBar.width * scaleMain * 0.8f;
    DrawRectangle(HealthBarPos.x + StatBar.width * scaleMain / 10, HealthBarPos.y + StatBar.height * scaleMain * 0.2f, HpFillWidth, StatBar.height * scaleMain * 0.6f, RED);

    // We draw the text :
    Vector2 textLifeSize = MeasureTextEx(font, TextFormat("%d/%d", HpActuel, HpMax), fontSize, 1);
    Vector2 textLifePos = (Vector2){HealthBarPos.x + StatBar.width * scaleMain + textLeftargin, HealthBarPos.y + ((StatBar.height * scaleMain) / 4)};
    DrawTextEx(font, TextFormat("%d/%d", HpActuel, HpMax), textLifePos, fontSize, 1, LIGHTGRAY);

    // We draw the healthBar container :
    DrawTextureEx(StatBar, HealthBarPos, 0, scaleMain, WHITE);

    //-----------------------------------------------------------
    // Mana :
    // We calculate the ManaBar container position :
    Vector2 ManaBarPos = (Vector2){StatBoardPos.x + Statboard.width * scaleMain * 0.20f, StatBoardPos.y + Statboard.height * scaleMain * 0.77f};

    // We draw the blue rectangle of the current mana :
    float ManaFillWidth = (float)ManaActuel / (float)ManaMax * StatBar.width * scaleMain * 0.8f;
    DrawRectangle(ManaBarPos.x + StatBar.width * scaleMain / 10, ManaBarPos.y + StatBar.height * scaleMain * 0.2f, ManaFillWidth, StatBar.height * scaleMain * 0.6f, BLUE);

    // We draw the text :
    Vector2 textManaSize = MeasureTextEx(font, TextFormat("%d/%d", ManaActuel, ManaMax), fontSize, 1);
    Vector2 textManaPos = (Vector2){ManaBarPos.x + StatBar.width * scaleMain + textLeftargin, ManaBarPos.y + ((StatBar.height * scaleMain) / 4)};
    DrawTextEx(font, TextFormat("%d/%d", ManaActuel, ManaMax), textManaPos, fontSize, 1, LIGHTGRAY);

    // We draw the manaBar container :
    DrawTextureEx(StatBar, ManaBarPos, 0, scaleMain, WHITE);
}

void InitCombatScreen(void)
{
    finishScreen = 0;
    printf("Combat Screen Init\n");

    StatBar = LoadTexture("./asset/Board/Bar/StatBar.png");
    HeartIcon = LoadTexture("./asset/Board/Bar/unit/heart.png");
    Statboard = LoadTexture("./asset/Board/Bar/StatBoard.png");

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

    ClearBackground(GetColor(0x3f3f74ff));

    float scaleEnnemy = 3.0f;
    const Vector2 ennemyPos = {GetScreenWidth() / 2 - ennemySprite.frameRec.width * scaleEnnemy / 2, 20};
    drawSprite(&ennemySprite, ennemyPos, 0.0f, scaleEnnemy, WHITE);

    drawStatBoard();

    // POUR TEST COMBAT : (A RETIRER PLUS TARD)
    int buttonWidth = 150;
    int buttonHeight = 50;
    if (GuiButton((Rectangle){10, GetScreenHeight() - buttonHeight - 10, buttonWidth, buttonHeight}, "MAP", -1))
    {
        finishScreen = 1;
    }
}
void UnloadCombatScreen(void)
{
}
int FinishCombatScreen(void)
{
    return finishScreen;
}