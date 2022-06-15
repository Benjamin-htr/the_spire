#include "./../../../include/raylib.h"
#include "combat.h"
#include "./../utils/utils.h"
#include <stdio.h>

static Texture2D StatBar = {0};
static Texture2D Statboard = {0};
static Texture2D EnergyICon = {0};

static Texture2D BasicCardPatch = {0};
NPatchInfo cardInfo = {0};

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

    // int EnergyMax = 3;
    int EnergyActuel = 3;

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
    // Vector2 textLifeSize = MeasureTextEx(font, TextFormat("%d/%d", HpActuel, HpMax), fontSize, 1);
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
    // Vector2 textManaSize = MeasureTextEx(font, TextFormat("%d/%d", ManaActuel, ManaMax), fontSize, 1);
    Vector2 textManaPos = (Vector2){ManaBarPos.x + StatBar.width * scaleMain + textLeftargin, ManaBarPos.y + ((StatBar.height * scaleMain) / 4)};
    DrawTextEx(font, TextFormat("%d/%d", ManaActuel, ManaMax), textManaPos, fontSize, 1, LIGHTGRAY);

    // We draw the manaBar container :
    DrawTextureEx(StatBar, ManaBarPos, 0, scaleMain, WHITE);

    //-----------------------------------------------------------
    // Energy :
    float gap = (float)((2.0f / 124.0f) * Statboard.width * scaleMain);
    Vector2 EnergyPos = (Vector2){StatBoardPos.x + Statboard.width * scaleMain * 0.49f, StatBoardPos.y + Statboard.height * scaleMain * 0.083f};
    for (int i = 0; i < EnergyActuel; i++)
    {
        DrawTextureEx(EnergyICon, (Vector2){EnergyPos.x + (EnergyICon.width * i * scaleMain) + (gap * i), EnergyPos.y}, 0, scaleMain, WHITE);
    }
}

int GuiCard(Vector2 position, float scaleFactor, int forcedState)
{
    int nbFrames = 2;
    float cardWidth = (float)cardInfo.source.width * scaleFactor;
    float cardHeight = (float)cardInfo.source.height * scaleFactor;
    Rectangle bounds = (Rectangle){position.x, position.y, cardWidth, cardHeight};

    const int titleCardColor = 0xdfdfbeff;

    int state = (forcedState >= 0) ? forcedState : 0; // NORMAL
    bool pressed = false;
    // Vector2 textSize = MeasureTextEx(font, text, font.baseSize, 1);

    int textPosAdd = 0;

    // Update control
    //--------------------------------------------------------------------
    if ((state < 3) && (forcedState < 0))
    {
        Vector2 mousePoint = GetMousePosition();

        // Check button state
        if (CheckCollisionPointRec(mousePoint, bounds))
        {
            // PRESSED
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            {
                state = 2;
                textPosAdd = 10;
            }
            else
                state = 1; // FOCUSED

            if (IsGestureDetected(GESTURE_TAP))
            {
                pressed = true;
                PlaySound(buttonSound);
            }
        }
    }

    cardInfo.source.x = 96 * state;

    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    // DrawRectangleRec(bounds, GREEN);
    // DrawRectangleLinesEx(bounds, 4, DARKGREEN);
    DrawTextureNPatch(BasicCardPatch, cardInfo, bounds, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
    // DrawTextEx(font, text, (Vector2){bounds.x + bounds.width / 2 - textSize.x / 2, bounds.y + bounds.height / 2 - textSize.y / 2 - 4 + textPosAdd}, font.baseSize, 1, GetColor(textColor[state]));
    //------------------------------------------------------------------

    return pressed;
}

void InitCombatScreen(void)
{
    finishScreen = 0;
    printf("Combat Screen Init\n");

    StatBar = LoadTexture("./asset/Board/Bar/StatBar.png");
    Statboard = LoadTexture("./asset/Board/Bar/StatBoard.png");
    EnergyICon = LoadTexture("./asset/Board/Bar/unit/Energy.png");
    BasicCardPatch = LoadTexture("./asset/Board/card-basic.png");
    cardInfo.source = (Rectangle){0, 0, 96, 156},
    cardInfo.left = 00;
    cardInfo.top = 00;
    cardInfo.right = 00;
    cardInfo.bottom = 00;

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

    if (GuiCard((Vector2){50, 50}, 3.0f, -1))
    {
        printf("card click");
    }
}
void UnloadCombatScreen(void)
{
}
int FinishCombatScreen(void)
{
    return finishScreen;
}