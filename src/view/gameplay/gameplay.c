#include "./../../../include/raylib.h"
#include "gameplay.h"
#include "./../utils/utils.h"
#include "./../../model/game/game.h"
#include "./../../model/game/map/map.h"

#include <stdio.h>

static int finishScreen = 0;

static Texture2D arrowButtonBack = {0};
static Texture2D arrow = {0};
static Texture2D StatBar = {0};
static Texture2D HeartIcon = {0};

static Sprite roomSpriteStart = {0};
static Sprite roomSprite3Doors = {0};
static Sprite roomSprite2Doors_top_blocked = {0};
static Sprite roomSprite2Doors_bottom_blocked = {0};
static Sprite roomSpriteEnd = {0};

static int roomGapX = -200;

// static int etage = 0;
// static int room = 1;

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
bool ArrowButton(Rectangle bounds, float rotation, int forcedState)
{
    NPatchInfo buttonArrowInfo = {0};

    int state = (forcedState >= 0) ? forcedState : 0; // NORMAL
    bool pressed = false;

    int arrowPosAdd = 0;

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
                arrowPosAdd = 10;
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

    buttonArrowInfo.source = (Rectangle){0, 0, 66, arrowButtonBack.height},
    buttonArrowInfo.left = 30;
    buttonArrowInfo.top = 30;
    buttonArrowInfo.right = 30;
    buttonArrowInfo.bottom = 30;
    buttonArrowInfo.source.x = 66 * state;

    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    // DrawRectangleRec(bounds, GREEN);
    // DrawRectangleLinesEx(bounds, 4, DARKGREEN);
    DrawTextureNPatch(arrowButtonBack, buttonArrowInfo, bounds, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);

    Vector2 posArrow = {0};
    // DrawTextEx(font, text, (Vector2){ bounds.x + bounds.width/2 - arrow.width/2, bounds.y + bounds.height/2 - arrow.height/2 - 4 + arrowPosAdd }, font.baseSize, 1, GetColor(textColor[state]));
    if (rotation == -180)
        posArrow = (Vector2){bounds.x + bounds.width / 2 + arrow.width / 2, bounds.y + bounds.height / 2 + arrow.height / 2 - 4 + arrowPosAdd};
    else if (rotation == -90)
        posArrow = (Vector2){bounds.x + bounds.width / 2 - arrow.height / 2, bounds.y + bounds.height / 2 + arrow.width / 2 - 6 + arrowPosAdd};
    else if (rotation == 90)
        posArrow = (Vector2){bounds.x + bounds.width / 2 + arrow.height / 2, bounds.y + bounds.height / 2 - arrow.width / 2 - 6 + arrowPosAdd};
    else
        posArrow = (Vector2){bounds.x + bounds.width / 2 - arrow.width / 2, bounds.y + bounds.height / 2 - arrow.height / 2 - 4 + arrowPosAdd};

    DrawTextureEx(arrow, posArrow, rotation, 1.0f, WHITE);
    //------------------------------------------------------------------

    return pressed;
}
void drawLifeBar()
{
    int HpMax = 75;
    int HpActuel = 10;

    float scaleBar = 4.0f;
    float scaleHeart = 3.0f;

    int padding = 15;
    int gap = 20;

    int fontSize = 20;
    Vector2 textSize = MeasureTextEx(font, TextFormat("%d/%d hp", HpActuel, HpMax), fontSize, 1);
    Vector2 textPos = (Vector2){GetScreenWidth() - textSize.x - padding, GetScreenHeight() - ((StatBar.height * scaleBar / 2) + textSize.y + padding)};
    DrawTextEx(font, TextFormat("%d/%d hp", HpActuel, HpMax), textPos, fontSize, 1, LIGHTGRAY);

    Vector2 StatBarPos = (Vector2){textPos.x - (StatBar.width * scaleBar + gap), GetScreenHeight() - ((HeartIcon.height * scaleHeart / 2) + (StatBar.height * scaleBar / 2) + padding)};
    DrawTextureEx(HeartIcon, (Vector2){StatBarPos.x - (HeartIcon.width * scaleHeart + gap), GetScreenHeight() - (HeartIcon.height * scaleHeart + padding)}, 0, scaleHeart, WHITE);

    float HpBarWidth = (float)HpActuel / (float)HpMax * StatBar.width * scaleBar * 0.8f;
    DrawRectangle(StatBarPos.x + StatBar.width * scaleBar / 10, StatBarPos.y + StatBar.height * scaleBar * 0.2f, HpBarWidth, StatBar.height * scaleBar * 0.6f, RED);

    DrawTextureEx(StatBar, StatBarPos, 0, scaleBar, WHITE);
}

//----------------------------------------------------------------------------------
// Screen functions :
//----------------------------------------------------------------------------------
void InitGameplayScreen(void)
{
    printf("Gameplay Screen Init\n");

    initGame();

    arrow = LoadTexture("./asset/UI_assets/arrow.png");
    arrowButtonBack = LoadTexture("./asset/UI_assets/button-arrow.png");
    StatBar = LoadTexture("./asset/Board/Bar/StatBar.png");
    HeartIcon = LoadTexture("./asset/Board/Bar/unit/heart.png");

    // load 3 sprites rooms :
    constructSprite(&roomSpriteStart, "./asset/map/room_start.png", 3, 1);
    constructSprite(&roomSprite3Doors, "./asset/map/room_3_doors.png", 3, 1);
    constructSprite(&roomSprite2Doors_top_blocked, "./asset/map/room_2_doors_top_blocked.png", 3, 1);
    constructSprite(&roomSprite2Doors_bottom_blocked, "./asset/map/room_2_doors_bottom_blocked.png", 3, 1);
    constructSprite(&roomSpriteEnd, "./asset/map/room_end.png", 3, 1);
}
void UpdateGameplayScreen(void)
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        showInGameMenu = !showInGameMenu;
    }

    position_player playerPos = player_position(game->mapData);
    int etage = playerPos.x;
    int room = playerPos.y;

    if (etage == 0)
        updateSprite(&roomSpriteStart);
    else if (etage > 0 && etage < 10 && !(room == 3 || room == 0))
        updateSprite(&roomSprite3Doors);
    else if (etage > 0 && etage < 10 && room == 0)
        updateSprite(&roomSprite2Doors_top_blocked);
    else if (etage > 0 && etage < 10 && room == 3)
        updateSprite(&roomSprite2Doors_bottom_blocked);
    else if (etage == 10)
        updateSprite(&roomSpriteEnd);
}
void DrawGameplayScreen(void)
{
    ClearBackground(BLACK);

    position_player playerPos = player_position(game->mapData);
    int etage = playerPos.x;
    int room = playerPos.y;

    drawLifeBar();

    // POUR TEST COMBAT : (A RETIRER PLUS TARD)
    int buttonWidth = 150;
    int buttonHeight = 50;
    if (GuiButton((Rectangle){10, GetScreenHeight() - buttonHeight - 10, buttonWidth, buttonHeight}, "COMBAT", -1))
    {
        // TransitionToScreen(COMBAT);
        ChangeToScreen(COMBAT_SCREEN);
    }

    // Vector2 textSize = MeasureTextEx(font, TextFormat("ETAGE %d", etage), 20, 1);
    DrawTextEx(font, TextFormat("ETAGE %d", etage), (Vector2){10, 10}, 20, 1, LIGHTGRAY);

    float scaleFactor = 4.0f;
    // DrawTextureEx(arrowButton, (Vector2){GetScreenWidth() / 2 - arrowButton.width * scaleFactor / 2, GetScreenHeight() / 2 - arrowButton.height * scaleFactor / 2}, 0, scaleFactor, WHITE);

    scaleFactor = 1.2f;
    float roomWidth = (roomSpriteStart.frameRec.width * scaleFactor);
    float roomHeight = (roomSpriteStart.frameRec.height * scaleFactor);
    const Vector2 position = {GetScreenWidth() / 2 - roomWidth / 2 + roomGapX, GetScreenHeight() / 2 - roomHeight / 2};

    if (etage == 0)
        drawSprite(&roomSpriteStart, position, 0.0f, scaleFactor, WHITE);
    else if (etage > 0 && etage < 10 && !(room == 3 || room == 0))
        drawSprite(&roomSprite3Doors, position, 0.0f, scaleFactor, WHITE);
    else if (etage > 0 && etage < 10 && room == 0)
        drawSprite(&roomSprite2Doors_top_blocked, position, 0.0f, scaleFactor, WHITE);
    else if (etage > 0 && etage < 10 && room == 3)
        drawSprite(&roomSprite2Doors_bottom_blocked, position, 0.0f, scaleFactor, WHITE);
    else if (etage == 10)
        drawSprite(&roomSpriteEnd, position, 0.0f, scaleFactor, WHITE);

    int arrowButtonWidth = 72;
    int padding = 4;

    if (ArrowButton((Rectangle){GetScreenWidth() / 2 + (roomWidth / 2) + padding + roomGapX, GetScreenHeight() / 2 - arrowButtonWidth / 2, arrowButtonWidth, arrowButtonWidth}, 0, -1))
    {
        printf("ArrowButton RIGHT\n");
        move_player(game->mapData, room);
    }
    if (etage == 0)
    {
        if (ArrowButton((Rectangle){GetScreenWidth() / 2 - (roomWidth / 2) - arrowButtonWidth - padding + roomGapX, GetScreenHeight() / 2 - arrowButtonWidth / 2, arrowButtonWidth, arrowButtonWidth}, -180, -1))
        {
            printf("ArrowButton LEFT\n");
            move_player(game->mapData, room + 2);
        }
    }
    if (room != 0 && etage != 10)
    {
        if (ArrowButton((Rectangle){GetScreenWidth() / 2 - arrowButtonWidth / 2 + roomGapX, GetScreenHeight() / 2 - (roomHeight / 2) - arrowButtonWidth - padding, arrowButtonWidth, arrowButtonWidth}, -90, -1))
        {
            printf("ArrowButton TOP\n");
            move_player(game->mapData, room - 1);
        }
    }
    if (room != 3 && etage != 10)
    {
        if (ArrowButton((Rectangle){GetScreenWidth() / 2 - arrowButtonWidth / 2 + roomGapX, GetScreenHeight() / 2 + (roomHeight / 2) + padding, arrowButtonWidth, arrowButtonWidth}, 90, -1))
        {
            printf("ArrowButton BOTTOM\n");
            move_player(game->mapData, room + 1);
        }
    }
}
void UnloadGameplayScreen(void)
{
    UnloadTexture(arrow);
    UnloadTexture(arrowButtonBack);
    UnloadTexture(roomSpriteStart.texture);
    UnloadTexture(roomSprite2Doors_bottom_blocked.texture);
    UnloadTexture(roomSprite2Doors_top_blocked.texture);
    UnloadTexture(roomSprite3Doors.texture);
    UnloadTexture(roomSpriteEnd.texture);
    UnloadTexture(StatBar);
}
int FinishGameplayScreen(void)
{
    return finishScreen;
}
